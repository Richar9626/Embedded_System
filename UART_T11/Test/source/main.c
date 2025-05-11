/*
 * main.c
 *
 *  Created on: 14 may. 2021
 *      Author: Daniel Ruvalcaba and Miguel Ortiz
 *
 */

#include "SDK_GPIO.h"

#include "SDK_DAC.h"

#define EXAMPLE_DMA    DMA0
#define EXAMPLE_DMAMUX DMAMUX0
#define BUFFER_LENGTH       32
#define DAC_DATA_REG_ADDR 0x400cc000U
#define TCD_QUEUE_SIZE      2U
#define DEMO_EDMA_CHANNEL_0 0
#define SIN 0
#define TRI 1
#define SQR 2
bool pitIsrFlag = false;

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

void PIT_init(void)
{
    /* Structure of initialize PIT */
    pit_config_t pitConfig;

    /* Initialize and enable LED */
//    LED_INIT();

    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(DEMO_PIT_BASEADDR, &pitConfig);

    /* Set timer period for channel 0 */
//    PIT_SetTimerPeriod(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, USEC_TO_COUNT(1000000U, PIT_SOURCE_CLOCK));
    PIT_SetTimerPeriod(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, USEC_TO_COUNT(100U, PIT_SOURCE_CLOCK));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, kPIT_TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQ_ID);

    /* Start channel 0 */
    PRINTF("\r\nStarting channel No.0 ...");
    PIT_StartTimer(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL);
}

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

void DMA_init(void)
{
    /* Configure DMAMUX */
    DMAMUX_Init(EXAMPLE_DMAMUX);
    DMAMUX_EnableAlwaysOn(EXAMPLE_DMAMUX, DEMO_EDMA_CHANNEL_0, true);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, DEMO_EDMA_CHANNEL_0);

    edma_config_t userConfig;

    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(EXAMPLE_DMA, &userConfig);
    EDMA_CreateHandle(&g_EDMA_Handle, EXAMPLE_DMA, DEMO_EDMA_CHANNEL_0);
    EDMA_SetCallback(&g_EDMA_Handle, Edma_Callback, NULL);
//    EDMA_PrepareTransfer(&transferConfig, (void *)(g_SinWave + g_index), sizeof(uint16_t),
//                         (void *)DAC_DATA_REG_ADDR, sizeof(uint16_t), DAC_DATL_COUNT * sizeof(uint16_t),
//                         DAC_DATL_COUNT * sizeof(uint16_t), kEDMA_MemoryToPeripheral);
    EDMA_PrepareTransfer(&transferConfig, (void *)(g_SinWave + g_index), sizeof(uint16_t),
                         (void *)DAC_DATA_REG_ADDR, sizeof(uint16_t), 1 * sizeof(uint16_t),
                         1 * sizeof(uint16_t), kEDMA_MemoryToPeripheral);
    EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
    /* Enable interrupt when transfer is done. */
    EDMA_EnableChannelInterrupts(EXAMPLE_DMA, DEMO_EDMA_CHANNEL_0, kEDMA_MajorInterruptEnable);
#if defined(FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT) && FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT
    /* Enable async DMA request. */
    EDMA_EnableAsyncRequest(EXAMPLE_DMA, DEMO_EDMA_CHANNEL_0, true);
#endif /* FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT */
    /* Enable transfer. */
    EDMA_StartTransfer(&g_EDMA_Handle);
}

static inline void DMAMUX_EnableAlwaysOn(DMAMUX_Type *base, uint32_t channel, bool enable)
{
    assert(channel < (uint32_t)FSL_FEATURE_DMAMUX_MODULE_CHANNEL);
    base->CHCFG[channel] |= 0x7A;
}


static void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
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
int main()
{
	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);
	GPIO_Init();
	PIT_init();
	DMA_init();
	DAC_init();
	for(;;)
	{
	}
	return 0;
}


