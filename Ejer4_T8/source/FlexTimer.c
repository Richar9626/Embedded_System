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

void PWMOutput_CenterAlignment(void)
{
	SIM->SCGC6 |= SIM_SCGC6_FTM0(11);//enable FTM0 and FTM1 module clock
	SIM->SCGC5 |= 0x3E00;//enable port A/B/C/D/E clock
	FTM0->CONF = FTM_CONF_BDMMODE(11);//set up BDM in 11
	FTM0->FMS = 0x00;//clear the WPEN so that WPDIS is set in FTM0_MODE reg
	FTM0->MODE |= 0x05;//enable write the FTM CnV register
	FTM0->MOD = 1000;
	FTM0->CONTROLS[0].CnSC = FTM_CnSC_MSB(1) | FTM_CnSC_ELSB(1);////center-alignment, PWM begins with High
	FTM0->CONTROLS[1].CnSC= FTM_CnSC_MSB(1) | FTM_CnSC_ELSB(1);//PWM waveform is high-low-high
	FTM0->COMBINE = 0x02;//complementary mode for CH0&CH1 of FTM0
	FTM0->COMBINE |= 0x10;// dead timer insertion enabled in complementary mode for //CH0&CH1 of FTM0
	FTM0->DEADTIME = 0x1F;//dead time is 16 system clock cycles
	FTM0->CONTROLS[1].CnV = 500;
	FTM0->CONTROLS[0].CnV = 500;
	FTM0->CNTIN = 0x00;

	FTM0->CONTROLS[2].CnSC= FTM_CnSC_MSB(1) | FTM_CnSC_ELSB(1);
	FTM0->CONTROLS[3].CnSC= FTM_CnSC_MSB(1) | FTM_CnSC_ELSB(1);
	FTM0->COMBINE |= 0x0200;
	FTM0->COMBINE |= 0x1000;
	FTM0->CONTROLS[3].CnV=250;
	FTM0->CONTROLS[2].CnV=250;
	FTM0->SC=0x68;
}

