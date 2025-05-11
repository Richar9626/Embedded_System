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

uint32_t previous_value = 0;
uint32_t actual_value= 0;
float frequency= 0;
const float FTMCK = 164062.5; /*This variable corresponds to the FTMK due to prescaler times 2 due to rising edge caption*/

void FTM2_IRQHandler()
{

	if(FTM2->STATUS & 0x01)
	{
		previous_value = actual_value;
		actual_value = FTM2->CONTROLS[0].CnV; /*we read the value of the register, which has difference between rising edges*/

		/*We apply the calculation of the frequency */
		frequency =  1/((actual_value - previous_value)*(1/FTMCK));
		FTM2->STATUS &= 0xFE;
	}
	/**Clearing the overflow interrupt flag*/
	FTM2->SC &= ~ FTM_SC_TOF_MASK;
}


float get_Frequency(void)
{
	return (frequency);
}

void FlexTimer_Init(void)
{
	/**Clock gating for FlexTimer*/
	SIM->SCGC6 |= SIM_SCGC6_FTM2(1);

	/*We make sure to have the rising edge capture mode*/
	FTM2->COMBINE = 0x00;

	/**Configure FlexTimer in input capture in rising edge and  to generate an interrupt*/
	FTM2->CONTROLS[0].CnSC |=  FTM_CnSC_CHIE(1) | FTM_CnSC_ELSA(1);
	/**Select clock source and prescaler*/
	FTM2->SC = FTM_SC_CLKS (FLEX_TIMER_CLKS_1)| FTM_SC_PS(FLEX_TIMER_PS_128);
	/*Make sure to enable interrupts*/
	FTM2->SC |= FTM_SC_TOIE_MASK;
}
