/*
 * RGB_sequence.h
 *
 *  Created on: Apr 14, 2021
 *      Author: Fernando
 */

#ifndef RGB_SEQUENCE_H_
#define RGB_SEQUENCE_H_

#include "stdint.h"

#define RGB_sequence_max (10U)

typedef enum {OFF_led, blue, red, green, purple, white} RGB_sequence_color_t;
typedef enum{Execute_OFF, Execute} RGB_sequence_execute_state_t;

void RGB_sequence_add(uint8_t n_letter);
void RGB_sequence_execute(void);
void RGB_sequence_write_color(void);
void RGB_sequence_reset(void);
void RGB_sequence_state(uint8_t state);
void RGB_sequence_delay(void);

#endif /* RGB_SEQUENCE_H_ */
