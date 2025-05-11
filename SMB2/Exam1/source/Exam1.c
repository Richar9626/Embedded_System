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
 * @file    signalGenerator.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"


/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
// define three event flag bit variable
#define TASK1_BIT   (1UL << 0UL) // zero shift for bit0
#define TASK2_BIT   (1UL << 1UL) // 1 shift for flag  bit 1
#define TASK3_BIT   (1UL << 2UL) // 2 shift for flag bit 2

typedef enum
{
    task_1,
    task_2
} taskType_t;

typedef struct
{
	taskType_t taskTipe;
    uint8_t data;
} taskMsg_t;

/*
 * @brief   Application entry point.
 */

void taskA(void* param);
void task1(void* param);
void task2(void* param);
void task3(void* param);



QueueHandle_t xQueue;

SemaphoreHandle_t senderMutex;

SemaphoreHandle_t printMutex;

SemaphoreHandle_t receiveSem;

//  declare a event group handler variable
EventGroupHandle_t  xEventGroup;

int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    PRINTF("Hello World\n");
    xQueue = xQueueCreate( 3, sizeof( taskMsg_t ) );
    senderMutex = xSemaphoreCreateMutex();
    printMutex = xSemaphoreCreateMutex();

    receiveSem = xSemaphoreCreateBinary();

    //create event group and assign it a earlier created referene handler
    xEventGroup  =  xEventGroupCreate();
    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
	xTaskCreate(taskA, "task_A", 200, NULL, 4, NULL);

	vTaskStartScheduler();
    while(1) {
        i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}

void taskA(void* param)
{
	xTaskCreate(task1, "task_1", 200, NULL, 2, NULL);
	xTaskCreate(task2, "task_2", 200, NULL, 2, NULL);
	xTaskCreate(task3, "task_3", 200, NULL, 2, NULL);
	while(1)
	{
		vTaskDelay(portMAX_DELAY);
	}
}


void task1(void* param)
{
	portBASE_TYPE xStatus;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	uint8_t character = 'h';
	taskMsg_t msg;
	msg.taskTipe = task_1;
	msg.data = character;

	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; )
	{
		xSemaphoreTake(senderMutex,portMAX_DELAY);
		xStatus = xQueueSendToBack( xQueue, &msg, portMAX_DELAY );
		xSemaphoreGive(senderMutex);

		if( xStatus != pdPASS )
		{
			/* We could not write to the queue because it was full - this must
			be an error as the receiving task should make space in the queue
			as soon as both sending tasks are in the Blocked state. */
			PRINTF( "Could not send to the queue.\r\n" );
		}

		else
		{
	        // set flag bit TASK1_BIT
	       xEventGroupSetBits(xEventGroup, TASK1_BIT); //esto desbloquea al receptor
	       vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(150));
		}


		/* Allow the other sender task to execute. */
		taskYIELD();
	}
}
void task2(void* param)
{
	portBASE_TYPE xStatus;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	uint8_t character = 'a';
	taskMsg_t msg;
	msg.taskTipe = task_2;
	msg.data = character;

	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; )
	{
		xSemaphoreTake(senderMutex,portMAX_DELAY); //sincronizamos las tareas, mientras una envia la otra no
		xStatus = xQueueSendToBack( xQueue, &msg, portMAX_DELAY );
		xSemaphoreGive(senderMutex);

		if( xStatus != pdPASS )
		{
			/* We could not write to the queue because it was full - this must
			be an error as the receiving task should make space in the queue
			as soon as both sending tasks are in the Blocked state. */
			PRINTF( "Could not send to the queue.\r\n" );
		}
		else
		{
	        // set flag bit TASK1_BIT
	       xEventGroupSetBits(xEventGroup, TASK2_BIT);
	       vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200)); //bloqueamos por determinado tiempo
		}

		/* Allow the other sender task to execute. */
		taskYIELD();
	}
}
void task3(void* param)
{
	/* Declare the structure that will hold the values received from the queue. */
	taskMsg_t data;
	portBASE_TYPE xStatus;
	const EventBits_t xBitsToWaitFor  = (TASK1_BIT | TASK2_BIT);
	EventBits_t xEventGroupValue;

	/* This task is also defined within an infinite loop. */
	for( ;; )
	{
		xEventGroupValue  = xEventGroupWaitBits(xEventGroup,
												xBitsToWaitFor,
												pdFALSE,
												pdFALSE,
												portMAX_DELAY
												);
		if( uxQueueMessagesWaiting( xQueue ) != 3 )
		{
			PRINTF( "Queue should have been full!\r\n" );
		}


		if((xEventGroupValue & TASK1_BIT) !=0)
		{
			xStatus = xQueueReceive( xQueue, &data, portMAX_DELAY );
			if( xStatus == pdPASS )
			{
				xEventGroupClearBits(xEventGroup, TASK1_BIT);
				xSemaphoreTake(printMutex, portMAX_DELAY);
				PRINTF( "From Task 1 = %d\r\n", data.data );
				xSemaphoreGive(printMutex);
			}
			else
			{
				/* We did not receive anything from the queue.  This must be an error
				as this task should only run when the queue is full. */
				PRINTF( "Could not receive from the queue.\r\n" );
			}
		}

		if((xEventGroupValue & TASK2_BIT) !=0)
		{
			xStatus = xQueueReceive( xQueue, &data, portMAX_DELAY );
			if( xStatus == pdPASS )
			{
				xEventGroupClearBits(xEventGroup, TASK2_BIT);
				xSemaphoreTake(printMutex, portMAX_DELAY);
				PRINTF( "From Task 2 = %d\r\n", data.data );
				xSemaphoreGive(printMutex);
			}
			else
			{
				/* We did not receive anything from the queue.  This must be an error
				as this task should only run when the queue is full. */
				PRINTF( "Could not receive from the queue.\r\n" );
			}
		}



	}
}

