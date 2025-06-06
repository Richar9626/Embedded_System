/*
 * PIT.c
 *
 *  Created on: Feb 27, 2021
 *      Author: rjimenez
 */


#include "stdint.h"
#include "MK64F12.h"
#include "GPIO.h"
#include "PIT.h"
#include "Bits.h"

//we define out internal flags
uint8_t g_flag_PIT_0 = FALSE;
uint8_t g_flag_PIT_1 = FALSE;
uint8_t g_flag_PIT_2 = FALSE;
uint8_t g_flag_PIT_3 = FALSE;

//definition of this variable to avoid bug
uint32_t dummyRead = FALSE;

//ISR definition
void PIT0_IRQHandler(void)
{
	//set of our internal flag for the interrupt
	g_flag_PIT_0 = TRUE;
	PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK; //clears TFLG
	dummyRead = PIT->CHANNEL[0].TCTRL;     //read control register to clean PIT flag, silicon bug
}

void PIT1_IRQHandler(void)
{
	//set of our internal flag for the interrupt
	g_flag_PIT_1 = TRUE;
	PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK; //clears TFLG
	dummyRead = PIT->CHANNEL[1].TCTRL;     //read control register to clean PIT flag, silicon bug
}

void PIT2_IRQHandler(void)
{
	//set of our internal flag for the interrupt
	g_flag_PIT_2 = TRUE;
	PIT->CHANNEL[2].TFLG |= PIT_TFLG_TIF_MASK; //clears TFLG
	dummyRead = PIT->CHANNEL[2].TCTRL;     //read control register to clean PIT flag, silicon bug
}

void PIT3_IRQHandler(void)
{
	//set of our internal flag for the interrupt
	g_flag_PIT_3 = TRUE;
	PIT->CHANNEL[3].TFLG |= PIT_TFLG_TIF_MASK; //clears TFLG
	dummyRead = PIT->CHANNEL[3].TCTRL;     //read control register to clean PIT flag, silicon bug
}

void PIT_delay(PIT_timer_t pit_timer, My_float_pit_t system_clock , My_float_pit_t delay)
{
	PIT->CHANNEL[pit_timer].LDVAL |= (uint32_t)(delay*(system_clock/2))-1; //from reference manual we have trigger=(period/clock_period)-1
	PIT->CHANNEL[pit_timer].TCTRL |= PIT_TCTRL_TEN_MASK; //Macro PIT_TCTRL_TEN_MASK = 0x1U, desired bit
	return;
}

void PIT_clock_gating(void)
{
	SIM->SCGC6 |= PIT_CLOCK_GATING; //enabling 23rd bit for register as specified in manual
	return;

}

uint8_t PIT_get_interrupt_flag_status(PIT_timer_t pit)
{
	switch(pit)
	{
		case PIT_0:
			return (g_flag_PIT_0); //return  flag status FALSE/TRUE
		case PIT_1:
			return (g_flag_PIT_1); //return  flag status FALSE/TRUE
		case PIT_2:
			return (g_flag_PIT_2); //return  flag status FALSE/TRUE
		case PIT_3:
			return (g_flag_PIT_3); //return  flag status FALSE/TRUE
		default:
			return(2); /**If doesn't exist the option turns an error code 2*/
	}
}

void PIT_clear_interrupt_flag(PIT_timer_t pit)
{
	switch(pit)
	{
		case PIT_0:
			g_flag_PIT_0 = FALSE; // flag variable set FALSE
			return;

		case PIT_1:
			g_flag_PIT_1 = FALSE; // flag variable set FALSE
			return;

		case PIT_2:
			g_flag_PIT_2 = FALSE; // flag variable set FALSE
			return;

		case PIT_3:
			g_flag_PIT_3 = FALSE; // flag variable set FALSE
			return;
		default:
			return;
	}
}

void PIT_enable(void)
{
	PIT->MCR &= ~(PIT_MCR_MDIS_MASK); //Macro shows the desired bit, but it is set with a 0
	return;
}

void PIT_enable_interrupt(PIT_timer_t pit)
{
	PIT->CHANNEL[pit].TCTRL |= PIT_TCTRL_TIE_MASK;//Macro value says the position of desire bit, set on 1
	return;
}

void PIT_stop(PIT_timer_t pit_timer)
{
	PIT->CHANNEL[pit_timer].TCTRL &= ~(PIT_TCTRL_TEN_MASK); //Macro PIT_TCTRL_TEN_MASK = 0x1U, desired bit
}
