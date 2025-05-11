/*
 * button_handler.h
 *
 *  Created on: Apr 12, 2021
 *      Author: rjimenez
 */

#ifndef BUTTON_HANDLER_H_
#define BUTTON_HANDLER_H_

#include "stdint.h"
#include "bits.h"

typedef enum {B0, B1, B2, B3, B4, B5, B6, SW2, SW3} button_t;

void button_state(void);

uint8_t get_button_state(button_t button);
void set_button_state(button_t button, boolean_t state);

#endif /* BUTTON_HANDLER_H_ */
