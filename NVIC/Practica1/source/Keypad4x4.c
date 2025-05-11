/*
 * Keyboard4x4.c
 *
 *  Created on: Mar 8, 2021
 *      Author: rjimenez
 */

#include "Keypad4x4.h"
#include "MK64F12.h"
#include "GPIO.h"
#include "bits.h"
#include "NVIC.h"
#include "PIT.h"
#include <stdio.h>

//we add an or to enable pull up resistors g_rowS, a filter to avoid bouncing, and enabling interrupts
gpio_pin_control_register_t g_pcr_gpiob_pin_2 = GPIO_MUX1|GPIO_PFE|GPIO_PE|GPIO_PS|INTR_FALLING_EDGE;
gpio_pin_control_register_t g_pcr_gpiob_pin_3 = GPIO_MUX1|GPIO_PFE|GPIO_PE|GPIO_PS|INTR_FALLING_EDGE;
gpio_pin_control_register_t g_pcr_gpiob_pin_10 = GPIO_MUX1|GPIO_PFE|GPIO_PE|GPIO_PS|INTR_FALLING_EDGE;
gpio_pin_control_register_t g_pcr_gpiob_pin_11 = GPIO_MUX1|GPIO_PFE|GPIO_PE|GPIO_PS|INTR_FALLING_EDGE;

/*variables to setup pins as output g_columnS*/
gpio_pin_control_register_t g_pcr_gpiod_pin_0 = GPIO_MUX1;
gpio_pin_control_register_t g_pcr_gpiod_pin_2 = GPIO_MUX1;
gpio_pin_control_register_t g_pcr_gpiod_pin_3 = GPIO_MUX1;
gpio_pin_control_register_t g_pcr_gpiod_pin_1 = GPIO_MUX1;

/* definition of pins to be used on k64*/
const uint8_t g_columns_pins[columns_number] = {bit_0,bit_2,bit_3,bit_1};
const uint8_t g_rows_pins[rows_number] = {bit_2,bit_3,bit_10,bit_11};

/*initialization of flags and variables*/
uint8_t g_column = 0;
uint8_t g_row = 0;
uint8_t g_last_pressed = 0;
uint8_t g_pit_inter_status = FALSE;
uint8_t g_delay_completion = FALSE;

/*matrix for keypad, could be changed for another keypad*/
uint8_t keys[rows_number][columns_number] =
{
	{ '1','2','3', 'A' },
	{ '4','5','6', 'B' },
	{ '7','8','9', 'C' },
	{ '*','0','#', 'D' }
};

/*enabling pins needed for keypad*/
void keypad_init(void)
{
	/**Pin control configuration of GPIOB  with pull-up resistor enabled and passive filter*/
	GPIO_pin_control_register(GPIO_B,bit_2,&g_pcr_gpiob_pin_2);
	GPIO_pin_control_register(GPIO_B,bit_3,&g_pcr_gpiob_pin_3);
	GPIO_pin_control_register(GPIO_B,bit_10,&g_pcr_gpiob_pin_10);
	GPIO_pin_control_register(GPIO_B,bit_11,&g_pcr_gpiob_pin_11);

	/**Pin control configuration of GPIOB  with passive filter enabled*/
	GPIO_pin_control_register(GPIO_D,bit_0,&g_pcr_gpiod_pin_0);
	GPIO_pin_control_register(GPIO_D,bit_2,&g_pcr_gpiod_pin_2);
	GPIO_pin_control_register(GPIO_D,bit_3,&g_pcr_gpiod_pin_3);
	GPIO_pin_control_register(GPIO_D,bit_1,&g_pcr_gpiod_pin_1);

	/**Assigns a safe value to the output pin*/
	GPIO_write_port(GPIO_D, 0x00000000);

	/**Configures pins as output*/
	GPIO_data_direction_pin(GPIO_D,GPIO_OUTPUT, bit_0);
	GPIO_data_direction_pin(GPIO_D,GPIO_OUTPUT,bit_2);
	GPIO_data_direction_pin(GPIO_D,GPIO_OUTPUT,bit_3);
	GPIO_data_direction_pin(GPIO_D,GPIO_OUTPUT,bit_1);

	/**Configures pins as input*/
	GPIO_data_direction_pin(GPIO_B,GPIO_INPUT, bit_2);
	GPIO_data_direction_pin(GPIO_B,GPIO_INPUT, bit_3);
	GPIO_data_direction_pin(GPIO_B,GPIO_INPUT, bit_10);
	GPIO_data_direction_pin(GPIO_B,GPIO_INPUT, bit_11);


}

/*Basically we have a zero shifted through g_columns and when it matches with pressed g_row returns key*/
void read_key(void)
{
	/* We set up this to F for zeros to be shifted enabling different g_rows*/
	GPIO_write_port(GPIO_D, 0x0000000F);
	for(g_column = 0; g_column < columns_number; g_column ++)
	{
		GPIO_clear_pin(GPIO_D, g_columns_pins[g_column]);

		for(g_row = 0; g_row < rows_number; g_row ++)
		{
			if(!GPIO_read_pin(GPIO_B, g_rows_pins[g_row]))
			{
				while(!GPIO_read_pin(GPIO_B, g_rows_pins[g_row]))  /* While used to have only one read after pushing and not while pushing*/
				{}
				g_last_pressed = keys[g_row][g_column];
			}
		}

		GPIO_set_pin(GPIO_D, g_columns_pins[g_column]); /*hot bit reset*/
	}
	/* We write zeros on port again to catch any pressed button, without this  button is detected after several pushes*/
	GPIO_write_port(GPIO_D, 0x00000000);
	return;
}

/*returns key to avoid extern*/
uint8_t get_last_key(void)
{
	PIT_delay(PIT_0, SYSTEM_CLOCK, DELAY_KEY);
	while(FALSE == g_delay_completion)
	{}
	g_delay_completion = FALSE;
	return(g_last_pressed);
}

/*Enters here when pit triggers after DELAY_KEY*/
void get_last_key_delay(void)
{
	g_delay_completion = TRUE;
	PIT_stop(PIT_0);
	PIT_clear_interrupt_flag(PIT_0);
}
