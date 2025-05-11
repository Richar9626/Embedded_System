/*
 * RTC.h
 *
 *  Created on: Apr 27, 2021
 *      Author: Fernando
 */

#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>

#define RTC_CONTROL_SLAVE_ADDR_7BIT (0x6FU)/*control address of RTCC*/
#define RTC_ADDRESS_size (1U)/*control address*/

#define HOUR_MASK (0x1FU)/*time format mask*/
#define HOUR_MAX (0x24U)/*time format mask*/
#define ST (0x80U)/*mask to start clock*/
#define RESET (0x00U)/*set zero*/

#define RTC_get_status_ASCII_limit (255U)/**/

/**
 *@brief write in the address for specific time or date component for time keeping
 *@param decimal number to write
 *@return state of transfer
 * */
uint32_t RTC_set_sec(uint8_t data);
uint32_t RTC_set_min(uint8_t data);
uint32_t RTC_set_hour(uint8_t data);
uint32_t RTC_set_date(uint8_t data);
uint32_t RTC_set_month(uint8_t data);
uint32_t RTC_set_year(uint8_t data);

/**
 *@brief write 0 in all address
 *@param none
 *@return state of transfer
 * */
uint32_t RTC_disable(void);
/**
 *@brief write in the ST address 1 to start clock
 *@param none
 * */
uint32_t RTC_start(void);

/**
 *@brief read in the address for specific time or date component
 *@param none
 *@return state of transfer
 * */
uint32_t RTC_get_sec(void);
uint32_t RTC_get_min(void);
uint32_t RTC_get_hour(void);
uint32_t RTC_get_date(void);
uint32_t RTC_get_month(void);
uint32_t RTC_get_year(void);

#endif /* RTC_H_ */
