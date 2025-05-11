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

Motor_state_name_t current_state = SEQUENCE3;

uint8_t pit_inter_status = FALSE;

Motor_state_name_t motor_state = SEQUENCE3;

uint8_t timer_sequence = INITIAL_SEQUENCE1;
uint8_t different_period_flag = FALSE;


void motor_init()
{
	gpio_pin_control_register_t input_intr_config = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_FALLING_EDGE;
	gpio_pin_control_register_t pcr_gpioc_pin = GPIO_MUX1;

	/**Activating the GPIOA, GPIOB and GPIOC clock gating*/
	GPIO_clock_gating(GPIO_A);
	GPIO_clock_gating(GPIO_B);
	GPIO_clock_gating(GPIO_C);


	/**Pin control configuration of GPIOB pin21 and pin21 as GPIO by using a special macro contained in Kinetis studio in the MK64F12. h file*/
	GPIO_pin_control_register(GPIO_C, bit_5, &pcr_gpioc_pin);
	GPIO_pin_control_register(GPIO_C, bit_7, &pcr_gpioc_pin);
	GPIO_pin_control_register(GPIO_C, bit_3, &pcr_gpioc_pin);

	/**Pin control configuration of GPIOA pin4 as a GPIO with pull-up resistor and interrupt in falling edge*/
	GPIO_pin_control_register(GPIO_C, bit_6, &input_intr_config);


	/**Assigns a safe value to the output pin21 of the GPIOB*/
	GPIO_clear_pin(GPIO_C, bit_3);
	GPIO_clear_pin(GPIO_C, bit_5);
	GPIO_clear_pin(GPIO_C, bit_7);

	/**Configures GPIOB pin21, pin22 and GPIOE pin26 as output*/
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT, bit_5);  //blue
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT,bit_7);  //red
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT,bit_3); //green

	//setup of SW3 and SW2 as input
	GPIO_data_direction_pin(GPIO_C,GPIO_INPUT, bit_6);
}

void motor_enable()
{
	current_state ++;
	if(SEQUENCE2+1 == current_state)
	{
		current_state = SEQUENCE3;
	}

	switch (current_state)
	{
		case SEQUENCE3:

			GPIO_clear_pin(GPIO_C, bit_3);
			GPIO_clear_pin(GPIO_C, bit_5);
			GPIO_clear_pin(GPIO_C, bit_7);
			#ifndef DEBUG_ON
				printf("Motor OFF \n");
			#endif
			PIT_stop(PIT_1);
		break;

		case SEQUENCE1:

			GPIO_set_pin(GPIO_C, bit_5);
			GPIO_clear_pin(GPIO_C, bit_7);

			#ifndef DEBUG_ON
				printf("Sequence 1 \n");
			#endif

			GPIO_set_pin(GPIO_C, bit_3);
			PIT_delay(PIT_1, SYSTEM_CLOCK, DELAY_1_sec);
			PIT_callback_init(PIT_1, motor_sequence_one_press);
		break;
		case SEQUENCE2:

			GPIO_clear_pin(GPIO_C, bit_5);
			GPIO_set_pin(GPIO_C, bit_7);
			#ifndef DEBUG_ON
				printf("Sequence 2 \n");
			#endif

			GPIO_set_pin(GPIO_C, bit_3); /*starts the ON/OFF with ON state*/
			PIT_callback_init(PIT_1, motor_sequence_two_press);
			PIT_delay(PIT_1, SYSTEM_CLOCK, DELAY_4_sec);
		break;
		default:
			break;
	}

	GPIO_clear_irq_status(GPIO_C);

}

uint8_t get_state(void)
{
	return(current_state);
}

void motor_sequence_one_press(void)
{
	if(different_period_flag)
	{
		GPIO_toogle_pin(GPIO_C, bit_3);
		#ifndef DEBUG_ON
			printf("Toggle motor \n");
		#endif

		timer_sequence = REST_OF_SEQUENCE1;
		PIT_stop(PIT_1);
		PIT_delay(PIT_1, SYSTEM_CLOCK, DELAY_1_sec);
		different_period_flag = FALSE;
	}
	else
	{
		timer_sequence ++;

		GPIO_toogle_pin(GPIO_C, bit_3);
		#ifndef DEBUG_ON
			printf("Toggle motor \n");
		#endif

		if(CHANGE_OF_PERIOD == timer_sequence)  /* we enter here when its time to make a 3seconds delay*/
		{
			PIT_stop(PIT_1);
			PIT_delay(PIT_1, SYSTEM_CLOCK, DELAY_3_sec);
			different_period_flag = TRUE;
		}
	}
}

void motor_sequence_two_press(void)
{
	timer_sequence = INITIAL_SEQUENCE1;
	different_period_flag = FALSE;
	//set of our internal flag for the interrupt
	GPIO_toogle_pin(GPIO_C, bit_3);
	#ifndef DEBUG_ON
		printf("Toggle motor \n");
	#endif
}


