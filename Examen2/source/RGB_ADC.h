/*
 * RGB_ADC.h
 *
 *  Created on: Apr 14, 2021
 *      Author: Fernando
 */

#ifndef RGB_ADC_H_
#define RGB_ADC_H_

#include "stdint.h"

#define voltage_diference (.8056640625F)
#define DEMO_ADC16_BASE          ADC0
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_USER_CHANNEL  12U
#define ADC_max_value 4096U

void RGB_ADC_init(void);
uint32_t RGB_ADC_get(void);

#endif /* RGB_ADC_H_ */
