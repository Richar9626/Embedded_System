/*
 * password.h
 *
 *  Created on: Mar 13, 2021
 *      Author: rjimenez
 */

#ifndef PASSWORD_H_
#define PASSWORD_H_

#define PASSWORD_LENGTH 4
#define PERIOD_1_sec (1) //set delay
#define TWO_BLINKS 4 //set delay

/*structures for structure and enum machine state*/
typedef enum {
	START,
	ONE_GOOD,
	TWO_GOOD,
	THREE_GOOD,
	ERROR,
	SUCCESS,
}State_name_t;

typedef enum {
	INCORRECT,
	CORRECT,
}Password_result;

/*due to password functionality, there is no output on each state, just jumps to the next one*/
typedef struct
{
	uint8_t next[5];
}State_t;

/*function that returns result*/
uint8_t check_password(uint8_t * password);

/*blink of led INICIO when password is correct*/
void correct_blink(void);

/*blink of led ERROR when password incorrect*/
void incorrect_blink(void);

/*delay of blinking, 1s period*/
void blink_delay(void);

#endif /* PASSWORD_H_ */
