/**
	\file
	\brief
		This project shows how to configure FlexTimer in output compare mode.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	21/03/2019
	\todo
	    Add configuration structures.
 */


#include "MK64F12.h" /* include peripheral declarations */
#include "GPIO.h"
#include "FlexTimer.h"
#include "NVIC.h"
#include "Delay.h"
#include "Bits.h"


/**
 * Output compare frequency = (bus clock)/(Prescaler(mod+1)).
 * Note that is the same frequency of the overflow flow.
 *
 *To calculate frequency:
 *freq captured = (1/difference_between_edges)*(1/FTMCLK)
 */



int main(void)
{
	float frequency = 0;
	/** Configuration for pin 1 on port c.*/
	gpio_pin_control_register_t	pinControlRegisterPORTC = GPIO_MUX3|GPIO_PE|GPIO_PS;

	/**Clock gating for port A and C*/
	GPIO_clock_gating(GPIO_A);
	GPIO_clock_gating(GPIO_B);
	GPIO_clock_gating(GPIO_C);
	 /* Output compare is on option 4 in the pin control register*/
	GPIO_pin_control_register(GPIO_B, bit_18, &pinControlRegisterPORTC);

	GPIO_data_direction_pin(GPIO_B,GPIO_INPUT, bit_18);

	/**Initialization of FlexTimer in capture mode*/
	FlexTimer_Init();

	NVIC_enable_interrupt_and_priotity(FTM2_IRQ,PRIORITY_9);

	NVIC_global_enable_interrupts;/** Enabling Global interrupts with PRIMASK bit*/
	for(;;)
	{
		frequency = get_Frequency();
		#ifndef DEBUG_ON
			printf("Frequency: %f\n", frequency); /*Adding <stdio.h> makes float to not be printed*/
		#endif

		delay(50000);
	}

	return 0;
}
