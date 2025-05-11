/**
	\file
	\brief
		This is the starter file of FlexTimer.
		In this file the FlexTimer is configured in overflow mode.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	21/03/2019
	\todo
	    Add configuration structures.
 */



#include "FlexTimer.h"
#include "MK64F12.h"



void FlexTimer_update_channel_value(float channel_value)
{
	/**Assigns a new value for the frequency*/
	FTM0->MOD = (uint16_t) channel_value;
	FTM0->CONTROLS[0].CnV = (uint16_t) channel_value*0.8;
}


void FlexTimer_Init(void)
{


	/**Clock gating for FlexTimer*/
	SIM->SCGC6 |= SIM_SCGC6_FTM0(1);
	/**When write protection is enabled (WPDIS = 0), write protected bits cannot be written.
	* When write protection is disabled (WPDIS = 1), write protected bits can be written.*/
	FTM0->MODE |= FTM_MODE_WPDIS_MASK;
	/**Enables the writing over all registers*/
	FTM0->MODE &= ~ FTM_MODE_FTMEN_MASK;
	/**Assigning an specific value for modulo register to get 1MHz of frequency*/
	FTM0->MOD = 20;
	/**Selects the Edge-Aligned PWM mode mode*/
	FTM0->CONTROLS[0].CnSC = FTM_CnSC_MSA(0) |FTM_CnSC_MSB(1) | FTM_CnSC_ELSB(1)|FTM_CnSC_ELSA(0);
	/**Assign a duty cycle of 80%*/
	FTM0->CONTROLS[0].CnV = FTM0->MOD*0.8;
	/**Configure the times*/
	FTM0->SC |= FTM_SC_CLKS(FLEX_TIMER_CLKS_1)| FTM_SC_PS(FLEX_TIMER_PS_1);
}

