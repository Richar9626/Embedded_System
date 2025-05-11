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
 * @file    Teclado.c
 * @brief   Application entry point.
 */

#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

#include "GPIO.h"
#include "Bits.h"
#include "stdint.h"
#include "Keypad4x4.h"
#include "NVIC.h"
#include "PIT.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

//variables to setup pins as output
gpio_pin_control_register_t g_pcr_gpiob_pin_21 = GPIO_MUX1;
gpio_pin_control_register_t g_pcr_gpiob_pin_22 = GPIO_MUX1;
gpio_pin_control_register_t g_pcr_gpioe_pin_26 = GPIO_MUX1;

int main(void) {

	keypad_init();

	GPIO_clock_gating(GPIO_B);
	GPIO_clock_gating(GPIO_E);

	/**Pin control configuration of GPIOB pin22, pin21 and GPIOE pin 26 as GPIO*/
	GPIO_pin_control_register(GPIO_B,bit_21,&g_pcr_gpiob_pin_21);
	GPIO_pin_control_register(GPIO_B,bit_22,&g_pcr_gpiob_pin_22);
	GPIO_pin_control_register(GPIO_E,bit_26,&g_pcr_gpioe_pin_26);

	/**Assigns a safe value to the output pin*/
	GPIO_write_port(GPIO_B, 0x00600000);
	GPIO_write_port(GPIO_E, 0x04000000);

	/**Configures GPIOB pin21, pin22 and GPIOE pin26 as output*/
	GPIO_data_direction_pin(GPIO_B,GPIO_OUTPUT, bit_21);  //blue
	GPIO_data_direction_pin(GPIO_B,GPIO_OUTPUT,bit_22);  //red
	GPIO_data_direction_pin(GPIO_E,GPIO_OUTPUT,bit_26); //green

	GPIO_callback_init(GPIO_B, read_key);

	PIT_clock_gating();
	PIT_enable();

	PIT_enable_interrupt(PIT_0);
/**Sets the threshold for interrupts, if the interrupt has higher priority constant that the BASEPRI, the interrupt will not be attended*/
	NVIC_set_basepri_threshold(PRIORITY_10);
	/**Enables and sets a particular interrupt and its priority*/
	NVIC_enable_interrupt_and_priotity(PORTB_IRQ,PRIORITY_5);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_8);

	NVIC_global_enable_interrupts;

	PIT_callback_init(PIT_0, get_last_key_delay);
	uint8_t key_pressed = 0;
	GPIO_clear_irq_status(GPIO_B);
    while(1)
    {
    	if(TRUE == GPIO_get_irq_status(GPIO_B))
		{

			key_pressed = get_last_key();
			#ifndef DEBUG_ON
				printf("Key pressed: %c\n", key_pressed);
			#endif
			GPIO_clear_irq_status(GPIO_B);

		}
    	if(key_pressed == '1')//to make a cycle, after yellow it starts again on green
		{
			GPIO_set_pin(GPIO_B, bit_21);
			GPIO_set_pin(GPIO_B, bit_22);
			GPIO_clear_pin(GPIO_E, bit_26);
		}

    	else if(key_pressed == '2')//to make a cycle, after yellow it starts again on green
		{
			GPIO_set_pin(GPIO_E, bit_26);
			GPIO_set_pin(GPIO_B, bit_22);
			GPIO_clear_pin(GPIO_B, bit_21);
		}

    	else if(key_pressed == '3')//to make a cycle, after yellow it starts again on green
		{
			GPIO_set_pin(GPIO_E, bit_26);
			GPIO_clear_pin(GPIO_B, bit_21);
			GPIO_clear_pin(GPIO_B, bit_22);
		}

    }
    return 0 ;
}
