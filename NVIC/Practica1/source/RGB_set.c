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
/**change the color of the RGB led to white*/
void RGB_set_WHITE()
{
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_E, bit_26);
	return;
}
void RGB_set_CIAN()
{
	GPIO_set_pin(GPIO_E, bit_26);
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_B, bit_22);
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
