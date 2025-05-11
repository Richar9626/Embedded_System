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

/*Case state machine structure*/

typedef enum {
	SEQUENCE3,
	SEQUENCE1,
	SEQUENCE2,
}Motor_state_name_t;

/*Initialization of pins to leds and motor*/
void motor_init(void);

/*function to change sequences and enabling leds*/
void motor_enable(void);

/* used for disabling motor on main*/
void motor_disable(void);

/*used for sequence 1, change of pit delays and motor toogle*/
void motor_sequence_two_press(void);

/*Toogle motor each 4 seconds*/
void motor_sequence_one_press(void);

uint8_t get_state(void);

void set_state(uint8_t);



#endif /* MOTOR_CONTROL_H_ */
