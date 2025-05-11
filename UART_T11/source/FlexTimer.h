/**
	\file
	\brief
		This is the header file for the FlexTimer divice driver.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	21/03/2019
	\todo
	    Add configuration structures.
 */

#ifndef FLEXTIMER_H_
#define FLEXTIMER_H_


#include "MK64F12.h"

#define FLEX_TIMER_CLKS_0  (0U)
#define FLEX_TIMER_CLKS_1  (1U)
#define FLEX_TIMER_CLKS_2  (2U)
#define FLEX_TIMER_CLKS_3  (3U)


#define FLEX_TIMER_PS_1    (0U)
#define FLEX_TIMER_PS_2    (1U)
#define FLEX_TIMER_PS_4    (2U)
#define FLEX_TIMER_PS_8    (3U)
#define FLEX_TIMER_PS_16    (4U)
#define FLEX_TIMER_PS_32    (5U)
#define FLEX_TIMER_PS_64    (6U)
#define FLEX_TIMER_PS_128    (7U)


#define  FLEX_TIMER_DMA   0x01
#define  FLEX_TIMER_ELSA  0x04
#define  FLEX_TIMER_ELSB  0x08
#define  FLEX_TIMER_MSA   0x10
#define  FLEX_TIMER_MSB   0x20
#define  FLEX_TIMER_CHIE  0x40
#define  FLEX_TIMER_CHF   0x80

/*Type for FTM modules*/
typedef enum {FTM_0,
			  FTM_1,
			  FTM_2
			  } ftm_module_t;

/*Type for FTM channels*/
typedef enum {CH_0,
			  CH_1,
			  CH_2,
			  CH_3,
			  CH_4,
			  CH_5,
			  CH_6,
			  CH_7,
			  CH_8
			  } ftm_channel_t;

/*Type that is used for SPI configuration, It contains all the information needed for a SPI module*/
typedef struct
{
	ftm_module_t ftm_module;
	uint8_t ftm_mode_config;
	uint16_t ftm_modulo_value;
	ftm_channel_t ftm_channel;
	uint8_t ftm_CnSC_config;
	uint16_t ftm_CnV_value;
	uint8_t ftm_SC_clk;
	uint8_t ftm_SC_ps;
} ftm_config_t;

/**
 *@brief changes the intensity of the ex_RBG_led in PORT_C bit_1 bit_2 bit_3
 *@param number from 0  to 1024U
 * */
void Blue_FlexTimer_update_channel_value(float channel_value);
void Red_FlexTimer_update_channel_value(float channel_value);
void Green_FlexTimer_update_channel_value(float channel_value);

/**
 *@brief FTM configuration
 * */
void FTM_clk(ftm_module_t module);
void FTM_mode(ftm_module_t module, uint8_t value);
void FTM_modulo(ftm_module_t module, uint16_t value);
void FTM_CnSC(ftm_module_t module, ftm_channel_t channel, uint8_t value);
void FTM_CnV(ftm_module_t module, ftm_channel_t channel, uint16_t value);
void FTM_SC(ftm_module_t module, uint8_t clk_value, uint8_t ps_value);
void FTM_init(const ftm_config_t* config_struct);

float get_Frequency(void);
void FlexTimer_Frequency_Init(void);

#endif /* FLEXTIMER_H_ */
