/*
 *
 *The functionality of this code is to show a sequence of colors on the led
 *the sequence depends on the push button pressed
 *Ricardo Jimenez Sanchez
 *Fernando Curiel Solis
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
#define DELAY_CONST 650

//we use enumeration to avoid magic numbers for colors
typedef enum
{
	GREEN, /*!< First element and first color to appear*/
	BLUE, /*!1 */
	PURPLE, /*!2*/
	RED, /*3 */
	YELLOW, /*4 */
	WHITE  /*5 */
} led_color;

//variables to setup pins as output
gpio_pin_control_register_t g_pcr_gpiob_pin_21 = GPIO_MUX1;
gpio_pin_control_register_t g_pcr_gpiob_pin_22 = GPIO_MUX1;
gpio_pin_control_register_t g_pcr_gpioe_pin_26 = GPIO_MUX1;

//we add an or to enable pull up resistors
gpio_pin_control_register_t g_pcr_gpioc_pin_6 = GPIO_MUX1|GPIO_PE|GPIO_PS;
gpio_pin_control_register_t g_pcr_gpioa_pin_4 = GPIO_MUX1|GPIO_PE|GPIO_PS;


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

	//declare another variable to scroll between colors
	uint8_t led_position = GREEN;

	for(;;)
	{

		if(!GPIO_read_pin(GPIO_C, bit_6) && GPIO_read_pin(GPIO_A, bit_4)) //checks for sw2
		{
			//capture sw2 until falling edge
			while(!GPIO_read_pin(GPIO_C, bit_6) && GPIO_read_pin(GPIO_A, bit_4))
			{
				delay(DELAY_CONST); //we add a delay to eliminate ups and downs after push
			}

			if(led_position == GREEN ) //to make a cycle, after green backwards it goes yellow
			{
				led_position = YELLOW;
			}
			//moves cycle backwards
			else
			{
				led_position --;
			}
		}

		if(GPIO_read_pin(GPIO_C, bit_6) && !GPIO_read_pin(GPIO_A, bit_4)) //checks for sw3
		{
			//capture sw3 until falling edge
			while(GPIO_read_pin(GPIO_C, bit_6) && !GPIO_read_pin(GPIO_A, bit_4))
			{
				delay(DELAY_CONST);
			}

			if(led_position >= YELLOW)//to make a cycle, after yellow it starts again on green
			{
				led_position = GREEN;
			}
			else
			{
				led_position ++;
			}
		}

		if(!GPIO_read_pin(GPIO_C, bit_6) && !GPIO_read_pin(GPIO_A, bit_4)) //checks for sw3 and sw2 for white color
		{
			//capture sw2 + sw3 until falling edge
			while(!GPIO_read_pin(GPIO_C, bit_6) || !GPIO_read_pin(GPIO_A, bit_4))
			{
				delay(DELAY_CONST);
			}
			led_position = WHITE;
		}

		switch(led_position)
		{
				case GREEN:/** GPIO A is selected, we set GPIOs to get LED color green*/
					GPIO_set_pin(GPIO_B, bit_21);
					GPIO_set_pin(GPIO_B, bit_22);
					GPIO_clear_pin(GPIO_E, bit_26);
				break;
				case BLUE:/** GPIO B is selected, we set GPIOs to get LED color blue*/
					GPIO_set_pin(GPIO_E, bit_26);
					GPIO_set_pin(GPIO_B, bit_22);
					GPIO_clear_pin(GPIO_B, bit_21);
				break;
				case PURPLE:/** GPIO C is selected, we set GPIOs to get LED color purple*/
					GPIO_set_pin(GPIO_E, bit_26);
					GPIO_clear_pin(GPIO_B, bit_21);
					GPIO_clear_pin(GPIO_B, bit_22);
				break;
				case RED:/** GPIO D is selected, we set GPIOs to get LED color red*/
					GPIO_set_pin(GPIO_B, bit_21);
					GPIO_set_pin(GPIO_E, bit_26);
					GPIO_clear_pin(GPIO_B, bit_22);
				break;
				case YELLOW: /** GPIO E is selected, we set GPIOs to get LED color yellow*/
					GPIO_set_pin(GPIO_B, bit_21);
					GPIO_clear_pin(GPIO_E, bit_26);
					GPIO_clear_pin(GPIO_B, bit_22);
				break;
				case WHITE: /** GPIO E is selected, we set GPIOs to get LED color white*/
					GPIO_clear_pin(GPIO_B, bit_21);
					GPIO_clear_pin(GPIO_B, bit_22);
					GPIO_clear_pin(GPIO_E, bit_26);
				break;
				default:/**If doesn't exist the option turns led off*/
					GPIO_set_pin(GPIO_B, bit_21);
					GPIO_set_pin(GPIO_B, bit_22);
					GPIO_set_pin(GPIO_E, bit_26);
				break;
		}
	}

    return 0 ;
}
