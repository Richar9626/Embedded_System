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
#include <stdio.h>


//definition of this variable to avoid bug
uint32_t dummyRead = FALSE;

static void (*pit_0_callback)(void) = 0;
static void (*pit_1_callback)(void) = 0;
static void (*pit_2_callback)(void) = 0;
static void (*pit_3_callback)(void) = 0;

static pit_interrupt_flags_t g_PIT_intr_status_flag = {0};

void PIT_callback_init(PIT_timer_t pit_timer,void (*handler)(void))
{
	switch(pit_timer)/** Selecting the GPIO for clock enabling*/
	{
		case PIT_0: /** GPIO A is selected*/
			pit_0_callback = handler;
			break;
		case PIT_1: /** GPIO B is selected*/
			pit_1_callback = handler;
			break;
		case PIT_2: /** GPIO C is selected*/
			pit_2_callback = handler;
			break;
		default: /** GPIO E is selected*/
			pit_3_callback = handler;
			break;
	}
}

//ISR definition
void PIT0_IRQHandler(void)
{
	g_PIT_intr_status_flag.flag_pit_0 = TRUE;
	if(pit_0_callback)
	{
		pit_0_callback();
	}

	PIT->CHANNEL[PIT_0].TFLG |= PIT_TFLG_TIF_MASK; //clears TFLG
	dummyRead = PIT->CHANNEL[PIT_0].TCTRL;     //read control register to clean PIT flag, silicon bug
}

void PIT1_IRQHandler(void)
{

	g_PIT_intr_status_flag.flag_pit_1 = TRUE;
	if(pit_1_callback)
	{
		pit_1_callback();
	}

	PIT->CHANNEL[PIT_1].TFLG |= PIT_TFLG_TIF_MASK; //clears TFLG
	dummyRead = PIT->CHANNEL[PIT_1].TCTRL;     //read control register to clean PIT flag, silicon bug
}

void PIT2_IRQHandler(void)
{
	g_PIT_intr_status_flag.flag_pit_2 = TRUE;
	if(pit_2_callback)
	{
		pit_2_callback();
	}

	PIT->CHANNEL[PIT_2].TFLG |= PIT_TFLG_TIF_MASK; //clears TFLG
	dummyRead = PIT->CHANNEL[PIT_2].TCTRL;     //read control register to clean PIT flag, silicon bug
}

void PIT3_IRQHandler(void)
{
	g_PIT_intr_status_flag.flag_pit_3 = TRUE;
	if(pit_3_callback)
	{
		pit_3_callback();
	}

	PIT->CHANNEL[PIT_3].TFLG |= PIT_TFLG_TIF_MASK; //clears TFLG
	dummyRead = PIT->CHANNEL[PIT_3].TCTRL;     //read control register to clean PIT flag, silicon bug
}

void PIT_delay(PIT_timer_t pit_timer, My_float_pit_t system_clock , My_float_pit_t delay)
{
	PIT->CHANNEL[pit_timer].LDVAL = (uint32_t)(delay*(system_clock/2))-1; //from reference manual we have trigger=(period/clock_period)-1
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
			return (g_PIT_intr_status_flag.flag_pit_0); //return  flag status FALSE/TRUE
		case PIT_1:
			return (g_PIT_intr_status_flag.flag_pit_1); //return  flag status FALSE/TRUE
		case PIT_2:
			return (g_PIT_intr_status_flag.flag_pit_2); //return  flag status FALSE/TRUE
		case PIT_3:
			return (g_PIT_intr_status_flag.flag_pit_3); //return  flag status FALSE/TRUE
		default:
			return(2); /**If doesn't exist the option turns an error code 2*/
	}
}

void PIT_clear_interrupt_flag(PIT_timer_t pit)
{
	switch(pit)
	{
		case PIT_0:
			g_PIT_intr_status_flag.flag_pit_0 = FALSE; // flag variable set FALSE
			return;

		case PIT_1:
			g_PIT_intr_status_flag.flag_pit_1 = FALSE; // flag variable set FALSE
			return;

		case PIT_2:
			g_PIT_intr_status_flag.flag_pit_2 = FALSE; // flag variable set FALSE
			return;

		case PIT_3:
			g_PIT_intr_status_flag.flag_pit_3 = FALSE; // flag variable set FALSE
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
