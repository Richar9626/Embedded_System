/*
 * I2C.h
 *
*  Created on: May 5, 2021
 *      Author: Fernando
 */

#ifndef I2C_H_
#define I2C_H_

#include <string.h>
#include <stdint.h>
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "clock_config.h"
#include "Bits.h"
#include "MCP7940M_register_map.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* I2C source clock */
#define I2C_MASTER_CLK_SRC          I2C0_CLK_SRC 			/*bus clock*//*not used*/
#define I2C_MASTER_CLK_FREQ         CLOCK_GetFreq(I2C0_CLK_SRC)/*bus clock = 20971520*/
#define EXAMPLE_I2C_MASTER_BASEADDR I2C0
#define WAIT_TIME                   10U
#define I2C_DATA_LENGTH (0x01U)

#define I2C_BAUDRATE               400000U

#define PIN3_IDX                        3u   /*!< Pin number for pin 24 in a port */
#define PIN2_IDX                        2u   /*!< Pin number for pin 25 in a port */

/**
 *@brief initialization of all variables for I2C
 *@param none
 * */
void I2C_init(void);
/**
 *@brief write in the specific address to the specific slave
 *@param address of slave
 *@param control address of slave
 *@param size in bytes of the data in the slave
 *@param string to save in address of slave
 * */
void I2C_write(uint16_t register_map_address, uint8_t I2C_CONTROL_SLAVE_ADDR_7BIT, uint8_t register_map_address_size, uint8_t DATA_byte);
/**
 *@brief read the specific address to the specific slave
 *@param address of slave
 *@param control address of slave
 *@param size in bytes of the data in the slave
 *@return value from address read
 * */
uint8_t I2C_read(uint16_t register_map_address, uint8_t I2C_CONTROL_SLAVE_ADDR_7BIT, uint8_t register_map_address_size);
/**
 *@brief send flag status of transferred data
 *@param none
 *@return state of transfer
 * */
uint32_t I2C_get_status_transfer (void);

#endif /* I2C_H_ */
