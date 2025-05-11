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
#include "timers.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

TimerCallbackFunction_t taska_Callback(void* param);
TimerCallbackFunction_t taskb_Callback(void* param);
TimerCallbackFunction_t taskc_Callback(void* param);
TimerCallbackFunction_t taskd_Callback(void* param);

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

    /* Counter on general stack */
    volatile static int i = 0 ;

    /*definition of timers*/
    TimerHandle_t taska, taskb, taskc, taskd;

    /*Assigning parameters to the handles*/
    taska = xTimerCreate("TIMER 0", pdMS_TO_TICKS(5), pdTRUE, (void*) 0, taska_Callback);
    taskb = xTimerCreate("TIMER 1", pdMS_TO_TICKS(50), pdTRUE, (void*) 1, taskb_Callback);
    taskc = xTimerCreate("TIMER 2", pdMS_TO_TICKS(200), pdTRUE, (void*) 2, taskc_Callback);
    taskd = xTimerCreate("TIMER 3", pdMS_TO_TICKS(75), pdTRUE, (void*) 3, taskd_Callback);

    /*Pins as OUTPUT*/
	CLOCK_EnableClock(kCLOCK_PortC);
	PORT_SetPinMux(PORTC, 3U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC, 4U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC, 16U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC, 8U, kPORT_MuxAsGpio);

	gpio_pin_config_t timer_pin = {
			.pinDirection = kGPIO_DigitalOutput,
			.outputLogic = 0U
		};

	GPIO_PinInit(GPIOC, 3U, &timer_pin);
	GPIO_PinInit(GPIOC, 4U, &timer_pin);
	GPIO_PinInit(GPIOC, 16U, &timer_pin);
	GPIO_PinInit(GPIOC, 8U, &timer_pin);

	/*we start this timers with 0*/
	xTimerStart (taska, 0);
	xTimerStart (taskb, 0);
	xTimerStart (taskc, 0);
	xTimerStart (taskd, 0);
	vTaskStartScheduler();
    while(1) {
        i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}

/* Callback definitions*/
TimerCallbackFunction_t taska_Callback(void* param)
{
	GPIO_PortToggle(GPIOC, 1<<3);
	return 0;
}

TimerCallbackFunction_t taskb_Callback(void* param)
{
	GPIO_PortToggle(GPIOC, 1<<4);
	return 0;
}

TimerCallbackFunction_t taskc_Callback(void* param)
{
	GPIO_PortToggle(GPIOC, 1<<16);
	return 0;
}

TimerCallbackFunction_t taskd_Callback(void* param)
{
	GPIO_PortToggle(GPIOC, 1<<8);
	return 0;
}
