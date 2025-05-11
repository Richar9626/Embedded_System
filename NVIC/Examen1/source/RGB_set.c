/*
 * RGB_set.c
 *
 *  Created on: Feb 25, 2021
 *      Author: Fernando
 */

#include "MK64F12.h"
#include "GPIO.h"
#include "bits.h"
#include "RGB_set.h"
#include "PIT.h"
#include <stdio.h>

uint8_t g_blink = 0;
uint8_t pit_delay_flag = FALSE;
uint8_t g_period_completed = FALSE;

Color_state_name_t color_to_display = OFF;

//definition of functions array, it points to the function address
void (*color_function[])() = {RGB_set_OFF, RGB_set_BLUE, RGB_set_GREEN, RGB_set_RED, RGB_set_YELLOW,RGB_set_PURPLE,RGB_set_CIAN,RGB_set_WHITE};

/**change the color of the RGB led to green*/
void RGB_set_GREEN()
{
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_E, bit_26);
	return;
}
/**change the color of the RGB led to blue*/
void RGB_set_BLUE()
{
	GPIO_set_pin(GPIO_E, bit_26);
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_B, bit_21);
	return;
}
/**change the color of the RGB led to purple*/
void RGB_set_PURPLE()
{
	GPIO_set_pin(GPIO_E, bit_26);
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_B, bit_22);
	return;
}
/**change the color of the RGB led to red*/
void RGB_set_RED()
{
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_E, bit_26);
	GPIO_clear_pin(GPIO_B, bit_22);
	return;
}
/**change the color of the RGB led to yellow*/
void RGB_set_YELLOW()
{
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_E, bit_26);
	GPIO_clear_pin(GPIO_B, bit_22);
	return;
}
void RGB_set_CIAN()
{
	GPIO_clear_pin(GPIO_E, bit_26);
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);
	return;
}
/**change the color of the RGB led to white*/
void RGB_set_WHITE()
{
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_E, bit_26);
	return;
}
/**change the color of the RGB led to black=OFF*/
void RGB_set_OFF()
{
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_E, bit_26);
	return;
}


void start_sequence()
{
	#ifndef DEBUG_ON
				printf("start sequence:\n");
			#endif

	PIT_delay(PIT_0, SYSTEM_CLOCK, PERIOD_3_sec);
	do
	{
		pit_delay_flag = PIT_get_interrupt_flag_status(PIT_0);
	}while(!GPIO_read_pin(GPIO_C, bit_6) && FALSE == pit_delay_flag);

	if(TRUE == PIT_get_interrupt_flag_status(PIT_0))
	{
		RGB_blink();

		GPIO_callback_init(GPIO_C, color_sequence);
	}
	PIT_stop(PIT_0);
	PIT_clear_interrupt_flag(PIT_0);

return;

}

void color_sequence(void)
{
#ifndef DEBUG_ON
				printf("color sequence:\n");
			#endif
	PIT_delay(PIT_2, SYSTEM_CLOCK, PERIOD_1_sec);
	while(FALSE == PIT_get_interrupt_flag_status(PIT_2))
	{
		 RGB_set_WHITE();

	}
	RGB_set_OFF();
	PIT_stop(PIT_2);
	PIT_clear_interrupt_flag(PIT_2);
	PIT_delay(PIT_2, SYSTEM_CLOCK, DELAY_5_sec);
	GPIO_callback_init(GPIO_A, color_count);
	while(FALSE == PIT_get_interrupt_flag_status(PIT_2))
	{}

	if(CIAN < color_to_display)
	{
		color_function[CIAN]();
	}
	else
	{
		color_function[color_to_display]();
	}
	color_to_display = OFF;
	GPIO_callback_init(GPIO_A, disable_color_count);
	PIT_stop(PIT_2);
	PIT_clear_interrupt_flag(PIT_2);
}

void disable_color_count(void)
{
}

void color_count(void)
{
#ifndef DEBUG_ON
				printf("count\n");
			#endif
	color_to_display ++;
}

/*blink for led INICIO*/
void RGB_blink()
{
#ifndef DEBUG_ON
				printf("blink\n");
			#endif
	RGB_set_WHITE();
	for(g_blink = 0; g_blink < TWO_BLINKS; g_blink++)
	{
		PIT_delay(PIT_1, SYSTEM_CLOCK, PERIOD_1_sec);
		//GPIO_set_pin(GPIO_C, bit_1);
		while(FALSE == g_period_completed)
		{}
		GPIO_toogle_pin(GPIO_B, bit_21);
		GPIO_toogle_pin(GPIO_B, bit_22);
		GPIO_toogle_pin(GPIO_E, bit_26);
		g_period_completed = FALSE;

	}

	RGB_set_OFF();
	GPIO_callback_init(GPIO_C, color_sequence);
	PIT_stop(PIT_1);
	PIT_clear_interrupt_flag(PIT_1);

}


void blink_delay()
{
	g_period_completed = TRUE;
	PIT_stop(PIT_1);
	PIT_clear_interrupt_flag(PIT_1);

}
