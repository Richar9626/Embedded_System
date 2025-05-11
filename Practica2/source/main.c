/*
 * PRACTICA 2
 * SISTEMAS EMBEBIDOS
 *
 * BY:
 * RICARDO JIMENEZ SANCHEZ
 * FERNANDO CURIEL SOLIS
 */
 
#include "SPI.h"
#include "GPIO.h"
#include "Bits.h"
#include "LCD_nokia.h"
#include "LCD_nokia_images.h"
#include "stdint.h"
#include "Delay.h"
#include "PIT.h"
#include "NVIC.h"
#include "RGB_set.h"
#include "FlexTimer.h"
#include "button_handler.h"
#include "RGB_sequence.h"
#include "RGB_ADC.h"


#define ASCII_CONSTANT 48
#define MAX_CHANNEL_VALUE 4096
#define MIN_CHANNEL_VALUE 0
#define FIVE_DIGITS 10000
#define FOUR_DIGITS 1000
#define THREE_DIGITS 100
#define RGB_ADC_digit_limit 1241U
#define Percentage_Duty_Cycle_10 100

typedef enum{WALLPAPER, MAIN_MENU, RGB_MANUAL, RGB_ADC, RGB_SEQUENCE, RGB_FREQUENCY} function_t;
typedef enum{OFF, ON} functionality_state_t;

/** \brief This is the configuration structure to configure the LCD.
 * Note that is constants and it is because is only a variable used for configuration*/
const spi_config_t g_spi_config = {
							SPI_DISABLE_FIFO,
							SPI_LOW_POLARITY,
							SPI_LOW_PHASE,
							SPI_MSB,
							SPI_0,
							SPI_MASTER,
							GPIO_MUX2|GPIO_DSE,
							SPI_BAUD_RATE_8,
							SPI_FSIZE_8,
							{GPIO_D, bit_0, bit_1, bit_2, bit_3} };

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

/* Images on K64 available
 * ITESO
 * Patriots*/


int main(void)
{
	uint8_t string_menu_1[]="1)RGB Manual"; /*! String to be printed in the LCD*/
	uint8_t string_menu_2[]="2)RGB ADC"; /*! String to be printed in the LCD*/
	uint8_t string_menu_3[]="3)RGB Secuencia"; /*! String to be printed in the LCD*/
	uint8_t string_menu_4[]="4)RGB Frecuencia"; /*! String to be printed in the LCD*/

	/*Strings to be printed on Main menu*/
	uint8_t string_manual_1[]="RGB Manual";
	uint8_t string_manual_2[]="ON";

	uint8_t string_sequence_1[]="RGB Secuencia";
	uint8_t string_sequence_2[]="ON";

	uint8_t string_adc_1[]="Voltaje";
	uint8_t string_adc_2[10] = "00.00";
	uint8_t string_adc_3[10] = "00.00";
	uint8_t string_adc_4[]="v";
	uint8_t string_adc_5[]="SW3 = ON";
	uint8_t string_adc_6[]="SW2 = OFF";

	/*Strings to be printed on frecuency function*/
	uint8_t string_frequency_1[]="Freq";
	uint8_t string_frequency_2[10] = "00.00";
	uint8_t string_frequency_3[10] = "00.00";
	uint8_t string_frequency_4[]="kHz";
	uint8_t string_frequency_5[]="SW3 = ON";
	uint8_t string_frequency_6[]="SW2 = OFF";

	SPI_init(&g_spi_config); /*! Configuration function for the LCD port*/
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

	/**Pin control configuration of as GPIO For buttons*/
	GPIO_pin_control_register(GPIO_C, bit_5, &g_pcr_gpio_button);
	GPIO_pin_control_register(GPIO_C, bit_4, &g_pcr_gpio_button);
	GPIO_pin_control_register(GPIO_D, bit_0, &g_pcr_gpio_button);
	GPIO_pin_control_register(GPIO_D, bit_3, &g_pcr_gpio_button);
	GPIO_pin_control_register(GPIO_B, bit_11, &g_pcr_gpio_button);
	GPIO_pin_control_register(GPIO_C, bit_11, &g_pcr_gpio_button);
	GPIO_pin_control_register(GPIO_C, bit_10, &g_pcr_gpio_button);

	GPIO_pin_control_register(GPIO_A, bit_4, &g_pcr_gpio_switch);
	GPIO_pin_control_register(GPIO_C, bit_6, &g_pcr_gpio_switch);

	 /* input capture is on option 3 in the pin control register*/
	GPIO_pin_control_register(GPIO_B, bit_18, &g_pcr_gpio_input_ftm);

	GPIO_pin_control_register(GPIO_B, bit_2, &g_pcr_gpio_input_adc);

	/**Configures GPIOC as output*/
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT, bit_1);  /*blue*/
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT,bit_2);  /* red  */
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT,bit_3); /* green */

	/*Buttons as input*/
	GPIO_data_direction_pin(GPIO_C, GPIO_INPUT, bit_5);
	GPIO_data_direction_pin(GPIO_C, GPIO_INPUT, bit_4);
	GPIO_data_direction_pin(GPIO_D, GPIO_INPUT, bit_0);
	GPIO_data_direction_pin(GPIO_D, GPIO_INPUT, bit_3);
	GPIO_data_direction_pin(GPIO_B, GPIO_INPUT, bit_11);
	GPIO_data_direction_pin(GPIO_C, GPIO_INPUT, bit_11);
	GPIO_data_direction_pin(GPIO_C, GPIO_INPUT, bit_10);
	GPIO_data_direction_pin(GPIO_B, GPIO_INPUT, bit_2);

	/*SW3 as input*/
	GPIO_data_direction_pin(GPIO_A, GPIO_INPUT, bit_4);
	/*SW2 as input*/
	GPIO_data_direction_pin(GPIO_C, GPIO_INPUT, bit_6);

	/*FTM2_CH0 as input for frequency*/
	GPIO_data_direction_pin(GPIO_B,GPIO_INPUT, bit_18);


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


	LCD_nokia_init(); /*! Configuration function for the LCD */

	/*Enabling PIT for 1 second delay*/
	PIT_clock_gating();
	PIT_enable();
	PIT_enable_interrupt(PIT_3);
	PIT_enable_interrupt(PIT_2);
	PIT_callback_init(PIT_3, led_delay);
	PIT_callback_init(PIT_2, RGB_sequence_delay);

	/* We apply callbacks for the same function to distinguish which button has been pressed*/
	GPIO_callback_init(GPIO_A, button_state);
	GPIO_callback_init(GPIO_B, button_state);
	GPIO_callback_init(GPIO_C, button_state);
	GPIO_callback_init(GPIO_D, button_state);
	/*Enabling interrupts*/
	NVIC_global_enable_interrupts;

	/*Image to be displayed as wallpaper*/
	uint8_t *LCD_Display = get_image(Patriots);

	/* Variables to be used */
	function_t function_state = WALLPAPER;
	functionality_state_t functionality_state = OFF;
	int16_t blue_duty_cycle = 0;
	int16_t red_duty_cycle = 0;
	int16_t green_duty_cycle = 0;
	float frequency = 0;

	/*This values come from formula y = mx + b to map values*/
	const float frequency_slope = 0.455111;
	const float frequency_displacement = -455.111;

	LCD_nokia_clear();/*! It clears the information printed in the LCD*/
	LCD_nokia_bitmap(LCD_Display); /*! It prints an array that hold an image, in this case is the initial picture*/
	/*Function to show initial sequence blue-red-green*/
	init_color_change();
	RGB_ADC_init();

	for(;;)
	{
		if(TRUE == get_button_state(B0) && WALLPAPER == function_state)
		{
			set_button_state(B0,FALSE); /* we clear button state */
			function_state = MAIN_MENU;
		}
/******************  WALLPAPER FUNCTION   **************************************/
		if(WALLPAPER == function_state)
		{
			LCD_nokia_clear();/*! It clears the information printed in the LCD*/
			LCD_nokia_bitmap(LCD_Display); /*! It prints an array that hold an image, in this case is the initial picture*/
			delay(LCD_DELAY); /*we add a delay, otherwise the image seems blinking*/
			/*We set the buttons to false avoid entering to another function after this*/
			set_button_state(B1,FALSE);
			set_button_state(B2,FALSE);
			set_button_state(B3,FALSE);
			set_button_state(B4,FALSE);
			set_button_state(B5,FALSE);
			set_button_state(B6,FALSE);
			set_button_state(SW2,FALSE);
			set_button_state(SW3,FALSE);
		}
/******************  MAIN MENU FUNCTION   **************************************/
		if(MAIN_MENU == function_state)
		{
			LCD_nokia_clear();
			LCD_nokia_goto_xy(0,0); /*! It establishes the position to print the messages in the LCD*/
			LCD_nokia_send_string(string_menu_1); /*! It print a string stored in an array*/
			LCD_nokia_goto_xy(0,1);
			LCD_nokia_send_string(string_menu_2); /*! It print a string stored in an array*/
			LCD_nokia_goto_xy(0,2);
			LCD_nokia_send_string(string_menu_3); /*! It print a string stored in an array*/
			LCD_nokia_goto_xy(0,4);
			LCD_nokia_send_string(string_menu_4); /*! It print a string stored in an array*/

			if(TRUE == get_button_state(B0)) /* return to wallpaper */
			{
				set_button_state(B0,FALSE);
				function_state = WALLPAPER;
			}
			else if(TRUE == get_button_state(B1)) /* go to first function RGB manual*/
			{
				set_button_state(B1,FALSE);
				function_state = RGB_MANUAL;
			}
			else if(TRUE == get_button_state(B2)) /* go to second function RGB ADC*/
			{
				set_button_state(B2,FALSE);
				function_state = RGB_ADC;
			}
			else if(TRUE == get_button_state(B3)) /*go to sequence function*/
			{
				set_button_state(B3,FALSE);
				function_state = RGB_SEQUENCE;
			}
			else if(TRUE == get_button_state(B4)) /* go to frequency function */
			{
				set_button_state(B4,FALSE);
				function_state = RGB_FREQUENCY;
			}
			/* we set this buttons to false to avoid them to be as TRUE on a future function*/
			set_button_state(B5,FALSE);
			set_button_state(B6,FALSE);
			set_button_state(SW2,FALSE);
			set_button_state(SW3,FALSE);
		}
/******************  RGB MANUAL FUNCTION   **************************************/
/* this function will increment or decrement the led color depending on button pressed
 * b1 -> blue increase 10%
 * b2 -> blue decrease 10%
 * b3 -> red increase 10%
 * b4 -> red decrease 10%
 * b5 -> green increase 10%
 * b6 -> green decrease 10%*/
		if(RGB_MANUAL == function_state)
		{
			LCD_nokia_clear();
			LCD_nokia_goto_xy(6,2); /*! It establishes the position to print the messages in the LCD*/
			LCD_nokia_send_string(string_manual_1); /*! It print a string stored in an array*/
			LCD_nokia_goto_xy(30,3);
			LCD_nokia_send_string(string_manual_2); /*! It print a string stored in an array*/

			if(ON == functionality_state)
			{
		/*  When a button is pressed, the duty cycle is incremented or decremented 100*/
		/*  100 is aprox. 10% of 1024, functions are on Flex Timer.c*/
				if(TRUE == get_button_state(B1))
				{
					set_button_state(B1,FALSE); /* always reset button*/
					blue_duty_cycle = blue_duty_cycle + Percentage_Duty_Cycle_10;
					/* condition added to avoid passing off the max value of 1024*/
					if(MAX_LUM_LED < blue_duty_cycle)
					{
						blue_duty_cycle = MAX_LUM_LED;
					}
					Blue_FlexTimer_update_channel_value(blue_duty_cycle);
					delay(LCD_DELAY);
				}
				if(TRUE == get_button_state(B2))
				{
					set_button_state(B2,FALSE);
					/* condition added to avoid negative numbers and extra clicks necessary to see a change*/
					blue_duty_cycle = blue_duty_cycle - Percentage_Duty_Cycle_10;
					if(LED_OFF > blue_duty_cycle)
					{
						blue_duty_cycle = LED_OFF;
					}
					Blue_FlexTimer_update_channel_value(blue_duty_cycle);
					delay(LCD_DELAY);

				}
				if(TRUE == get_button_state(B3))
				{
					set_button_state(B3,FALSE);
					red_duty_cycle = red_duty_cycle + Percentage_Duty_Cycle_10;
					if(MAX_LUM_LED < red_duty_cycle)
					{
						red_duty_cycle = MAX_LUM_LED;
					}
					Red_FlexTimer_update_channel_value(red_duty_cycle);
					delay(LCD_DELAY);
				}
				if(TRUE == get_button_state(B4))
				{
					set_button_state(B4,FALSE);
					red_duty_cycle = red_duty_cycle - Percentage_Duty_Cycle_10;
					if(LED_OFF > red_duty_cycle)
					{
						red_duty_cycle = LED_OFF;
					}
					Red_FlexTimer_update_channel_value(red_duty_cycle);
					delay(LCD_DELAY);
				}
				if(TRUE == get_button_state(B5))
				{
					set_button_state(B5,FALSE);
					green_duty_cycle = green_duty_cycle + Percentage_Duty_Cycle_10;
					if(MAX_LUM_LED < green_duty_cycle)
					{
						green_duty_cycle = MAX_LUM_LED;
					}
					Green_FlexTimer_update_channel_value(green_duty_cycle);
					delay(LCD_DELAY);
				}
				if(TRUE == get_button_state(B6))
				{
					set_button_state(B6,FALSE);
					green_duty_cycle = green_duty_cycle - Percentage_Duty_Cycle_10;
					if(LED_OFF > green_duty_cycle)
					{
						green_duty_cycle = LED_OFF;
					}
					Green_FlexTimer_update_channel_value(green_duty_cycle);
					delay(LCD_DELAY);
				}
			}

			if(TRUE == get_button_state(SW3))
			{
				/*we initialize functionality, and clear other buttons to ensure led starts off*/
				set_button_state(SW3,FALSE);
				functionality_state = ON;
				set_button_state(B1,FALSE);
				set_button_state(B2,FALSE);
				set_button_state(B3,FALSE);
				set_button_state(B4,FALSE);
				set_button_state(B5,FALSE);
				set_button_state(B6,FALSE);
			}
			if(TRUE == get_button_state(SW2))
			{
				/* We reset values and turn functionality off*/
				set_button_state(SW2,FALSE);
				functionality_state = OFF;
				blue_duty_cycle = 0;
				red_duty_cycle = 0;
				green_duty_cycle = 0;
				RGB_set_OFF();
			}
			if(TRUE == get_button_state(B0))
			{
				/* We return to mainmenu and reset values*/
				set_button_state(B0,FALSE);
				function_state = MAIN_MENU;
				functionality_state = OFF;
				blue_duty_cycle = 0;
				red_duty_cycle = 0;
				green_duty_cycle = 0;
				RGB_set_OFF();
			}
		}
/******************  RGB ADC FUNCTION   **************************************/
/* This function will display on LCD the voltage value given by the potenciometer connected to PTB2
 * Also the RGB led will show a color depending on the voltage value corresponding to pH color*/
		if(RGB_ADC == function_state)
		{
			if(ON == functionality_state)
			{
				uint8_t digit = 0;
				uint16_t integer_number = (uint16_t) RGB_ADC_get();

				if(RGB_ADC_digit_limit >= RGB_ADC_get())
				{
					string_adc_2[3] = 0;
				}
				while(integer_number != 0)
				{
					string_adc_2[digit] = integer_number % 10;
					integer_number /= 10;
					digit++;
				}

				LCD_nokia_clear();
				LCD_nokia_goto_xy(12,1); /*! It establishes the position to print the messages in the LCD*/
				LCD_nokia_send_string(string_adc_1); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(20,2);
				LCD_nokia_send_char('0'); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(26,2);
				LCD_nokia_send_char(string_adc_2[3] + ASCII_CONSTANT); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(32,2);
				LCD_nokia_send_char('.'); /*! It print a string stored in an array*/
				LCD_nexecuteokia_goto_xy(38,2);
				LCD_nokia_send_char(string_adc_2[2] + ASCII_CONSTANT); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(44,2);
				LCD_nokia_send_char(string_adc_2[1] + ASCII_CONSTANT); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(50,2);
				LCD_nokia_send_string(string_adc_4); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(12,3);
				LCD_nokia_send_string(string_adc_5); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(12,4);
				LCD_nokia_send_string(string_adc_6); /*! It print a string stored in an array*/

				PH_Scale_color(RGB_ADC_get());
				delay(LCD_DELAY);
			}
			else
			{
				LCD_nokia_clear();
				LCD_nokia_goto_xy(12,1); /*! It establishes the position to print the messages in the LCD*/
				LCD_nokia_send_string(string_adc_1); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(20,2);
				LCD_nokia_send_string(string_adc_3); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(56,2);
				LCD_nokia_send_string(string_adc_4); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(12,3);
				LCD_nokia_send_string(string_adc_5); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(12,4);
				LCD_nokia_send_string(string_adc_6); /*! It print a string stored in an array*/
				RGB_set_OFF();
				delay(LCD_DELAY);
			}
			if(TRUE == get_button_state(SW2))
			{
				set_button_state(SW2,FALSE);
				functionality_state = OFF;
				delay(LCD_DELAY);
			}
			if(TRUE == get_button_state(SW3))
			{
				set_button_state(SW3,FALSE);
				functionality_state = ON;
				delay(LCD_DELAY);
			}
			if(TRUE == get_button_state(B0))
			{
				set_button_state(B0,FALSE);
				functionality_state = OFF;
				function_state = MAIN_MENU;
				RGB_set_OFF();
			}
		}

/******************  RGB SEQUENCE FUNCTION   **************************************/
/* This function will show a RGB led color sequence depending on the input
 * LCD will show the sequence to be displayed on led
 * Led will change color with 1 sec delay after pressing sw3
 * The input for sequence will be on buttons:
 * b1 -> blue
 * b2 -> red
 * b3 -> green
 * b4 -> yellow
 * b5 -> purple
 * b6 -> white*/
		if(RGB_SEQUENCE == function_state)
		{
			LCD_nokia_clear();
			LCD_nokia_goto_xy(6,2); /*! It establishes the position to print the messages in the LCD*/
			LCD_nokia_send_string(string_sequence_1); /*! It print a string stored in an array*/
			LCD_nokia_goto_xy(30,3);
			LCD_nokia_send_string(string_sequence_2); /*! It print a string stored in an array*/
			LCD_nokia_goto_xy(0,4);
			RGB_sequence_write_color();

			if(TRUE == get_button_state(B1))
			{
				set_button_state(B1,FALSE);
				RGB_sequence_add(B1);
				delay(LCD_DELAY);
			}
			if(TRUE == get_button_state(B2))
			{
				set_button_state(B2,FALSE);
				RGB_sequence_add(B2);
				delay(LCD_DELAY);

			}
			if(TRUE == get_button_state(B3))
			{
				set_button_state(B3,FALSE);
				RGB_sequence_add(B3);
				delay(LCD_DELAY);
			}
			if(TRUE == get_button_state(B4))
			{
				set_button_state(B4,FALSE);
				RGB_sequence_add(B4);
				delay(LCD_DELAY);
			}
			if(TRUE == get_button_state(B5))
			{
				set_button_state(B5,FALSE);
				RGB_sequence_add(B5);
				delay(LCD_DELAY);
			}
			if(TRUE == get_button_state(B6))
			{
				set_button_state(B6,FALSE);
				RGB_sequence_add(B6);
				delay(LCD_DELAY);
			}
			if(TRUE == get_button_state(SW2))
			{
				set_button_state(SW2,FALSE);
				RGB_sequence_state(FALSE);
				RGB_sequence_reset();
				delay(LCD_DELAY);
			}
			if(TRUE == get_button_state(SW3))
			{
				set_button_state(SW3,FALSE);
				RGB_sequence_state(TRUE);
				RGB_sequence_execute();
				delay(LCD_DELAY);
			}
			if(TRUE == get_button_state(B0))
			{
				set_button_state(B0,FALSE);
				function_state = MAIN_MENU;
				RGB_sequence_reset();
				RGB_sequence_state(FALSE);
			}
		}


/******************  RGB FREQUENCY FUNCTION   **************************************/
/* This function will show on LCD the frequency given by the external source connected to PTB18
 * Also the RGB color will show a color corresponding to the frequency, being 1k min and 10k max*/
		if(RGB_FREQUENCY == function_state)
		{

			if(ON == functionality_state) /* we enter here when SW3 is pressed*/
			{
				uint8_t digit = 0;
				float mapped_frequency = 0;
				uint16_t integer_mapped_frequency = 0;
				frequency = get_Frequency();
				uint16_t integer_frequency = (uint16_t) frequency; /* convert float to integer*/

				/* we map from 1k-10k of frequency to 0-4096 */
				mapped_frequency = frequency_slope*frequency + frequency_displacement;
				if(MIN_CHANNEL_VALUE > mapped_frequency) /* we ensure it doesn't pass the min malue 0*/
				{
					integer_mapped_frequency = MIN_CHANNEL_VALUE;
				}
				else if(MAX_CHANNEL_VALUE < mapped_frequency) /*we ensure it doesn't pass the max value 4096*/
				{
					integer_mapped_frequency = MAX_CHANNEL_VALUE;
				}
				else
				{
					integer_mapped_frequency = (uint16_t) mapped_frequency; /* changing from float to int*/
				}

				/*on this function is converted from 0-4096 to ph color scale available on document*/
				PH_Scale_color(integer_mapped_frequency);

				/* this conditions are to avoid a number to stay on LCD when it decrements*/
				if(FIVE_DIGITS > frequency)
				{
					string_frequency_2[4] = 0;
				}
				if(FOUR_DIGITS > frequency)
				{
					string_frequency_2[3] = 0;
				}
				if(THREE_DIGITS > frequency)
				{
					string_frequency_2[2] = 0;
				}

				/* cycle to separate frequency value on digits to be displayed*/
				while(0 != integer_frequency ) /* when reaches 0 all digits have been saved*/
				{
					string_frequency_2[digit] = integer_frequency % 10;
					integer_frequency /= 10;
					digit++;
				}

				LCD_nokia_clear();
				LCD_nokia_goto_xy(30,1); /*! It establishes the position to print the messages in the LCD*/
				LCD_nokia_send_string(string_frequency_1); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(20,2);
				LCD_nokia_send_char(string_frequency_2[4] + ASCII_CONSTANT); /* ASCII has to be added due to conversion from int to char*/
				LCD_nokia_goto_xy(26,2);
				LCD_nokia_send_char(string_frequency_2[3] + ASCII_CONSTANT); /* ASCII has to be added due to conversion from int to char*/
				LCD_nokia_goto_xy(32,2);
				LCD_nokia_send_char('.'); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(38,2);
				LCD_nokia_send_char(string_frequency_2[2] + ASCII_CONSTANT); /* ASCII has to be added due to conversion from int to char*/
				LCD_nokia_goto_xy(44,2);
				LCD_nokia_send_char(string_frequency_2[1] + ASCII_CONSTANT); /* ASCII has to be added due to conversion from int to char*/
				LCD_nokia_goto_xy(50,2);
				LCD_nokia_send_string(string_frequency_4); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(12,3);
				LCD_nokia_send_string(string_frequency_5); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(12,4);
				LCD_nokia_send_string(string_frequency_6); /*! It print a string stored in an array*/
				delay(LCD_DELAY); /* Delay to avoid blinking on LCD text*/
			}

			/* Enters this else when functionality is off*/
			else
			{
				/*prints frequency functionality initial display*/
				LCD_nokia_clear();
				LCD_nokia_goto_xy(30,1); /*! It establishes the position to print the messages in the LCD*/
				LCD_nokia_send_string(string_frequency_1); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(20,2);
				LCD_nokia_send_string(string_frequency_3); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(56,2);
				LCD_nokia_send_string(string_frequency_4); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(12,3);
				LCD_nokia_send_string(string_frequency_5); /*! It print a string stored in an array*/
				LCD_nokia_goto_xy(12,4);
				LCD_nokia_send_string(string_frequency_6); /*! It print a string stored in an array*/
				delay(LCD_DELAY);
			}

			if(TRUE == get_button_state(SW3)) /*frequency displayed and led activated*/
			{
				set_button_state(SW3,FALSE);
				functionality_state = ON;
			}
			if(TRUE == get_button_state(SW2)) /* turn off functionality and led*/
			{
				set_button_state(SW2,FALSE);
				functionality_state = OFF;
				Blue_FlexTimer_update_channel_value(LED_OFF);
				Red_FlexTimer_update_channel_value(LED_OFF);
				Green_FlexTimer_update_channel_value(LED_OFF);
			}

			if(TRUE == get_button_state(B0)) /* we return to main menu state and turn off leds*/
			{
				set_button_state(B0,FALSE);
				function_state = MAIN_MENU;
				functionality_state = OFF;
				Blue_FlexTimer_update_channel_value(LED_OFF);
				Red_FlexTimer_update_channel_value(LED_OFF);
				Green_FlexTimer_update_channel_value(LED_OFF);
			}
		}

	}
	
	return 0;
}

