/*
 * RGB_sequence.c
 *
 *  Created on: Apr 14, 2021
 *      Author: Fernando
 */

#include "RGB_sequence.h"
#include "LCD_nokia.h"
#include "Bits.h"
#include "button_handler.h"
#include "GPIO.h"
#include "FlexTimer.h"
#include "RGB_set.h"
#include "Delay.h"
#include "PIT.h"

static uint8_t g_period = FALSE;

static uint8_t g_string_sequence_array[RGB_sequence_max] = {0};
static uint8_t g_sequence_counter = FALSE;
static RGB_sequence_execute_state_t g_RGB_sequence_state = Execute_OFF;

void RGB_sequence_add(uint8_t n_letter)
{
	g_string_sequence_array[g_sequence_counter] = n_letter;

	if(RGB_sequence_max <= g_sequence_counter)
	{
		g_sequence_counter = FALSE;
		LCD_nokia_goto_xy(0,4);
	}
	else
	{
		g_sequence_counter ++;
	}

return;
}

void RGB_sequence_execute(void)
{
	for(uint8_t counter = FALSE; RGB_sequence_max > counter; counter++)
	{
		if(Execute == g_RGB_sequence_state)
		{
			switch(g_string_sequence_array[counter])
			{
				case blue:
					RGB_set_BLUE();
					PIT_delay(PIT_2, SYSTEM_CLOCK, PERIOD_1_sec);
					while(FALSE == g_period && FALSE == get_button_state(SW2))
					{}
				break;
				case red:
					RGB_set_RED();
					PIT_delay(PIT_2, SYSTEM_CLOCK, PERIOD_1_sec);
					while(FALSE == g_period && FALSE == get_button_state(SW2))
					{}
				break;
				case green:
					RGB_set_GREEN();
					PIT_delay(PIT_2, SYSTEM_CLOCK, PERIOD_1_sec);
					while(FALSE == g_period && FALSE == get_button_state(SW2))
					{}
				break;
				case yellow:
					RGB_set_YELLOW();
					PIT_delay(PIT_2, SYSTEM_CLOCK, PERIOD_1_sec);
					while(FALSE == g_period && FALSE == get_button_state(SW2))
					{}
				break;
				case purple:
					RGB_set_PURPLE();
					PIT_delay(PIT_2, SYSTEM_CLOCK, PERIOD_1_sec);
					while(FALSE == g_period && FALSE == get_button_state(SW2))
					{}
				break;
				case white:
					RGB_set_WHITE();
					PIT_delay(PIT_2, SYSTEM_CLOCK, PERIOD_1_sec);
					while(FALSE == g_period && FALSE == get_button_state(SW2))
					{}
				break;
				default:
					RGB_set_OFF();
				break;
			}
		}
		g_period = FALSE;
	}
	RGB_set_OFF();
	PIT_stop(PIT_2);
	PIT_clear_interrupt_flag(PIT_2);

return;
}

void RGB_sequence_write_color(void)
{
	for(uint8_t counter = FALSE; RGB_sequence_max > counter; counter++)
	{
		switch(g_string_sequence_array[counter])
		{
			case blue:
				LCD_nokia_send_char('Z');
			break;
			case red:
				LCD_nokia_send_char('R');
			break;
			case green:
				LCD_nokia_send_char('V');
			break;
			case yellow:
				LCD_nokia_send_char('A');
			break;
			case purple:
				LCD_nokia_send_char('M');
			break;
			case white:
				LCD_nokia_send_char('B');
			break;
			default:
				LCD_nokia_send_char('-');
			break;
		}
	}
return;
}

void RGB_sequence_reset(void)
{
	for(uint8_t counter = FALSE; RGB_sequence_max >= counter; counter++)
	{
		g_string_sequence_array[counter] = 0;
	}
	g_sequence_counter = FALSE;
	RGB_set_OFF();
	PIT_stop(PIT_2);
	PIT_clear_interrupt_flag(PIT_2);
return;
}

void RGB_sequence_state(uint8_t state)
{
	if(state)
		g_RGB_sequence_state = Execute;
	else
		g_RGB_sequence_state = Execute_OFF;
}

/*Function owned by callback*/
void RGB_sequence_delay(void)
{
	g_period = TRUE;
	PIT_stop(PIT_2);
	PIT_clear_interrupt_flag(PIT_2);

}
