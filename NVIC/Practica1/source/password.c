/*
 * password.c
 *
 *  Created on: Mar 13, 2021
 *      Author: rjimenez
 */

#include "MK64F12.h"
#include "GPIO.h"
#include "bits.h"
#include "password.h"
#include "Keypad4x4.h"
#include <stdio.h>
#include "PIT.h"

/*state machine, has only two states on each one*/
const State_t FSM_Moore[5]=
{
	{{ERROR, ONE_GOOD}},/* start */
	{{ERROR, TWO_GOOD}},/* one_good */
	{{ERROR, THREE_GOOD}},/* two_good */
	{{ERROR, SUCCESS}},/* three-good */
	{{ERROR, START}}/* error */

};

uint8_t g_period_completed = FALSE;

uint8_t g_blink = 0;

uint8_t check_password(uint8_t * password)
{
	uint8_t digit = 0;
	uint8_t key_pressed = 0;
	State_name_t current_state = START;
	/*CHECKING PASSWORD*/
	#ifndef DEBUG_ON
		printf("Checking password \n");
	#endif
	/*we use a while to return result after 4 bits read*/
	while(digit < PASSWORD_LENGTH)
	{
		if(TRUE == GPIO_get_irq_status(GPIO_B))
		{
			key_pressed = get_last_key();

			/*DIGIT TRIED*/
			#ifndef DEBUG_ON
				printf("Key pressed: %c\n", key_pressed);
			#endif
			if(ERROR == current_state)
			{
				/*the only way to get out of error state is after entering 4 bits*/
				current_state = FSM_Moore[current_state].next[digit == PASSWORD_LENGTH];
			}
			else
			{
				/*if one digit is incorrect it will go to error state*/
				current_state = FSM_Moore[current_state].next[key_pressed == password[digit]];
			}

			digit ++;
			GPIO_clear_irq_status(GPIO_B);

		}
	}
	if(SUCCESS == current_state)
	{
		/*CORRECT PASSWORD*/
		#ifndef DEBUG_ON
			printf("Correct password \n");
		#endif
		return(CORRECT);
	}
	else
	{
		/*INCORRECT PASSWORD*/
		#ifndef DEBUG_ON
			printf("incorrect password \n");
		#endif
		return(INCORRECT);
	}
}

/*blink for led INICIO*/
void correct_blink(void)
{

	for(g_blink = 0; g_blink < TWO_BLINKS; g_blink++)
	{
		PIT_delay(PIT_3, SYSTEM_CLOCK, PERIOD_1_sec);
		//GPIO_set_pin(GPIO_C, bit_1);
		while(FALSE == g_period_completed)
		{}
		GPIO_toogle_pin(GPIO_C, bit_1);
		g_period_completed = FALSE;
	}

	GPIO_set_pin(GPIO_C, bit_1);
	PIT_stop(PIT_3);
	PIT_clear_interrupt_flag(PIT_3);

}

/*blink for led ERROR*/
void incorrect_blink(void)
{
	for(g_blink = 0; g_blink < TWO_BLINKS; g_blink++)
	{
		PIT_delay(PIT_3, SYSTEM_CLOCK, PERIOD_1_sec);
		while(FALSE == g_period_completed)
		{}
		GPIO_toogle_pin(GPIO_B, bit_19);
		g_period_completed = FALSE;
	}

	GPIO_clear_pin(GPIO_B, bit_19);
	PIT_stop(PIT_3);
	PIT_clear_interrupt_flag(PIT_3);
}

/*Function owned by callback*/
void blink_delay(void)
{
	g_period_completed = TRUE;
	PIT_stop(PIT_3);
	PIT_clear_interrupt_flag(PIT_3);

}
