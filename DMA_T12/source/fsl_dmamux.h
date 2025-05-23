/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_DMAMUX_H_
#define _FSL_DMAMUX_H_

#include "fsl_common.h"

#define DEMO_DAC_BASEADDR           DAC0
#define DEMO_DMAMUX_BASEADDR        DMAMUX
#define DEMO_DMA_CHANNEL            0U
#define DEMO_DMA_DAC_SOURCE         45U
#define DEMO_DMA_BASEADDR           DMA0
#define DAC_DATA_REG_ADDR           0x400cc000U
#define DEMO_DMA_IRQ_ID             DMA0_IRQn
/*!
 * @addtogroup dmamux
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief DMAMUX driver version 2.0.5. */
#define FSL_DMAMUX_DRIVER_VERSION (MAKE_VERSION(2, 0, 5))
/*@}*/

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @name DMAMUX Initialization and de-initialization
 * @{
 */

/*!
 * @brief Initializes the DMAMUX peripheral.
 *
 * This function ungates the DMAMUX clock.
 *
 * @param base DMAMUX peripheral base address.
 *
 */
void DMAMUX_Init(DMAMUX_Type *base);

/*!
 * @brief Deinitializes the DMAMUX peripheral.
 *
 * This function gates the DMAMUX clock.
 *
 * @param base DMAMUX peripheral base address.
 */
void DMAMUX_Deinit(DMAMUX_Type *base);

/* @} */
/*!
 * @name DMAMUX Channel Operation
 * @{
 */
void EDMA_Configuration(void);

void DMAMUX_Configuration(void);

void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds);
/*!
 * @brief Enables the DMAMUX channel.
 *
 * This function enables the DMAMUX channel.
 *
 * @param base DMAMUX peripheral base address.
 * @param channel DMAMUX channel number.
 */
static inline void DMAMUX_EnableChannel(DMAMUX_Type *base, uint32_t channel)
{
    assert(channel < (uint32_t)FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->CHCFG[channel] |= DMAMUX_CHCFG_ENBL_MASK;
}

/*!
 * @brief Disables the DMAMUX channel.
 *
 * This function disables the DMAMUX channel.
 *
 * @note The user must disable the DMAMUX channel before configuring it.
 * @param base DMAMUX peripheral base address.
 * @param channel DMAMUX channel number.
 */
static inline void DMAMUX_DisableChannel(DMAMUX_Type *base, uint32_t channel)
{
    assert(channel < (uint32_t)FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

#if defined FSL_FEATURE_DMAMUX_CHCFG_REGISTER_WIDTH && (FSL_FEATURE_DMAMUX_CHCFG_REGISTER_WIDTH == 32U)
    base->CHCFG[channel] &= ~DMAMUX_CHCFG_ENBL_MASK;
#else
    base->CHCFG[channel] &= ~(uint8_t)DMAMUX_CHCFG_ENBL_MASK;
#endif
}

/*!
 * @brief Configures the DMAMUX channel source.
 *
 * @param base DMAMUX peripheral base address.
 * @param channel DMAMUX channel number.
 * @param source Channel source, which is used to trigger the DMA transfer.
 */
static inline void DMAMUX_SetSource(DMAMUX_Type *base, uint32_t channel, uint32_t source)
{
    assert(channel < (uint32_t)FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

#if defined FSL_FEATURE_DMAMUX_CHCFG_REGISTER_WIDTH && (FSL_FEATURE_DMAMUX_CHCFG_REGISTER_WIDTH == 32U)
    base->CHCFG[channel] = ((base->CHCFG[channel] & ~DMAMUX_CHCFG_SOURCE_MASK) | DMAMUX_CHCFG_SOURCE(source));
#else
    base->CHCFG[channel] = (uint8_t)((base->CHCFG[channel] & ~DMAMUX_CHCFG_SOURCE_MASK) | DMAMUX_CHCFG_SOURCE(source));
#endif
}

#if defined(FSL_FEATURE_DMAMUX_HAS_TRIG) && FSL_FEATURE_DMAMUX_HAS_TRIG > 0U
/*!
 * @brief Enables the DMAMUX period trigger.
 *
 * This function enables the DMAMUX period trigger feature.
 *
 * @param base DMAMUX peripheral base address.
 * @param channel DMAMUX channel number.
 */
static inline void DMAMUX_EnablePeriodTrigger(DMAMUX_Type *base, uint32_t channel)
{
    assert(channel < (uint32_t)FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->CHCFG[channel] |= DMAMUX_CHCFG_TRIG_MASK;
}

/*!
 * @brief Disables the DMAMUX period trigger.
 *
 * This function disables the DMAMUX period trigger.
 *
 * @param base DMAMUX peripheral base address.
 * @param channel DMAMUX channel number.
 */
static inline void DMAMUX_DisablePeriodTrigger(DMAMUX_Type *base, uint32_t channel)
{
    assert(channel < (uint32_t)FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

#if defined FSL_FEATURE_DMAMUX_CHCFG_REGISTER_WIDTH && (FSL_FEATURE_DMAMUX_CHCFG_REGISTER_WIDTH == 32U)
    base->CHCFG[channel] &= ~DMAMUX_CHCFG_TRIG_MASK;
#else
    base->CHCFG[channel] &= ~(uint8_t)DMAMUX_CHCFG_TRIG_MASK;
#endif
}
#endif /* FSL_FEATURE_DMAMUX_HAS_TRIG */

#if (defined(FSL_FEATURE_DMAMUX_HAS_A_ON) && FSL_FEATURE_DMAMUX_HAS_A_ON)
/*!
 * @brief Enables the DMA channel to be always ON.
 *
 * This function enables the DMAMUX channel always ON feature.
 *
 * @param base DMAMUX peripheral base address.
 * @param channel DMAMUX channel number.
 * @param enable Switcher of the always ON feature. "true" means enabled, "false" means disabled.
 */
static inline void DMAMUX_EnableAlwaysOn(DMAMUX_Type *base, uint32_t channel, bool enable)
{
    assert(channel < (uint32_t)FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    if (enable)
    {
        base->CHCFG[channel] |= DMAMUX_CHCFG_A_ON_MASK;
    }
    else
    {
        base->CHCFG[channel] &= ~DMAMUX_CHCFG_A_ON_MASK;
    }
}
#endif /* FSL_FEATURE_DMAMUX_HAS_A_ON */

/* @} */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/* @} */

#endif /* _FSL_DMAMUX_H_ */
