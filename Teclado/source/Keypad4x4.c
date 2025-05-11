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

//we add an or to enable pull up resistors ROWS
gpio_pin_control_register_t g_pcr_gpiob_pin_2 = GPIO_MUX1|GPIO_PFE|GPIO_PE|GPIO_PS;
gpio_pin_control_register_t g_pcr_gpiob_pin_3 = GPIO_MUX1|GPIO_PFE|GPIO_PE|GPIO_PS;
gpio_pin_control_register_t g_pcr_gpiob_pin_10 = GPIO_MUX1|GPIO_PFE|GPIO_PE|GPIO_PS;
gpio_pin_control_register_t g_pcr_gpiob_pin_11 = GPIO_MUX1|GPIO_PFE|GPIO_PE|GPIO_PS;

//variables to setup pins as output COLUMNS
gpio_pin_control_register_t g_pcr_gpiod_pin_0 = GPIO_MUX1|GPIO_PFE;
gpio_pin_control_register_t g_pcr_gpiod_pin_2 = GPIO_MUX1|GPIO_PFE;
gpio_pin_control_register_t g_pcr_gpiod_pin_3 = GPIO_MUX1|GPIO_PFE;
gpio_pin_control_register_t g_pcr_gpiod_pin_1 = GPIO_MUX1|GPIO_PFE;


const uint8_t columns_pins[columns_number] = {bit_0,bit_2,bit_3,bit_1};
const uint8_t rows_pins[rows_number] = {bit_2,bit_3,bit_10,bit_11};

uint8_t column = 0;
uint8_t row = 0;

uint8_t last_pressed = 0;

uint8_t keys[rows_number][columns_number] =
{
	{ '1','2','3', 'A' },
	{ '4','5','6', 'B' },
	{ '7','8','9', 'C' },
	{ '*','0','#', 'D' }
};

void keypad_init(void)
{
	/**Activating the GPIOB and GPIOD clock gating*/
	GPIO_clock_gating(GPIO_B);
	GPIO_clock_gating(GPIO_D);

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
	GPIO_write_port(GPIO_D, 0x0000000F);

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

uint8_t read_key(void)
{
	for(column = 0; column < columns_number; column ++)
	{
		GPIO_clear_pin(GPIO_D, columns_pins[column]);

		for(row = 0; row < rows_number; row ++)
		{
			if(!GPIO_read_pin(GPIO_B, rows_pins[row]))
			{
				while(!GPIO_read_pin(GPIO_B, rows_pins[row]))
				{}
				last_pressed = keys[row][column];
				return (last_pressed);
			}

		}

		GPIO_set_pin(GPIO_D, columns_pins[column]);
	}

	return last_pressed;
}
