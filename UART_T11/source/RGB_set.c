/*
 * RGB_set.c
 *
 *  Created on: Feb 25, 2021
 *      Author: Fernando
 */

#include "MK64F12.h"
#include "GPIO.h"
#include "Bits.h"
#include "RGB_set.h"
#include "PIT.h"
#include <stdio.h>
#include "FlexTimer.h"

static uint8_t g_period_completed = FALSE;
static color_t g_color_sequence = BLUE;

/* upcoming values calculated to map to different ranges are m and b
 * Formulas used: m= y2-y1/x2-x1
 * b = y - mx
 * y = mx + b*/
static const float yellow_to_green_slope = -1.1689;
static const float yellow_to_green_displacement = 2218.6158;
static const float green_to_cian_slope = 1.7535;
static const float green_to_cian_displacement = -3328;
static const float cian_to_blue_slope = -1.7535;
static const float cian_to_blue_displacement = 5376;
static const float blue_to_purple_slope = 1.7534;
static const float blue_to_purple_displacement = -5376.91;
static const float purple_to_dark_slope = -1.1479;
static const float purple_to_dark_displacement = 5214.134;

/**change the color of the RGB led to green*/
void RGB_set_GREEN()
{
	Blue_FlexTimer_update_channel_value(LED_OFF);
	Red_FlexTimer_update_channel_value(LED_OFF);
	Green_FlexTimer_update_channel_value(MAX_LUM_LED);
	return;
}
/**change the color of the RGB led to blue*/
void RGB_set_BLUE()
{
	Blue_FlexTimer_update_channel_value(MAX_LUM_LED);
	Red_FlexTimer_update_channel_value(LED_OFF);
	Green_FlexTimer_update_channel_value(LED_OFF);
	return;
}
/**change the color of the RGB led to purple*/
void RGB_set_PURPLE()
{
	Blue_FlexTimer_update_channel_value(MAX_LUM_LED);
	Red_FlexTimer_update_channel_value(MAX_LUM_LED);
	Green_FlexTimer_update_channel_value(LED_OFF);
	return;
}
/**change the color of the RGB led to red*/
void RGB_set_RED()
{
	Blue_FlexTimer_update_channel_value(LED_OFF);
	Red_FlexTimer_update_channel_value(MAX_LUM_LED);
	Green_FlexTimer_update_channel_value(LED_OFF);
	return;
}
/**change the color of the RGB led to yellow*/
void RGB_set_YELLOW()
{
	Blue_FlexTimer_update_channel_value(LED_OFF);
	Red_FlexTimer_update_channel_value(MAX_LUM_LED);
	Green_FlexTimer_update_channel_value(MAX_LUM_LED);
	return;
}
/**change the color of the RGB led to white*/
void RGB_set_WHITE()
{
	Blue_FlexTimer_update_channel_value(MAX_LUM_LED);
	Red_FlexTimer_update_channel_value(MAX_LUM_LED);
	Green_FlexTimer_update_channel_value(MAX_LUM_LED);
	return;
}
void RGB_set_CIAN()
{
	Blue_FlexTimer_update_channel_value(MAX_LUM_LED);
	Red_FlexTimer_update_channel_value(LED_OFF);
	Green_FlexTimer_update_channel_value(MAX_LUM_LED);
	return;
}
/**change the color of the RGB led to black=OFF*/
void RGB_set_OFF()
{
	Blue_FlexTimer_update_channel_value(LED_OFF);
	Red_FlexTimer_update_channel_value(LED_OFF);
	Green_FlexTimer_update_channel_value(LED_OFF);
	return;
}

/*Expected value from 0 to 4096 to correspond with ADC*/
void PH_Scale_color(uint16_t value)
{
	/* we add a float variable to map with slopes and displacements calculated */
	float float_value = (float) value;
	/* Separated into ranges since depending on the range a color is changing and others are static*/
	if(YELLOW_SCALE > value)
	{
		/* in this range green color is incrementing and red set to max*/
		Blue_FlexTimer_update_channel_value(LED_OFF);
		Red_FlexTimer_update_channel_value(MAX_LUM_LED);
		Green_FlexTimer_update_channel_value(value);
	}

	else if(GREEN_SCALE > value && YELLOW_SCALE <= value)
	{
		/* in this range green is max and red decreases*/
		Blue_FlexTimer_update_channel_value(LED_OFF);
		Red_FlexTimer_update_channel_value((uint16_t)(float_value * yellow_to_green_slope + yellow_to_green_displacement));
		Green_FlexTimer_update_channel_value(MAX_LUM_LED);
	}

	else if(CIAN_SCALE > value && GREEN_SCALE <= value)
	{
		/* in this range green still on max and blue increments*/
		Blue_FlexTimer_update_channel_value((uint16_t)(float_value * green_to_cian_slope + green_to_cian_displacement));
		Red_FlexTimer_update_channel_value(LED_OFF);
		Green_FlexTimer_update_channel_value(MAX_LUM_LED);
	}

	else if(BLUE_SCALE > value && CIAN_SCALE <= value)
	{
		/* in this range green is decremented to get only blue*/
		Blue_FlexTimer_update_channel_value(MAX_LUM_LED);
		Red_FlexTimer_update_channel_value(LED_OFF);
		Green_FlexTimer_update_channel_value((uint16_t)(float_value * cian_to_blue_slope + cian_to_blue_displacement));
	}

	else if(PURPLE_SCALE > value && BLUE_SCALE <= value)
	{
		/* in this range red increments to reach purple color*/
		Blue_FlexTimer_update_channel_value(MAX_LUM_LED);
		Red_FlexTimer_update_channel_value((uint16_t)(float_value * blue_to_purple_slope + blue_to_purple_displacement));
		Green_FlexTimer_update_channel_value(LED_OFF);
	}

	else if(PURPLE_SCALE < value)
	{
		/* in this range we decrease red to half until it reaches a darker purple*/
		Blue_FlexTimer_update_channel_value(MAX_LUM_LED);
		Red_FlexTimer_update_channel_value((uint16_t)(float_value * purple_to_dark_slope + purple_to_dark_displacement));
		Green_FlexTimer_update_channel_value(LED_OFF);
	}
	return;
}

/*blink for led at program start*/
void init_color_change(void)
{

	RGB_set_BLUE(); /*initialize on blue color*/
	for(g_color_sequence = RED; g_color_sequence < TWO_COLORS; g_color_sequence++) /* this for is to switch between colors, but only two colors*/
	{
		PIT_delay(PIT_3, SYSTEM_CLOCK, PERIOD_1_sec); /* we launch pit counting*/

		while(FALSE == g_period_completed) /* waiting for pit interrupt*/
		{}

		switch(g_color_sequence) /*depending on for cycle a color will be displayed*/
		{
			case RED:
				RGB_set_RED();
				break;
			case BLUE: /* will not enter here since it starts on Red bc blue is on before*/
				RGB_set_BLUE();
				break;
			case GREEN:
				RGB_set_GREEN();
				break;
			default:
				RGB_set_OFF();
				break;
		}

		g_period_completed = FALSE;
	}

	RGB_set_OFF(); /* we turn off led and disable pit to stop counting and interrupting*/
	PIT_stop(PIT_3);
	PIT_clear_interrupt_flag(PIT_3);

}

/*Function owned by callback*/
void led_delay(void)
{
	g_period_completed = TRUE; /* Flag to be asked on for delay*/
	PIT_stop(PIT_3);
	PIT_clear_interrupt_flag(PIT_3);

}
