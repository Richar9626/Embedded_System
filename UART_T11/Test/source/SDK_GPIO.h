/*
 * SDK_GPIO.h
 *
 *  Created on: 14 may. 2021
 *      Author: ruval
 */

#ifndef SDK_GPIO_H_
#define SDK_GPIO_H_

#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_LED_GPIO     BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_RED_GPIO_PIN

#define BOARD_SW_GPIO        BOARD_SW2_GPIO
#define BOARD_SW_PORT        BOARD_SW2_PORT
#define BOARD_SW_GPIO_PIN    BOARD_SW2_GPIO_PIN
#define BOARD_SW_IRQ         BOARD_SW2_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_SW2_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_SW2_NAME

void GPIO_Init(void);
void BOARD_SW_IRQ_HANDLER(void);
uint8_t GPIO_Signal_Select(void);

#endif /* SDK_GPIO_H_ */
