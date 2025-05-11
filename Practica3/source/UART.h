/*
 * UART.h
 *
 *  Created on: May 2, 2021
 *      Author: rjimenez
 */

#ifndef UART_H_
#define UART_H_

#include "stdint.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* UART instance and clock */
#define DEMO_UART            UART0
#define DEMO_UART_CLKSRC     UART0_CLK_SRC
#define DEMO_UART_CLK_FREQ   CLOCK_GetFreq(UART0_CLK_SRC)
#define DEMO_UART_IRQn       UART0_RX_TX_IRQn
#define DEMO_UART_IRQHandler UART0_RX_TX_IRQHandler

#define DEMO_UART4            UART4
#define DEMO_UART4_CLKSRC     UART4_CLK_SRC
#define DEMO_UART4_CLK_FREQ   CLOCK_GetFreq(UART4_CLK_SRC)
#define DEMO_UART4_IRQn       UART4_RX_TX_IRQn
#define DEMO_UART4_IRQHandler UART4_RX_TX_IRQHandler

#define BOARD_DEBUG_UART_BAUDRATE 115200
#define UART_get_status_ASCII_limit (255U)
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in c port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in c port */

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 16
#define SOPT5_UART0TXSRC_UART_TX      0x00u   /*!< UART 0 transmit data source select: UART0_TX pin */

#define UART_DELAY 600000
#define UART_2DELAY 1000000

/**
 *@brief initialization of all variables for UART0 and UART4
 *@param none
 * */
void UART_init(void);


/**
 *@brief set interrupt flag of data in to FALSE
 *@param none
 * */
void UART0_flag_interrupt_off(void);
void UART4_flag_interrupt_off(void);
/**
 *@brief write in console the specific menu of the state that the UART is in
 *@param UART to write from
 * */
void UART_menu_print(UART_Type *base);
void UART_set_clock_submenu_print(UART_Type *base);
void UART_set_date_submenu_print(UART_Type *base);
void UART_clock_submenu_print(UART_Type *base);
void UART_date_submenu_print(UART_Type *base);
void UART_set_memory_submenu_print(UART_Type *base);
void UART_get_memory_submenu_print(UART_Type *base);
void UART_busy_submenu_print(UART_Type *base);
/**
 *@brief clear and write in console depending of the status returned, 0 clear/ !=0 ERROR
 *@param status of transfer
 *@param UART
 *@return status if needed
 * */
uint8_t UART_status_read_write(uint32_t I2C_status, UART_Type *base);
/**
 *@brief interrupt flag for status keeping
 *@param none
 *@return interrupt flag
 * */
uint8_t UART0_interrupt_status(void);
uint8_t UART4_interrupt_status(void);
/**
 *@brief return the key press for to write
 *@param none
 *@return buffer of key press
 * */
uint8_t UART0_pressed_key(void);
uint8_t UART4_pressed_key(void);



#endif /* UART_H_ */
