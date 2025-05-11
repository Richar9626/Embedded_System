/*
 * Copyright 2016-2020 NXP
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

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_common.h"

#define BOARD_SW_GPIO BOARD_SW3_GPIO
#define BOARD_SW_PORT BOARD_SW3_PORT
#define BOARD_SW_GPIO_PIN BOARD_SW3_GPIO_PIN
#define BOARD_SW_IRQ BOARD_SW3_IRQ

void PORTA_IRQHandler(void);
void task(void);

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#if SYSVIEW_DEBUG
//#define PRINTF(x) SEGGER_SYSVIEW_Print(x)

#define DWR_CTRL (*(volatile uint32_t*)0xE0001000)
#endif

/* Definitions for the event bits in the event group. */
#define mainFIRST_TASK_BIT  ( 1UL << 0UL ) /* Event bit 0, which is set by a task. */
#define mainSECOND_TASK_BIT ( 1UL << 1UL ) /* Event bit 1, which is set by a task. */
#define mainISR_BIT     ( 1UL << 2UL ) /* Event bit 2, which is set by an ISR. */

/* The tasks to be created. */
static void vEventBitSettingTask( void *pvParameters );
static void vEventBitReadingTask( void *pvParameters );

/* Declare the event group in which bits are set from both a task and an ISR. */
EventGroupHandle_t xEventGroup;

int main(void)
{
  /* Init board hardware. */
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitBootPeripherals();
  /* Init FSL debug console. */
  BOARD_InitDebugConsole();

#if SYSVIEW_DEBUG
    DWR_CTRL |= (1 << 0);

    SEGGER_SYSVIEW_Conf();

    SEGGER_SYSVIEW_Start();
#endif

  PRINTF("Hello World\n\r");

  /* Define the init structure for the input switch pin */
  gpio_pin_config_t sw_config =
  { kGPIO_DigitalInput, 0, };

  /* Port A Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortA);

  const port_pin_config_t porta10_pinM9_config =
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
  PORT_SetPinConfig(PORTA, 10U, &porta10_pinM9_config);

  PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN,
    kPORT_InterruptFallingEdge);

  NVIC_SetPriority(PORTA_IRQn, 0x3);

  EnableIRQ(BOARD_SW_IRQ);
  GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);

  /* Before an event group can be used it must first be created. */
  xEventGroup = xEventGroupCreate();

  /* Create the task that sets event bits in the event group. */
  xTaskCreate( vEventBitSettingTask, "BitSetter", 1000, NULL, 1, NULL );

  /* Create the task that waits for event bits to get set in the event
  group. */
  xTaskCreate( vEventBitReadingTask, "BitReader", 1000, NULL, 2, NULL );

  /* Start the scheduler so the created tasks start executing. */
  vTaskStartScheduler();

  /* The following line should never be reached because vTaskStartScheduler()
  will only return if there was not enough FreeRTOS heap memory available to
  create the Idle and (if configured) Timer tasks.  Heap management, and
  techniques for trapping heap exhaustion, are described in the book text. */
  for( ;; );
  return 0;
}

static void vEventBitSettingTask( void *pvParameters )
{
const TickType_t xDelay200ms = pdMS_TO_TICKS( 500UL );

  for( ;; )
  {
    /* Delay for a short while before starting the next loop. */
    vTaskDelay( xDelay200ms );

    /* Print out a message to say event bit 0 is about to be set by the
    task, then set event bit 0. */
    PRINTF( "Bit setting task -\t about to set bit 0.\r\n" );
    xEventGroupSetBits( xEventGroup, mainFIRST_TASK_BIT );

    /* Delay for a short while before setting the other bit set within this
    task. */
    vTaskDelay( xDelay200ms );

    /* Print out a message to say event bit 1 is about to be set by the
    task, then set event bit 1. */
    PRINTF( "Bit setting task -\t about to set bit 1.\r\n" );
    xEventGroupSetBits( xEventGroup, mainSECOND_TASK_BIT );
  }
}
/*-----------------------------------------------------------*/

void PORTA_IRQHandler(void)
{
BaseType_t xHigherPriorityTaskWoken;

  /* As always, xHigherPriorityTaskWoken is initialized to pdFALSE. */
  xHigherPriorityTaskWoken = pdFALSE;

  GPIO_PortClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);

  /* Set bit 2 in the event group. */
  xEventGroupSetBitsFromISR( xEventGroup, mainISR_BIT, &xHigherPriorityTaskWoken );

  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
/*-----------------------------------------------------------*/

static void vEventBitReadingTask( void *pvParameters )
{
const EventBits_t xBitsToWaitFor = ( mainFIRST_TASK_BIT | mainSECOND_TASK_BIT | mainISR_BIT );
EventBits_t xEventGroupValue;

  for( ;; )
  {
    /* Block to wait for event bits to become set within the event group. */
    xEventGroupValue = xEventGroupWaitBits( /* The event group to read. */
                        xEventGroup,

                        /* Bits to test. */
                        xBitsToWaitFor,

                        /* Clear bits on exit if the
                        unblock condition is met. */
                        pdTRUE,

                        /* Don't wait for all bits. */
                        pdFALSE,

                        /* Don't time out. */
                        portMAX_DELAY );

    /* Print a message for each bit that was set. */
    if( ( xEventGroupValue & mainFIRST_TASK_BIT ) != 0 )
    {
      PRINTF( "Bit reading task -\t event bit 0 was set\r\n" );
    }

    if( ( xEventGroupValue & mainSECOND_TASK_BIT ) != 0 )
    {
      PRINTF( "Bit reading task -\t event bit 1 was set\r\n" );
    }

    if( ( xEventGroupValue & mainISR_BIT ) != 0 )
    {
      PRINTF( "Bit reading task -\t event bit 2 was set\r\n" );
    }

    PRINTF( "\r\n" );
  }
}
