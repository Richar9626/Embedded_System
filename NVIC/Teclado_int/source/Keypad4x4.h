/*
 * Keyboard4x4.h
 *
 *  Created on: Mar 8, 2021
 *      Author: rjimenez
 */

#ifndef KEYPAD4X4_H_
#define KEYPAD4X4_H_

#include "stdint.h"
#define SYSTEM_CLOCK (21000000U)
#define DELAY_KEY (0.1F) //set delay
#define columns_number 4
#define rows_number 4

void keypad_init(void);

void read_key(void);

uint8_t get_last_key(void);

void get_last_key_delay(void);

#endif /* KEYPAD4X4_H_ */
