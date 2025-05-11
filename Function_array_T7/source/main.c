/*
 *
 *The functionality of this code is to show a sequence of colors on the led
 *you just have to press SW2
 *Ricardo Jimenez Sanchez
 */

/**
 * @file    Device_Driver_T5.c
 * @brief   Application entry point.
 */
#include "MK64F12.h"
#include "GPIO.h"
#include "Bits.h"
#include "Delay.h"

//constant to have a delay for push buttons
#define DELAY_CONST 650000

//we use enumeration to avoid magic numbers for colors
typedef enum
{
	YELLOW, /*!< First element and first color to appear*/
	RED, /*!1 */
	PURPLE, /*!2*/
	BLUE, /*3 */
	GREEN, /*4 */

} led_color;


//variables to setup pins as output
gpio_pin_control_register_t g_pcr_gpiob_pin_21 = GPIO_MUX1;
gpio_pin_control_register_t g_pcr_gpiob_pin_22 = GPIO_MUX1;
gpio_pin_control_register_t g_pcr_gpioe_pin_26 = GPIO_MUX1;

//we add an or to enable pull up resistors
gpio_pin_control_register_t g_pcr_gpioc_pin_6 = GPIO_MUX1|GPIO_PE|GPIO_PS;
gpio_pin_control_register_t g_pcr_gpioa_pin_4 = GPIO_MUX1|GPIO_PE|GPIO_PS;

static void YELLOW_color()
{
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_E, bit_26);
	GPIO_clear_pin(GPIO_B, bit_22);
	return;
}

static void RED_color()
{
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_E, bit_26);
	GPIO_clear_pin(GPIO_B, bit_22);
	return;
}

static void PURPLE_color()
{
	GPIO_set_pin(GPIO_E, bit_26);
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_B, bit_22);
	return;
}

static void BLUE_color()
{
	GPIO_set_pin(GPIO_E, bit_26);
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_B, bit_21);
	return;
}

static void GREEN_color()
{
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_E, bit_26);
	return;
}


int main(void)
{

	/**Activating the GPIOB, GPIOC, GPIOA and GPIOE clock gating*/
	GPIO_clock_gating(GPIO_A);
	GPIO_clock_gating(GPIO_B);
	GPIO_clock_gating(GPIO_C);
	GPIO_clock_gating(GPIO_E);

	/**Pin control configuration of GPIOB pin22, pin21 and GPIOE pin 26 as GPIO*/
	GPIO_pin_control_register(GPIO_B,bit_21,&g_pcr_gpiob_pin_21);
	GPIO_pin_control_register(GPIO_B,bit_22,&g_pcr_gpiob_pin_22);
	GPIO_pin_control_register(GPIO_E,bit_26,&g_pcr_gpioe_pin_26);

	/**Pin control configuration of GPIOC pin6 and GPIOA pin 4 as GPIO with is pull-up resistor enabled*/
	GPIO_pin_control_register(GPIO_C,bit_6,&g_pcr_gpioc_pin_6);
	GPIO_pin_control_register(GPIO_A,bit_4,&g_pcr_gpioa_pin_4);

	/**Assigns a safe value to the output pin*/
	GPIO_write_port(GPIO_B, 0x00600000);
	GPIO_write_port(GPIO_E, 0x04000000);

	/**Configures GPIOB pin21, pin22 and GPIOE pin26 as output*/
	GPIO_data_direction_pin(GPIO_B,GPIO_OUTPUT, bit_21);  //blue
	GPIO_data_direction_pin(GPIO_B,GPIO_OUTPUT,bit_22);  //red
	GPIO_data_direction_pin(GPIO_E,GPIO_OUTPUT,bit_26); //green

	//setup of SW3 and SW2 as input
	GPIO_data_direction_pin(GPIO_C,GPIO_INPUT, bit_6);
	GPIO_data_direction_pin(GPIO_A,GPIO_INPUT, bit_4);

	//definition of functions array, it points to the function address
	void (*color_function[])() = {YELLOW_color, RED_color, PURPLE_color, BLUE_color, GREEN_color};

	//declare another variable to scroll between colors
	uint8_t led_position = YELLOW;

	//initialization oF Led on yellow
	color_function[led_position]();
	for(;;)
	{

		while(!GPIO_read_pin(GPIO_C, bit_6)) //checks for sw2
		{

			//there is a function used depending on the led_position
			color_function[led_position]();
			delay(DELAY_CONST);

			if(led_position >= GREEN) //to make a cycle, after yellow it goes green
			{
				led_position = YELLOW;
			}
			//moves cycle backwards
			else
			{
				led_position ++;
			}

		}
	}

    return 0 ;
}
