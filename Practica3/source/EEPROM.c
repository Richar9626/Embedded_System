/*
 * EEPROM.c
 *
 *  Created on: May 5, 2021
 *      Author: Fernando
 */

#include "EEPROM.h"
#include "I2C.h"
#include "stdint.h"
#include "Bits.h"
#include "fsl_uart.h"
#include "UART.h"


uint32_t EEPROM_WRITE(uint16_t data_address, uint8_t lenght_data, uint8_t *data)
{
	uint16_t data_address_hex = data_address;

	for(uint8_t array_counter = FALSE;lenght_data > array_counter; array_counter++) /*send to EEPROM all the array by moving through the array*/
	{
		I2C_write(data_address_hex, EEPROM_CONTROL_SLAVE_ADDR_7BIT, EEPROM_ADDRESS_size, data[array_counter]);/*use IC2 to write the EERPOM*/
#ifndef DEBUG_ON
	printf("escribio: %c\n", data[array_counter]);
#endif
		data_address_hex++;/*move to next address*/
		delay(1000);/*delay for nack*/
	}
return I2C_get_status_transfer();
}

uint32_t EEPROM_get_string(uint16_t data_address, uint8_t lenght_data, uint8_t *data)
{
	uint8_t data_address_hex = data_address;
	uint8_t g_rxBuff[EEPROM_DATA_LENGTH_read] = {0};

	for(uint8_t array_counter = FALSE;lenght_data > array_counter; array_counter++)/*send to EEPROM all the array by moving through the array*/
	{
		g_rxBuff[array_counter] = I2C_read(data_address_hex, EEPROM_CONTROL_SLAVE_ADDR_7BIT, EEPROM_ADDRESS_size); /*use IC2 to read the EERPOM*/
		data[array_counter] = g_rxBuff[array_counter]; /*write the value from EEPROM to main array*/
		data_address_hex++;/*move to next address*/
		delay(1000);/*delay for nack*/
	}
return I2C_get_status_transfer();
}

uint8_t EEPROM_change_hex(uint8_t character)
{
	uint8_t hex_value;
	switch(character) /*search for value of char to change*/
	{
		case A_hex:
			hex_value = 0x0A;
			break;
		case B_hex:
			hex_value = 0x0B;
			break;
		case C_hex:
			hex_value = 0x0C;
			break;
		case D_hex:
			hex_value = 0x0D;
			break;
		case E_hex:
			hex_value = 0x0E;
			break;
		case F_hex:
			hex_value = 0x0F;
			break;
		default:
			hex_value = character - ASCII; /*if not found return decimal value - 48*/
			break;
	}
	return hex_value;
}
