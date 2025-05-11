/*
 * Function_gen.c
 *
 *  Created on: Mar 13, 2021
 *      Author: Fernando
 */
#include "Wave_gen.h"
#include "MK64F12.h"
#include "GPIO.h"
#include "bits.h"
#include "PIT.h"
#include "DAC.h"
#include "RGB_set.h"

uint16_t g_wave_counter = FALSE; /**moves through the waveforms table*/
wave_select_t g_wave_select = SQUARE; /**selects the waveform output*/
uint8_t g_wave_gen_flag = FALSE;/**flag for enabling module*/

/**square waveform table*/
uint16_t square_wave_table[]={0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,
								4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,
								4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,
								4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,
								4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,};
/**sin waveform table*/
uint16_t sine_wave_table[]={2048, 2176, 2304, 2431, 2557, 2680, 2801, 2919, 3034, 3145,
							3251, 3353, 3449, 3540, 3625, 3704, 3776, 3842, 3900, 3951,
							3995, 4031, 4059, 4079, 4091, 4095, 4091, 4079, 4059, 4031,
							3995, 3951, 3900, 3842, 3776, 3704, 3625, 3540, 3449, 3353,
							3251, 3145, 3034, 2919, 2801, 2680, 2557, 2431, 2304, 2176,
							2048, 1919, 1791, 1664, 1538, 1415, 1294, 1176, 1061, 950,
							844, 742, 646, 555, 470, 391, 319, 253, 195, 144,100, 64,
							36, 16, 4, 0, 4, 16, 36, 64,100, 144, 195, 253, 319, 391,
							470, 555, 646, 742,844, 950, 1061, 1176, 1294, 1415, 1538,
							1664, 1791, 1919};
/**triangle waveform table*/
uint16_t triangle_wave_table[]={0, 82, 164, 246, 328, 410, 491, 573, 655, 737,
								819, 901, 983, 1065, 1147, 1229, 1310, 1392, 1474, 1556,
								1638, 1720, 1802, 1884, 1966, 2048, 2129, 2211, 2293, 2375,
								2457, 2539, 2621, 2703, 2785, 2867, 2948, 3030, 3112, 3194,
								3276, 3358, 3440, 3522, 3604, 3686, 3767, 3849, 3931, 4013,
								4095, 4013, 3931, 3849, 3767, 3686, 3604, 3522, 3440, 3358,
								3276, 3194, 3112, 3030, 2948, 2867, 2785, 2703, 2621, 2539,
								2457, 2375, 2293, 2211, 2129, 2048, 1966, 1884, 1802, 1720,
								1638, 1556, 1474, 1392, 1310, 1229, 1147, 1065, 983, 901,
								819, 737, 655, 573, 491, 410, 328, 246, 164, 82};

/**array of functions for the PIT callback*/
void (*wave_select[])()={square_wave,sine_wave,triangle_wave};

/**handles the square waveform table of values*/
void square_wave(void)
{
	if(table_limit == g_wave_counter)
	{
		g_wave_counter = FALSE;
	}
	DAC_set_value(square_wave_table[g_wave_counter]);/**sends value form table to the DAC word*/
	g_wave_counter++;
	return;
}
/**handles the sin waveform table of values*/
void sine_wave(void)
{
	if(table_limit == g_wave_counter)
	{
		g_wave_counter = FALSE;
	}
	DAC_set_value(sine_wave_table[g_wave_counter]);/**sends value form table to the DAC word*/
	g_wave_counter++;
	return;
}

/**handles the triangular waveform table of values*/
void triangle_wave(void)
{
	if(table_limit == g_wave_counter)
	{
		g_wave_counter = FALSE;
	}
	DAC_set_value(triangle_wave_table[g_wave_counter]);/**sends value form table to the DAC word*/
	g_wave_counter++;
	return;
}
void wave_mngr(void)
{
	PIT_callback_init(PIT_2,wave_select[g_wave_select]);/**sends function to the PIT callback */
	PIT_delay(PIT_2, SYSTEM_CLOCK, DELAY); /**set the delay for the waveform around 5Hz*/

	switch(g_wave_select)/**set RGB led to specific color, set external LEDs and move to next state to be ready for the next press*/
	{
		case SQUARE:
			RGB_set_BLUE();
			GPIO_set_pin(GPIO_C,bit_0);
			GPIO_clear_pin(GPIO_C,bit_9);
			g_wave_select = SIN;
		break;
		case SIN:
			RGB_set_RED();
			GPIO_clear_pin(GPIO_C,bit_0);
			GPIO_set_pin(GPIO_C,bit_9);
			g_wave_select = TRIANGULAR;
		break;
		case TRIANGULAR:
			RGB_set_GREEN();
			GPIO_clear_pin(GPIO_C,bit_0);
			GPIO_clear_pin(GPIO_C,bit_9);
			g_wave_select = SQUARE;
		break;
		default:
		break;
	}
	g_wave_counter = FALSE;

return;
}

void wave_gen_password(void)
{
	if(g_wave_gen_flag)
	{
		wave_mngr();/**move to manager if flag TRUE*/
	}
	else
	{
		GPIO_set_pin(GPIO_B, bit_21);
		GPIO_set_pin(GPIO_B, bit_22);
		GPIO_set_pin(GPIO_E, bit_26);
		GPIO_clear_pin(GPIO_C,bit_0);/**stop pit and set led off if flag is not TRUE*/
		GPIO_clear_pin(GPIO_C,bit_9);
		PIT_stop(PIT_2);
		g_wave_select = FALSE;
	}
return;
}

void wave_gen_set_flag(uint8_t flag)
{
	g_wave_gen_flag = flag;/**flag for enabling module*/
return;
}

void wave_gen_set_state(uint8_t state)
{
	g_wave_select = state;
return;
}
