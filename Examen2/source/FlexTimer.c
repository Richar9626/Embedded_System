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

static uint32_t previous_value = 0;
static uint32_t actual_value= 0;
static float frequency= 0;
static const float FTMCK = 164062.5; /*This variable corresponds to the FTMK due to prescaler times 2 due to rising edge caption*/


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

/* get to avoid extern variables */
float get_Frequency(void)
{
	return (frequency);
}

void Blue_FlexTimer_update_channel_value(float channel_value)
{
	/**Assigns a new value for the duty cycle for Blue color*/
	FTM0->CONTROLS[0].CnV = channel_value;
}

void Red_FlexTimer_update_channel_value(float channel_value)
{
	/**Assigns a new value for the duty cycle for red color*/
	FTM0->CONTROLS[1].CnV = channel_value;
}

void Green_FlexTimer_update_channel_value(float channel_value)
{
	/**Assigns a new value for the duty cycle for green color*/
	FTM0->CONTROLS[2].CnV = channel_value;
}

/*It activates the clock gating */
void FTM_clk(ftm_module_t module)
{
	switch(module)/** Selecting the FTM for clock enabling*/
	{
		case FTM_0: /** FTM_0 is selected*/
			SIM->SCGC6 |= SIM_SCGC6_FTM0(1);
			break;
		case FTM_1: /** FTM_1  is selected*/
			SIM->SCGC6 |= SIM_SCGC6_FTM1(1);
			break;
		default: /** FTM_2  is selected*/
			SIM->SCGC6 |=SIM_SCGC6_FTM2(1);
			break;
	}
	return;
}

/*It assigns a value to the MODE register */
void FTM_mode(ftm_module_t module, uint8_t value)
{
	switch(module)/** Selecting the FTM*/
	{
		case FTM_0: /** FTM_0 is selected*/
			FTM0->MODE = value;
			break;
		case FTM_1: /** FTM_1  is selected*/
			FTM1->MODE = value;
			break;
		default: /** FTM_2  is selected*/
			FTM2->MODE = value;
			break;
	}
	return;
}


/*It assigns a value to the modulo register */
void FTM_modulo(ftm_module_t module, uint16_t value)
{
	switch(module)/** Selecting the FTM*/
	{
		case FTM_0: /** FTM_0 is selected*/
			FTM0->MOD = value;
			break;
		case FTM_1: /** FTM_1  is selected*/
			FTM1->MOD = value;
			break;
		default: /** FTM_2  is selected*/
			FTM2->MOD = value;
			break;
	}
	return;
}

/*It assigns a value to the CnSC register */
void FTM_CnSC(ftm_module_t module, ftm_channel_t channel, uint8_t value)
{
	switch(module)/** Selecting the FTM*/
	{
		case FTM_0: /** FTM_0 is selected*/
			FTM0->CONTROLS[channel].CnSC = value;
			break;
		case FTM_1: /** FTM_1  is selected*/
			FTM1->CONTROLS[channel].CnSC = value;
			break;
		default: /** FTM_2  is selected*/
			FTM2->CONTROLS[channel].CnSC = value;
			break;
	}
	return;
}

/*It assigns a value to the CV register */
void FTM_CnV(ftm_module_t module, ftm_channel_t channel, uint16_t value)
{
	switch(module)/** Selecting the FTM*/
	{
		case FTM_0: /** FTM_0 is selected*/
			FTM0->CONTROLS[channel].CnV = value;
			break;
		case FTM_1: /** FTM_1  is selected*/
			FTM1->CONTROLS[channel].CnV = value;
			break;
		default: /** FTM_2  is selected*/
			FTM2->CONTROLS[channel].CnV = value;
			break;
	}
	return;
}

/*It assigns a value to the modulo register */
void FTM_SC(ftm_module_t module, uint8_t clk_value, uint8_t ps_value)
{
	switch(module)/** Selecting the FTM*/
	{
		case FTM_0: /** FTM_0 is selected*/
			FTM0->SC |= FTM_SC_CLKS(clk_value)| FTM_SC_PS(ps_value);
			break;
		case FTM_1: /** FTM_1  is selected*/
			FTM1->SC |= FTM_SC_CLKS(clk_value)| FTM_SC_PS(ps_value);
			break;
		default: /** FTM_2  is selected*/
			FTM2->SC |= FTM_SC_CLKS(clk_value)| FTM_SC_PS(ps_value);
			break;
	}
	return;
}

/*It configures the FTM, this function as arguments receives a pointer to a constant structure where are all
 * the configuration parameters*/
void FTM_init(const ftm_config_t* config_struct)
{
	FTM_clk(config_struct->ftm_module);
	FTM_mode(config_struct->ftm_module, config_struct->ftm_mode_config);
	FTM_modulo(config_struct->ftm_module, config_struct->ftm_modulo_value);
	FTM_CnSC(config_struct->ftm_module, config_struct->ftm_channel, config_struct->ftm_CnSC_config);
	FTM_CnV(config_struct->ftm_module, config_struct->ftm_channel, config_struct->ftm_CnV_value);
	FTM_SC(config_struct->ftm_module, config_struct->ftm_SC_clk, config_struct->ftm_SC_ps);

	return;
}

