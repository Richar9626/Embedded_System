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
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

void task1(void* param);
void taska(void* param);
void taskb(void* param);
void taskc(void* param);
void taskd(void* param);

struct task_param_t{
	uint8_t task_pin;
	uint8_t task_period;
};

struct task_param_t pcParamForTaska = {3U,5};
struct task_param_t pcParamForTaskb = {4U,50};
struct task_param_t pcParamForTaskc = {16U,200};
struct task_param_t pcParamForTaskd = {8U,75};

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

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
	xTaskCreate(task1, "task_1", 200, NULL, 4, NULL);

	vTaskStartScheduler();
    while(1) {
        i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}

void task1(void* param)
{
	xTaskCreate(taska, "task_a", 200, (void*) &pcParamForTaska, 4, NULL);
	xTaskCreate(taska, "task_b", 200, (void*) &pcParamForTaskb, 3, NULL);
	xTaskCreate(taska, "task_c", 200, (void*) &pcParamForTaskc, 1, NULL);
	xTaskCreate(taska, "task_d", 200, (void*) &pcParamForTaskd, 2, NULL);
	while(1)
	{
		vTaskDelay(portMAX_DELAY);
	}
}

void taska(void* param)
{
	uint8_t TaskPin;
	uint8_t TaskPeriod;
	struct task_param_t *taskParam = (void*) param;
	TaskPin = (uint8_t) taskParam->task_pin;
	TaskPeriod = (uint8_t) taskParam->task_period;

	CLOCK_EnableClock(kCLOCK_PortC);
	PORT_SetPinMux(PORTC, TaskPin, kPORT_MuxAsGpio);

	gpio_pin_config_t task_a_pin = {
		.pinDirection = kGPIO_DigitalOutput,
		.outputLogic = 0U
	};
	/* Initialize GPIO functionality on pin PTA10 (pin M9)  */
	GPIO_PinInit(GPIOC, TaskPin, &task_a_pin);

	TickType_t getTickCount = xTaskGetTickCount();

	for(;;)
	{
		vTaskDelayUntil(&getTickCount, pdMS_TO_TICKS(TaskPeriod));
		GPIO_PortToggle(GPIOC, 1<<TaskPin);
	}

}


