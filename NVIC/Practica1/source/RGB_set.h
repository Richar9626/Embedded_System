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

/**note: This module doesn't have a initialization for the ports and bits needed to be use.*/
void RGB_set_GREEN();
void RGB_set_BLUE();
void RGB_set_PURPLE();
void RGB_set_RED();
void RGB_set_YELLOW();
void RGB_set_WHITE();
void RGB_set_OFF();



#endif /* RGB_SET_H_ */
