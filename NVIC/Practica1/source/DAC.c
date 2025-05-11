/*
 * DAC.c
 *
 *  Created on: Mar 13, 2021
 *      Author: Fernando
 */

#include "MK64F12.h"
#include "DAC.h"
#include "bits.h"

/**initialize the clock, enables DAC, and sets the DAC buffer to 0*/
void DAC_init(DAC_name_t DAC_name){
	DAC_clock_gating(DAC_name);
	DAC_enable(DAC_name);
	DAC_set_value(FALSE);
}

void DAC_clock_gating(DAC_name_t DAC_name){
	switch(DAC_name)/** Selecting the DACn for clock enabling*/
	{
		case DAC_0: /** DAC0 A is selected*/
			SIM->SCGC2 |= SIM_SCGC2_DAC0_MASK; /** Bit 12 of SIM_SCGC2 is  set*/
		break;
		case DAC_1: /** DAC1 B is selected*/
			SIM->SCGC2 |= SIM_SCGC2_DAC1_MASK; /** Bit 13 of SIM_SCGC2 is set*/
		break;
		default: /**If doesn't exist the option*/
			return;
		break;
	}
	return;
}

void DAC_enable(DAC_name_t DAC_name){
	switch(DAC_name)/** Selecting the DACn to enable*/
	{
		case DAC_0: /** DAC0 A is selected*/
			DAC0->C0 |= DAC0_REF2_mask; /** Bit 12 of SIM_SCGC2 is  set*/
		break;
		case DAC_1: /** DAC1 B is selected*/
			DAC1->C0 |= DAC0_REF2_mask; /** Bit 13 of SIM_SCGC2 is set*/
		break;
		default: /**If doesn't exist the option*/
			return;
		break;
	}
	return;
}
void DAC_set_value(uint16_t value){
	uint8_t low_word = (uint8_t)value;
	uint8_t high_word = (uint8_t)(value >> 8);
	if(0xFFF >= value)
	{
		DAC0->DAT[bit_0].DATL = low_word;/**Set low register to the biggest value possible*/
		DAC0->DAT[bit_0].DATH = high_word;/**Set high register to the biggest value possible*/
	}
	else
	{
		DAC0->DAT[bit_0].DATL = DAC_DATL_DATA0_MASK;/**Set low register to the first 8 bits of value*/
		DAC0->DAT[bit_0].DATH = DAC_DATL_DATA0_MASK;/**Set high register to the last 4 bits of value*/
	}
	return;
}
