/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include <string.h>
/*  SDK Included Files */
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "fsl_uart.h"
#include "clock_config.h"
/*Custom field*/
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

#define I2C_MASTER_SLAVE_ADDR_7BIT 0x6FU
#define I2C_BAUDRATE               100000U
#define I2C_DATA_LENGTH            1U

/*UART polling define*/
#define DEMO_UART          UART0							/*UART chosen*/
#define DEMO_UART_CLKSRC   UART0_CLK_SRC					/*core/system clock*//*not used*/
#define DEMO_UART_CLK_FREQ CLOCK_GetFreq(UART0_CLK_SRC) 	/*base clock DEMO_UART_CLK_FREQ = 120000000*/

/*custom structure*/
typedef enum{MAIN_MENU, CLOCK, SET_CLOCK} function_t;		/*menu of states*/
typedef enum{OFF, ON} functionality_state_t;				/*print menu or submenu*/
/*******************************************************************************
 * Variables uart
 ******************************************************************************/
uint8_t rxbuff[20] = {0};
/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_master_txBuff[I2C_DATA_LENGTH];
uint8_t g_master_rxBuff[I2C_DATA_LENGTH];

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
	uint8_t ch;												/*char character variable for UART*/
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

    i2c_master_config_t masterConfig;
    uint32_t sourceClock;
    i2c_master_transfer_t masterXfer;

    BOARD_InitPins();										/*pin configuration*//*important*/
    BOARD_BootClockRUN();
    //BOARD_InitDebugConsole();

	UART_Init(DEMO_UART, &config, DEMO_UART_CLK_FREQ);

	function_t function_state = MAIN_MENU;					/*starting menu state*/
	functionality_state_t functionality_state = TRUE;		/*starting print*/
//////////////////////////////////////////////////////////////////////////////////////////////////

    /* Set up i2c master to send data to slave*/
    /* First data in txBuff is data length of the transmiting data. */
    g_master_txBuff[0] = I2C_DATA_LENGTH - 1U;
    for (uint32_t i = 1U; i < I2C_DATA_LENGTH; i++)
    {
        g_master_txBuff[i] = i - 1;
    }

    /*
     * masterConfig->baudRate_Bps = 100000U;
     * masterConfig->enableStopHold = false;
     * masterConfig->glitchFilterWidth = 0U;
     * masterConfig->enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = I2C_BAUDRATE;

    sourceClock = I2C_MASTER_CLK_FREQ;

    I2C_MasterInit(EXAMPLE_I2C_MASTER_BASEADDR, &masterConfig, sourceClock);
    g_master_txBuff[0]= 15;
    memset(&masterXfer, 0, sizeof(masterXfer));

    /* subAddress = 0x01, data = g_master_txBuff - write to slave.
      start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
    uint8_t deviceAddress     = 0x01U;
    masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = (uint32_t)RTCMIN;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = g_master_txBuff;/*data from master*/
    masterXfer.dataSize       = 0x01u;
    masterXfer.flags          = kI2C_TransferDefaultFlag;


    I2C_MasterTransferBlocking(EXAMPLE_I2C_MASTER_BASEADDR, &masterXfer); /*Master will send data*/

    /* Wait until the slave is ready for transmit, wait time depend on user's case.
       Slave devices that need some time to process received byte or are not ready yet to
       send the next byte, can pull the clock low to signal to the master that it should wait.*/
    for (uint32_t i = 0U; i < WAIT_TIME; i++)
    {
        __NOP();
    }

    /* subAddress = 0x01, data = g_master_rxBuff - read from slave.
      start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
    masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction      = kI2C_Read;
    masterXfer.subaddress     = (uint32_t)RTCMIN;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = g_master_rxBuff; /*data from slave*/
    masterXfer.dataSize       = I2C_DATA_LENGTH - 1U;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(EXAMPLE_I2C_MASTER_BASEADDR, &masterXfer); /*Receive from slave*/

    for (uint32_t i = 0U; i < I2C_DATA_LENGTH - 1; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_rxBuff[i]);
    }
    PRINTF("\r\n\r\n");

    /* Transfer completed. Check the data.
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH - 1U; i++)
    {
        if (g_master_rxBuff[i] != g_master_txBuff[i + 1])
        {
            PRINTF("\r\nError occurred in the transfer ! \r\n");
            break;
        }
    }*/

    while (1)
    {
    	for(;;)
		{
			if(MAIN_MENU == function_state)
			{
				if(TRUE == functionality_state)
				{
					UART_WriteBlocking(DEMO_UART, "\033[0;35;43m", sizeof("\033[0;35;43m") - 1);
					UART_WriteBlocking(DEMO_UART, "\033[2J", sizeof("\033[2J") - 1);
					UART_WriteBlocking(DEMO_UART, "\033[10;10H", sizeof("\033[10;10H") - 1);
					UART_WriteBlocking(DEMO_UART, "1) Leer Hora", sizeof("1) Leer Hora") - 1);
					UART_WriteBlocking(DEMO_UART, "\033[11;10H", sizeof("\033[11;10H") - 1);
					UART_WriteBlocking(DEMO_UART, "2) Configurar Hora", sizeof("2) Configurar Hora") - 1);
					UART_WriteBlocking(DEMO_UART, "\033[12;10H", sizeof("\033[12;10H") - 1);
					functionality_state = FALSE;
				}
				if(49 == ch)
				{
					function_state = CLOCK;
					functionality_state = TRUE;
				}
				if(50 == ch)
				{
					function_state = SET_CLOCK;
					functionality_state = TRUE;
				}
			}
			if(CLOCK == function_state)
			{
				if(TRUE == functionality_state)
				{
					UART_WriteBlocking(DEMO_UART, "\033[0;35;43m", sizeof("\033[0;35;43m") - 1);
					UART_WriteBlocking(DEMO_UART, "\033[2J", sizeof("\033[2J") - 1);
					UART_WriteBlocking(DEMO_UART, "\033[10;10H", sizeof("\033[10;10H") - 1);
					UART_WriteBlocking(DEMO_UART, "1) Leer Hora", sizeof("1) Leer Hora") - 1);
					UART_WriteBlocking(DEMO_UART, "\033[11;10H", sizeof("\033[11;10H") - 1);
					UART_WriteBlocking(DEMO_UART, "12:3520 am", sizeof("12:3520 am") - 1);
					UART_WriteBlocking(DEMO_UART, "\033[12;10H", sizeof("\033[12;10H") - 1);
					functionality_state = FALSE;
				}
				else
				{

				}
				if(27 == ch)
				{
					function_state = MAIN_MENU;
					functionality_state = TRUE;
				}
			}
			if(SET_CLOCK == function_state)
			{
				if(TRUE == functionality_state)
				{
					UART_WriteBlocking(DEMO_UART, "\033[0;35;43m", sizeof("\033[0;35;43m") - 1);
					UART_WriteBlocking(DEMO_UART, "\033[2J", sizeof("\033[2J") - 1);
					UART_WriteBlocking(DEMO_UART, "\033[10;10H", sizeof("\033[10;10H") - 1);
					UART_WriteBlocking(DEMO_UART, "Introducir hora actual en formato HH:MM:SS", sizeof("Introducir hora actual en formato HH:MM:SS") - 1);
					UART_WriteBlocking(DEMO_UART, "\033[11;10H", sizeof("\033[11;10H") - 1);
					functionality_state = FALSE;
				}
				else
				{

				}
				if(27 == ch)
				{
					function_state = MAIN_MENU;
					functionality_state = TRUE;
				}
			}
			UART_ReadBlocking(DEMO_UART, &ch, 1);
			#ifndef DEBUG_ON
				printf("PRESSED %d\n",ch);
			#endif
		}
    }
}
