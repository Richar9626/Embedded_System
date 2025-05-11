/*
 * freertos_i2c.h
 *
 *  Created on: 6 nov 2021
 *      Author: Ricardo
 */


#ifndef FREERTOS_I2C_H_
#define FREERTOS_I2C_H_


#include "stdint.h"
#include "fsl_i2c.h"

#define SDA_PIN 3
#define SCL_PIN 2
#define I2C_BAUDRATE 100000U

typedef enum
{
  freertos_i2c0,
  freertos_i2c1
} freertos_i2c_number_t;

typedef enum
{
  freertos_i2c_portA,
  freertos_i2c_portB,
  freertos_i2c_portC,
  freertos_i2c_portD,
  freertos_i2c_portE
} freertos_i2c_port_t;

typedef enum
{
  freertos_i2c_sucess,
  freertos_i2c_fail
} freertos_i2c_flag_t;

typedef enum{
	freertos_i2c_master,
	freertos_i2c_slave
}freertos_i2c_type_t;

typedef struct
{
	uint32_t baudrate;
	freertos_i2c_number_t i2c_number;
	freertos_i2c_port_t port;
	uint8_t sda_pin;
	uint8_t scl_pin;
	freertos_i2c_type_t type_i2c;
	uint8_t slave_address;
	i2c_slave_address_mode_t slave_address_mode;
	uint8_t pull_up;
} freertos_i2c_config_t;

freertos_i2c_flag_t freertos_i2c_init(freertos_i2c_config_t config);

freertos_i2c_flag_t freertos_i2c_master_transfer(freertos_i2c_number_t i2c_number, uint8_t * buffer, uint16_t lenght, i2c_direction_t direction, uint8_t slaveAddR,uint32_t flag_xfer );

freertos_i2c_flag_t freertos_i2c_slave_transfer(freertos_i2c_number_t i2c_number, uint8_t * buffer, uint16_t lenght);

freertos_i2c_flag_t freertos_i2c_receive(freertos_i2c_number_t i2c_number, uint8_t * buffer, uint16_t lenght);

#endif /* FREERTOS_I2C_H_ */
