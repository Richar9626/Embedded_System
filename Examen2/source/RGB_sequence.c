/*
 * RGB_sequence.c
 *
 *  Created on: Apr 14, 2021
 *      Author: Fernando
 */

#include "RGB_sequence.h"
#include "Bits.h"
#include "GPIO.h"
#include "FlexTimer.h"
#include "RGB_set.h"
#include "Delay.h"
#include "PIT.h"
#include "UART.h"

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
		UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);
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
					while(FALSE == g_period)
					{}
				break;
				case red:
					RGB_set_RED();
					PIT_delay(PIT_2, SYSTEM_CLOCK, PERIOD_1_sec);
					while(FALSE == g_period )
					{}
				break;
				case green:
					RGB_set_GREEN();
					PIT_delay(PIT_2, SYSTEM_CLOCK, PERIOD_1_sec);
					while(FALSE == g_period)
					{}
				break;
				case purple:
					RGB_set_PURPLE();
					PIT_delay(PIT_2, SYSTEM_CLOCK, PERIOD_1_sec);
					while(FALSE == g_period )
					{}
				break;
				case white:
					RGB_set_WHITE();
					PIT_delay(PIT_2, SYSTEM_CLOCK, PERIOD_1_sec);
					while(FALSE == g_period )
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
				UART_WriteBlocking(UART0, "z", sizeof("z") - 1);
				UART_WriteBlocking(UART0, "\033[C", sizeof("\033[C") - 1);
			break;
			case red:
				UART_WriteBlocking(UART0, "r", sizeof("r") - 1);
				UART_WriteBlocking(UART0, "\033[C", sizeof("\033[C") - 1);
			break;
			case green:
				UART_WriteBlocking(UART0, "v", sizeof("v") - 1);
				UART_WriteBlocking(UART0, "\033[C", sizeof("\033[C") - 1);

			break;
			case purple:
				UART_WriteBlocking(UART0, "m", sizeof("m") - 1);
				UART_WriteBlocking(UART0, "\033[C", sizeof("\033[C") - 1);

			break;
			case white:
				UART_WriteBlocking(UART0, "b", sizeof("b") - 1);
				UART_WriteBlocking(UART0, "\033[C", sizeof("\033[C") - 1);

			break;
			default:
				UART_WriteBlocking(UART0, ",", sizeof(",") - 1);
				UART_WriteBlocking(UART0, "\033[C", sizeof("\033[C") - 1);

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
