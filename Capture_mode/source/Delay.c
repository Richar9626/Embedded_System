/*
 * Delay.c
 *
 *  Created on: 11 feb. 2021
 *      Author: LUISPIZANO
 */

#include "stdint.h"

void delay(uint16_t delay)
{
	volatile int counter, counter2;

	for(counter2=16; counter2 > 0; counter2--)
	{
		for(counter=delay; counter > 0; counter--);

	}
}
