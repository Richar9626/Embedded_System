/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_dac.h"
#include "fsl_edma.h"
#include "fsl_pdb.h"
#include "fsl_dmamux.h"
#include "fsl_pit.h"
#include "fsl_common.h"
#include "Delay.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


#define DEMO_DAC_USED_BUFFER_SIZE 32U
#define DEMO_PIT_BASEADDR PIT
#define DEMO_PIT_CHANNEL  kPIT_Chnl_0
#define PIT_LED_HANDLER   PIT0_IRQHandler
#define PIT_IRQ_ID        PIT0_IRQn
/* Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
#define LED_INIT()       LED_RED_INIT(LOGIC_LED_ON)
#define LED_TOGGLE()     LED_RED_TOGGLE()

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Initialize the EDMA.
 */
static void EDMA_Configuration(void);
/*!
 * @brief Initialize the DMAMUX.
 */
static void DMAMUX_Configuration(void);
/*!
 * @brief Initialize the PDB.
 */
static void PDB_Configuration(void);
/*!
 * @brief Initialize the DAC.
 */
static void DAC_Configuration(void);
/*!
 * @brief Callback function for EDMA.
 */
static void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds);
/*******************************************************************************
 * Variables
 ******************************************************************************/
edma_handle_t g_EDMA_Handle;                             /* Edma handler */
edma_transfer_config_t g_transferConfig;                 /* Edma transfer config. */
volatile uint32_t g_index                          = 0U; /* Index of the g_dacDataArray array. */
volatile bool pitIsrFlag = false;
uint16_t g_dacDataArray[DEMO_DAC_USED_BUFFER_SIZE] = {
    0U,    401U,  799U,  1188U, 1567U, 1930U, 2275U, 2598U, 2895U, 3165U, 3405U, 3611U, 3783U, 3918U, 4016U, 4075U,
    4095U, 4075U, 4016U, 3918U, 3783U, 3611U, 3405U, 3165U, 2895U, 2598U, 2275U, 1930U, 1567U, 1188U, 799U,  401U};
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
void PIT_LED_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, kPIT_TimerFlag);
    pitIsrFlag = true;
    /* Added for, and affects, all PIT handlers. For CPU clock which is much larger than the IP bus clock,
     * CPU can run out of the interrupt handler before the interrupt flag being cleared, resulting in the
     * CPU's entering the handler again and again. Adding DSB can prevent the issue from happening.
     */
    __DSB();
}

static inline void DMAMUX_EnableAlwaysOn(DMAMUX_Type *base, uint32_t channel, bool enable)
{
    assert(channel < (uint32_t)FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->CHCFG[channel] |= 122;

}

int main(void)
{
    /* Initialize hardware. */
    BOARD_InitPins();
    BOARD_InitDebugConsole();
    pit_config_t pitConfig;

    PRINTF("DAC CONTINUOUS PDB EDMA DEMO\r\n");

    /* Initialize DMAMUX. */
    DMAMUX_Configuration();
    /* Initialize EDMA. */
    EDMA_Configuration();
    /* Initialize the HW trigger source. */
    //PDB_Configuration();
    /* Initialize DAC. */
    DAC_Configuration();

	/**Sets the threshold for interrupts, if the interrupt has higher priority constant that the BASEPRI, the interrupt will not be attended*/
	NVIC_set_basepri_threshold(PRIORITY_0);
	NVIC_enable_interrupt_and_priotity(PORTC_IRQ, PRIORITY_4);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_3);

	/** Callbacks for GPIO */
	GPIO_callback_init(GPIO_C, change_signal);
    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(DEMO_PIT_BASEADDR, &pitConfig);

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, USEC_TO_COUNT(1000000U, PIT_SOURCE_CLOCK));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, kPIT_TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQ_ID);

    /* Start channel 0 */
    PRINTF("\r\nStarting channel No.0 ...");
    PIT_StartTimer(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL);


    PRINTF("Please probe the DAC output with a oscilloscope.\r\n");
    while (1)
    {
        /* Check whether occur interupt and toggle LED */
        if (true == pitIsrFlag)
        {
            PRINTF("\r\n Channel No.0 interrupt is occurred !");
           // EDMA_StartTransfer(&g_EDMA_Handle);
           // EDMA_ClearChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, kEDMA_InterruptFlag);

            pitIsrFlag = false;
        }
    }
}

static void EDMA_Configuration(void)
{
    edma_config_t userConfig;

    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(DEMO_DMA_BASEADDR, &userConfig);
    EDMA_CreateHandle(&g_EDMA_Handle, DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL);
    EDMA_SetCallback(&g_EDMA_Handle, Edma_Callback, NULL);
    EDMA_PrepareTransfer(&g_transferConfig, (void *)(g_dacDataArray + g_index), sizeof(uint16_t),
                         (void *)DAC_DATA_REG_ADDR, sizeof(uint16_t), DAC_DATL_COUNT * sizeof(uint16_t),
                         DAC_DATL_COUNT * sizeof(uint16_t), kEDMA_MemoryToMemory);
    EDMA_SubmitTransfer(&g_EDMA_Handle, &g_transferConfig);
    /* Enable interrupt when transfer is done. */
    EDMA_EnableChannelInterrupts(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, kEDMA_MajorInterruptEnable);
#if defined(FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT) && FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT
    /* Enable async DMA request. */
    EDMA_EnableAsyncRequest(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, true);
#endif /* FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT */
    /* Enable transfer. */
    EDMA_StartTransfer(&g_EDMA_Handle);
}

static void DMAMUX_Configuration(void)
{
    /* Configure DMAMUX */
    DMAMUX_Init(DEMO_DMAMUX_BASEADDR);
    DMAMUX_SetSource(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL, DEMO_DMA_DAC_SOURCE); /* Map ADC source to channel 0 */
    //DMAMUX_EnableAlwaysOn(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL, true);
    DMAMUX_EnablePeriodTrigger(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL);
    DMAMUX_EnableAlwaysOn(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL, true);
    DMAMUX_EnableChannel(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL);
}

static void DAC_Configuration(void)
{
    dac_config_t dacConfigStruct;
    dac_buffer_config_t dacBufferConfigStruct;

    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DEMO_DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DEMO_DAC_BASEADDR, true); /* Enable output. */
    DAC_EnableBuffer(DEMO_DAC_BASEADDR, true);

    DAC_GetDefaultBufferConfig(&dacBufferConfigStruct);
    dacBufferConfigStruct.triggerMode = kDAC_BufferTriggerByHardwareMode;
    DAC_SetBufferConfig(DEMO_DAC_BASEADDR, &dacBufferConfigStruct);
    DAC_SetBufferReadPointer(DEMO_DAC_BASEADDR, 0U); /* Make sure the read pointer to the start. */

    /* Enable DMA. */
    /* Configure the DAC buffer. */
   // DAC_EnableBuffer(DEMO_DAC_BASEADDR, true);
    DAC_EnableBufferInterrupts(DEMO_DAC_BASEADDR, kDAC_BufferReadPointerTopInterruptEnable);
    DAC_EnableBufferDMA(DEMO_DAC_BASEADDR, true);
  //  DAC_DoSoftwareTriggerBuffer(DEMO_DAC_BASEADDR);
}

static void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
	 PRINTF("Entro callb ack.\r\n");
    /* Clear Edma interrupt flag. */
    EDMA_ClearChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, kEDMA_InterruptFlag);
    /* Setup transfer */
    g_index += DAC_DATL_COUNT;
    if (g_index == DEMO_DAC_USED_BUFFER_SIZE)
    {
        g_index = 0U;
    }
    EDMA_PrepareTransfer(&g_transferConfig, (void *)(g_dacDataArray + g_index), sizeof(uint16_t),
                         (void *)DAC_DATA_REG_ADDR, sizeof(uint16_t), DAC_DATL_COUNT * sizeof(uint16_t),
                         DAC_DATL_COUNT * sizeof(uint16_t), kEDMA_MemoryToMemory);
    EDMA_SetTransferConfig(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, &g_transferConfig, NULL);
    /* Enable transfer. */
    EDMA_StartTransfer(&g_EDMA_Handle);

}
