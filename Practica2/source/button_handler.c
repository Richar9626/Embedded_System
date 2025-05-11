/*
 * button_handler.c
 *
 *  Created on: Apr 12, 2021
 *      Author: rjimenez
 *
 *      this function is to handle interrupts from different pins on GPIO
 *      Since there are multiple buttons for same ports
 */

#include "button_handler.h"
#include "bits.h"
#include "GPIO.h"
#include <stdio.h>
/*
 * How buttons are setup:
B0 -> PTC5
B1 -> PTC4
B2 -> PTD0
B3 -> PTD3
B4 -> PTB11
B5 -> PTC11
B6 -> PTC10
SW2 -> PTC6
SW3 -> PTA4
*/

/*initialization of button flags*/
static uint8_t g_B0 = FALSE;
static uint8_t g_B1 = FALSE;
static uint8_t g_B2 = FALSE;
static uint8_t g_B3 = FALSE;
static uint8_t g_B4 = FALSE;
static uint8_t g_B5 = FALSE;
static uint8_t g_B6 = FALSE;
static uint8_t g_SW2 = FALSE;
static uint8_t g_SW3 = FALSE;

/*will enter this function when a button is pressed*/
void button_state(void)
{
	/*depending on the button pressed will set the flag to TRue
	 * it goes from B0 to SW3*/
	if(GPIO_read_pin(GPIO_C, bit_5))
	{
		while(GPIO_read_pin(GPIO_C, bit_5))  /* While used to have only one read after pushing and not while pushing*/
		{}
		g_B0 = TRUE;
		#ifndef DEBUG_ON
			printf("B0 PRESSED\n");
		#endif
	}

	else if(GPIO_read_pin(GPIO_C, bit_4))
	{
		while(GPIO_read_pin(GPIO_C, bit_4))  /* While used to have only one read after pushing and not while pushing*/
		{}
		g_B1 = TRUE;
#ifndef DEBUG_ON
			printf("B1 PRESSED\n");
		#endif
	}

	else if(GPIO_read_pin(GPIO_D, bit_0))
	{
		while(GPIO_read_pin(GPIO_D, bit_0))  /* While used to have only one read after pushing and not while pushing*/
		{}
		g_B2 = TRUE;
#ifndef DEBUG_ON
			printf("B2 PRESSED\n");
		#endif
	}

	else if(GPIO_read_pin(GPIO_D, bit_3))
	{
		while(GPIO_read_pin(GPIO_D, bit_3))  /* While used to have only one read after pushing and not while pushing*/
		{}
		g_B3 = TRUE;
#ifndef DEBUG_ON
			printf("B3 PRESSED\n");
		#endif
	}

	else if(GPIO_read_pin(GPIO_B, bit_11))
	{
		while(GPIO_read_pin(GPIO_B, bit_11))  /* While used to have only one read after pushing and not while pushing*/
		{}
		g_B4 = TRUE;
#ifndef DEBUG_ON
			printf("B4 PRESSED\n");
		#endif
	}

	else if(GPIO_read_pin(GPIO_C, bit_11))
	{
		while(GPIO_read_pin(GPIO_C, bit_11))  /* While used to have only one read after pushing and not while pushing*/
		{}
		g_B5 = TRUE;
#ifndef DEBUG_ON
			printf("B5 PRESSED\n");
		#endif
	}

	else if(GPIO_read_pin(GPIO_C, bit_10))
	{
		while(GPIO_read_pin(GPIO_C, bit_10))  /* While used to have only one read after pushing and not while pushing*/
		{}
		g_B6 = TRUE;
		#ifndef DEBUG_ON
			printf("B6 PRESSED\n");
		#endif
	}

	else if(!GPIO_read_pin(GPIO_C, bit_6))
	{
		while(!GPIO_read_pin(GPIO_C, bit_6))  /* While used to have only one read after pushing and not while pushing*/
		{}
		g_SW2 = TRUE;
		#ifndef DEBUG_ON
			printf("SW2 PRESSED\n");
		#endif
	}

	else if(!GPIO_read_pin(GPIO_A, bit_4))
	{
		while(!GPIO_read_pin(GPIO_A, bit_4))  /* While used to have only one read after pushing and not while pushing*/
		{}
		g_SW3 = TRUE;
		#ifndef DEBUG_ON
			printf("SW3 PRESSED\n");
		#endif
	}

	else /* if no button has b een pressed we ensure a false is set on flags*/
	{
		g_B0 = FALSE;
		g_B1 = FALSE;
		g_B2 = FALSE;
		g_B3 = FALSE;
		g_B4 = FALSE;
		g_B5 = FALSE;
		g_B6 = FALSE;
		g_SW2 = FALSE;
		g_SW3 = FALSE;

	}

}

/* Function to avoid using externs returns if button is pressed depending on button asked*/
uint8_t get_button_state(button_t button)
{
	uint8_t button_state = FALSE;//initialized to avoid random numbers
	switch(button)
	{
		case B0:/** B0 is selected*/
			button_state = g_B0;
		break;
		case B1:/** B1 is selected*/
			button_state = g_B1;
		break;
		case B2:/** B2 is selected*/
			button_state = g_B2;
		break;
		case B3:/**B3 is selected*/
			button_state = g_B3;
		break;
		case B4: /**B4 is selected*/
			button_state = g_B4;
		break;
		case B5: /** B5 is selected*/
			button_state = g_B5;
		break;
		case B6: /** B6 is selected*/
			button_state = g_B6;
		break;
		case SW2: /** SW2 is selected*/
			button_state = g_SW2;
		break;
		case SW3: /** SW3 is selected*/
			button_state = g_SW3;
		break;
		default:/**If doesn't exist the option*/
			return(button_state);
		break;
	}
		/**Successful configuration*/
	return(button_state);
}

/* function to set the value for the flag
 * Basically only used to set as false after being detected*/
void set_button_state(button_t button, boolean_t state)
{
	switch(button)
	{
		case B0:/** B0is selected*/
			g_B0 = state;
		break;
		case B1:/**  B1 is selected*/
			g_B1 = state;
		break;
		case B2:/** B2 is selected*/
			g_B2 = state;
		break;
		case B3:/** B3 is selected*/
			g_B3 = state;
		break;
		case B4: /** B4 is selected*/
			g_B4 = state;
		break;
		case B5: /** B5 is selected*/
			g_B5 = state;
		break;
		case B6: /** B6 is selected*/
			g_B6 = state;
		break;
		case SW2: /** sw2 is selected*/
			g_SW2 = state;
		break;
		case SW3: /** sw3 is selected*/
			g_SW3 = state;
		break;
		default:/**If doesn't exist the option*/
		break;
	}
	return;
}
