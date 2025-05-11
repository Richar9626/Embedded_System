/*
 * UART.c
 *
 *  Created on: May 2, 2021
 *      Author: rjimenez
 */

#include "fsl_uart.h"
#include "Bits.h"
#include "stdint.h"
#include "UART.h"

/*******************************************************************************
 * Variables uart
 ******************************************************************************/
/*
  Ring buffer for data input and output, in this example, input data are saved
  to ring buffer in IRQ handler. The main function polls the ring buffer status,
  if there are new data, then send them out.
  Ring buffer full: (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) == txIndex)
  Ring buffer empty: (rxIndex == txIndex)
*/
uint8_t buffer0;
uint8_t buffer4;
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */
boolean_t interrupt_flag_uart0 = FALSE;
boolean_t interrupt_flag_uart4 = FALSE;
static uint32_t counter_ERROR = TRUE;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_UART_IRQHandler(void)
{
    uint8_t data;

    /* If new data arrived. */
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(DEMO_UART))
    {
        data = UART_ReadByte(DEMO_UART);
        buffer0 = data;
        interrupt_flag_uart0 = TRUE;
    }
    SDK_ISR_EXIT_BARRIER;
}

void DEMO_UART4_IRQHandler(void)
{
    uint8_t data;

    /* If new data arrived. */
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(DEMO_UART4))
    {
        data = UART_ReadByte(DEMO_UART4);
        buffer4 = data;
        interrupt_flag_uart4 = TRUE;
    }
    SDK_ISR_EXIT_BARRIER;
}


void UART0_init(void)
{
	uart_config_t config;									/*UART config struct*/

	/*
	 * config.baudRate_Bps = 115200U;
	 * config.parityMode = kUART_ParityDisabled;
	 * config.stopBitCount = kUART_OneStopBit;
	 * config.txFifoWatermark = 0;
	 * config.rxFifoWatermark = 1;
	 * config.enableTx = false;
	 * config.enableRx = false;
	 */
	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
	config.enableTx     = true;
	config.enableRx     = true;

	UART_Init(DEMO_UART, &config, DEMO_UART_CLK_FREQ);

    /* Enable RX interrupt. */
    UART_EnableInterrupts(DEMO_UART, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    EnableIRQ(DEMO_UART_IRQn);

}

void UART4_init(void)
{
	uart_config_t config;									/*UART config struct*/

	/*
	 * config.baudRate_Bps = 115200U;
	 * config.parityMode = kUART_ParityDisabled;
	 * config.stopBitCount = kUART_OneStopBit;
	 * config.txFifoWatermark = 0;
	 * config.rxFifoWatermark = 1;
	 * config.enableTx = false;
	 * config.enableRx = false;
	 */
	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
	config.enableTx     = true;
	config.enableRx     = true;

	UART_Init(DEMO_UART4, &config, DEMO_UART4_CLK_FREQ);

    /* Enable RX interrupt. */
    UART_EnableInterrupts(DEMO_UART4, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    EnableIRQ(DEMO_UART4_IRQn);

}

uint8_t UART0_interrupt_status(void)
{
	return interrupt_flag_uart0;
}

void UART0_flag_interrupt_off(void)
{
	interrupt_flag_uart0 = FALSE;
}

uint8_t UART0_pressed_key(void)
{
	return  buffer0;
}

uint8_t UART4_interrupt_status(void)
{
	return interrupt_flag_uart4;
}

void UART4_flag_interrupt_off(void)
{
	interrupt_flag_uart4 = FALSE;
}

uint8_t UART4_pressed_key(void)
{
	return  buffer4;
}

uint8_t UART_status_read_write(uint32_t I2C_status, UART_Type *base)
{
	if(UART_get_status_ASCII_limit < I2C_status)
	{
		UART_WriteBlocking(base, "\033[38:5:166m", sizeof("\033[38:5:166m") - 1);
		UART_WriteBlocking(base, "\033[48:5:0m", sizeof("\033[48:5:0m") - 1);
		UART_WriteBlocking(base, "\033[2J", sizeof("\033[2J") - 1);
		UART_WriteBlocking(base, "\033[10;10H", sizeof("\033[10;10H") - 1);
		//UART_WriteBlocking(base, counter_ERROR, sizeof(counter_ERROR) - 1);
		UART_WriteBlocking(base, ") ERROR: Sin comunicacion con dispositivo I2C", sizeof(") ERROR: Sin comunicación con dispositivo I2C") - 1);
		counter_ERROR ++;
		return FALSE;
	}
	else
	{
		return (uint8_t) I2C_status;
	}

}
void UART_menu_print(UART_Type *base)
{
	UART_WriteBlocking(base, "\033[38:5:166m", sizeof("\033[38:5:166m") - 1);
	UART_WriteBlocking(base, "\033[48:5:0m", sizeof("\033[48:5:0m") - 1);
	UART_WriteBlocking(base, "\033[2J", sizeof("\033[2J") - 1);
	UART_WriteBlocking(base, "\033[10;10H", sizeof("\033[10;10H") - 1);
	UART_WriteBlocking(base, "1) Introducir secuencia", sizeof("1) Introducir secuencia") - 1);
	UART_WriteBlocking(base, "\033[11;10H", sizeof("\033[11;10H") - 1);
	UART_WriteBlocking(base, "2) Introducir valores RGB", sizeof("2) Introducir valores RGB") - 1);
	UART_WriteBlocking(base, "\033[12;10H", sizeof("\033[12;10H"));
}
void UART_RGB_MANUAL_print(UART_Type *base)
{
	UART_WriteBlocking(base, "\033[u", sizeof("\033[u") - 1);
	UART_WriteBlocking(base, "\033[38:5:166m", sizeof("\033[38:5:166m") - 1);
	UART_WriteBlocking(base, "\033[48:5:0m", sizeof("\033[48:5:0m") - 1);
	UART_WriteBlocking(base, "\033[2J", sizeof("\033[2J") - 1);
	UART_WriteBlocking(base, "\033[10;10H", sizeof("\033[10;10H") - 1);
	UART_WriteBlocking(base, "Valores RGB en: 000,000,000", sizeof("Valores RGB en: 000,000,000") - 1);
	UART_WriteBlocking(base, "\033[11;10H", sizeof("\033[11;10H") - 1);
}
void UART_RGB_SEQUENCE_print(UART_Type *base)
{
	UART_WriteBlocking(base, "\033[u", sizeof("\033[u") - 1);
	UART_WriteBlocking(base, "\033[38:5:166m", sizeof("\033[38:5:166m") - 1);
	UART_WriteBlocking(base, "\033[48:5:0m", sizeof("\033[48:5:0m") - 1);
	UART_WriteBlocking(base, "\033[2J", sizeof("\033[2J") - 1);
	UART_WriteBlocking(base, "\033[10;10H", sizeof("\033[10;10H") - 1);
	UART_WriteBlocking(base, "Secuencia", sizeof("Secuencia") - 1);
	UART_WriteBlocking(base, "\033[11;10H", sizeof("\033[11;10H") - 1);
}
