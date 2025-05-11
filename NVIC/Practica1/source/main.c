/*
 * PRACTICA 1
 * SISTEMAS EMBEBIDOS
 *
 * BY:
 * RICARDO JIMENEZ SANCHEZ
 * FERNANDO CURIEL SOLIS
 */

/**
 * @file    Teclado.c
 * @brief   Application entry point.
 */

#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

#include "GPIO.h"
#include "Bits.h"
#include "stdint.h"
#include "Keypad4x4.h"
#include "NVIC.h"
#include "password.h"
#include "motor_control.h"
#include "PIT.h"
#include "Wave_gen.h"
#include "DAC.h"


typedef enum {
	OFF,
	ON,
}Process_state;

/*Passwords to be used*/
uint8_t master_key_password[4] = {'1','2','3','4'};
uint8_t motor_control_password[4] = {'4','5','6','7'};
uint8_t signal_generator_password[4] = {'7','8','9','0'};

int main(void) {

	/*Enabling clocks for GPIOS*/
	GPIO_clock_gating(GPIO_A);
	GPIO_clock_gating(GPIO_B);
	GPIO_clock_gating(GPIO_C);
	GPIO_clock_gating(GPIO_D);
	GPIO_clock_gating(GPIO_E);

	/*variables to setup pins as output*/
	gpio_pin_control_register_t g_pcr_gpio_switch = GPIO_MUX1 | GPIO_PS | GPIO_PE | INTR_FALLING_EDGE;
	gpio_pin_control_register_t g_pcr_gpio_led = GPIO_MUX1;

	/**Pin control configuration of GPIO for Led INICIO and ERROR*/
	GPIO_pin_control_register(GPIO_C,bit_1,&g_pcr_gpio_led);
	GPIO_pin_control_register(GPIO_B,bit_19,&g_pcr_gpio_led);
	GPIO_pin_control_register(GPIO_A, bit_4, &g_pcr_gpio_switch);
	GPIO_pin_control_register(GPIO_B,bit_21,&g_pcr_gpio_led);
	GPIO_pin_control_register(GPIO_B,bit_22,&g_pcr_gpio_led);
	GPIO_pin_control_register(GPIO_E,bit_26,&g_pcr_gpio_led);
	GPIO_pin_control_register(GPIO_C,bit_0,&g_pcr_gpio_led);
	GPIO_pin_control_register(GPIO_C,bit_9,&g_pcr_gpio_led);


	/**Assigns a safe value to the output pin*/
	GPIO_clear_pin(GPIO_C, bit_1);
	GPIO_clear_pin(GPIO_B, bit_19);
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_E, bit_26);
	GPIO_clear_pin(GPIO_C, bit_0);
	GPIO_clear_pin(GPIO_C, bit_9);


	/**Configures GPIO INICIO and ERROR as output*/
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT, bit_1);
	GPIO_data_direction_pin(GPIO_B,GPIO_OUTPUT, bit_19);

	/*SW3 as input*/
	GPIO_data_direction_pin(GPIO_A, GPIO_INPUT, bit_4);

	/*Outputs for RGB and signal LEds*/
	GPIO_data_direction_pin(GPIO_B,GPIO_OUTPUT, bit_21);
	GPIO_data_direction_pin(GPIO_B,GPIO_OUTPUT,bit_22);
	GPIO_data_direction_pin(GPIO_E,GPIO_OUTPUT,bit_26);
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT,bit_0);
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT,bit_9);

	/*assigning safe value to RGB LEd*/
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_E, bit_26);

	DAC_init(DAC_0);

	/*initialization of keypad*/
	keypad_init();

	/*initialization of motor*/
	motor_init();

	/*important to enable pit*/
	PIT_clock_gating();
	PIT_enable();

	/**Sets the threshold for interrupts, if the interrupt has higher priority constant that the BASEPRI, the interrupt will not be attended*/
	NVIC_set_basepri_threshold(PRIORITY_10);
	/**Enables and sets a particular interrupt and its priority*/
	NVIC_enable_interrupt_and_priotity(PORTA_IRQ,PRIORITY_5);
	NVIC_enable_interrupt_and_priotity(PORTC_IRQ,PRIORITY_5);
	NVIC_enable_interrupt_and_priotity(PORTB_IRQ,PRIORITY_4);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_8);
	NVIC_enable_interrupt_and_priotity(PIT_CH1_IRQ, PRIORITY_6);
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_6);
	NVIC_enable_interrupt_and_priotity(PIT_CH3_IRQ, PRIORITY_6);


	/*enable all pits*/
	PIT_enable_interrupt(PIT_0);
	PIT_enable_interrupt(PIT_1);
	PIT_enable_interrupt(PIT_2);
	PIT_enable_interrupt(PIT_3);

	/*Callbacks setup*/
	GPIO_callback_init(GPIO_B, read_key);
	GPIO_callback_init(GPIO_A, wave_gen_password);
	PIT_callback_init(PIT_0, get_last_key_delay);
	PIT_callback_init(PIT_3, blink_delay);

	NVIC_global_enable_interrupts;

	/* initialization of states and useful variables*/
	Password_result result = INCORRECT;
	Process_state motor_control = OFF;
	Process_state signal_generator = OFF;
	uint8_t key_pressed = 0;
	GPIO_clear_irq_status(GPIO_B);

	/*while to remain here when password fail*/
	do
	{
		result = check_password(master_key_password); /*we pass the password value*/
	}while(INCORRECT == result);

	/*password correct lights on the INICIO led*/
	GPIO_set_pin(GPIO_C, bit_1);

	/*CORRECT PASSWORD*/
	#ifndef DEBUG_ON
		printf("Correct password \n");
	#endif

	key_pressed = 0;

    while(1)
    {
    	/*We read the pressed key here and assigned to key_pressed*/
    	if(TRUE == GPIO_get_irq_status(GPIO_B))
		{

			key_pressed = get_last_key();
			/* KEY PRESSED */
			#ifndef DEBUG_ON
				printf("Key pressed: %c\n", key_pressed);
			#endif
			GPIO_clear_irq_status(GPIO_B);

		}

    	/*After main key password there are 2 valid buttons, A and B to enable functions*/
    	if('A' == key_pressed)
    	{
    		if(OFF == motor_control)
			{
				result = check_password(motor_control_password); /*This returns true or false*/
				if(CORRECT == result)
				{
					/*MOTOR ENABLED*/
					#ifndef DEBUG_ON
						printf("motor enabled\n");
					#endif
					motor_control = ON;
					GPIO_callback_init(GPIO_C, motor_enable); /*We assign GPIOC IRS to the motor_enable function*/
					correct_blink(); /*Blinking for led, it is on password.c*/

				}
				else
				{
					/*INCORRECT PASSWORD*/
					#ifndef DEBUG_ON
						printf("not correct\n");
					#endif
					incorrect_blink(); /*If wrong password is entered only a blink on ERROR led happens*/
				}
    		}
    		else
    		{
    			/*Enters here when A button is pressed for 2nd time, meaning you have to disable motor*/
				/*MOTOR DISABLED*/
				#ifndef DEBUG_ON
					printf("motor disabled\n");
				#endif
				motor_control = OFF;
				set_state(SEQUENCE3); /*Resets state machine for motor*/
				GPIO_clear_pin(GPIO_C, bit_3); /*Motor*/
				GPIO_clear_pin(GPIO_C, bit_5); /*led 1 motor*/
				GPIO_clear_pin(GPIO_C, bit_7); /*Led 2 motor*/
				GPIO_callback_init(GPIO_C, motor_disable); /*We assign another function to GPIOC, only to disable motor*/
				PIT_stop(PIT_1);
    		}

    	}

    	/*Same functionality as 'A' pressed*/
    	if('B' == key_pressed)
		{
    		if(OFF == signal_generator)
    		{
				result = check_password(signal_generator_password);
				if(CORRECT == result)
				{
					/*WAVE GEN enABLED*/
					#ifndef DEBUG_ON
						printf("Signal enabled\n");
					#endif
					signal_generator = ON;
					wave_gen_set_flag(TRUE); /*we enable the flag, callbakc already sent*/
					correct_blink(); /*Blinking for led, it is on password.c*/
				}
				else
				{
					/*INCORRECT PASSWORD*/
					#ifndef DEBUG_ON
						printf("not correct\n");
					#endif
					wave_gen_set_flag(FALSE);
					incorrect_blink(); /*If wrong password is entered only a blink on ERROR led happens*/
				}
    		}
    		else
    		{
				/*SIGNAL GEN DISABLED*/
				#ifndef DEBUG_ON
					printf("Signal generator disabled\n");
				#endif
    			signal_generator = OFF;
    			wave_gen_set_flag(FALSE);
    			wave_gen_set_state(SQUARE);
    			GPIO_clear_pin(GPIO_C,bit_0);
    			GPIO_clear_pin(GPIO_C,bit_9);
    			GPIO_set_pin(GPIO_B, bit_21);
    			GPIO_set_pin(GPIO_B, bit_22);
    			GPIO_set_pin(GPIO_E, bit_26);
    			PIT_stop(PIT_2);
    		}
		}

    	key_pressed = 0;
    }
    return 0 ;
}
