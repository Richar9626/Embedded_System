/*
 * Function_gen.h
 *
 *  Created on: Mar 13, 2021
 *      Author: Fernando
 */

#ifndef WAVE_GEN_H_
#define WAVE_GEN_H_

#include "stdint.h"

#define SYSTEM_CLOCK (21000000U) /**clock for the PIT delay function*/
#define DELAY (0.004F) /**time needed to make all wave forms around 5Hz*/
#define table_limit 100 /**limit for the waves tables*/

/**enum for changing the wave form output*/
typedef enum {SQUARE,
			  SIN,
			  TRIANGULAR,
			 }wave_select_t;
/**
 * @brief handles the square waveform table of values
 * @param none
 * */
void square_wave(void);
/**
 * @brief handles the sin waveform table of values
 * @param none
 * */
void sine_wave(void);
/**
 * @brief handles the triangle waveform table of values
 * @param none
 * */
void triangle_wave(void);
/**
 * @brief controls what waveform sends out, changes LEDs and wave form depending on the sw press
 * @param none
 * */
void wave_mngr(void);
/**
 * @brief checks the flag if TRUE moves to wave_mngr, if FALSE stops PIT and sets leds OFF
 * @param none
 * */
void wave_gen_password(void);
/**
 * @brief changes the flag to TRUE/FALSE to make the module work depending on the password
 * @param TRUE/FALSE
 * */
void wave_gen_set_flag(uint8_t flag);
/**
 * @brief changes the flag to TRUE/FALSE to make the module work depending on the password
 * @param SQUARE/SIN/TRIANGULAR
 * */
void wave_gen_set_state(uint8_t state);

#endif /* WAVE_GEN_H_ */
