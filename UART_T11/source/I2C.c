/*
 * I2C.c
 *
*  Created on: May 5, 2021
 *      Author: Fernando
 */


#include "stdint.h"
#include "I2C.h"

static uint8_t g_master_txBuff[I2C_DATA_LENGTH];
static uint8_t g_master_rxBuff[I2C_DATA_LENGTH];
static i2c_master_config_t g_masterConfig;
static i2c_master_transfer_t g_masterXfer;
 uint32_t g_status_transfer = FALSE;

void I2C_init(void)
{
	/*FUNCTION**********************************************************************
	 *
	 * Function Name : BOARD_InitPins
	 * Description   : Configures pin routing and optionally pin electrical features.
	 *
	 *END**************************************************************************/

	CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

	PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAlt3);           /* PORTB16 (pin 62) is configured as UART0_RX */
	PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAlt3);           /* PORTB17 (pin 63) is configured as UART0_TX */
	const port_pin_config_t portb_pin2_config = {
		kPORT_PullDisable,                                            /* Internal pull-up resistor is enabled */
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
		kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
		kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
		kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
		kPORT_MuxAlt2,                                           /* Pin is configured as I2C0_SCL */
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};
	PORT_SetPinConfig(PORTB, PIN2_IDX, &portb_pin2_config); /* PORTE24 (pin 31) is configured as I2C0_SCL */
	const port_pin_config_t portb_pin3_config = {
		kPORT_PullDisable,                                            /* Internal pull-up resistor is enabled */
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
		kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
		kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
		kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
		kPORT_MuxAlt2,                                           /* Pin is configured as I2C0_SDA */
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};
	PORT_SetPinConfig(PORTB, PIN3_IDX, &portb_pin3_config); /* PORTE25 (pin 32) is configured as I2C0_SDA */
	SIM->SOPT5 = ((SIM->SOPT5 &
	(~(SIM_SOPT5_UART0TXSRC_MASK)))                          /* Mask bits to zero which are setting */
	  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART 0 transmit data source select: UART0_TX pin */
	);

	BOARD_BootClockRUN();
	/*
	     * masterConfig->baudRate_Bps = 100000U;
	     * masterConfig->enableStopHold = false;
	     * masterConfig->glitchFilterWidth = 0U;
	     * masterConfig->enableMaster = true;
	     */
	I2C_MasterGetDefaultConfig(&g_masterConfig);
	g_masterConfig.baudRate_Bps = I2C_BAUDRATE;
	I2C_MasterInit(EXAMPLE_I2C_MASTER_BASEADDR, &g_masterConfig, I2C_MASTER_CLK_FREQ);
	memset(&g_masterXfer, 0, sizeof(g_masterXfer));
}

void I2C_write(uint16_t register_map_address, uint8_t I2C_CONTROL_SLAVE_ADDR_7BIT, uint8_t register_map_address_size, uint8_t DATA_byte)
{
	g_master_txBuff[0] = DATA_byte;
	uint8_t deviceAddress     = register_map_address;
	/* subAddress = 0x01, data = g_master_txBuff - write to slave.
	start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
	g_masterXfer.slaveAddress   = I2C_CONTROL_SLAVE_ADDR_7BIT;
	g_masterXfer.direction      = kI2C_Write;
	g_masterXfer.subaddress     = (uint32_t)deviceAddress;
	g_masterXfer.subaddressSize = register_map_address_size;
	g_masterXfer.data           = g_master_txBuff;
	g_masterXfer.dataSize       = I2C_DATA_LENGTH;
	g_masterXfer.flags          = kI2C_TransferDefaultFlag;
	g_status_transfer = I2C_MasterTransferBlocking(EXAMPLE_I2C_MASTER_BASEADDR, &g_masterXfer);
#ifndef DEBUG_ON
	printf("status %d\n",g_status_transfer);
#endif
}

uint8_t I2C_read(uint16_t register_map_address, uint8_t I2C_CONTROL_SLAVE_ADDR_7BIT, uint8_t register_map_address_size)
{
	uint8_t deviceAddress = register_map_address;
    /* subAddress = 0x01, data = g_master_rxBuff - read from slave.
      start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
	g_masterXfer.slaveAddress   = I2C_CONTROL_SLAVE_ADDR_7BIT;
	g_masterXfer.direction      = kI2C_Read;
	g_masterXfer.subaddress     = (uint32_t)deviceAddress;
	g_masterXfer.subaddressSize = register_map_address_size;
	g_masterXfer.data           = g_master_rxBuff;
	g_masterXfer.dataSize       = I2C_DATA_LENGTH;
	g_masterXfer.flags          = kI2C_TransferDefaultFlag;
	g_status_transfer = I2C_MasterTransferBlocking(EXAMPLE_I2C_MASTER_BASEADDR, &g_masterXfer);
#ifndef DEBUG_ON
	printf("status %d\n",g_status_transfer);
#endif
return g_master_rxBuff[0];
}

uint32_t I2C_get_status_transfer(void)
{
	return g_status_transfer;
}
