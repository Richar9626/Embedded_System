/*
 * UART.c
 *
 *  Created on: May 2, 2021
 *      Author: rjimenez
 */

#include "fsl_uart.h"
#include "fsl_port.h"
#include "Bits.h"

#include "stdint.h"
#include "UART.h"
#include "I2C.h"

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
static uint8_t g_buffer0;
static uint8_t g_buffer4;
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */
static boolean_t g_interrupt_flag_uart0 = FALSE;
static boolean_t g_interrupt_flag_uart4 = FALSE;


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
        g_buffer0 = data;
        g_interrupt_flag_uart0 = TRUE;
    }
    SDK_ISR_EXIT_BARRIER;
}

void UART4_RX_TX_IRQHandler(void)
{
    uint8_t data;

    /* If new data arrived. */
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(DEMO_UART4))
    {
        data = UART_ReadByte(DEMO_UART4);
        g_buffer4 = data;
        g_interrupt_flag_uart4 = TRUE;
    }
    SDK_ISR_EXIT_BARRIER;
}


void UART_init(void)
{
	uart_config_t config;									/*UART config struct*/

	PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAlt3);           /* PORTB16 (pin 62) is configured as UART0_RX */
	PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAlt3);           /* PORTB17 (pin 63) is configured as UART0_TX */
	PORT_SetPinMux(PORTC, PIN14_IDX, kPORT_MuxAlt3);           /* PORTB16 (pin 62) is configured as UART0_RX */
	PORT_SetPinMux(PORTC, PIN15_IDX, kPORT_MuxAlt3);           /* PORTB17 (pin 63) is configured as UART0_TX */

	SIM->SOPT5 = ((SIM->SOPT5 &
	(~(SIM_SOPT5_UART0TXSRC_MASK)))                          /* Mask bits to zero which are setting */
	  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART 0 transmit data source select: UART0_TX pin */
	);
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

	UART_Init(UART4, &config, DEMO_UART4_CLK_FREQ);

    /* Enable RX interrupt. */
    UART_EnableInterrupts(UART4, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    EnableIRQ(UART4_RX_TX_IRQn);
}


uint8_t UART0_interrupt_status(void)
{
	return g_interrupt_flag_uart0;
}

void UART0_flag_interrupt_off(void)
{
	g_interrupt_flag_uart0 = FALSE;
}

uint8_t UART0_pressed_key(void)
{
	return  g_buffer0;
}



void UART_menu_print(UART_Type *base)/*write in specific UART the menu*/
{
	UART_WriteBlocking(base, "\033[38:5:166m", sizeof("\033[38:5:166m") - 1);
	UART_WriteBlocking(base, "\033[48:5:0m", sizeof("\033[48:5:0m") - 1);
	UART_WriteBlocking(base, "\033[2J", sizeof("\033[2J") - 1);
	UART_WriteBlocking(base, "\033[10;10H", sizeof("\033[10;10H") - 1);
	UART_WriteBlocking(base, "1) Introducir secuencia", sizeof("1) Introducir secuencia") - 1);
	UART_WriteBlocking(base, "\033[11;10H", sizeof("\033[11;10H") - 1);
	UART_WriteBlocking(base, "2) Introducir valores RGB", sizeof("2) Introducir valores RGB") - 1);
}

void UART_sequence_print(UART_Type *base)/*write in specific UART the menu*/
{
	UART_WriteBlocking(base, "\033[38:5:166m", sizeof("\033[38:5:166m") - 1);
	UART_WriteBlocking(base, "\033[48:5:0m", sizeof("\033[48:5:0m") - 1);
	UART_WriteBlocking(base, "\033[2J", sizeof("\033[2J") - 1);
	UART_WriteBlocking(base, "\033[10;10H", sizeof("\033[10;10H") - 1);
	UART_WriteBlocking(base, "secuencia", sizeof("secuencia") - 1);
	UART_WriteBlocking(base, "\033[11;10H", sizeof("\033[11;10H") - 1);
	//UART_WriteBlocking(base, "2) Introducir valores RGB", sizeof("2) Introducir valores RGB") - 1);
}

void UART_values_print(UART_Type *base)/*write in specific UART the menu*/
{
	UART_WriteBlocking(base, "\033[38:5:166m", sizeof("\033[38:5:166m") - 1);
	UART_WriteBlocking(base, "\033[48:5:0m", sizeof("\033[48:5:0m") - 1);
	UART_WriteBlocking(base, "\033[2J", sizeof("\033[2J") - 1);
	UART_WriteBlocking(base, "\033[10;10H", sizeof("\033[10;10H") - 1);
	UART_WriteBlocking(base, "Valores:", sizeof("Valores:") - 1);
	UART_WriteBlocking(base, "\033[11;10H", sizeof("\033[11;10H") - 1);
	//UART_WriteBlocking(base, "2) Introducir valores RGB", sizeof("2) Introducir valores RGB") - 1);
}
