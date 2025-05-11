/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This header file is for MyWirelessApp Demo Coordinator application.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_H_
#define _APP_H_


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include <stdint.h>
/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
#ifdef gPHY_802_15_4g_d
  #define mDefaultValueOfChannel_c (0x0001FFFF)
  #define mDefaultMaxChannel_c     (0x11)
#else
  #define mDefaultValueOfChannel_c (0x07FFF800)
#endif

#define mMacExtendedAddress_c    (0x1111111111111112) //changed as requested

/* Set the Coordinator short address */ 
#define mDefaultValueOfShortAddress_c     0x0000 //changed as requested

/* Set the Coordinator PanID */ 
#define mDefaultValueOfPanId_c            0x0404

/* Maximum number of outstanding packets */
#define mDefaultValueOfMaxPendingDataPackets_c 2

/* Maximum number of nodes connected */
#define MaxNodes 5

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/* The various states of the applications state machines. */
enum {
  stateInit,
  stateScanEdStart,
  stateScanEdWaitConfirm,
  stateStartCoordinator,
  stateStartCoordinatorWaitConfirm,
  stateListen
};

/* Events that are passed to the application task. 
   Are defined as byte masks to make possible 
   send multiple events to the task */

#define gAppEvtDummyEvent_c            (1 << 0)
#define gAppEvtRxFromUart_c            (1 << 1)
#define gAppEvtMessageFromMLME_c       (1 << 2)
#define gAppEvtMessageFromMCPS_c       (1 << 3)
#define gAppEvtStartCoordinator_c      (1 << 4)

/* Error codes */
enum {
  errorNoError,
  errorWrongConfirm,
  errorNotSuccessful,
  errorNoMessage,
  errorAllocFailed,
  errorInvalidParameter,
  errorNoScanResults
};

typedef enum
{
	FFD,
	RFD
}DeviceType_t;

/*!  The MLME-ASSOCIATE.response primitive is used to initiate a response to an MLME-ASSOCIATE.indication primitive. */
typedef struct
{
    uint64_t                exttAddress[MaxNodes]; /*!< The address of the device requesting association. */
    uint16_t                shrtAddress[MaxNodes]; /*!< Contains the short device address allocated by the coordinator on successful association.
                                                    This parameter is set to 0xfffe if the no short address was allocated. The 64-bit address must be used.
                                                    This parameter is set to 0xffff if the association was unsuccessful */
    uint8_t					RxOnWhenIdle[MaxNodes]; /* this parameter accepts TRUE or FALSE*/

    DeviceType_t			DeviceType[MaxNodes];  /* Defines from pMsgIn->msgData.associateInd.capabilityInfo if it is full function device or reduced*/
    uint8_t					numberOfNodes;  /* integer that has the value of how many nodes have been connected*/
 } node_Associated_t;

/******************************************************************************
*******************************************************************************
* Public Prototypes
*******************************************************************************
******************************************************************************/
#ifdef __cplusplus
    extern "C" {
#endif


#ifdef __cplusplus
}
#endif

/**********************************************************************************/
#endif /* _MAPP_H_ */
