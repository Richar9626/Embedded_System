/*
 * Delay.c
 *
 *  Created on: 19/08/2017
 *      Author: jlpe
 */

#include "Delay.h"
#include "Bits.h"

void Delay_cycles(uint32_t cycles)
{
	__asm (	"ldr r1, = 0"); /** Load r1 with zero*/
	__asm (	"ldr r2, = 0xffffffff"); /**Load r2 with -1 (0xffffffff)*/
	__asm (	"LOOP:"); /** Label to implement a loop*/
	__asm (	"add	r0, r0, r2 "); /** Add r2 with the cycles*/
	__asm ( "cmp r1, r0 "); /** Compares r1 and r0*/
	__asm ( "bne LOOP "); /* Conditional branch completes in a single cycle if the branch is not taken*/

}



void Delay_ms_or_us(uint16_t time, uint32_t system_clock, uint8_t ms_or_us)
{
	uint32_t number_of_clocks = 0;

	float time_float  = (0.0F);

	if(TRUE == ms_or_us)
	{
		time_float = (time / u_SECONDS_CONST);
	}
	else
	{
		time_float = (time / m_SECONDS_CONST);
	}


	time_float = 1 / time_float;

	number_of_clocks = (uint32_t)(system_clock / time_float);
	number_of_clocks = number_of_clocks >> 2;

	Delay_cycles(number_of_clocks);
}
