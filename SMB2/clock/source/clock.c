/*
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    alarm.c
 * @brief   Application entry point.
 */

// Added in board.h to SW2:
///*! @brief Define the port interrupt number for the board switches */
#ifndef BOARD_SW2_GPIO
#define BOARD_SW2_GPIO GPIOD
#endif
#ifndef BOARD_SW2_PORT
#define BOARD_SW2_PORT PORTD
#endif
#ifndef BOARD_SW2_GPIO_PIN
#define BOARD_SW2_GPIO_PIN 11U
#endif
#define BOARD_SW2_IRQ         PORTD_IRQn
#define BOARD_SW2_IRQ_HANDLER PORTD_IRQHandler
#define BOARD_SW2_NAME        "SW2"

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_common.h"

#define BOARD_SW_GPIO BOARD_SW2_GPIO
#define BOARD_SW_PORT BOARD_SW2_PORT
#define BOARD_SW_GPIO_PIN BOARD_SW2_GPIO_PIN
#define BOARD_SW_IRQ BOARD_SW2_IRQ

// define three event flag bit variable
#define TASK1_BIT   (1UL << 0UL) // zero shift for bit0
#define TASK2_BIT   (1UL << 1UL) // 1 shift for flag  bit 1
#define TASK3_BIT   (1UL << 2UL) // 2 shift for flag bit 2
#define SW_BIT      (1UL << 3UL) // 3 shift for flag sw
/* TODO: insert other definitions and declarations here. */
typedef enum
{
    second_time,
    minute_time,
    hour_time
} timeType_t;

typedef struct
{
    timeType_t timeTipe;
    uint8_t data;
} timeMsg_t;

typedef struct
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
} timeAlarm_t;

timeAlarm_t alarm;

void PORTD_IRQHandler(void);
void hour_task (void *param);
void minute_task(void *param);
void second_task(void *param);
void print_task(void *param);
void alarm_task(void *param);

SemaphoreHandle_t minuteSem;

SemaphoreHandle_t hourSem;

SemaphoreHandle_t mutexPrintf;

QueueHandle_t print_queue;

//  declare a event group handler variable
EventGroupHandle_t  xEventGroup;
int main(void) {
  alarm.second = 10;
  alarm.minute = 1;
  alarm.hour = 0;

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config =
    { kGPIO_DigitalInput, 0, };

    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    const port_pin_config_t portd11_pin_config =
    {/* Internal pull-up resistor is enabled */
    kPORT_PullUp,
    /* Fast slew rate is configured */
    kPORT_FastSlewRate,
    /* Passive filter is disabled */
    kPORT_PassiveFilterDisable,
    /* Open drain is disabled */
    kPORT_OpenDrainDisable,
    /* Low drive strength is configured */
    kPORT_LowDriveStrength,
    /* Pin is configured as PTA10 */
    kPORT_MuxAsGpio,
    /* Pin Control Register fields [15:0] are not locked */
    kPORT_UnlockRegister };
    /* PORTA10 (pin M9) is configured as PTA10 */
    PORT_SetPinConfig(PORTD, 11U, &portd11_pin_config);

    PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN,
      kPORT_InterruptFallingEdge);

    NVIC_SetPriority(PORTD_IRQn, 0x3);

    EnableIRQ(BOARD_SW_IRQ);
    GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);

    PRINTF("Hello World\n\r");

    print_queue = xQueueCreate(3, sizeof(timeMsg_t));

    minuteSem = xSemaphoreCreateBinary();

    hourSem = xSemaphoreCreateBinary();

    mutexPrintf = xSemaphoreCreateMutex();
    //create event group and assign it a earlier created referene handler
    xEventGroup  =  xEventGroupCreate();

    xTaskCreate(hour_task, "Hour_Task", 200, NULL, 4, NULL);

    xTaskCreate(minute_task, "Minute_Task", 200, NULL, 3, NULL);

    xTaskCreate(second_task, "Second Task", 200, NULL, 2  , NULL);

    xTaskCreate(print_task, "Print Task", 200, NULL, 2  , NULL);

    xTaskCreate(alarm_task, "Alarm Task", 200, NULL, 1  , NULL);

    vTaskStartScheduler();
    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}

void hour_task (void *param)
{
    uint8_t hours = 0;
    timeMsg_t msg;
    msg.timeTipe = hour_time;

    for(;;)
    {
		if(alarm.hour == hours)
		{
			 // set flag bit TASK3_BIT
			xEventGroupSetBits(xEventGroup, TASK3_BIT);
		}
		else
		{
			xEventGroupClearBits(xEventGroup, TASK3_BIT);
		}
		xSemaphoreTake(hourSem, portMAX_DELAY);
		hours += 1;
		if(hours == 24)
		{
			hours = 0;
		}
		msg.data = hours;
		xQueueSendToBack(print_queue, &msg, portMAX_DELAY);
    }
}

void minute_task(void *param)
{
  uint8_t minutes = 0;
  timeMsg_t msg_minute;
  msg_minute.timeTipe = minute_time;
  for(;;)
  {
    xSemaphoreTake(minuteSem,portMAX_DELAY);
    minutes += 1;
    if(alarm.minute == minutes)
	{
		// set flag bit TASK2_BIT
	   xEventGroupSetBits(xEventGroup, TASK2_BIT);
	}
	else
	{
		xEventGroupClearBits(xEventGroup, TASK2_BIT);
	}
    if(minutes == 60)
    {
      minutes = 0;
      xSemaphoreGive(hourSem);
    }
    msg_minute.data = minutes;
    xQueueSendToBack(print_queue, &msg_minute, portMAX_DELAY);
  }
}

void second_task(void *param)
{
  uint8_t seconds = 50;
  timeMsg_t second_data;
  second_data.timeTipe = second_time;
  TickType_t xLastWakeTime = xTaskGetTickCount();

  for(;;)
  {
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
    seconds += 1;
    if(60 == seconds)
    {
      seconds = 0;
      second_data.data = seconds;
      //xQueueSendToBack(print_queue, &second_data, portMAX_DELAY);
      xSemaphoreGive(minuteSem);
    }
    if(alarm.second == seconds)
    {
        // set flag bit TASK1_BIT
       xEventGroupSetBits(xEventGroup, TASK1_BIT);
    }
    else
    {
    	xEventGroupClearBits(xEventGroup, TASK1_BIT);
    }
    second_data.data = seconds;
    xQueueSendToBack(print_queue, &second_data, portMAX_DELAY);
  }
}

void print_task(void *param)
{
  timeMsg_t data;

  timeAlarm_t currentTime;

  currentTime.hour = 0;
  currentTime.minute = 0;
  currentTime.second = 0;

  for (;;)
  {
     xQueueReceive(print_queue, &data, portMAX_DELAY);
      switch(data.timeTipe)
      {
        case second_time:
            currentTime.second = data.data;
        break;

        case minute_time:
          currentTime.minute = data.data;
        break;

        case hour_time:
          currentTime.hour = data.data;
        break;
        default:
        break;
      }

    xSemaphoreTake(mutexPrintf, portMAX_DELAY);
    PRINTF("%2d:%2d:%2d\r", currentTime.hour, currentTime.minute, currentTime.second);
    xSemaphoreGive(mutexPrintf);
    }
}

void alarm_task(void *param)
{
	// define a variable which holds the state of events
	const EventBits_t xBitsToWaitFor  = (TASK1_BIT | TASK2_BIT | TASK3_BIT | SW_BIT);
	EventBits_t xEventGroupValue;
	for (;;)
	{
		xEventGroupValue  = xEventGroupWaitBits(xEventGroup,
												xBitsToWaitFor,
												pdFALSE,
												pdFALSE,
												portMAX_DELAY
												);
		if((xEventGroupValue & TASK1_BIT) !=0 && (xEventGroupValue & TASK2_BIT) !=0 && (xEventGroupValue & TASK3_BIT) !=0)
		{
			xSemaphoreTake(mutexPrintf, portMAX_DELAY);
			PRINTF("\n ALARM \n\r");
			xEventGroupClearBits(xEventGroup, SW_BIT);
			xEventGroupClearBits(xEventGroup, TASK1_BIT);
			xEventGroupClearBits(xEventGroup, TASK2_BIT);
			xEventGroupClearBits(xEventGroup, TASK3_BIT);
		}

		if((xEventGroupValue & SW_BIT) !=0)
		{
			xSemaphoreGive(mutexPrintf);
			//PRINTF("\n semaphore \n");
			xEventGroupClearBits(xEventGroup, SW_BIT);
		}

	}
}

void PORTD_IRQHandler(void)
{
BaseType_t xHigherPriorityTaskWoken;

  /* As always, xHigherPriorityTaskWoken is initialized to pdFALSE. */
  xHigherPriorityTaskWoken = pdFALSE;

  GPIO_PortClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
  /* Set bit 2 in the event group. */
  xEventGroupSetBitsFromISR( xEventGroup, SW_BIT, &xHigherPriorityTaskWoken );
  //PRINTF("\n interrupt \n");

  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
