/*
 * EXAMEN2
 *
 * IMOLEMENTACION DE UARTS Y DISPOSITIVOS I2C
 *
 * POR:
 * RICARDO JIMENEZ SANCHEZ
 *
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
#include "RGB_sequence.h"
#include "Bits.h"
#include "GPIO.h"
#include "FlexTimer.h"
#include "RGB_set.h"
#include "Delay.h"
#include "PIT.h"
#include "RGB_ADC.h"
#include "Bits.h"
#include "NVIC.h"

#include "I2C.h"
#include "Delay.h"
#include "UART.h"


/* VARIABLES TO AVOID MAGIC NUMBERS*/
#define ASCII 48u	/*to convert characters to integers and viceversa */
/*Definition of keys to be pressed*/
#define press_1 49u
#define press_2 50u
#define PRESS_Z 122u
#define PRESS_R 114u
#define PRESS_V 118u
#define PRESS_M 109u
#define PRESS_B 98u
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
typedef enum{MAIN_MENU,SEQUENCE,RGB_VALUES} function_t;		/*menu of states*/
typedef enum{ADDRESS,LENGTH,TEXT} write_read_memory_state_t;		/*States for memory read and write*/
typedef enum{OFF, ON} functionality_state_t;				/*print menu or submenu*/

/* configuration structures for FTM leds PWM
 * how they are setup is on FlexTimer.h*/
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

	//variables to setup pins as output ftm
	gpio_pin_control_register_t g_pcr_gpio_led_ftm =  GPIO_MUX4;
	/** Configuration for FTM input*/
	gpio_pin_control_register_t	g_pcr_gpio_input_ftm = GPIO_MUX3|GPIO_PE|GPIO_PS;
	/*variables to setup pins as inputs*/
	gpio_pin_control_register_t g_pcr_gpio_button = GPIO_MUX1 | GPIO_PFE | GPIO_PE | INTR_FALLING_EDGE;
	/*variables to setup pins as inputs*/
	gpio_pin_control_register_t g_pcr_gpio_switch = GPIO_MUX1 | GPIO_PFE | GPIO_PS | GPIO_PE | INTR_FALLING_EDGE;
	/*ADC input*/
	gpio_pin_control_register_t	g_pcr_gpio_input_adc = GPIO_MUX3;

	/*Enabling clocks for GPIOS*/
	GPIO_clock_gating(GPIO_A);
	GPIO_clock_gating(GPIO_B);
	GPIO_clock_gating(GPIO_C);
	GPIO_clock_gating(GPIO_D);
	GPIO_clock_gating(GPIO_E);

	/**Pin control configuration of as GPIO For Led*/
	GPIO_pin_control_register(GPIO_C,bit_1,&g_pcr_gpio_led_ftm);
	GPIO_pin_control_register(GPIO_C,bit_2,&g_pcr_gpio_led_ftm);
	GPIO_pin_control_register(GPIO_C,bit_3,&g_pcr_gpio_led_ftm);

	GPIO_pin_control_register(GPIO_B, bit_2, &g_pcr_gpio_input_adc);

	/**Configures GPIOC as output*/
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT, bit_1);  /*blue*/
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT,bit_2);  /* red  */
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT,bit_3); /* green */



	/**Sets the threshold for interrupts, if the interrupt has higher priority constant that the BASEPRI, the interrupt will not be attended*/
	NVIC_set_basepri_threshold(PRIORITY_10);
	/**Enables and sets a particular interrupt and its priority*/
	NVIC_enable_interrupt_and_priotity(PORTA_IRQ,PRIORITY_5);
	NVIC_enable_interrupt_and_priotity(PORTC_IRQ,PRIORITY_5);
	NVIC_enable_interrupt_and_priotity(PORTB_IRQ,PRIORITY_5);
	NVIC_enable_interrupt_and_priotity(PORTD_IRQ,PRIORITY_5);
	NVIC_enable_interrupt_and_priotity(PIT_CH3_IRQ, PRIORITY_6);
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_6);
	NVIC_enable_interrupt_and_priotity(FTM2_IRQ,PRIORITY_9);

	/*Enabling Flex timer*/
	FTM_init(&g_ftm_config_input);
	FTM_init(&g_ftm_config_led1); /*! Configuration function for the PWM leds*/
	FTM_init(&g_ftm_config_led2);
	FTM_init(&g_ftm_config_led3);

	/*Enabling PIT for 1 second delay*/
	PIT_clock_gating();
	PIT_enable();
	PIT_enable_interrupt(PIT_3);
	PIT_enable_interrupt(PIT_2);
	PIT_callback_init(PIT_3, led_delay);
	PIT_callback_init(PIT_2, RGB_sequence_delay);

	/*Enabling interrupts*/
	NVIC_global_enable_interrupts;

	int16_t blue_duty_cycle = 0;
	int16_t red_duty_cycle = 0;
	int16_t green_duty_cycle = 0;
	uint8_t date_sequence_buff[20];
	/*Function to show initial sequence blue-red-green*/
	init_color_change();
	RGB_ADC_init();

    /* In here we initialize both I2C and UART pins, clocks and enabling*/
    I2C_init();
    UART_init();


       /* Wait until the slave is ready for transmit, wait time depend on user's case.
          Slave devices that need some time to process received byte or are not ready yet to
          send the next byte, can pull the clock low to signal to the master that it should wait.*/

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
				UART0_function_state = SEQUENCE;
				UART0_functionality_state = ON;

			}
			if(press_2 == ch0)
			{
				UART0_function_state = RGB_VALUES;
				UART0_functionality_state = ON;

			}
		}

		/*Copy of above function to have concurrency between uarts*/
		if(SEQUENCE == UART0_function_state)
		{

			if(ON == UART0_functionality_state)
			{
				UART_sequence_print(UART0);
				UART0_functionality_state = OFF;
			}
			if(TRUE == UART0_pressed_button_flag && press_enter != ch0)
			{
				if(20 <= UART0_array_index)
				{
					UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);
					UART0_array_index = 0;
				}
				if(TRUE == UART0_first_pressed_button_flag)
				{
					UART_WriteBlocking(UART0, "\033[11;10H", sizeof("\033[11;10H") - 1);/*Sets on this specific position the cursor*/
					UART0_array_index = 0;
					UART0_pressed_button_flag = FALSE;
				}
				else
				{
					UART_WriteByte(UART0, ch0);
					date_sequence_buff[UART0_array_index] = ch0;
					UART0_pressed_button_flag = FALSE;
					UART0_array_index ++;
				}
				UART0_first_pressed_button_flag = FALSE;
			}
			if(press_enter == ch0 && TRUE == UART0_pressed_button_flag)
			{
				UART0_pressed_button_flag = FALSE;
				UART0_array_index = 0;
				for(;UART0_array_index < TIME_ARRAY_LIMIT; UART0_array_index++)
				{
#ifndef DEBUG_ON
	printf("sequence: %c \n",date_sequence_buff[UART0_array_index]);
#endif
					switch(date_sequence_buff[UART0_array_index])
					{
						case PRESS_V:
							RGB_set_GREEN();
							init_color_change();
			#ifndef DEBUG_ON
				printf("verde: %c \n");
			#endif
						break;
						case PRESS_Z:
							RGB_set_BLUE();
							init_color_change();
#ifndef DEBUG_ON
	printf("azul: %c \n");
#endif
						break;
						case PRESS_M:
							RGB_set_PURPLE();
							init_color_change();
#ifndef DEBUG_ON
	printf("morado: %c \n");
#endif
						break;
						case PRESS_R:
							RGB_set_RED();
							init_color_change();
#ifndef DEBUG_ON
	printf("rojo: %c \n");
#endif
						break;
						case PRESS_B:
							RGB_set_WHITE();
							init_color_change();
#ifndef DEBUG_ON
	printf("blanco: %c \n");
#endif
						break;
						default:
						break;
					}
				}
				UART0_pressed_button_flag = FALSE;
			}
			if(press_ESC == ch0)/*go back to menu*/
			{
				UART0_function_state = MAIN_MENU;
				UART0_functionality_state = ON;
				RGB_sequence_reset();
				RGB_sequence_state(FALSE);
				UART0_first_pressed_button_flag = TRUE;

			}

		}

		/*Copy of above function to have concurrency between uarts*/
		if(RGB_VALUES == UART0_function_state)
		{

			if(ON == UART0_functionality_state)
			{
				UART_values_print(UART0);
				UART0_functionality_state = OFF;
			}
			if(TRUE == UART0_pressed_button_flag && press_enter != ch0)
			{

				UART0_pressed_button_flag = FALSE;
			}
			if(press_enter == ch0 && TRUE == UART0_pressed_button_flag)
			{
				Red_FlexTimer_update_channel_value(red_duty_cycle);
				Green_FlexTimer_update_channel_value(green_duty_cycle);
				Blue_FlexTimer_update_channel_value(blue_duty_cycle);
				UART0_pressed_button_flag = FALSE;
			}
			if(press_ESC == ch0)/*go back to menu*/
			{
				UART0_function_state = MAIN_MENU;
				UART0_functionality_state = ON;
			}

		}


	}
}
