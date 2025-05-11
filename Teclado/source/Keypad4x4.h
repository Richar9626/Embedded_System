/*
 * Keyboard4x4.h
 *
 *  Created on: Mar 8, 2021
 *      Author: rjimenez
 */

#ifndef KEYPAD4X4_H_
#define KEYPAD4X4_H_

#include "stdint.h"

#define columns_number 4
#define rows_number 4

void keypad_init(void);

uint8_t read_key(void);

#endif /* KEYPAD4X4_H_ */
