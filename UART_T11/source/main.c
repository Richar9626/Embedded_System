#include "string.h"
/*  SDK Included Files */
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "fsl_uart.h"
#include "clock_config.h"
/*Custom field*/
#include "Bits.h"
#include "I2C.h"
#include "UART.h"
#include "FlexTimer.h"
#include "RGB_set.h"
#include "PIT.h"
#include <math.h>
#include "NVIC.h"


#define press_1 49u
#define press_2 50u
#define press_ESC 27u
#define press_enter 13u
#define TIME_ARRAY_LIMIT 20u
#define DATE_ARRAY_LIMIT 20u
#define press_z 122u
#define press_r 114u
#define press_v 118u
#define press_m 109u
#define press_b 98u

/*States for main menu*/
typedef enum{MAIN_MENU, RGB_SEQUENCE, RGB_MANUAL} function_t;
typedef enum{OFF, ON} functionality_state_t;
static uint8_t g_period = FALSE;

const ftm_config_t g_ftm_config_led1 = {
		FTM_0,
		FTM_MODE_WPDIS_MASK,
		MAX_LUM_LED,
		CH_0,
		FLEX_TIMER_ELSB | FLEX_TIMER_MSB,
		OFF,
		FLEX_TIMER_CLKS_1,
		FLEX_TIMER_PS_128};

const ftm_config_t g_ftm_config_led2 = {
		FTM_0,
		FTM_MODE_WPDIS_MASK,
		MAX_LUM_LED,
		CH_1,
		FLEX_TIMER_ELSB | FLEX_TIMER_MSB,
		OFF,
		FLEX_TIMER_CLKS_1,
		FLEX_TIMER_PS_128};

const ftm_config_t g_ftm_config_led3 = {
		FTM_0,
		FTM_MODE_WPDIS_MASK,
		MAX_LUM_LED,
		CH_2,
		FLEX_TIMER_ELSB | FLEX_TIMER_MSB,
		OFF,
		FLEX_TIMER_CLKS_1,
		FLEX_TIMER_PS_128};

const ftm_config_t g_ftm_config_input = {
		FTM_2,
		FTM_MODE_WPDIS_MASK,
		OFF,
		CH_0,
		FLEX_TIMER_CHIE | FLEX_TIMER_ELSA,
		OFF,
		FLEX_TIMER_CLKS_1,
		FLEX_TIMER_PS_128};


int main(void)
{

	function_t function_state = MAIN_MENU;
	functionality_state_t functionality_state = ON;
	boolean_t pressed_button_flag = FALSE;
	uint8_t ch0;
    uint8_t array_index = 0;
    uint8_t date_sequence_buff[DATE_ARRAY_LIMIT];
    boolean_t first_pressed_button_flag = TRUE;

	NVIC_enable_interrupt_and_priotity(PIT_CH3_IRQ, PRIORITY_6);

	/*Enabling Flex timer*/
	FTM_init(&g_ftm_config_input);
	FTM_init(&g_ftm_config_led1); /*! Configuration function for the PWM leds*/
	FTM_init(&g_ftm_config_led2);
	FTM_init(&g_ftm_config_led3);

	/*Enabling PIT for 1 second delay*/
	PIT_clock_gating();
	PIT_enable();
	PIT_enable_interrupt(PIT_3);
	PIT_callback_init(PIT_3, led_delay);
    I2C_init();
    UART0_init();

	for(;;)
	{
    	if (UART0_interrupt_status())
        {
			ch0 = UART0_pressed_key();
			pressed_button_flag = TRUE;
			UART0_flag_interrupt_off();
        }

		if(MAIN_MENU == function_state)
		{
			if(ON == functionality_state)
			{
				functionality_state = OFF;
				UART_menu_print(UART0);
			}

			if(press_1 == ch0) /*go to sequence function*/
			{
				function_state = RGB_SEQUENCE;
				functionality_state = ON;
			}
			else if(press_2 == ch0) /* go to first function RGB manual*/
			{
				function_state = RGB_MANUAL;
				functionality_state = ON;
			}
		}

		if(RGB_SEQUENCE == function_state)
		{
			if(ON == functionality_state)
			{
				UART_RGB_SEQUENCE_print(UART0);
				functionality_state = OFF;
			}

			if(TRUE == pressed_button_flag && press_enter != ch0)
			{
				if(TIME_ARRAY_LIMIT <= array_index)
				{
					UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);
					array_index = 0;
				}
				if(TRUE == first_pressed_button_flag)
				{
					UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);
					array_index = 0;
					pressed_button_flag = FALSE;
				}
				else
				{
					UART_WriteByte(UART0, ch0);
					date_sequence_buff[array_index] = ch0;
					pressed_button_flag = FALSE;
					array_index ++;
				}
				first_pressed_button_flag = FALSE;
			}

			if(press_enter == ch0 && TRUE == pressed_button_flag)
			{
				array_index = 0;
				for(;array_index < TIME_ARRAY_LIMIT; array_index++)
				{
					switch(date_sequence_buff[array_index])
					{
						case press_v:
							RGB_set_GREEN();
							init_color_change();
						break;
						case press_z:
							RGB_set_BLUE();
							init_color_change();
						break;
						case press_m:
							RGB_set_PURPLE();
							init_color_change();
						break;
						case press_r:
							RGB_set_RED();
							init_color_change();
						break;
						case press_b:
							RGB_set_WHITE();
							init_color_change();
						break;
						default:
						break;
					}
				}

				pressed_button_flag = FALSE;
			}
			if(press_ESC == ch0)
			{
				function_state = MAIN_MENU;
				functionality_state = ON;
				first_pressed_button_flag = TRUE;
				array_index = 0;
			}
		}

		if(RGB_MANUAL == function_state)
		{
			if(ON == functionality_state)
			{
				UART_RGB_MANUAL_print(UART0);
				functionality_state = OFF;
			}
			if(TRUE == pressed_button_flag && press_enter != ch0)
			{
				if(TIME_ARRAY_LIMIT <= array_index)
				{
					UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);
					array_index = 0;
				}
				if(TRUE == first_pressed_button_flag)
				{
					UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);
					array_index = 0;
					pressed_button_flag = FALSE;
				}
				else
				{
					pressed_button_flag = FALSE;
					array_index ++;
				}
				first_pressed_button_flag = FALSE;
			}

			if(press_enter == ch0 && TRUE == pressed_button_flag)
			{


				pressed_button_flag = FALSE;
				array_index = 0;
			}
			if(press_ESC == ch0)
			{
				function_state = MAIN_MENU;
				functionality_state = ON;
				first_pressed_button_flag = TRUE;
				array_index = 0;
			}
		}
	}

	return 0;
}


