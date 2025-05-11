/*
 * RGB.c
 *
 *  Created on: 19/02/2019
 *      Author: jlpe
 */
#include "RGB.h"
#include "MK64F12.h"

void RGB_red_on_off(void)
{
	static uint8_t state_port_A = 0;
	if(state_port_A)
	{
		GPIOB->PDOR |= 0x00400000;/**Read led off*/
	}
	else
	{
		GPIOB->PDOR &= ~(0x00400000);/**Read led on*/
	}
	state_port_A = !state_port_A;
}

void RGB_blue_on_off(void)
{
	static uint8_t state_port_C = 0;
	;
	if(state_port_C)
	{
		GPIOB->PDOR |= 0x00200000;/**Blue led off*/
	}
	else
	{
		GPIOB->PDOR &= ~(0x00200000);/**Blue led on*/
	}
	state_port_C = !state_port_C;
}
