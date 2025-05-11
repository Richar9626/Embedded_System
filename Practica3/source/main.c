/*
 * PRACTICA 3
 *
 * IMOLEMENTACION DE UARTS Y DISPOSITIVOS I2C
 *
 * POR:
 * RICARDO JIMENEZ SANCHEZ
 * FERNANDO CURIEL SOLIS
 */
/*  Standard C Included Files */
#include "string.h"
/*  SDK Included Files */
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "fsl_uart.h"
#include "clock_config.h"

#include "Bits.h"

#include "I2C.h"
#include "RTC.h"
#include "Delay.h"
#include "UART.h"
#include "MCP7940M_register_map.h"
#include <math.h>

/* VARIABLES TO AVOID MAGIC NUMBERS*/
#define ASCII 48u	/*to convert characters to integers and viceversa */
/*Definition of keys to be pressed*/
#define press_1 49u
#define press_2 50u
#define press_3 51u
#define press_4 52u
#define press_5 53u
#define press_6 54u
#define press_7 55u
#define press_ESC 27u
#define press_enter 13u
/* miscellaneous definitions*/
#define TIME_ARRAY_LIMIT 8u
#define DATE_ARRAY_LIMIT 8u
#define HEXA_CONVERTER 16u
#define MEMORY_ADDRESS_LIMIT 6u
#define MAX_BYTES_LENGTH 20u
#define MAX_MEMORY_TEXT 200u
#define RESET_MEMORY_STATE -1
#define NIBBLE_MASK 0x0F
#define I2C_DELAY 50000  /* delay for the inbetween I2C writes or reads*/

/*custom structure*/
typedef enum{MAIN_MENU,SET_CLOCK,SET_DATE,CLOCK,DATE,WRITE_MEMORY,READ_MEMORY,CHAT} function_t;		/*menu of states*/
typedef enum{ADDRESS,LENGTH,TEXT} write_read_memory_state_t;		/*States for memory read and write*/
typedef enum{OFF, ON} functionality_state_t;				/*print menu or submenu*/


/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 * Functionality consists on having two terminals able to connect to both hardware: the memory and the real time clock.
 * Terminals communication will be trough UART and communication to additional hardware is through I2C
 * There is a menu to select functionality which are: configuration of clock, clock values, write and read memory and chat between terminals.
 */
int main(void)
{
	/*char characters variables for UARTs*/
	uint8_t ch0;
	uint8_t ch4;
	/*Variables for the clock*/
    uint8_t seconds = 0;
    uint8_t minutes = 0;
    uint8_t hours = 0;
    uint8_t day = 0;
    uint8_t month = 0;
    uint8_t year = 0;
    /* this index will be usefull to have the arayys length*/
    uint8_t UART0_array_index = 0;
    uint8_t UART4_array_index = 0;
    uint16_t HEX_address = 0;
    uint8_t length = 0;

    /*arrays to save the different values to be displayed or send to hardware*/
    uint8_t hourbuff[TIME_ARRAY_LIMIT];
    uint8_t datebuff[DATE_ARRAY_LIMIT];
    uint8_t set_time_array[TIME_ARRAY_LIMIT];
    uint8_t set_date_array[DATE_ARRAY_LIMIT];
    uint8_t memory_address[MEMORY_ADDRESS_LIMIT];
    uint8_t length_array[MAX_BYTES_LENGTH];
    uint8_t memory_text[MAX_MEMORY_TEXT] = {0};
    uint8_t memory_text_read[MAX_MEMORY_TEXT] = {0};
    uint8_t UART0_chat_text[MAX_MEMORY_TEXT];
    uint8_t UART4_chat_text[MAX_MEMORY_TEXT];

    /* In here we initialize both I2C and UART pins, clocks and enabling*/
    I2C_init();
    UART_init();

    /*Restart of clock values to begin with 0*/
    UART_status_read_write(RTC_disable(),UART0);
    delay(I2C_DELAY);
    UART_status_read_write(RTC_set_min(0),UART0);
    UART_status_read_write(RTC_set_hour(0),UART0);
    UART_status_read_write(RTC_set_sec(0),UART0);
    UART_status_read_write(RTC_set_date(0),UART0);
    UART_status_read_write(RTC_set_month(0),UART0);
    UART_status_read_write(RTC_set_year(0),UART0);

       /* Wait until the slave is ready for transmit, wait time depend on user's case.
          Slave devices that need some time to process received byte or are not ready yet to
          send the next byte, can pull the clock low to signal to the master that it should wait.*/
       for (uint32_t i = 0U; i < WAIT_TIME; i++)
       {
           __NOP();
       }


    /*Initialization of states and flags*/
	write_read_memory_state_t write_read_memory_state = ADDRESS;
	function_t UART0_function_state = MAIN_MENU;					/*starting menu state*/
	function_t UART4_function_state = MAIN_MENU;
	functionality_state_t UART0_functionality_state = ON;
	functionality_state_t UART4_functionality_state = ON;
	functionality_state_t UART0_chat_state = OFF;
	functionality_state_t UART4_chat_state = OFF;
	boolean_t UART0_pressed_button_flag = FALSE;
	boolean_t UART4_pressed_button_flag = FALSE;
	boolean_t UART0_first_pressed_button_flag = TRUE;
	boolean_t UART4_first_pressed_button_flag = TRUE;
	boolean_t UART0_busy_flag = FALSE;
	boolean_t UART4_busy_flag = FALSE;
//////////////////////////////////////////////////////////////////////////////////////////////////

    while (1)
    {
        /* If new data arrived. from terminal 1 UART0*/
    	if (UART0_interrupt_status())
        {
			ch0 = UART0_pressed_key();
			UART0_pressed_button_flag = TRUE; /*Flag used to determine if it was pressed or not*/
			UART0_flag_interrupt_off(); /* we reset the interrupt flag*/
        }
    	/* If new data arrived. from terminal 2 UART4*/
    	if (UART4_interrupt_status())
        {
			ch4 = UART4_pressed_key();
			UART4_pressed_button_flag = TRUE; /*Flag used to determine if it was pressed or not*/
			UART4_flag_interrupt_off(); /* we reset the interrupt flag*/
        }

/*********************		MAIN MENU STATE 	****************************/
/*This is the initial states for both UARTS
 * depending on the pressed key there will be a new functionality
 * after an ESC it will return to this*/

    	/*UART0 MENU*/
		if(MAIN_MENU == UART0_function_state)
		{
			if(ON == UART0_functionality_state)
			{
				UART_menu_print(UART0);
				UART0_functionality_state = OFF;
			}

			if(press_1 == ch0)
			{
				if(SET_CLOCK == UART4_function_state) /*we determine if it is being used by the other terminal*/
				{
					if(FALSE == UART4_busy_flag)
					{
						UART_busy_submenu_print(UART0);
						UART4_busy_flag = TRUE;
					}
				}
				else
				{
					UART0_function_state = SET_CLOCK;
					UART0_functionality_state = ON;
					UART4_busy_flag = FALSE;
				}
			}
			if(press_2 == ch0)
			{
				if(SET_DATE == UART4_function_state)/*we determine if it is being used by the other terminal*/
				{
					if(FALSE == UART4_busy_flag)
					{
						UART_busy_submenu_print(UART0);
						UART4_busy_flag = TRUE;
					}
				}
				else
				{
					UART0_function_state = SET_DATE;
					UART0_functionality_state = ON;
					UART4_busy_flag = FALSE;
				}
			}
			if(press_3 == ch0)
			{
				if(CLOCK == UART4_function_state)/*we determine if it is being used by the other terminal*/
				{
					if(FALSE == UART4_busy_flag)
					{
						UART_busy_submenu_print(UART0);
						UART4_busy_flag = TRUE;
					}
				}
				else
				{
					UART0_function_state = CLOCK;
					UART0_functionality_state = ON;
					UART4_busy_flag = FALSE;
				}
			}
			if(press_4 == ch0)
			{
				if(DATE == UART4_function_state)/*we determine if it is being used by the other terminal*/
				{
					if(FALSE == UART4_busy_flag)
					{
						UART_busy_submenu_print(UART0);
						UART4_busy_flag = TRUE;
					}
				}
				else
				{
					UART0_function_state = DATE;
					UART0_functionality_state = ON;
					UART4_busy_flag = FALSE;
				}
			}
			if(press_5 == ch0)
			{
				if(WRITE_MEMORY == UART4_function_state)/*we determine if it is being used by the other terminal*/
				{
					if(FALSE == UART4_busy_flag)
					{
						UART_busy_submenu_print(UART0);
						UART4_busy_flag = TRUE;
					}
				}
				else
				{
					UART0_function_state = WRITE_MEMORY;
					UART0_functionality_state = ON;
					UART4_busy_flag = FALSE;
				}
			}
			if(press_6 == ch0)
			{
				if(READ_MEMORY == UART4_function_state)/*we determine if it is being used by the other terminal*/
				{
					if(FALSE == UART4_busy_flag)
					{
						UART_busy_submenu_print(UART0);
						UART4_busy_flag = TRUE;
					}
				}
				else
				{
					UART0_function_state = READ_MEMORY;
					UART0_functionality_state = ON;
					UART4_busy_flag = FALSE;
				}
			}
			if(press_7 == ch0)
			{
				UART0_function_state = CHAT;
				UART0_functionality_state = ON;
				UART0_chat_state = ON;
			}
		}


		/*UART4 MENU*/
		if(MAIN_MENU == UART4_function_state)
		{
			if(ON == UART4_functionality_state)
			{
				UART_menu_print(UART4);
				UART4_functionality_state = OFF;
			}

			if(press_1 == ch4)
			{
				if(SET_CLOCK == UART0_function_state)
				{
					if(FALSE == UART0_busy_flag)
					{
						UART_busy_submenu_print(UART4);
						UART0_busy_flag = TRUE;
					}
				}
				else
				{
					UART4_function_state = SET_CLOCK;
					UART4_functionality_state = ON;
					UART0_busy_flag = FALSE;
				}
			}
			if(press_2 == ch4)
			{
				if(SET_DATE == UART0_function_state)
				{
					if(FALSE == UART0_busy_flag)
					{
						UART_busy_submenu_print(UART4);
						UART0_busy_flag = TRUE;
					}
				}
				else
				{
					UART4_function_state = SET_DATE;
					UART4_functionality_state = ON;
					UART0_busy_flag = FALSE;
				}
			}
			if(press_3 == ch4)
			{
				if(CLOCK == UART0_function_state)
				{
					if(FALSE == UART0_busy_flag)
					{
						UART_busy_submenu_print(UART4);
						UART0_busy_flag = TRUE;
					}
				}
				else
				{
					UART4_function_state = CLOCK;
					UART4_functionality_state = ON;
					UART0_busy_flag = FALSE;
				}
			}
			if(press_4 == ch4)
			{
				if(DATE == UART0_function_state)
				{
					if(FALSE == UART0_busy_flag)
					{
						UART_busy_submenu_print(UART4);
						UART0_busy_flag = TRUE;
					}
				}
				else
				{
					UART4_function_state = DATE;
					UART4_functionality_state = ON;
					UART0_busy_flag = FALSE;
				}
			}
			if(press_5 == ch4)
			{
				if(WRITE_MEMORY == UART0_function_state)
				{
					if(FALSE == UART0_busy_flag)
					{
						UART_busy_submenu_print(UART4);
						UART0_busy_flag = TRUE;
					}
				}
				else
				{
					UART4_function_state = WRITE_MEMORY;
					UART4_functionality_state = ON;
					UART0_busy_flag = FALSE;
				}
			}
			if(press_6 == ch4)
			{
				if(READ_MEMORY == UART0_function_state)
				{
					if(FALSE == UART0_busy_flag)
					{
						UART_busy_submenu_print(UART4);
						UART0_busy_flag = TRUE;
					}
				}
				else
				{
					UART4_function_state = READ_MEMORY;
					UART4_functionality_state = ON;
					UART0_busy_flag = FALSE;
				}
			}
			if(press_7 == ch4)
			{
				UART4_function_state = CHAT;
				UART4_functionality_state = ON;
				UART4_chat_state = ON;
			}
		}

/******************		SET CLOCK SUBMENU		****************************/
/*Submenu to change the clock value
 * value will go effective after an ENTER
 * Will cycle the values entered until Enter*/

		if(SET_CLOCK == UART0_function_state)
		{
			if(ON == UART0_functionality_state) /* condition added to avoid always printing resulting on blinking*/
			{
				UART_set_clock_submenu_print(UART0); /* function implemented on UART.C */
				UART0_functionality_state = OFF;
			}

			if(TRUE == UART0_pressed_button_flag && press_enter != ch0) /* enters here only when a new key is pressed different from ENTER*/
			{
				if(TIME_ARRAY_LIMIT <= UART0_array_index) /* this is to keep asking for values and reseting the value*/
				{
					UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1); /*Sets on this specific position the cursor*/
					UART0_array_index = 0;
				}
				if(TRUE == UART0_first_pressed_button_flag) /* condition added to remove from the terminal the option previoused selected*/
				{
					UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1); /*Sets on this specific position the cursor*/
					UART0_array_index = 0;
					UART0_pressed_button_flag = FALSE;
				}
				else
				{
					/*We do an echo to show on the terminal the pressed keys*/
					UART_WriteByte(UART0, ch0);
					set_time_array[UART0_array_index] = ch0; /*saving on an array pressed keys*/
					UART0_pressed_button_flag = FALSE;
					UART0_array_index ++; /*we keep count of elements saved*/
				}
				UART0_first_pressed_button_flag = FALSE;
			}

			if(press_enter == ch0 && TRUE == UART0_pressed_button_flag) /*gets in here after pressing enter*/
			{
			    UART_status_read_write(RTC_disable(), UART0); /*we need to disable the RTC first*/
			    delay(I2C_DELAY);
			    /*here we separate the elements from the array to save them on RTC registers, they have to be concerted to BCD*/
			    UART_status_read_write(RTC_set_min((set_time_array[3]-ASCII)*HEXA_CONVERTER + set_time_array[4]-ASCII),UART0);
			    UART_status_read_write(RTC_set_hour((set_time_array[0]-ASCII)*HEXA_CONVERTER + set_time_array[1]-ASCII),UART0);
			    UART_status_read_write(RTC_set_sec((set_time_array[6]-ASCII)*HEXA_CONVERTER + set_time_array[7]-ASCII),UART0);

			    UART_WriteBlocking(UART0, "\033[12;10H", sizeof("\033[12;10H") - 1);
				UART_WriteBlocking(UART0, "La hora ha sido cambiada ...", sizeof("La hora ha sido cambiada ...") - 1);
				UART0_pressed_button_flag = FALSE;
				UART0_array_index = 0;
			}
			if(press_ESC == ch0) /*Goes back to main menu and resets values*/
			{
				UART0_function_state = MAIN_MENU;
				UART0_functionality_state = ON;
				UART0_first_pressed_button_flag = TRUE;
				UART0_array_index = 0;
			}
		}

		/*Copy of above function to have concurrency between uarts*/
		if(SET_CLOCK == UART4_function_state)
		{
			if(ON == UART4_functionality_state)
			{
				UART_set_clock_submenu_print(UART4);
				UART4_functionality_state = OFF;
			}

			if(TRUE == UART4_pressed_button_flag && press_enter != ch4)
			{
				if(TIME_ARRAY_LIMIT <= UART4_array_index)
				{
					UART_WriteBlocking(UART4, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
					UART4_array_index = 0;
				}
				if(TRUE == UART4_first_pressed_button_flag)
				{
					UART_WriteBlocking(UART4, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
					UART4_array_index = 0;
					UART4_pressed_button_flag = FALSE;
				}
				else
				{
					UART_WriteByte(UART4, ch4);
					set_time_array[UART4_array_index] = ch4;
					UART4_pressed_button_flag = FALSE;
					UART4_array_index ++;
				}
				UART4_first_pressed_button_flag = FALSE;
			}

			if(press_enter == ch4 && TRUE == UART4_pressed_button_flag)
			{
			    UART_status_read_write(RTC_disable(), UART4);
			    delay(I2C_DELAY);
			    UART_status_read_write(RTC_set_min((set_time_array[3]-ASCII)*HEXA_CONVERTER + set_time_array[4]-ASCII),UART4);
			    UART_status_read_write(RTC_set_hour((set_time_array[0]-ASCII)*HEXA_CONVERTER + set_time_array[1]-ASCII),UART4);
			    UART_status_read_write(RTC_set_sec((set_time_array[6]-ASCII)*HEXA_CONVERTER + set_time_array[7]-ASCII),UART4);

			    UART_WriteBlocking(UART4, "\033[12;10H", sizeof("\033[12;10H") - 1);/*Sets on this specific position the cursor*/
				UART_WriteBlocking(UART4, "La hora ha sido cambiada ...", sizeof("La hora ha sido cambiada ...") - 1);
				UART4_pressed_button_flag = FALSE;
				UART4_array_index = 0;
			}
			if(press_ESC == ch4)
			{
				UART4_function_state = MAIN_MENU;
				UART4_functionality_state = ON;
				UART4_first_pressed_button_flag = TRUE;
				UART4_array_index = 0;
			}
		}

/**********************		SET DATE SUBMENU		***************************/
/* This submenu is to set the date on RTC
 * basically reads what you put on the terminal, converts it to BCD and writes them on the RTC
 * it will keep asking for characters until you press ENTER*/
		if(SET_DATE == UART0_function_state)
		{
			if(ON == UART0_functionality_state) /*To avoid blinking on prints*/
			{
				UART_set_date_submenu_print(UART0);
				UART0_functionality_state = OFF;
			}

			if(TRUE == UART0_pressed_button_flag && press_enter != ch0)
			{
				if(DATE_ARRAY_LIMIT <= UART0_array_index) /* we make a ring to enter characters for the value*/
				{
					UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
					UART0_array_index = 0;
				}
				if(TRUE == UART0_first_pressed_button_flag)/*to eliminate the extra character that was added after pressing on menu*/
				{
					UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
					UART0_array_index = 0;
					UART0_pressed_button_flag = FALSE;
				}
				else
				{
					UART_WriteByte(UART0, ch0);/*echo of characters entered*/
					set_date_array[UART0_array_index] = ch0;
					UART0_pressed_button_flag = FALSE;
					UART0_array_index ++;
				}
				UART0_first_pressed_button_flag = FALSE;
			}

			if(press_enter == ch0 && TRUE == UART0_pressed_button_flag)
			{
				/*we make a convertion to BCD and send results to RTC*/
				UART_status_read_write(RTC_set_date((set_date_array[0]-ASCII)*HEXA_CONVERTER + set_date_array[1]-ASCII),UART0);
				delay(I2C_DELAY);
				UART_status_read_write(RTC_set_month((set_date_array[3]-ASCII)*HEXA_CONVERTER + set_date_array[4]-ASCII),UART0);
				delay(I2C_DELAY);
				UART_status_read_write(RTC_set_year((set_date_array[6]-ASCII)*HEXA_CONVERTER + set_date_array[7]-ASCII),UART0);
				delay(I2C_DELAY);

				UART_WriteBlocking(UART0, "\033[12;10H", sizeof("\033[12;10H") - 1);/*Sets on this specific position the cursor*/
				UART_WriteBlocking(UART0, "La fecha ha sido cambiada ...", sizeof("La fecha ha sido cambiada ...") - 1);
				UART0_pressed_button_flag = FALSE;
				UART0_array_index = 0;
			}
			if(press_ESC == ch0)/*go back to menu*/
			{
				UART0_function_state = MAIN_MENU;
				UART0_functionality_state = ON;
				UART0_first_pressed_button_flag = TRUE;
				UART0_array_index = 0;
			}
		}

		/*Copy of above function to have concurrency between uarts*/
		if(SET_DATE == UART4_function_state)
		{
			if(ON == UART4_functionality_state)
			{
				UART_set_date_submenu_print(UART4);
				UART4_functionality_state = OFF;
			}

			if(TRUE == UART4_pressed_button_flag && press_enter != ch4)
			{
				if(DATE_ARRAY_LIMIT <= UART4_array_index)
				{
					UART_WriteBlocking(UART4, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
					UART4_array_index = 0;
				}
				if(TRUE == UART4_first_pressed_button_flag)
				{
					UART_WriteBlocking(UART4, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
					UART4_array_index = 0;
					UART4_pressed_button_flag = FALSE;
				}
				else
				{
					UART_WriteByte(UART4, ch4);
					set_date_array[UART4_array_index] = ch4;
					UART4_pressed_button_flag = FALSE;
					UART4_array_index ++;
				}
				UART4_first_pressed_button_flag = FALSE;
			}

			if(press_enter == ch4 && TRUE == UART4_pressed_button_flag)
			{
				UART_status_read_write(RTC_set_date((set_date_array[0]-ASCII)*HEXA_CONVERTER + set_date_array[1]-ASCII),UART4);
				delay(I2C_DELAY);
				UART_status_read_write(RTC_set_month((set_date_array[3]-ASCII)*HEXA_CONVERTER + set_date_array[4]-ASCII),UART4);
				delay(I2C_DELAY);
				UART_status_read_write(RTC_set_year((set_date_array[6]-ASCII)*HEXA_CONVERTER + set_date_array[7]-ASCII),UART4);
				delay(I2C_DELAY);

				UART_WriteBlocking(UART4, "\033[12;10H", sizeof("\033[12;10H") - 1);/*Sets on this specific position the cursor*/
				UART_WriteBlocking(UART4, "La fecha ha sido cambiada ...", sizeof("La fecha ha sido cambiada ...") - 1);
				UART4_pressed_button_flag = FALSE;
				UART4_array_index = 0;
			}
			if(press_ESC == ch4)/*go back to menu*/
			{
				UART4_function_state = MAIN_MENU;
				UART4_functionality_state = ON;
				UART4_first_pressed_button_flag = TRUE;
				UART4_array_index = 0;
			}
		}

/*******************		CLOCK VALUE SUBMENU		************************/
/* Over here we doisplay current RTC timestamp
 *  we read registers, convert them and send them to terminal*/
		if(CLOCK == UART0_function_state)
		{

			if(ON == UART0_functionality_state)
			{
				UART_clock_submenu_print(UART0);
				UART0_functionality_state = OFF;
			}

			/*reading cregister values and store them on variables*/
			seconds = UART_status_read_write(RTC_get_sec(),UART0);
			minutes = UART_status_read_write(RTC_get_min(),UART0);
			hours = UART_status_read_write(RTC_get_hour(),UART0);

			//*wee decompose those variables to display them on the terminal*/
			hourbuff[7] = (seconds & NIBBLE_MASK) + ASCII;
			hourbuff[6] = (seconds >> 4) + ASCII;
			hourbuff[5] = ':';
			hourbuff[4] = (minutes & NIBBLE_MASK) + ASCII;
			hourbuff[3] = (minutes >> 4) + ASCII;
			hourbuff[2] = ':';
			hourbuff[1] = (hours & NIBBLE_MASK) + ASCII;
			hourbuff[0] = (hours >> 4) + ASCII;

			delay(UART_DELAY); /*to avoid blinking*/
			UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
			UART_WriteBlocking(UART0, hourbuff, sizeof(hourbuff));

			if(press_ESC == ch0)/*go back to menu*/
			{
				UART0_function_state = MAIN_MENU;
				UART0_functionality_state = ON;
			}

		}
		/*Copy of above function to have concurrency between uarts*/
		if(CLOCK == UART4_function_state)
		{

			if(ON == UART4_functionality_state)
			{
				UART_clock_submenu_print(UART4);
				UART4_functionality_state = OFF;
			}

			seconds = UART_status_read_write(RTC_get_sec(),UART4);
			minutes = UART_status_read_write(RTC_get_min(),UART4);
			hours = UART_status_read_write(RTC_get_hour(),UART4);

			hourbuff[7] = (seconds & NIBBLE_MASK) + ASCII;
			hourbuff[6] = (seconds >> 4) + ASCII;
			hourbuff[5] = ':';
			hourbuff[4] = (minutes & NIBBLE_MASK) + ASCII;
			hourbuff[3] = (minutes >> 4) + ASCII;
			hourbuff[2] = ':';
			hourbuff[1] = (hours & NIBBLE_MASK) + ASCII;
			hourbuff[0] = (hours >> 4) + ASCII;

			delay(UART_DELAY);
			UART_WriteBlocking(UART4, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
			UART_WriteBlocking(UART4, hourbuff, sizeof(hourbuff));

			if(press_ESC == ch4)/*go back to menu*/
			{
				UART4_function_state = MAIN_MENU;
				UART4_functionality_state = ON;
			}

		}


/**********************		DATE VALUE SUBMENU		***********************/
/*Displays date values stored on the RTC chip
 * we read, convert and display values*/
		if(DATE == UART0_function_state)
		{

			if(ON == UART0_functionality_state)
			{
				UART_date_submenu_print(UART0);
				UART0_functionality_state = OFF;
			}

			/*storing registers to variables*/
			day = UART_status_read_write(RTC_get_date(),UART0);
			delay(I2C_DELAY);
			month = UART_status_read_write(RTC_get_month(),UART0);
			delay(I2C_DELAY);
			year = UART_status_read_write(RTC_get_year(),UART0);

			/*we have to convert them to characters*/
			datebuff[7] = (year & NIBBLE_MASK) + ASCII;
			datebuff[6] = (year >> 4) + ASCII;
			datebuff[5] = '/';
			datebuff[4] = (month & NIBBLE_MASK) + ASCII;
			datebuff[3] = ((month >> 4)& 0x01) + ASCII; /* we add a mask to exclude the LPYR bit of register*/
			datebuff[2] = '/';
			datebuff[1] = (day & NIBBLE_MASK) + ASCII;
			datebuff[0] = (day >> 4) + ASCII;
			delay(UART_DELAY); /*avoids blinking*/

			UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
			UART_WriteBlocking(UART0, datebuff, sizeof(datebuff));

			if(press_ESC == ch0)/*go back to menu*/
			{
				UART0_function_state = MAIN_MENU;
				UART0_functionality_state = ON;
			}

		}

		/*Copy of above function to have concurrency between uarts*/
		if(DATE == UART4_function_state)
		{

			if(ON == UART4_functionality_state)
			{
				UART_date_submenu_print(UART4);
				UART4_functionality_state = OFF;
			}

			day = UART_status_read_write(RTC_get_date(),UART4);
			delay(I2C_DELAY);
			month = UART_status_read_write(RTC_get_month(),UART4);
			delay(I2C_DELAY);
			year = UART_status_read_write(RTC_get_year(),UART4);

			datebuff[7] = (year & NIBBLE_MASK) + ASCII;
			datebuff[6] = (year >> 4) + ASCII;
			datebuff[5] = '/';
			datebuff[4] = (month & NIBBLE_MASK) + ASCII;
			datebuff[3] = ((month >> 4)& 0x01) + ASCII; /* we add a mask to exclude the LPYR bit of register*/
			datebuff[2] = '/';
			datebuff[1] = (day & NIBBLE_MASK) + ASCII;
			datebuff[0] = (day >> 4) + ASCII;
			delay(UART_DELAY);

			UART_WriteBlocking(UART4, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
			UART_WriteBlocking(UART4, datebuff, sizeof(datebuff));

			if(press_ESC == ch4)/*go back to menu*/
			{
				UART4_function_state = MAIN_MENU;
				UART4_functionality_state = ON;
			}

		}
/******************		WRITE MEMORY	**********************/
/*In this function we writee on mewmory
 * we ask for differnet parameters like address, length and data
 * address has to be on a specific format
 * if text exceeds length will be cut*/
		if(WRITE_MEMORY == UART0_function_state)
		{
			if(ON == UART0_functionality_state)
			{
				UART_set_memory_submenu_print(UART0);
				UART0_functionality_state = OFF;
			}

			if(TRUE == UART0_pressed_button_flag && press_enter != ch0)
			{
				if(TRUE == UART0_first_pressed_button_flag)
				{
					UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
					UART0_array_index = 0;
					UART0_pressed_button_flag = FALSE;
				}
				else
				{
					switch(write_read_memory_state)/*we have states to switch between data to be entered*/
					{
						case ADDRESS:/*user enters the address */
							if(MEMORY_ADDRESS_LIMIT <= UART0_array_index)
							{
								UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
								UART0_array_index = 0;
							}
							else
							{
								UART_WriteByte(UART0, ch0);/*we have an echo to check on the address entered*/
								memory_address[UART0_array_index] = ch0;/*save values on an array*/
								UART0_pressed_button_flag = FALSE;
								UART0_array_index ++;
							}
						break;

						case LENGTH:/*length to be entered by user*/
							UART_WriteByte(UART0, ch0);/*echo*/
							length_array[UART0_array_index] = ch0;
							UART0_pressed_button_flag = FALSE;
							UART0_array_index ++;
						break;

						case TEXT:/*text to be eneterd by user*/
							UART_WriteByte(UART0, ch0);/*echo*/
							memory_text[UART0_array_index] = ch0;
							UART0_pressed_button_flag = FALSE;
							UART0_array_index ++;
						break;

						default:
						break;
					}

				}
				UART0_first_pressed_button_flag = FALSE;
			}

			if(press_enter == ch0 && TRUE == UART0_pressed_button_flag)
			{
				switch(write_read_memory_state)/*we have the states use after an ENTER is pressed*/
				{
					case ADDRESS:/*the address is converted to hexa to be send to the EEPROM*/
						HEX_address = ((EEPROM_change_hex(memory_address[2]) << 12) + (EEPROM_change_hex(memory_address[3]) << 8)
								+ (EEPROM_change_hex(memory_address[4]) << 4) + EEPROM_change_hex(memory_address[5]));
						UART_WriteBlocking(UART0, "\033[12;10H", sizeof("\033[12;10H") - 1);
						UART_WriteBlocking(UART0, "Longitud en bytes:", sizeof("Longitud en bytes:") - 1);
						UART_WriteBlocking(UART0, "\033[13;10H", sizeof("\033[13;10H") - 1);
						length = 0;
					break;

					case LENGTH:
						for(uint8_t index = 0; index < UART0_array_index; index++)
						{
							length = length + (length_array[index] - ASCII)*(uint8_t)pow(10,UART0_array_index-index-1);/*we make this to convert characters to an integer*/
						}

						UART_WriteBlocking(UART0, "\033[14;10H", sizeof("\033[14;10H") - 1);/*Sets on this specific position the cursor*/
						UART_WriteBlocking(UART0, "Texto a guardar:", sizeof("Texto a guardar:") - 1);
						UART_WriteBlocking(UART0, "\033[15;10H", sizeof("\033[15;10H") - 1);/*Sets on this specific position the cursor*/
					break;

					case TEXT:
						memory_text[UART0_array_index] = '\0'; /*we indicate the end of string, works for printing on debug mode*/
						UART_status_read_write(EEPROM_WRITE(HEX_address, length, memory_text),UART0);/*we send all values captured*/
						for(uint8_t counter = FALSE; MAX_MEMORY_TEXT > counter; counter++)
						{
							memory_text[counter] = FALSE; /* cycle to reset values to 0*/
						}

						UART_WriteBlocking(UART0, "\033[16;10H", sizeof("\033[14;10H") - 1);/*Sets on this specific position the cursor*/
						UART_WriteBlocking(UART0, "Su texto ha sido guardado...", sizeof("Su texto ha sido guardado...") - 1);
					break;

					default:
						UART0_functionality_state = ON;
						write_read_memory_state = RESET_MEMORY_STATE; /*to go back to main state after another ENTEr*/
					break;
				}
				UART0_pressed_button_flag = FALSE;
				write_read_memory_state ++; /*this sum to change state*/
				UART0_array_index = 0;
			}
			if(press_ESC == ch0)/*go back to menu*/
			{
				UART0_function_state = MAIN_MENU;
				UART0_functionality_state = ON;
				UART0_first_pressed_button_flag = TRUE;
				UART0_array_index = 0;
				write_read_memory_state = ADDRESS;
			}
		}


		/*Copy of above function to have concurrency between uarts*/
		if(WRITE_MEMORY == UART4_function_state)
		{
			if(ON == UART4_functionality_state)
			{
				UART_set_memory_submenu_print(UART4);
				UART4_functionality_state = OFF;
			}

			if(TRUE == UART4_pressed_button_flag && press_enter != ch4)
			{
				if(TRUE == UART4_first_pressed_button_flag)
				{
					UART_WriteBlocking(UART4, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
					UART4_array_index = 0;
					UART4_pressed_button_flag = FALSE;
				}
				else
				{
					switch(write_read_memory_state)
					{
						case ADDRESS:
							if(MEMORY_ADDRESS_LIMIT <= UART4_array_index)
							{
								UART_WriteBlocking(UART4, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
								UART4_array_index = 0;
							}
							else
							{
								UART_WriteByte(UART4, ch4);
								memory_address[UART4_array_index] = ch4;
								UART4_pressed_button_flag = FALSE;
								UART4_array_index ++;
							}
						break;

						case LENGTH:
							UART_WriteByte(UART4, ch4);
							length_array[UART4_array_index] = ch4;
							UART4_pressed_button_flag = FALSE;
							UART4_array_index ++;
						break;

						case TEXT:
							UART_WriteByte(UART4, ch4);
							memory_text[UART4_array_index] = ch4;
							UART4_pressed_button_flag = FALSE;
							UART4_array_index ++;
						break;

						default:
						break;
					}

				}
				UART4_first_pressed_button_flag = FALSE;
			}

			if(press_enter == ch4 && TRUE == UART4_pressed_button_flag)
			{
				switch(write_read_memory_state)
				{
					case ADDRESS:
						HEX_address = ((EEPROM_change_hex(memory_address[2]) << 12) + (EEPROM_change_hex(memory_address[3]) << 8)
								+ (EEPROM_change_hex(memory_address[4]) << 4) + EEPROM_change_hex(memory_address[5]));
						UART_WriteBlocking(UART4, "\033[12;10H", sizeof("\033[12;10H") - 1);/*Sets on this specific position the cursor*/
						UART_WriteBlocking(UART4, "Longitud en bytes:", sizeof("Longitud en bytes:") - 1);
						UART_WriteBlocking(UART4, "\033[13;10H", sizeof("\033[13;10H") - 1);/*Sets on this specific position the cursor*/
						length = 0;
					break;

					case LENGTH:
						for(uint8_t index = 0; index < UART4_array_index; index++)
						{
							length = length + (length_array[index] - ASCII)*(uint8_t)pow(10,UART4_array_index-index-1);
						}
						#ifndef DEBUG_ON
							printf("length: %d\n",length);
						#endif
						UART_WriteBlocking(UART4, "\033[14;10H", sizeof("\033[14;10H") - 1);/*Sets on this specific position the cursor*/
						UART_WriteBlocking(UART4, "Texto a guardar:", sizeof("Texto a guardar:") - 1);
						UART_WriteBlocking(UART4, "\033[15;10H", sizeof("\033[15;10H") - 1);/*Sets on this specific position the cursor*/
					break;

					case TEXT:
						memory_text[UART4_array_index] = '\0';
						UART_status_read_write(EEPROM_WRITE(HEX_address, length, memory_text),UART4);
						for(uint8_t counter = FALSE; MAX_MEMORY_TEXT > counter; counter++)
						{
							memory_text[counter] = FALSE;
						}
						#ifndef DEBUG_ON
							printf("texto inicial: %s\n", memory_text);
						#endif
						UART_WriteBlocking(UART4, "\033[16;10H", sizeof("\033[14;10H") - 1);/*Sets on this specific position the cursor*/
						UART_WriteBlocking(UART4, "Su texto ha sido guardado...", sizeof("Su texto ha sido guardado...") - 1);
					break;

					default:
						UART4_functionality_state = ON;
						write_read_memory_state = RESET_MEMORY_STATE;
					break;
				}
				UART4_pressed_button_flag = FALSE;
				write_read_memory_state ++;
				UART4_array_index = 0;
			}
			if(press_ESC == ch4)/*go back to menu*/
			{
				UART4_function_state = MAIN_MENU;
				UART4_functionality_state = ON;
				UART4_first_pressed_button_flag = TRUE;
				UART4_array_index = 0;
				write_read_memory_state = ADDRESS;
			}
		}

/*******************	READ MEMORY	************************/
/*we read what was captured before on the memory
 * for this we need to specify the address and length of data*/

		if(READ_MEMORY == UART0_function_state)
		{
			if(ON == UART0_functionality_state)
			{
				UART_get_memory_submenu_print(UART0);
				UART0_functionality_state = OFF;
			}

			if(TRUE == UART0_pressed_button_flag && press_enter != ch0)
			{
				if(TRUE == UART0_first_pressed_button_flag)
				{
					UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
					UART0_array_index = 0;
					UART0_pressed_button_flag = FALSE;
				}
				else
				{
					switch(write_read_memory_state)/*same principle as write, here are 2 states to get data captured*/
					{
						case ADDRESS:
							if(MEMORY_ADDRESS_LIMIT <= UART0_array_index)
							{
								UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
								UART0_array_index = 0;
							}
							else
							{
								UART_WriteByte(UART0, ch0);/*echo to terminal*/
								memory_address[UART0_array_index] = ch0;/*we store characters on an array for later*/
								UART0_pressed_button_flag = FALSE;
								UART0_array_index ++;
							}
						break;

						case LENGTH: /*we input the data length*/
							UART_WriteByte(UART0, ch0);
							length_array[UART0_array_index] = ch0;
							UART0_pressed_button_flag = FALSE;
							UART0_array_index ++;
						break;

						default:
						break;

					}

				}
				UART0_first_pressed_button_flag = FALSE;
			}

			if(press_enter == ch0 && TRUE == UART0_pressed_button_flag) /*enters here after an ENTER*/
			{
				switch(write_read_memory_state)/*converts a value to memory capable depending on state*/
				{
					case ADDRESS:/*conver to hexa to be sent to the memory*/
						HEX_address = ((EEPROM_change_hex(memory_address[2]) << 12) + (EEPROM_change_hex(memory_address[3]) << 8)
								+ (EEPROM_change_hex(memory_address[4]) << 4) + EEPROM_change_hex(memory_address[5]));
						UART_WriteBlocking(UART0, "\033[12;10H", sizeof("\033[12;10H") - 1);/*Sets on this specific position the cursor*/
						UART_WriteBlocking(UART0, "Longitud en bytes:", sizeof("Longitud en bytes:") - 1);
						UART_WriteBlocking(UART0, "\033[13;10H", sizeof("\033[13;10H") - 1);/*Sets on this specific position the cursor*/
						length = 0;
					break;

					case LENGTH:
						for(uint8_t index = 0; index < UART0_array_index; index++)
						{
							length = length + (length_array[index] - ASCII)*(uint8_t)pow(10,UART0_array_index-index-1); /*conbverts from characters to an integer*/
						}
						UART_WriteBlocking(UART0, "\033[14;10H", sizeof("\033[14;10H") - 1);/*Sets on this specific position the cursor*/
						UART_WriteBlocking(UART0, "Contenido:", sizeof("Contenido:") - 1);
						UART_WriteBlocking(UART0, "\033[15;10H", sizeof("\033[15;10H") - 1);/*Sets on this specific position the cursor*/
						UART_status_read_write(EEPROM_get_string(HEX_address,length, memory_text_read),UART0); /*we read gthe value from the memory*/
						UART_WriteBlocking(UART0, memory_text_read, sizeof(memory_text_read));
						for(uint8_t counter = FALSE; MAX_MEMORY_TEXT > counter; counter++)
						{
							memory_text_read[counter] = FALSE;/*resetting the array values*/
						}
						UART_WriteBlocking(UART0, "\033[16;10H", sizeof("\033[14;10H") - 1);/*Sets on this specific position the cursor*/
						UART_WriteBlocking(UART0, "Presiona una tecla para continuar...", sizeof("Presiona una tecla para continuar...") - 1);
					break;
					case TEXT: /*after hitting enter on this state it goes to the begining*/
						UART0_functionality_state = ON;
						write_read_memory_state = RESET_MEMORY_STATE;
					break;

					default:
						UART0_functionality_state = ON;
						write_read_memory_state = RESET_MEMORY_STATE;
					break;
				}
				UART0_pressed_button_flag = FALSE;
				write_read_memory_state ++;
				UART0_array_index = 0;
			}
			if(press_ESC == ch0)/*go back to menu*/
			{
				UART0_function_state = MAIN_MENU;
				UART0_functionality_state = ON;
				UART0_first_pressed_button_flag = TRUE;
				UART0_array_index = 0;
				write_read_memory_state = ADDRESS;
			}
		}

		/*Copy of above function to have concurrency between uarts*/
		if(READ_MEMORY == UART4_function_state)
		{
			if(ON == UART4_functionality_state)
			{
				UART_get_memory_submenu_print(UART4);
				UART4_functionality_state = OFF;
			}

			if(TRUE == UART4_pressed_button_flag && press_enter != ch4)
			{
				if(TRUE == UART4_first_pressed_button_flag)
				{
					UART_WriteBlocking(UART4, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
					UART4_array_index = 0;
					UART4_pressed_button_flag = FALSE;
				}
				else
				{
					switch(write_read_memory_state)
					{
						case ADDRESS:
							if(MEMORY_ADDRESS_LIMIT <= UART4_array_index)
							{
								UART_WriteBlocking(UART4, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
								UART4_array_index = 0;
							}
							else
							{
								UART_WriteByte(UART4, ch4);
								memory_address[UART4_array_index] = ch4;
								UART4_pressed_button_flag = FALSE;
								UART4_array_index ++;
							}
						break;

						case LENGTH:
							UART_WriteByte(UART4, ch4);
							length_array[UART4_array_index] = ch4;
							UART4_pressed_button_flag = FALSE;
							UART4_array_index ++;
						break;

						default:
						break;

					}

				}
				UART4_first_pressed_button_flag = FALSE;
			}

			if(press_enter == ch4 && TRUE == UART4_pressed_button_flag)
			{
				switch(write_read_memory_state)
				{
					case ADDRESS:
						HEX_address = ((EEPROM_change_hex(memory_address[2]) << 12) + (EEPROM_change_hex(memory_address[3]) << 8)
								+ (EEPROM_change_hex(memory_address[4]) << 4) + EEPROM_change_hex(memory_address[5]));
						UART_WriteBlocking(UART4, "\033[12;10H", sizeof("\033[12;10H") - 1);/*Sets on this specific position the cursor*/
						UART_WriteBlocking(UART4, "Longitud en bytes:", sizeof("Longitud en bytes:") - 1);
						UART_WriteBlocking(UART4, "\033[13;10H", sizeof("\033[13;10H") - 1);/*Sets on this specific position the cursor*/
						length = 0;
					break;

					case LENGTH:
						for(uint8_t index = 0; index < UART4_array_index; index++)
						{
							length = length + (length_array[index] - ASCII)*(uint8_t)pow(10,UART4_array_index-index-1);
						}
						UART_WriteBlocking(UART4, "\033[14;10H", sizeof("\033[14;10H") - 1);/*Sets on this specific position the cursor*/
						UART_WriteBlocking(UART4, "Contenido:", sizeof("Contenido:") - 1);
						UART_WriteBlocking(UART4, "\033[15;10H", sizeof("\033[15;10H") - 1);/*Sets on this specific position the cursor*/
						UART_status_read_write(EEPROM_get_string(HEX_address,length, memory_text_read),UART4);
						UART_WriteBlocking(UART4, memory_text_read, sizeof(memory_text_read));
						for(uint8_t counter = FALSE; MAX_MEMORY_TEXT > counter; counter++)
						{
							memory_text_read[counter] = FALSE;
						}
						UART_WriteBlocking(UART4, "\033[16;10H", sizeof("\033[14;10H") - 1);/*Sets on this specific position the cursor*/
						UART_WriteBlocking(UART4, "Presiona una tecla para continuar...", sizeof("Presiona una tecla para continuar...") - 1);
					break;
					case TEXT:
						UART4_functionality_state = ON;
						write_read_memory_state = RESET_MEMORY_STATE;
					break;

					default:
						UART4_functionality_state = ON;
						write_read_memory_state = RESET_MEMORY_STATE;
					break;
				}
				UART4_pressed_button_flag = FALSE;
				write_read_memory_state ++;
				UART4_array_index = 0;
			}
			if(press_ESC == ch4)/*go back to menu*/
			{
				UART4_function_state = MAIN_MENU;
				UART4_functionality_state = ON;
				UART4_first_pressed_button_flag = TRUE;
				UART4_array_index = 0;
				write_read_memory_state = ADDRESS;
			}
		}

/***************************  CHAT  **********************************/
/*both terminals can send data at same time and will display on both
 * there is no echo on pressed keys due to cursor posittioning
 * issue with uart 4 is that USB to TTL only runs on windows 7 or below*/
		if(CHAT == UART4_function_state || CHAT == UART0_function_state )
		{
			if(ON == UART0_functionality_state)/*resets uart0 terminal when entering chat*/
			{
				UART_WriteBlocking(UART0, "\033[u", sizeof("\033[u") - 1);
				UART_WriteBlocking(UART0, "\033[38:5:166m", sizeof("\033[38:5:166m") - 1);
				UART_WriteBlocking(UART0, "\033[48:5:0m", sizeof("\033[48:5:0m") - 1);
				UART_WriteBlocking(UART0, "\033[2J", sizeof("\033[2J") - 1);
				UART_WriteBlocking(UART0, "\033[H", sizeof("\033[H") - 1);
				UART0_functionality_state = OFF;
			}

			if(ON == UART4_functionality_state)/*resets uart4 terminal when entering chat*/
			{
				UART_WriteBlocking(UART4, "\033[u", sizeof("\033[u") - 1);
				UART_WriteBlocking(UART4, "\033[38:5:166m", sizeof("\033[38:5:166m") - 1);
				UART_WriteBlocking(UART4, "\033[48:5:0m", sizeof("\033[48:5:0m") - 1);
				UART_WriteBlocking(UART4, "\033[2J", sizeof("\033[2J") - 1);
				UART_WriteBlocking(UART4, "\033[H", sizeof("\033[H") - 1);
				UART4_functionality_state = OFF;
			}
			if(TRUE == UART0_pressed_button_flag && press_enter != ch0 && press_ESC != ch0) /*button pressed to be displayed*/
			{
				if(TRUE == UART0_first_pressed_button_flag)/*removing the button pressed to acces to this submenu*/
				{
					UART_WriteBlocking(UART0, "\033[H", sizeof("\033[H") - 1);
					UART0_array_index = 0;
					UART0_pressed_button_flag = FALSE;
				}
				else
				{
					UART0_chat_text[UART0_array_index] = ch0;/*saving array to display*/
					UART0_pressed_button_flag = FALSE;
					UART0_array_index ++;
				}
				UART0_first_pressed_button_flag = FALSE;
			}

			if(press_enter == ch0 && TRUE == UART0_pressed_button_flag)/*after pressing enter*/
			{
				UART0_chat_text[UART0_array_index] = '\0';/*indicate end of string*/

				if(ON == UART4_chat_state)/*depending on which terminal is on this mode is were it will display*/
				{
					UART_WriteBlocking(UART4, "\033[B", sizeof("\033[B") - 1);
					UART_WriteBlocking(UART4, "Terminal 1:", sizeof("Terminal 1:") - 1);
					UART_WriteBlocking(UART4, "\033[B", sizeof("\033[B") - 1);/*cursor down*/
					UART_WriteBlocking(UART4, "\033[100D", sizeof("\033[100D") - 1);/*resetting cursor to begining*/
					UART_WriteBlocking(UART4, UART0_chat_text, sizeof(uint8_t)*UART0_array_index);/*we only send the data that was captured*/
					UART_WriteBlocking(UART4, "\033[100D", sizeof("\033[100D") - 1);
				}
				if(ON == UART0_chat_state)/*depending on which terminal is on this mode is were it will display*/
				{
					UART_WriteBlocking(UART0, "\033[B", sizeof("\033[B") - 1);
					UART_WriteBlocking(UART0, "Terminal 1:", sizeof("Terminal 1:") - 1);
					UART_WriteBlocking(UART0, "\033[B", sizeof("\033[B") - 1);
					UART_WriteBlocking(UART0, "\033[100D", sizeof("\033[100D") - 1);/*resetting cursor to begining*/
					UART_WriteBlocking(UART0, UART0_chat_text, sizeof(uint8_t)*UART0_array_index);/*we only send the data that was captured*/
					UART_WriteBlocking(UART0, "\033[100D", sizeof("\033[100D") - 1);/*resetting cursor to begining*/
				}

				UART0_pressed_button_flag = FALSE;
				UART0_array_index = 0;
			}

			if(TRUE == UART4_pressed_button_flag && press_enter != ch4 && press_ESC != ch4)
			{
				if(TRUE == UART4_first_pressed_button_flag)
				{
					UART_WriteBlocking(UART4, "\033[H", sizeof("\033[H") - 1);
					UART4_array_index = 0;
					UART4_pressed_button_flag = FALSE;
				}
				else
				{
					UART4_chat_text[UART4_array_index] = ch4;/*capturing data*/
					UART4_pressed_button_flag = FALSE;
					UART4_array_index ++;
				}
				UART4_first_pressed_button_flag = FALSE;
			}
			if(press_enter == ch4 && TRUE == UART4_pressed_button_flag)
			{
				UART4_chat_text[UART4_array_index] = '\0';

				if(ON == UART4_chat_state)/*depending on which terminal is on this mode is were it will display*/
				{
					UART_WriteBlocking(UART4, "\033[B", sizeof("\033[B") - 1);
					UART_WriteBlocking(UART4, "Terminal 2:", sizeof("Terminal 2:") - 1);
					UART_WriteBlocking(UART4, "\033[B", sizeof("\033[B") - 1);
					UART_WriteBlocking(UART4, "\033[100D", sizeof("\033[100D") - 1);/*resetting cursor to begining*/
					UART_WriteBlocking(UART4, UART4_chat_text, sizeof(uint8_t)*UART4_array_index);
					UART_WriteBlocking(UART4, "\033[100D", sizeof("\033[100D") - 1);/*resetting cursor to begining*/
				}
				if(ON == UART0_chat_state)/*depending on which terminal is on this mode is were it will display*/
				{
					UART_WriteBlocking(UART0, "\033[B", sizeof("\033[B") - 1);
					UART_WriteBlocking(UART0, "Terminal 2:", sizeof("Terminal 2:") - 1);
					UART_WriteBlocking(UART0, "\033[B", sizeof("\033[B") - 1);
					UART_WriteBlocking(UART0, "\033[100D", sizeof("\033[100D") - 1);/*resetting cursor to begining*/
					UART_WriteBlocking(UART0, UART4_chat_text, sizeof(uint8_t)*UART4_array_index);
					UART_WriteBlocking(UART0, "\033[100D", sizeof("\033[100D") - 1);/*resetting cursor to begining*/
				}

				UART4_pressed_button_flag = FALSE;
				UART4_array_index = 0;
			}

			if(press_ESC == ch4 && TRUE == UART4_pressed_button_flag)/*go back to menu*/
			{
				UART4_function_state = MAIN_MENU;
				UART4_functionality_state = ON;
				UART4_chat_state = OFF;
				UART4_pressed_button_flag = FALSE;
				UART4_first_pressed_button_flag = TRUE;
				if(ON == UART0_chat_state)/*will display the bye text only if it is on*/
				{
					UART_WriteBlocking(UART0, "\033[B", sizeof("\033[B") - 1);
					UART_WriteBlocking(UART0, "Terminal 2 se desconecto...", sizeof("Terminal 1 se desconecto...") - 1);
					UART_WriteBlocking(UART0, "\033[100D", sizeof("\033[100D") - 1);/*resetting cursor to begining*/
				}
			}

			if(press_ESC == ch0 && TRUE == UART0_pressed_button_flag)/*go back to menu*/
			{
				UART0_function_state = MAIN_MENU;
				UART0_functionality_state = ON;
				UART0_chat_state = OFF;
				UART0_pressed_button_flag = FALSE;
				UART0_first_pressed_button_flag = TRUE;
				if(ON == UART4_chat_state)/*will display the bye text only if it is on*/
				{
					UART_WriteBlocking(UART4, "\033[B", sizeof("\033[B") - 1);
					UART_WriteBlocking(UART4, "Terminal 1 se desconecto...", sizeof("Terminal 1 se desconecto...") - 1);
					UART_WriteBlocking(UART4, "\033[100D", sizeof("\033[100D") - 1);/*resetting cursor to begining*/
				}
			}
		}

	}
}
