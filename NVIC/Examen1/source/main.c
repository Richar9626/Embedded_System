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
 * @file    Examen1.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

#include "GPIO.h"
#include "Bits.h"
#include "stdint.h"
#include "NVIC.h"
#include "PIT.h"
#include "RGB_set.h"

int main(void) {

	/*Enabling clocks for GPIOS*/
		GPIO_clock_gating(GPIO_A);
		GPIO_clock_gating(GPIO_B);
		GPIO_clock_gating(GPIO_C);
		GPIO_clock_gating(GPIO_D);
		GPIO_clock_gating(GPIO_E);

		/*variables to setup pins as output*/
		gpio_pin_control_register_t g_pcr_gpio_switch = GPIO_MUX1 | GPIO_PS | GPIO_PE | INTR_FALLING_EDGE;
		gpio_pin_control_register_t g_pcr_gpio_led = GPIO_MUX1;

		/**Pin control configuration of GPIO for Led INICIO and ERROR*/

		GPIO_pin_control_register(GPIO_A, bit_4, &g_pcr_gpio_switch);
		GPIO_pin_control_register(GPIO_C, bit_6, &g_pcr_gpio_switch);
		GPIO_pin_control_register(GPIO_B,bit_21,&g_pcr_gpio_led);
		GPIO_pin_control_register(GPIO_B,bit_22,&g_pcr_gpio_led);
		GPIO_pin_control_register(GPIO_E,bit_26,&g_pcr_gpio_led);

		/*SW3 as input*/
		GPIO_data_direction_pin(GPIO_A, GPIO_INPUT, bit_4);
		GPIO_data_direction_pin(GPIO_C, GPIO_INPUT, bit_6);

		/*Outputs for RGB and signal LEds*/
		GPIO_data_direction_pin(GPIO_B,GPIO_OUTPUT, bit_21);
		GPIO_data_direction_pin(GPIO_B,GPIO_OUTPUT,bit_22);
		GPIO_data_direction_pin(GPIO_E,GPIO_OUTPUT,bit_26);

		/*assigning safe value to RGB LEd*/
		GPIO_set_pin(GPIO_B, bit_21);
		GPIO_set_pin(GPIO_B, bit_22);
		GPIO_set_pin(GPIO_E, bit_26);

		/*important to enable pit*/
		PIT_clock_gating();
		PIT_enable();

		/**Sets the threshold for interrupts, if the interrupt has higher priority constant that the BASEPRI, the interrupt will not be attended*/
		NVIC_set_basepri_threshold(PRIORITY_10);
		/**Enables and sets a particular interrupt and its priority*/
		NVIC_enable_interrupt_and_priotity(PORTA_IRQ,PRIORITY_4);
		NVIC_enable_interrupt_and_priotity(PORTC_IRQ,PRIORITY_7);

		NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_6);
		NVIC_enable_interrupt_and_priotity(PIT_CH1_IRQ, PRIORITY_6);
		NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_6);
		NVIC_enable_interrupt_and_priotity(PIT_CH3_IRQ, PRIORITY_6);


		/*enable all pits*/
		PIT_enable_interrupt(PIT_0);
		PIT_enable_interrupt(PIT_1);
		PIT_enable_interrupt(PIT_2);
		PIT_enable_interrupt(PIT_3);

		GPIO_callback_init(GPIO_C, start_sequence);
		PIT_callback_init(PIT_1, blink_delay);

		NVIC_global_enable_interrupts;

		GPIO_clear_irq_status(GPIO_A);
		GPIO_clear_irq_status(GPIO_C);

		while(1)
		{

		}



    return 0 ;
}
