/*
 * Delay.c
 *
 *  Created on: 11 feb. 2021
 *      Author: LUISPIZANO
 */

#include "stdint.h"

void delay(uint16_t delay)
{
	volatile uint16_t j;

	for(j = delay; j > 0; j--)
	{
		__asm("nop");

	}
}
