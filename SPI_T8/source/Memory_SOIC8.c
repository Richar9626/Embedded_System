/*
 * Memory_SOIC8.c
 *
 *  Created on: Mar 26, 2021
 *      Author: rjimenez
 */
#include <stdio.h>
#include "stdint.h"
#include "Bits.h"
#include "Memory_SOIC8.h"
#include "LCD_nokia.h"
#include "GPIO.h"
#include "SPI.h"


uint8_t  g_Image_1[Total_Image_Bytes] = {0}; /* Imagen garabada en memoria 0x7000 */
uint8_t  g_Image_2[Total_Image_Bytes] = {0}; /* Imagen garabada en memoria 0x8000 */
uint8_t  g_Image_3[Total_Image_Bytes] = {0}; /* Imagen garabada en memoria 0x9000 */
uint8_t  g_Image_4[Total_Image_Bytes] = {0}; /* Imagen garabada en memoria 0xA000 */
uint8_t  g_Image_5[Total_Image_Bytes] = {0}; /* Imagen garabada en memoria 0xB000 */

uint16_t index = 0;


uint8_t*  READ_IMAGE(Memory_Images_t image_number){
	uint16_t byte = 0;
	uint16_t Sector_address = 0;
	index = 0;

	switch(image_number)
	{
		case IMG_memory_1:
			Sector_address = Sector_Img1;
			break;
		case IMG_memory_2:
			Sector_address = Sector_Img2;
			break;
		case IMG_memory_3:
			Sector_address = Sector_Img3;
			break;
		case IMG_memory_4:
			Sector_address = Sector_Img4;
			break;
		case IMG_memory_5:
			Sector_address = Sector_Img5;
			break;
		default:
			break;
	}
	for (byte = 0 ; byte < Total_Image_Bytes; byte++)
		Memory_Read_byte(Sector_address+byte, image_number);

	switch(image_number)
	{
		case IMG_memory_1:
			return(g_Image_1);
		case IMG_memory_2:
			return(g_Image_2);
		case IMG_memory_3:
			return(g_Image_3);
		case IMG_memory_4:
			return(g_Image_4);
		case IMG_memory_5:
			return(g_Image_5);
		default:
			break;

	}
return(0);
}



void Memory_Read_byte(uint32_t Sector_address, Memory_Images_t Image )
{

	uint8_t DATA_read = 0;

	uint8_t  dirByte1	=  0x00;
	uint8_t  dirByte2	=  0x00;
	uint8_t  dirByte3	=  0x00;

	dirByte1 = (Sector_address >> 0 ) & (0xFF);
	dirByte2 = (Sector_address >> 8 ) & (0xFF);
	dirByte3 = (Sector_address >> 16) & (0xFF);

	GPIO_clear_pin(GPIO_D, CS_mem);

	SPI_start_tranference(SPI_0);
	DATA_read = SPI_tranference(SPI_0, Read_instruction);
	DATA_read = SPI_tranference(SPI_0, dirByte3);
	DATA_read = SPI_tranference(SPI_0, dirByte2);
	DATA_read = SPI_tranference(SPI_0, dirByte1);
	DATA_read = SPI_tranference(SPI_0, 0x00);

	GPIO_set_pin(GPIO_D, CS_mem);
	SPI_stop_tranference(SPI_0);

	if (index >= Total_Image_Bytes){
		index = 0;
	}

	if ((index >= 0) && (index < Total_Image_Bytes))
	{
		if (IMG_memory_1 == Image)
			g_Image_1[index] = DATA_read;
		if (IMG_memory_2 == Image)
			g_Image_2[index] = DATA_read;
		if (IMG_memory_3 == Image)
			g_Image_3[index] = DATA_read;
		if (IMG_memory_4 == Image)
			g_Image_4[index] = DATA_read;
		if (IMG_memory_5 == Image)
			g_Image_5[index] = DATA_read;

	}

	index++;
}

