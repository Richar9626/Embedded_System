/*
 * motor_control.c
 *
 *  Created on: Mar 13, 2021
 *      Author: rjimenez
 */

#include "MK64F12.h"
#include "GPIO.h"
#include "bits.h"
#include "motor_control.h"
#include <stdio.h>
#include "PIT.h"

/* Sequence 3 is equal to the start state, motor not moving  */
Motor_state_name_t g_current_state = SEQUENCE3;

Motor_state_name_t g_motor_state = SEQUENCE3;

/*Flags used to change of delay on sequences*/
uint8_t g_timer_sequence = INITIAL_SEQUENCE1;
uint8_t g_different_period_flag = FALSE;


/* Initialization of motor, just enabling pins to be used*/
void motor_init()
{
	gpio_pin_control_register_t input_intr_config = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_FALLING_EDGE;
	gpio_pin_control_register_t pcr_gpioc_pin = GPIO_MUX1;


	/**Pin control configuration of GPIOC for motor and for LEDs*/
	GPIO_pin_control_register(GPIO_C, bit_5, &pcr_gpioc_pin);
	GPIO_pin_control_register(GPIO_C, bit_7, &pcr_gpioc_pin);
	GPIO_pin_control_register(GPIO_C, bit_3, &pcr_gpioc_pin);

	/**Pin control configuration SW as a GPIO with pull-up resistor and interrupt in falling edge*/
	GPIO_pin_control_register(GPIO_C, bit_6, &input_intr_config);


	/**Assigns a safe value to the output pins of the GPIOC*/
	GPIO_clear_pin(GPIO_C, bit_3);
	GPIO_clear_pin(GPIO_C, bit_5);
	GPIO_clear_pin(GPIO_C, bit_7);

	/**Configures GPIOC pins for motor and LEDs as output*/
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT, bit_5);  /* Led1 motor*/
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT,bit_7);   /* Led2 motor*/
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT,bit_3);  /* motor*/

	//setup of SW2 as input
	GPIO_data_direction_pin(GPIO_C,GPIO_INPUT, bit_6);
}

void motor_enable()
{
	/* change state every time it enters functions
	 * entering functions only when pressing SW2*/
	g_current_state ++;

	/*condition to reset states to start = sequence3*/
	if(SEQUENCE2+1 == g_current_state)
	{
		g_current_state = SEQUENCE3;
	}

	/*On each sequence we enable corresponding leds and motor*/
	switch (g_current_state)
	{
		case SEQUENCE3:

			GPIO_clear_pin(GPIO_C, bit_3);
			GPIO_clear_pin(GPIO_C, bit_5);
			GPIO_clear_pin(GPIO_C, bit_7);
			/*MOTOR OFF*/
			#ifndef DEBUG_ON
				printf("Motor OFF \n");
			#endif
			PIT_stop(PIT_1);
		break;

		case SEQUENCE1:

			GPIO_set_pin(GPIO_C, bit_5);
			GPIO_clear_pin(GPIO_C, bit_7);

			/*SEQUENCE 1*/
			#ifndef DEBUG_ON
				printf("Sequence 1 \n");
			#endif
			/*we change the delay, no pit stop needed here since previous sequence has it*/
			GPIO_set_pin(GPIO_C, bit_3);
			PIT_delay(PIT_1, SYSTEM_CLOCK, DELAY_1_sec);
			PIT_callback_init(PIT_1, motor_sequence_one_press);
		break;
		case SEQUENCE2:

			GPIO_clear_pin(GPIO_C, bit_5);
			GPIO_set_pin(GPIO_C, bit_7);
			/*SEQUENCE 2*/
			#ifndef DEBUG_ON
				printf("Sequence 2 \n");
			#endif

			PIT_stop(PIT_1);
			GPIO_set_pin(GPIO_C, bit_3); /*starts the ON/OFF with ON state*/
			PIT_delay(PIT_1, SYSTEM_CLOCK, DELAY_4_sec);
			PIT_callback_init(PIT_1, motor_sequence_two_press);
		break;
		default:
			break;
	}

	GPIO_clear_irq_status(GPIO_C);

}

/* Fuction to disable motor by callback*/
void motor_disable(void)
{
	GPIO_clear_pin(GPIO_C, bit_3);
	GPIO_clear_pin(GPIO_C, bit_5);
	GPIO_clear_pin(GPIO_C, bit_7);
	PIT_stop(PIT_1);

}

/* Returns the state of machine state*/
uint8_t get_state(void)
{
	return(g_current_state);
}

/* To change current state from another function, used to ensure start on sequence3 = start*/
void set_state(uint8_t new_state)
{
	g_current_state = new_state;
}

/* This function makes the sequence1 of the motor
 * Consists on change the delay from 1s to 3s
 * sequence is: 1s ON, 1s OFF, 3s ON, 1s OFF -> repeat*/
void motor_sequence_one_press(void)
{
	/*We check if we need to change delay*/
	if(g_different_period_flag)
	{
		/*Changing state of motor*/
		GPIO_toogle_pin(GPIO_C, bit_3);
		/*we change delay to 1 second and timer sequence is set to -1
		 * set to this value since sequence starts being different than the rest
		 * with this value we ensure 1s OFF, 1s ON, 1s OFF*/
		g_timer_sequence = REST_OF_SEQUENCE1;
		PIT_stop(PIT_1);
		PIT_delay(PIT_1, SYSTEM_CLOCK, DELAY_1_sec);
		g_different_period_flag = FALSE; /* we reset flag*/
	}
	else
	{
		/*this timer is to determine which delay is next*/
		g_timer_sequence ++;

		/*Changing state of motor*/
		GPIO_toogle_pin(GPIO_C, bit_3);
		/*change of period value is 2, this to ensure the delay from 1s is changed to 3s on time*/
		if(CHANGE_OF_PERIOD == g_timer_sequence)  /* we enter here when its time to make a 3seconds delay*/
		{
			PIT_stop(PIT_1); /* we stop pit to avoid timer to change when previous countdown ends*/
			PIT_delay(PIT_1, SYSTEM_CLOCK, DELAY_3_sec);
			g_different_period_flag = TRUE;
		}
	}
}

/* In this sequence we change state every 4 seconds*/
void motor_sequence_two_press(void)
{
	/*we change timer sequence and different period flag for next state to start as it should*/
	g_timer_sequence = INITIAL_SEQUENCE1;
	g_different_period_flag = FALSE;
	/*change of state for motor*/
	GPIO_toogle_pin(GPIO_C, bit_3);
}


