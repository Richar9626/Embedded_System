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
#include "Bits.h"
//#include "GlobalFunctions.h"

/**
 * Output compare frequency = (bus clock)/(Prescaler(mod+1)).
 * Note that is the same frequency of the overflow flow.
 *
 * In this case we want a pulse after 1 second
 * not possiblke due to 16bit register and prescaler of 128
 * limit is on 0.1
 * So we set Mod register on max with 0xFFFF to reach limit
 */

int main(void)
{
	gpio_pin_control_register_t g_pcr_gpio_ftm = GPIO_MUX4;
	/*Enabling clocks for GPIOS*/
	GPIO_clock_gating(GPIO_A);
	GPIO_clock_gating(GPIO_C);
	/** Configuration for pin 1 on port C (FTM0_CH_0).
	 * Output compare is on option 4 in the pin control register*/
	GPIO_pin_control_register(GPIO_C, bit_1, &g_pcr_gpio_ftm);

	/**Initialization of FlexTimer in output compare mode*/
	FlexTimer_Init();


	for(;;) {


	}

	return 0;
}
