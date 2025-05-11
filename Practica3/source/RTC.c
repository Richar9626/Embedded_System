/*
 * RTC.c
 *
 *  Created on: Apr 27, 2021
 *      Author: Fernando
 */
#include "RTC.h"
#include "I2C.h"
#include "stdint.h"
#include "MCP7940M_register_map.h"

uint32_t RTC_disable(void) /*reset time data address*/
{
	I2C_write(RTCSEC, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size, RESET);
	I2C_write(RTCMIN, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size,  RESET);
	I2C_write(RTCHOUR, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size,  RESET);
	return I2C_get_status_transfer();
}
uint32_t RTC_start(void)/*start clock of RTCC*/
{
	I2C_write(RTCSEC, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size, ST);
	return I2C_get_status_transfer();
}

uint32_t RTC_set_sec(uint8_t data)/*set seconds data address*/
{
	I2C_write(RTCSEC, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size, data + ST);
	return I2C_get_status_transfer();
}

uint32_t RTC_set_min(uint8_t data)/*set minutes data address*/
{
	I2C_write(RTCMIN, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size, data);
	return I2C_get_status_transfer();
}

uint32_t RTC_set_hour(uint8_t data)/*set hours data address*/
{
	I2C_write(RTCHOUR, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size, data);
	return I2C_get_status_transfer();
}

uint32_t RTC_set_date(uint8_t data)/*set day data address*/
{
	I2C_write(RTCDATE, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size, data);
	return I2C_get_status_transfer();
}
uint32_t RTC_set_month(uint8_t data)/*set month data address*/
{
	I2C_write(RTCMTH, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size, data);
	return I2C_get_status_transfer();
}
uint32_t RTC_set_year(uint8_t data)/*set year data address*/
{
	I2C_write(RTCYEAR, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size, data);
	return I2C_get_status_transfer();
}

uint32_t RTC_get_sec(void)/*get seconds data address or return ERROR state data transfer if TRUE*/
{
	if(RTC_get_status_ASCII_limit < I2C_get_status_transfer())/*if the status transfer if over 255 ASCCII top value return ERROR*/
	{
		return I2C_get_status_transfer();
	}
	else
	{
		return I2C_read(RTCSEC, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size) - ST;
	}
}
uint32_t RTC_get_min(void)/*get minutes data address or return ERROR state data transfer if TRUE*/
{
	if(RTC_get_status_ASCII_limit < I2C_get_status_transfer())/*if the status transfer if over 255 ASCCII top value return ERROR*/
	{
		return I2C_get_status_transfer();
	}
	else
	{
		return I2C_read(RTCMIN, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size);
	}
}
uint32_t RTC_get_hour(void)/*get hours data address or return ERROR state data transfer if TRUE*/
{
	if(RTC_get_status_ASCII_limit < I2C_get_status_transfer())/*if the status transfer if over 255 ASCCII top value return ERROR*/
	{
		return I2C_get_status_transfer();
	}
	else
	{
		return I2C_read(RTCHOUR, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size);
	}
}

uint32_t RTC_get_date(void)/*get day data address or return ERROR state data transfer if TRUE*/
{
	if(RTC_get_status_ASCII_limit < I2C_get_status_transfer())/*if the status transfer if over 255 ASCCII top value return ERROR*/
	{
		return I2C_get_status_transfer();
	}
	else
	{
		return I2C_read(RTCDATE, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size);
	}
}
uint32_t RTC_get_month(void)/*get month data address or return ERROR state data transfer if TRUE*/
{
	if(RTC_get_status_ASCII_limit < I2C_get_status_transfer())/*if the status transfer if over 255 ASCCII top value return ERROR*/
	{
		return I2C_get_status_transfer();
	}
	else
	{
		return I2C_read(RTCMTH, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size);
	}
}
uint32_t RTC_get_year(void)/*get year data address or return ERROR state data transfer if TRUE*/
{
	if(RTC_get_status_ASCII_limit < I2C_get_status_transfer())/*if the status transfer if over 255 ASCCII top value return ERROR*/
	{
		return I2C_get_status_transfer();
	}
	else
	{
		return I2C_read(RTCYEAR, RTC_CONTROL_SLAVE_ADDR_7BIT, RTC_ADDRESS_size);
	}
}

