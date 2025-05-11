/**
	\file
	\brief
		This project shows how to configure FlexTimer in PWM mode.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	21/03/2019
	\todo
	    Add configuration structures.
 */

#include <stdio.h>
#include "MK64F12.h" /* include peripheral declarations */
#include "GPIO.h"
#include "FlexTimer.h"
#include "NVIC.h"
#include "stdint.h"
#include "Bits.h"
#include "Delay.h"

#define clock_frequency 21000000

/*constant to have a delay for push buttons*/
#define DELAY_CONST 650

int main(void)
{
	/*Initializing variables as it begins with 1MHz frequency*/
	float mod_register_value = 20;
	double frequency = 1000000;

	/** Configuration for pin 1 on port c and push buttons.*/
	gpio_pin_control_register_t g_pcr_gpio_ftm = GPIO_MUX4;
	gpio_pin_control_register_t	pcr_pin_a_4 = GPIO_MUX1|GPIO_PE|GPIO_PS;
	gpio_pin_control_register_t	pcr_pin_a_6 = GPIO_MUX1|GPIO_PE|GPIO_PS;

	/**Clock gating for port A and C*/
	GPIO_clock_gating(GPIO_A);
	GPIO_clock_gating(GPIO_C);

	/**Pin control register configuration for GPIO*/
	GPIO_pin_control_register(GPIO_C, bit_1, &g_pcr_gpio_ftm);
	GPIO_pin_control_register(GPIO_A, bit_4, &pcr_pin_a_4);
	GPIO_pin_control_register(GPIO_C, bit_6, &pcr_pin_a_6);

	/*setup of SW3 and SW2 as input*/
	GPIO_data_direction_pin(GPIO_C,GPIO_INPUT, bit_6);
	GPIO_data_direction_pin(GPIO_A,GPIO_INPUT, bit_4);

	/**Configuration function for FlexTimer*/
	FlexTimer_Init();

	for(;;) {

		if(GPIO_read_pin(GPIO_C, bit_6) && !GPIO_read_pin(GPIO_A, bit_4))/*frequency increase of %15  SW3*/
		{
			/*capture sw3 until falling edge*/
			while(GPIO_read_pin(GPIO_C, bit_6) && !GPIO_read_pin(GPIO_A, bit_4))
			{
				delay(DELAY_CONST);
			}

			frequency = frequency * 1.15; /*Increase of 15*/
			mod_register_value = (clock_frequency/frequency) - 1; /*we determine the value to update mod register*/
			FlexTimer_update_channel_value(mod_register_value);

		}
		if(!GPIO_read_pin(GPIO_C, bit_6) && GPIO_read_pin(GPIO_A, bit_4)) /*decrease 5%   SW2*/
		{
			/*capture sw2 until falling edge*/
			while(!GPIO_read_pin(GPIO_C, bit_6) && GPIO_read_pin(GPIO_A, bit_4))
			{
				delay(DELAY_CONST); /*we add a delay to eliminate ups and downs after push*/
			}

			frequency = frequency * 0.95;
			mod_register_value = (clock_frequency/frequency) - 1;
			FlexTimer_update_channel_value(mod_register_value);
		}

	}

	return 0;
}
