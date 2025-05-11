/**
	\file 
	\brief 
		This is a starter file to use the Nokia 5510 LCD. 
		The LCD is connected as follows:
		RST-PDT5
		CE-PTD6
		DC-PTD4
		DIN-PTD2
		CLK-PTD1
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	19/09/2019
	\todo
	    The SPI device driver needs to be completed.

	    read del SPI y el almacenamiento de datos de la memoria
 */

 
#include "SPI.h"
#include "GPIO.h"
#include "Bits.h"
#include "LCD_nokia.h"
#include "LCD_nokia_images.h"
#include "stdint.h"
#include "Delay.h"
#include "Memory_SOIC8.h"


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

/*! This array hold the initial picture that is shown in the LCD. Note that extern should be avoided*/

/* Images on K64 available
 * ITESO
 * Patriots*/


int main(void)
{
	uint8_t string_1[]="ITESO"; /*! String to be printed in the LCD*/
	uint8_t string_2[]="Sistemas"; /*! String to be printed in the LCD*/
	uint8_t string_3[]="Embebidos I"; /*! String to be printed in the LCD*/
	uint8_t string_4[]="IE"; /*! String to be printed in the LCD*/

	SPI_init(&g_spi_config); /*! Configuration function for the LCD port*/

	LCD_nokia_init(); /*! Configuration function for the LCD */

	uint8_t* ptr_array_IMG1 = READ_IMAGE(IMG_memory_1);
	uint8_t* ptr_array_IMG2 = READ_IMAGE(IMG_memory_2);
	uint8_t* ptr_array_IMG3 = READ_IMAGE(IMG_memory_3);
	uint8_t* ptr_array_IMG4 = READ_IMAGE(IMG_memory_4);
	uint8_t* ptr_array_IMG5 = READ_IMAGE(IMG_memory_5);

	uint8_t *LCD_Display = get_image(Patriots);
	for(;;) {

		LCD_nokia_clear();/*! It clears the information printed in the LCD*/
		LCD_nokia_bitmap(LCD_Display); /*! It prints an array that hold an image, in this case is the initial picture*/
		delay(650000);

/* */ 	//Modulo que imprime en LCD los arreglos leidos de memoria//
		LCD_nokia_clear();/*! It clears the information printed in the LCD*/
		LCD_nokia_bitmap(ptr_array_IMG1); /*! It prints an array that hold an image, in this case is the initial picture*/
		delay(650000);

		LCD_nokia_clear();/*! It clears the information printed in the LCD*/
		LCD_nokia_bitmap(ptr_array_IMG2); /*! It prints an array that hold an image, in this case is the initial picture*/
		delay(650000);

		LCD_nokia_clear();/*! It clears the information printed in the LCD*/
		LCD_nokia_bitmap(ptr_array_IMG3); /*! It prints an array that hold an image, in this case is the initial picture*/
		delay(65000);

		LCD_nokia_clear();/*! It clears the information printed in the LCD*/
		LCD_nokia_bitmap(ptr_array_IMG4); /*! It prints an array that hold an image, in this case is the initial picture*/
		delay(650000);

		LCD_nokia_clear();/*! It clears the information printed in the LCD*/
		LCD_nokia_bitmap(ptr_array_IMG5); /*! It prints an array that hold an image, in this case is the initial picture*/
		delay(650000);
/* */
		LCD_nokia_clear();
		delay(650000);
		LCD_nokia_clear();
		LCD_nokia_goto_xy(20,0); /*! It establishes the position to print the messages in the LCD*/
		LCD_nokia_send_string(&string_1[0]); /*! It print a string stored in an array*/
		delay(650000);
		LCD_nokia_goto_xy(12,1);
		LCD_nokia_send_string(string_2); /*! It print a string stored in an array*/
		delay(650000);
		LCD_nokia_goto_xy(4,2);
		LCD_nokia_send_string(string_3); /*! It print a string stored in an array*/
		delay(650000);
		LCD_nokia_goto_xy(30,3);
		LCD_nokia_send_string(string_4); /*! It print a string stored in an array*/
		delay(650000);
		LCD_nokia_goto_xy(24,4);
		LCD_nokia_send_char('2'); /*! It prints a character*/
		LCD_nokia_send_char('0'); /*! It prints a character*/
		LCD_nokia_send_char('2'); /*! It prints a character*/
		LCD_nokia_send_char('1'); /*! It prints a character*/
		delay(650000);
	}
	
	return 0;
}

