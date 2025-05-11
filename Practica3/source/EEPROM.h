/*
 * EEPROM.c
 *
 *  Created on: May 5, 2021
 *      Author: Fernando
 */

#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>

#define ASCII 48u/*variable to set char to hex*/

#define EEPROM_CONTROL_SLAVE_ADDR_7BIT (0x050U)/*control address of EEPROM*/
#define EEPROM_DATA_LENGTH_write (200U)/*length of array to write*/
#define EEPROM_DATA_LENGTH_read (200U) /*length of array to read / not use*/
#define EEPROM_ADDRESS_size (2U)/*address to write or read high and low limit*/

#define RESET (0x00U)

typedef enum {A_hex = 65, /*10 to 15 in hex, for char conversion*/
			  B_hex,
			  C_hex,
			  D_hex,
			  E_hex,
			  F_hex,
			} hex_t;

/**
 *@brief write the data pointer in the address with the length data in bytes
 *@param address 32 bits
 *@param lenght of string in bytes
 *@param string in array
 * */
uint32_t EEPROM_WRITE(uint16_t data_address,uint8_t lenght_data,uint8_t *data);
/**
 *@brief return data of the EEPROM from the address with the length in bytes to the data array
 *@param address 32 bits
 *@param lenght of string in bytes
 *@param blank array
 * */
uint32_t EEPROM_get_string(uint16_t data_address, uint8_t lenght_data, uint8_t *data);
/**
 *@brief return the char value in hex
 *@param char letter
 * */
uint8_t EEPROM_change_hex(uint8_t character);

#endif /* RTC_H_ */
