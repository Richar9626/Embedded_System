/*
 * RGB_set.h
 *
 *  Created on: Feb 25, 2021
 *      Author: Fernando
 */

#ifndef RGB_SET_H_
#define RGB_SET_H_

#include "stdint.h"
#define SYSTEM_CLOCK (21000000U)
#define PERIOD_3_sec (6) //set delay
#define TWO_BLINKS 4 //set delay
#define PERIOD_1_sec (2) //set delay
#define DELAY_5_sec (10) //set delay
/**
 *@brief changes the color of the RBG led to the color of the description by using functions of the GPIO.h
 *@param none
 * */

/**note: This module doesn't have a initialization for the ports and bits needed to be use.*/

typedef enum {
	OFF,
	BLUE,
	GREEN,
	RED,
	YELLOW,
	PURPLE,
	CIAN,
	WHITE,
}Color_state_name_t;

void RGB_set_GREEN();
void RGB_set_BLUE();
void RGB_set_PURPLE();
void RGB_set_RED();
void RGB_set_YELLOW();
void RGB_set_WHITE();
void RGB_set_OFF();
void RGB_set_CIAN();

void start_sequence(void);
void RGB_blink(void);
/*delay of blinking, 1s period*/
void blink_delay(void);

void color_count(void);
void color_sequence(void);
void disable_color_count(void);

#endif /* RGB_SET_H_ */
