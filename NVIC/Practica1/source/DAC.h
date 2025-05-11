/*
 * DAC.h
 *
 *  Created on: Mar 13, 2021
 *      Author: Fernando
 */

#ifndef DAC_H_
#define DAC_H_

#include "stdint.h"

#define DAC0_REF2_mask 0xC0U /**Mask for enabling DAC and set voltage to REF2*/

/**Names of all the DACs*/
typedef enum {DAC_0,
			  DAC_1
			 }DAC_name_t;

/**
 *@brief initialize the clock, enables DAC, and sets the DAC buffer to 0
 *@param DACn name
 */
void DAC_init(DAC_name_t DAC_name);

/**
 *@brief initialize the clock of the DAC
 *@param DACn name
 */
void DAC_clock_gating(DAC_name_t DAC_name);

/**
 *@brief enables the DAC and sets the voltage to voltageREF2
 *@param DACn name
 */
void DAC_enable(DAC_name_t DAC_name);

/**
 *@brief sets the value of the low and high word0 of the DAC to value,
 *@brief to make a voltage variable at the DAC_OUT pin.
 *@param value from 0 to 2^12
 */
void DAC_set_value(uint16_t value);

#endif /* DAC_H_ */
