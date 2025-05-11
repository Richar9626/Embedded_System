/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_CAN            CAN0
#define EXAMPLE_CAN_CLK_SOURCE (kFLEXCAN_ClkSrc1)
#define EXAMPLE_CAN_CLK_FREQ   CLOCK_GetFreq(kCLOCK_BusClk)
/* Set USE_IMPROVED_TIMING_CONFIG macro to use api to calculates the improved CAN / CAN FD timing values. */
#define USE_IMPROVED_TIMING_CONFIG (0U)
#define RX_MESSAGE_BUFFER_NUM      (9)
#define TX_MESSAGE_BUFFER_NUM      (8)
#define DLC                        (8)

/* Fix MISRA_C-2012 Rule 17.7. */
#define LOG_INFO (void)PRINTF
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void CAN_Tx_Task(void * args);
void CAN_Rx_Task(void * args);
void CAN_Rx_KA(void * args);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool txComplete = false;
volatile bool rxComplete = false;
flexcan_handle_t flexcanHandle;
flexcan_mb_transfer_t txXfer, rxXfer, rxKAfer;
flexcan_frame_t txFrame, rxFrame,rxKAFrame ;
flexcan_rx_mb_config_t mbConfig;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief FlexCAN Call Back function
 */
static FLEXCAN_CALLBACK(flexcan_callback)
{
    switch (status)
    {
        /* Process FlexCAN Rx event. */
        case kStatus_FLEXCAN_RxIdle:
            if (RX_MESSAGE_BUFFER_NUM == result)
            {
                rxComplete = true;
            }
            break;

        /* Process FlexCAN Tx event. */
        case kStatus_FLEXCAN_TxIdle:
            if (TX_MESSAGE_BUFFER_NUM == result)
            {
                txComplete = true;
            }
            break;

        default:
            break;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
	flexcan_config_t flexcanConfig;

    /* Initialize board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    LOG_INFO("\r\n==FlexCAN loopback example -- Start.==\r\n\r\n");

    /* Init FlexCAN module. */
    /*
     * flexcanConfig.clkSrc                 = kFLEXCAN_ClkSrc0;
     * flexcanConfig.baudRate               = 1000000U; se tiene que cambiar
     * flexcanConfig.baudRateFD             = 2000000U;
     * flexcanConfig.maxMbNum               = 16;
     * flexcanConfig.enableLoopBack         = false;
     * flexcanConfig.enableSelfWakeup       = false;
     * flexcanConfig.enableIndividMask      = false;
     * flexcanConfig.disableSelfReception   = false;
     * flexcanConfig.enableListenOnlyMode   = false;
     * flexcanConfig.enableDoze             = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig); /*Se tiene que cambiar el baudrate*/

#if defined(EXAMPLE_CAN_CLK_SOURCE)
    flexcanConfig.clkSrc = EXAMPLE_CAN_CLK_SOURCE;
#endif

//  flexcanConfig.enableLoopBack = true; /*Quitamos el loopback para hacerlo externo*/
    flexcanConfig.baudRate = 125000U; /*Modificamos el baudrate*/

    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);


    xTaskCreate(CAN_Tx_Task, "CAN_Tx_Task", configMINIMAL_STACK_SIZE, NULL, (configMAX_PRIORITIES - 1), NULL);
    xTaskCreate(CAN_Rx_Task, "CAN_Rx_Task", configMINIMAL_STACK_SIZE, NULL, (configMAX_PRIORITIES - 2), NULL);
    xTaskCreate(CAN_Rx_KA, "CAN_Rx_KA", configMINIMAL_STACK_SIZE, NULL, (configMAX_PRIORITIES - 2), NULL);

    vTaskStartScheduler();
    while(1){}
}
void CAN_Rx_KA(void * args)
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(50);
	xLastWakeTime = xTaskGetTickCount();
    /* Setup Rx Message Buffer. */
    mbConfig.format = kFLEXCAN_FrameFormatStandard; /* 11 bits*/
    mbConfig.type   = kFLEXCAN_FrameTypeData; /* Datos*/
    mbConfig.id     = FLEXCAN_ID_STD(0x100); /*Cambiamos el ID*/

    FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true); /* Mailbox 9*/

    /* Start receive data through Rx Message Buffer. */
	rxKAfer.mbIdx = (uint8_t)RX_MESSAGE_BUFFER_NUM; /*Pasamos ese mailbox a un frame*/
	rxKAfer.frame = &rxKAFrame;

	for(;;) /*Monitorear que tengamos datos disponibles en el buffer de recepción*/
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		(void)FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxKAfer); /*Monitorea cunado existan datos*/
		if(rxComplete)
		{
			LOG_INFO("\r\nReceived message from MB%d\r\n", RX_MESSAGE_BUFFER_NUM);
			LOG_INFO("rx word0 = 0x%x\r\n", rxKAFrame.dataWord0);
			LOG_INFO("rx word1 = 0x%x\r\n", rxKAFrame.dataWord1);
			rxComplete = false;
		}
	}
}
void CAN_Rx_Task(void * args)
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(50);
	xLastWakeTime = xTaskGetTickCount();
    /* Setup Rx Message Buffer. */
    mbConfig.format = kFLEXCAN_FrameFormatStandard; /* 11 bits*/
    mbConfig.type   = kFLEXCAN_FrameTypeData; /* Datos*/
    mbConfig.id     = FLEXCAN_ID_STD(0x25); /*Cambiamos el ID*/

    FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true); /* Mailbox 9*/

    /* Start receive data through Rx Message Buffer. */
	rxXfer.mbIdx = (uint8_t)RX_MESSAGE_BUFFER_NUM; /*Pasamos ese mailbox a un frame*/
	rxXfer.frame = &rxFrame;

	for(;;) /*Monitorear que tengamos datos disponibles en el buffer de recepción*/
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		(void)FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer); /*Monitorea cunado existan datos*/
		if(rxComplete)
		{
			LOG_INFO("\r\nReceived message from MB%d\r\n", RX_MESSAGE_BUFFER_NUM);
			LOG_INFO("rx word0 = 0x%x\r\n", rxFrame.dataWord0);
			LOG_INFO("rx word1 = 0x%x\r\n", rxFrame.dataWord1);
			rxComplete = false;
		}
	}
}

void CAN_Tx_Task(void * args)
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(5000);
	xLastWakeTime = xTaskGetTickCount();

	/* Setup Tx Message Buffer. */
	FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

	/* Prepare Tx Frame for sending. */
	txFrame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
	txFrame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
	txFrame.id     = FLEXCAN_ID_STD(0x124);
	txFrame.length = (uint8_t)DLC;

	/* Send data through Tx Message Buffer. */
	txXfer.mbIdx = (uint8_t)TX_MESSAGE_BUFFER_NUM;
	txXfer.frame = &txFrame;

	txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x88) | CAN_WORD0_DATA_BYTE_1(0x77) | CAN_WORD0_DATA_BYTE_2(0x66) |
						CAN_WORD0_DATA_BYTE_3(0x55);
	txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x44) | CAN_WORD1_DATA_BYTE_5(0x33) | CAN_WORD1_DATA_BYTE_6(0x22) |
						CAN_WORD1_DATA_BYTE_7(0x11);


	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

		/*Send data trough Tx Message Buffer*/
		(void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
		LOG_INFO("Send message from MB%d to MB%d\r\n", TX_MESSAGE_BUFFER_NUM, RX_MESSAGE_BUFFER_NUM);
		LOG_INFO("tx word0 = 0x%x\r\n", txFrame.dataWord0);
		LOG_INFO("tx word1 = 0x%x\r\n", txFrame.dataWord1);
		txComplete = false;


	}
}
