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

int main(void)
{
	/** Configuration for pin 1 on port c and push buttons.*/
	gpio_pin_control_register_t	pcr_pin_a_1 = GPIO_MUX4|GPIO_PE|GPIO_PS;
	gpio_pin_control_register_t	pcr_pin_a_2 = GPIO_MUX4|GPIO_PE|GPIO_PS;
	gpio_pin_control_register_t	pcr_pin_a_3 = GPIO_MUX4|GPIO_PE|GPIO_PS;
	gpio_pin_control_register_t	pcr_pin_a_4 = GPIO_MUX4|GPIO_PE|GPIO_PS;

	/**Clock gating for port A and C*/
	GPIO_clock_gating(GPIO_A);
	GPIO_clock_gating(GPIO_B);
	GPIO_clock_gating(GPIO_C);
	GPIO_clock_gating(GPIO_D);
	GPIO_clock_gating(GPIO_E);
	GPIO_clock_gating(GPIO_F);

	/**Pin control register configuration for GPIO*/
	GPIO_pin_control_register(GPIO_C, bit_1, &pcr_pin_a_1);
	GPIO_pin_control_register(GPIO_C, bit_2, &pcr_pin_a_2);
	GPIO_pin_control_register(GPIO_C, bit_3, &pcr_pin_a_3);
	GPIO_pin_control_register(GPIO_C, bit_4, &pcr_pin_a_4);

	/**Configuration function for FlexTimer*/
	PWMOutput_CenterAlignment();

	for(;;)
	{

	}

	return 0;
}
