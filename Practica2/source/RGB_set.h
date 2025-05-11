/*
 * RGB_set.h
 *
 *  Created on: Feb 25, 2021
 *      Author: Fernando
 */

#ifndef RGB_SET_H_
#define RGB_SET_H_

#include "stdint.h"

/**
 *@brief changes the color of the RBG led to the color of the description by using functions of the GPIO.h
 *@param none
 * */

#define PERIOD_1_sec (2) //set delay
#define TWO_COLORS 4 //set blink limit
#define SYSTEM_CLOCK (21000000U)
#define LED_OFF 0
#define MAX_LUM_LED 1024

#define YELLOW_SCALE 1022
#define GREEN_SCALE 1898
#define CIAN_SCALE 2482
#define BLUE_SCALE 3066
#define PURPLE_SCALE 3650

typedef enum {BLUE, RED, GREEN, NO_COLOR} color_t;

/**note: This module doesn't have a initialization for the ports and bits needed to be use.*/
void RGB_set_GREEN();
void RGB_set_BLUE();
void RGB_set_PURPLE();
void RGB_set_RED();
void RGB_set_YELLOW();
void RGB_set_WHITE();
void RGB_set_CIAN();
void RGB_set_OFF();

void PH_Scale_color(uint16_t value);
void led_delay(void);
void init_color_change(void);

#endif /* RGB_SET_H_ */
