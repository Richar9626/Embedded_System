/*
 * RGB_ADC.c
 *
 *  Created on: Apr 14, 2021
 *      Author: Fernando
 */

#include "RGB_sequence.h"

#include "Bits.h"

#include "GPIO.h"
#include "FlexTimer.h"
#include "RGB_set.h"
#include "Delay.h"
#include "PIT.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_adc16.h"
#include "RGB_ADC.h"
#include "RGB_set.h"

const uint32_t g_Adc16_12bitFullRange = ADC_max_value;
static adc16_config_t g_adc16ConfigStruct;
static adc16_channel_config_t g_adc16ChannelConfigStruct;

void RGB_ADC_init(void)
{
	ADC16_GetDefaultConfig(&g_adc16ConfigStruct);
    ADC16_Init(DEMO_ADC16_BASE, &g_adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(DEMO_ADC16_BASE, false); /* Make sure the software trigger is used. */
    g_adc16ChannelConfigStruct.channelNumber                        = DEMO_ADC16_USER_CHANNEL;
    g_adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
    return;
}

uint32_t RGB_ADC_get(void)
{
	uint16_t value;
    ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &g_adc16ChannelConfigStruct);
    while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP)))
    {}

    value = voltage_diference * ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);

return value;
}
