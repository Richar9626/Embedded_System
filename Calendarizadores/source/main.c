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

/**
 * @file    sebm2o20_calendarizador.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"
#include "fsl_tpm.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
#define TASKAMOUNT 4

volatile uint8_t currentTask = 0;
volatile bool tpmIsrFlag = false;
volatile bool sw2_intStatus = false;
volatile bool sw3_intStatus = false;

volatile uint32_t sysTick_counter = 0xFFFFFFFF;

void PORTA_IRQHandler (void)
{
	GPIO_PortClearInterruptFlags(GPIOA, 1U << 10U);

	/* Change state of button. */
	sw3_intStatus = true;
}

void PORTD_IRQHandler (void)
{
	GPIO_PortClearInterruptFlags(GPIOD, 1U << 11U);

	/* Change state of button. */
	sw2_intStatus = true;
}

void SysTick_Handler(void)
{
	currentTask++;

	currentTask = currentTask & 0x3;
}

void TPM2_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    TPM_ClearStatusFlags(TPM2, kTPM_TimeOverflowFlag);
    tpmIsrFlag = true;
    __DSB();
}

void task1 (uint8_t numTask)
{
	static uint32_t milisecondCounts = 0U;

	PRINTF(" Task %d running \r\n", numTask);

	if (tpmIsrFlag) {
		tpmIsrFlag = false;

		milisecondCounts++;
		if (milisecondCounts >= 250)
		{
			milisecondCounts = 0U;

			GPIO_PortToggle(GPIOC, 1U << 9U);
		}
	}

}

void task2 (uint8_t numTask)
{
	PRINTF(" Task %d running \r\n", numTask);

	if (sw2_intStatus) {
		sw2_intStatus = false;

		GPIO_PortToggle(GPIOE, 1U << 6U);
	}
}

void task3 (uint8_t numTask)
{
	PRINTF(" Task %d running \r\n", numTask);

	if (sw3_intStatus) {
		sw3_intStatus = false;

		GPIO_PortToggle(GPIOA, 1U << 11U);
	}
}

void task4 (uint8_t numTask)
{
	PRINTF(" Task %d running \r\n", numTask);
}

/*
 * @brief   Application entry point.
 */
int main(void)
{
	tpm_config_t tpmInfo;
	void (*tasks[])(uint8_t) = {task1, task2, task3, task4};

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif

	/* LED */
	gpio_pin_config_t led_gpioCofig = {
			kGPIO_DigitalOutput,
			0};

	/* LED Red */
	CLOCK_EnableClock(kCLOCK_PortC);
	PORT_SetPinMux(PORTC, 9U, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOC, 9U, &led_gpioCofig);

	/* LED Green */
	CLOCK_EnableClock(kCLOCK_PortE);
	PORT_SetPinMux(PORTE, 6U, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOE, 6U, &led_gpioCofig);

	/* LED Blue */
	CLOCK_EnableClock(kCLOCK_PortA);
	PORT_SetPinMux(PORTA, 11U, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOA, 11U, &led_gpioCofig);

	/* SW */
	const port_pin_config_t sw_portConfig = {
			/* Internal pull-up resistor is enabled */
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
			kPORT_UnlockRegister};

	gpio_pin_config_t sw_gpioConfig = {
			kGPIO_DigitalInput,
			0};

	/* SW2 */
	CLOCK_EnableClock(kCLOCK_PortD);
	PORT_SetPinConfig(PORTD, 11U, &sw_portConfig);
	PORT_SetPinInterruptConfig(PORTD, 11U, kPORT_InterruptFallingEdge);
	EnableIRQ(PORTD_IRQn);
	GPIO_PinInit(GPIOD, 11U, &sw_gpioConfig);

	/* SW3 */
	CLOCK_EnableClock(kCLOCK_PortA);
	PORT_SetPinConfig(PORTA, 10U, &sw_portConfig);
	PORT_SetPinInterruptConfig(PORTA, 10U, kPORT_InterruptFallingEdge);
	EnableIRQ(PORTA_IRQn);
	GPIO_PinInit(GPIOA, 10U, &sw_gpioConfig);

	TPM_GetDefaultConfig(&tpmInfo);
	tpmInfo.prescale = 4;
    TPM_Init(TPM2, &tpmInfo);

    TPM_SetTimerPeriod(TPM2, USEC_TO_COUNT(1000U, (CLOCK_GetFreq(kCLOCK_PllFllSelClk) / 4)));
    TPM_EnableInterrupts(TPM2, kTPM_TimeOverflowInterruptEnable);
    EnableIRQ(TPM2_IRQn);
    TPM_StartTimer(TPM2, kTPM_SystemClock);

//	SysTick_Config( SystemCoreClock / 90e6 );
	SysTick_Config( 5000 );

	while(1)
	{
		tasks[ currentTask ] ( currentTask );
	}

	return 0 ;
}
