/*
 * SDK_DAC.h
 *
 *  Created on: 15 may. 2021
 *      Author: Daniel Ruvalcaba and Miguel Ortiz
 */

#ifndef SDK_DAC_H_
#define SDK_DAC_H_

#include "stdint.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_dac.h"

#define DEMO_DAC_BASEADDR           DAC0

void DAC_init(void);


#endif /* SDK_DAC_H_ */
