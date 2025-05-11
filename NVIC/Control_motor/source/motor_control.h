/*
 * motor_control.h
 *
 *  Created on: Mar 13, 2021
 *      Author: rjimenez
 */

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#define SYSTEM_CLOCK (21000000U)
#define DELAY_4_sec (8) //set delay
#define DELAY_1_sec (2) //set delay
#define DELAY_3_sec (6) //set delay

#define REST_OF_SEQUENCE1 -1 //set delay
#define INITIAL_SEQUENCE1 0 /* on sequence1, starts different than rest of sequence*/
#define CHANGE_OF_PERIOD 2 //set delay



typedef enum {
	SEQUENCE3,
	SEQUENCE1,
	SEQUENCE2,
}Motor_state_name_t;

void motor_init(void);

void motor_enable(void);

void motor_sequence_two_press(void);
void motor_sequence_one_press(void);

uint8_t get_state(void);



#endif /* MOTOR_CONTROL_H_ */
