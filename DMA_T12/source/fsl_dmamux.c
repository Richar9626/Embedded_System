/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_dmamux.h"
#include "fsl_dac.h"
#include "fsl_edma.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.dmamux"
#endif

uint16_t g_SinWave[BUFFER_LENGTH] = {2047,2452,2841,3199,3511,3765,3951,4062,4093,4044,3915,3713,3445,3121,2755,2361,1954,1552,1168,820,520,280,111,18,5,73,218,437,719,1053,1427,1825};
//AT_NONCACHEABLE_SECTION_ALIGN_INIT(uint32_t g_SinWave[BUFFER_LENGTH], 16) = {2047,2452,2841,3199,3511,3765,3951,4062};
uint16_t g_TriangularWave [] = {0,130,260,389,519,649,779,908,1038,1168,1298,1428,1557,1687,1817,1947,2076,2206,2336,2466,2596,2725,2855,2985,3115,3244,3374,3504,3634,3764,3893,4023};
uint16_t g_SquareWave [] = {4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
edma_handle_t g_EDMA_Handle;
/* Allocate TCD memory poll */
AT_NONCACHEABLE_SECTION_ALIGN(edma_tcd_t tcdMemoryPoolPtr[TCD_QUEUE_SIZE + 1], sizeof(edma_tcd_t));
volatile bool g_Transfer_Done = false;
AT_NONCACHEABLE_SECTION_ALIGN_INIT(uint32_t srcAddr[BUFFER_LENGTH], 16)  = {0x01U, 0x02U, 0x03U, 0x04U,
                                                                           0x05U, 0x06U, 0x07U, 0x08U};
AT_NONCACHEABLE_SECTION_ALIGN_INIT(uint32_t destAddr[BUFFER_LENGTH], 16) = {0x00U, 0x00U, 0x00U, 0x00U,
                                                                            0x00U, 0x00U, 0x00U, 0x00U};
uint8_t g_index = 0;
edma_transfer_config_t transferConfig;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get instance number for DMAMUX.
 *
 * @param base DMAMUX peripheral base address.
 */
static uint32_t DMAMUX_GetInstance(DMAMUX_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Array to map DMAMUX instance number to base pointer. */
static DMAMUX_Type *const s_dmamuxBases[] = DMAMUX_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Array to map DMAMUX instance number to clock name. */
static const clock_ip_name_t s_dmamuxClockName[] = DMAMUX_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint32_t DMAMUX_GetInstance(DMAMUX_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_dmamuxBases); instance++)
    {
        if (s_dmamuxBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_dmamuxBases));

    return instance;
}

/*!
 * brief Initializes the DMAMUX peripheral.
 *
 * This function ungates the DMAMUX clock.
 *
 * param base DMAMUX peripheral base address.
 *
 */
void DMAMUX_Init(DMAMUX_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    CLOCK_EnableClock(s_dmamuxClockName[DMAMUX_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

/*!
 * brief Deinitializes the DMAMUX peripheral.
 *
 * This function gates the DMAMUX clock.
 *
 * param base DMAMUX peripheral base address.
 */
void DMAMUX_Deinit(DMAMUX_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    CLOCK_DisableClock(s_dmamuxClockName[DMAMUX_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}
static inline void DMAMUX_EnableAlwaysOn(DMAMUX_Type *base, uint32_t channel, bool enable)
{
    assert(channel < (uint32_t)FSL_FEATURE_DMAMUX_MODULE_CHANNEL);
    base->CHCFG[channel] |= 0x7A;
}

void EDMA_Configuration(void)
{
    edma_config_t userConfig;

    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(DEMO_DMA_BASEADDR, &userConfig);
    EDMA_CreateHandle(&g_EDMA_Handle, DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL);
    EDMA_SetCallback(&g_EDMA_Handle, Edma_Callback, NULL);
    EDMA_PrepareTransfer(&transferConfig, (void *)(g_SinWave + g_index), sizeof(uint16_t),
                         (void *)DAC_DATA_REG_ADDR, sizeof(uint16_t), 1 * sizeof(uint16_t),
                         1 * sizeof(uint16_t), kEDMA_MemoryToPeripheral);
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

void DMAMUX_Configuration(void)
{
    /* Configure DMAMUX */
    DMAMUX_Init(DEMO_DMAMUX_BASEADDR);
    DMAMUX_EnableAlwaysOn(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL, true);
    DMAMUX_EnableChannel(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL);
}
void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
	if(SIN == GPIO_Signal_Select())
	{
	    /* Clear Edma interrupt flag. */
	    EDMA_ClearChannelStatusFlags(EXAMPLE_DMA, DEMO_EDMA_CHANNEL_0, kEDMA_InterruptFlag);
	    /* Setup transfer */
	//    g_index += DAC_DATL_COUNT;
	    g_index += 1;
	    if (g_index == BUFFER_LENGTH)
	    {
	        g_index = 0U;
	    }

	    EDMA_PrepareTransfer(&transferConfig, (void *)(g_SinWave + g_index), sizeof(uint16_t),
	                             (void *)DAC_DATA_REG_ADDR, sizeof(uint16_t), 1 * sizeof(uint16_t),
	                             1 * sizeof(uint16_t), kEDMA_MemoryToPeripheral);

	    EDMA_SetTransferConfig(EXAMPLE_DMA, DEMO_EDMA_CHANNEL_0, &transferConfig, NULL);
	    /* Enable transfer. */
	    EDMA_StartTransfer(&g_EDMA_Handle);
	}

	else if(TRI == GPIO_Signal_Select())
	{
	    /* Clear Edma interrupt flag. */
	    EDMA_ClearChannelStatusFlags(EXAMPLE_DMA, DEMO_EDMA_CHANNEL_0, kEDMA_InterruptFlag);
	    /* Setup transfer */
	//    g_index += DAC_DATL_COUNT;
	    g_index += 1;
	    if (g_index == BUFFER_LENGTH)
	    {
	        g_index = 0U;
	    }
	    EDMA_PrepareTransfer(&transferConfig, (void *)(g_TriangularWave + g_index), sizeof(uint16_t),
	                             (void *)DAC_DATA_REG_ADDR, sizeof(uint16_t), 1 * sizeof(uint16_t),
	                             1 * sizeof(uint16_t), kEDMA_MemoryToPeripheral);

	    EDMA_SetTransferConfig(EXAMPLE_DMA, DEMO_EDMA_CHANNEL_0, &transferConfig, NULL);
	    /* Enable transfer. */
	    EDMA_StartTransfer(&g_EDMA_Handle);
	}

	else if(SQR == GPIO_Signal_Select())
	{
	    /* Clear Edma interrupt flag. */
	    EDMA_ClearChannelStatusFlags(EXAMPLE_DMA, DEMO_EDMA_CHANNEL_0, kEDMA_InterruptFlag);
	    /* Setup transfer */
	//    g_index += DAC_DATL_COUNT;
	    g_index += 1;
	    if (g_index == BUFFER_LENGTH)
	    {
	        g_index = 0U;
	    }
	    EDMA_PrepareTransfer(&transferConfig, (void *)(g_SquareWave + g_index), sizeof(uint16_t),
	                             (void *)DAC_DATA_REG_ADDR, sizeof(uint16_t), 1 * sizeof(uint16_t),
	                             1 * sizeof(uint16_t), kEDMA_MemoryToPeripheral);

	    EDMA_SetTransferConfig(EXAMPLE_DMA, DEMO_EDMA_CHANNEL_0, &transferConfig, NULL);
	    /* Enable transfer. */
	    EDMA_StartTransfer(&g_EDMA_Handle);
	}
}
