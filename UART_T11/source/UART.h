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

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 16

void UART0_init(void);
void UART4_init(void);

void UART0_flag_interrupt_off(void);
void UART4_flag_interrupt_off(void);
void UART_menu_print(UART_Type *base);
void UART_set_clock_submenu_print(UART_Type *base);
void UART_set_date_submenu_print(UART_Type *base);
void UART_clock_submenu_print(UART_Type *base);
void UART_date_submenu_print(UART_Type *base);
void UART_set_memory_submenu_print(UART_Type *base);
void UART_get_memory_submenu_print(UART_Type *base);
uint8_t UART_status_read_write(uint32_t I2C_status, UART_Type *base);
uint8_t UART0_interrupt_status(void);
uint8_t UART4_interrupt_status(void);
uint8_t UART0_pressed_key(void);
uint8_t UART4_pressed_key(void);



#endif /* UART_H_ */
