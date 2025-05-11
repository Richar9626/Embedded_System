/*
 * freertos_i2c.c
 *
 *  Created on: 6 nov 2021
 *      Author: Ricardo & Alexander
 */

#include "freertos_i2c.h"
#include "fsl_i2c.h"
#include "fsl_clock.h"
#include "fsl_port.h"
#include "FreeRTOS.h"
#include "semphr.h"

#define NO_I2C (1)
#define I2C_DATA_LENGTH 32U

typedef struct
{
  uint8_t is_init;
  i2c_master_handle_t fsl_i2c_handle_master;
  i2c_slave_handle_t fsl_i2c_handle_slave;
  SemaphoreHandle_t mutex_bus;
  SemaphoreHandle_t bus_sem;
} freertos_i2c_hanlde_t;

static freertos_i2c_number_t master;
static freertos_i2c_number_t slave;
volatile uint8_t g_slave_buff[I2C_DATA_LENGTH];
volatile uint8_t g_master_buff[I2C_DATA_LENGTH];
volatile uint8_t g_masterTxIndex = 0;
volatile uint8_t g_masterRxIndex = 0xFFU;
volatile uint8_t g_slaveTxIndex  = 0xFFU;
volatile uint8_t g_slaveRxIndex  = 0;
volatile bool g_masterReadBegin  = false;

static freertos_i2c_hanlde_t freertos_i2c_handles[NO_I2C] = {0};

static inline void freertos_i2c_enable_port_clock(freertos_i2c_port_t port);

static inline PORT_Type * freertos_i2c_get_port_base(freertos_i2c_port_t port);

static inline I2C_Type * freertos_i2c_get_i2c_base(freertos_i2c_number_t i2c_number);

static void fsl_i2c_callback_master(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData);

static void fsl_i2c_callback_slave(I2C_Type *base, i2c_slave_handle_t *handle, status_t status, void *userData);

static inline IRQn_Type freertos_i2c_get_IRQhandler(freertos_i2c_number_t i2c_number);

static inline clock_name_t freertos_i2c_get_clk(freertos_i2c_number_t i2c_number);

freertos_i2c_flag_t freertos_i2c_init(freertos_i2c_config_t config)
{
	freertos_i2c_flag_t retval = freertos_i2c_fail;
	i2c_master_config_t fsl_i2c_master_config;
	i2c_slave_config_t fsl_i2c_slave_config;

	const port_pin_config_t i2c_pin_config = {/* Internal pull-up resistor is enabled */
	                                                    config.pull_up,
	                                                    /* Fast slew rate is configured */
	                                                    kPORT_FastSlewRate,
	                                                    /* Passive filter is disabled */
	                                                    kPORT_PassiveFilterDisable,
	                                                    /* Open drain is enabled */
	                                                    kPORT_OpenDrainEnable,
	                                                    /* Low drive strength is configured */
	                                                    kPORT_LowDriveStrength,
	                                                    /* Pin is configured as I2C0_SCL */
	                                                    kPORT_MuxAlt2,
	                                                    /* Pin Control Register fields [15:0] are not locked */
	                                                    kPORT_UnlockRegister
	};

	if(config.i2c_number < NO_I2C)
	{
		if(!freertos_i2c_handles[config.i2c_number].is_init)
		{
		  freertos_i2c_handles[config.i2c_number].mutex_bus = xSemaphoreCreateMutex();
		  freertos_i2c_handles[config.i2c_number].bus_sem = xSemaphoreCreateBinary();

		  /* Clock Enable */
			freertos_i2c_enable_port_clock(config.port);
			if(freertos_i2c_master == config.type_i2c){
				I2C_MasterGetDefaultConfig(&fsl_i2c_master_config);
				fsl_i2c_master_config.baudRate_Bps = config.baudrate;
				master = config.i2c_number;
			}
			else{
				I2C_SlaveGetDefaultConfig(&fsl_i2c_slave_config);
				fsl_i2c_slave_config.addressingMode = config.slave_address_mode;
				fsl_i2c_slave_config.slaveAddress = config.slave_address;
				slave = config.i2c_number;
			}

			/* Port Config */
			PORT_SetPinConfig(freertos_i2c_get_port_base(config.port), config.sda_pin, &i2c_pin_config);
			PORT_SetPinConfig(freertos_i2c_get_port_base(config.port), config.scl_pin, &i2c_pin_config);

			if(freertos_i2c_master == config.type_i2c)
			{
				I2C_MasterInit(freertos_i2c_get_i2c_base(config.i2c_number), &fsl_i2c_master_config, CLOCK_GetFreq(freertos_i2c_get_clk(config.i2c_number)));
				NVIC_SetPriority(freertos_i2c_get_IRQhandler(config.i2c_number),5);
				I2C_MasterTransferCreateHandle(freertos_i2c_get_i2c_base(config.i2c_number),
						&freertos_i2c_handles[config.i2c_number].fsl_i2c_handle_master,
						(i2c_master_transfer_callback_t)fsl_i2c_callback_master,
                        NULL);
			}
			else
			{
				I2C_SlaveInit(freertos_i2c_get_i2c_base(config.i2c_number), &fsl_i2c_slave_config, CLOCK_GetFreq(freertos_i2c_get_clk(config.i2c_number)));
				NVIC_SetPriority(freertos_i2c_get_IRQhandler(config.i2c_number),5);
				I2C_SlaveTransferCreateHandle(freertos_i2c_get_i2c_base(config.i2c_number),
						&freertos_i2c_handles[config.i2c_number].fsl_i2c_handle_slave,
						(i2c_slave_transfer_callback_t)fsl_i2c_callback_slave,
						NULL);
			}

			freertos_i2c_handles[config.i2c_number].is_init = 1;

			retval = freertos_i2c_sucess;
		}

	}

	return retval;
}

freertos_i2c_flag_t freertos_i2c_master_transfer(freertos_i2c_number_t i2c_number, uint8_t * buffer, uint16_t lenght, i2c_direction_t direction, uint8_t slaveAddR,uint32_t flag_xfer )
{
	freertos_i2c_flag_t flag = freertos_i2c_fail;
	i2c_master_transfer_t xfer;

	if(freertos_i2c_handles[i2c_number].is_init)
	{
		xfer.data = buffer;
		xfer.dataSize = lenght;
		xfer.direction = direction;
		xfer.slaveAddress = slaveAddR;
		xfer.subaddressSize = 0;
		xfer.flags = flag_xfer ;

		xSemaphoreTake(freertos_i2c_handles[i2c_number].mutex_bus, portMAX_DELAY);

		I2C_MasterTransferNonBlocking(freertos_i2c_get_i2c_base(i2c_number), &freertos_i2c_handles[i2c_number].fsl_i2c_handle_master, &xfer);

		xSemaphoreTake(freertos_i2c_handles[i2c_number].bus_sem, portMAX_DELAY);

		xSemaphoreGive(freertos_i2c_handles[i2c_number].mutex_bus);

		flag = freertos_i2c_sucess;
	}

	return flag;
}

freertos_i2c_flag_t freertos_i2c_slave_transfer(freertos_i2c_number_t i2c_number, uint8_t * buffer, uint16_t lenght)
{
	freertos_i2c_flag_t flag = freertos_i2c_fail;

	if(freertos_i2c_handles[i2c_number].is_init)
	{
		freertos_i2c_handles[i2c_number].fsl_i2c_handle_slave.transfer.data = buffer;
		freertos_i2c_handles[i2c_number].fsl_i2c_handle_slave.transfer.dataSize = lenght;

		xSemaphoreTake(freertos_i2c_handles[i2c_number].mutex_bus, portMAX_DELAY);

		I2C_SlaveTransferNonBlocking(freertos_i2c_get_i2c_base(i2c_number), &freertos_i2c_handles[i2c_number].fsl_i2c_handle_slave, 0);

		xSemaphoreTake(freertos_i2c_handles[i2c_number].bus_sem, portMAX_DELAY);

		xSemaphoreGive(freertos_i2c_handles[i2c_number].mutex_bus);

		flag = freertos_i2c_sucess;
	}

	return flag;
}

static inline void freertos_i2c_enable_port_clock(freertos_i2c_port_t port)
{
	switch(port)
	{
    case freertos_i2c_portA:
      CLOCK_EnableClock(kCLOCK_PortA);
      break;
    case freertos_i2c_portB:
      CLOCK_EnableClock(kCLOCK_PortB);
      break;
    case freertos_i2c_portC:
      CLOCK_EnableClock(kCLOCK_PortC);
      break;
    case freertos_i2c_portD:
      CLOCK_EnableClock(kCLOCK_PortD);
      break;
    case freertos_i2c_portE:
      CLOCK_EnableClock(kCLOCK_PortE);
      break;
	}
}

static inline PORT_Type * freertos_i2c_get_port_base(freertos_i2c_port_t port)
{
  PORT_Type * port_base = PORTA;

  switch(port)
  {
    case freertos_i2c_portA:
      port_base = PORTA;
      break;
    case freertos_i2c_portB:
      port_base = PORTB;
      break;
    case freertos_i2c_portC:
      port_base = PORTC;
      break;
    case freertos_i2c_portD:
      port_base = PORTD;
      break;
    case freertos_i2c_portE:
      port_base = PORTE;
      break;
  }

  return port_base;
}

static inline I2C_Type * freertos_i2c_get_i2c_base(freertos_i2c_number_t i2c_number)
{
	I2C_Type * retval = I2C0;

	switch(i2c_number)
	{
    case freertos_i2c0:
      retval = I2C0;
      break;
    case freertos_i2c1:
      retval = I2C1;
      break;
	}

	return retval;
}

static void fsl_i2c_callback_master(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  if (kStatus_NoTransferInProgress == status)
  {

    xSemaphoreGiveFromISR(freertos_i2c_handles[master].bus_sem, &xHigherPriorityTaskWoken);

  }

  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

static void fsl_i2c_callback_slave(I2C_Type *base, i2c_slave_handle_t *handle, status_t status, void *userData)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  if (kStatus_NoTransferInProgress == status)
  {
     xSemaphoreGiveFromISR(freertos_i2c_handles[slave].bus_sem, &xHigherPriorityTaskWoken);
  }

  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

static inline clock_name_t freertos_i2c_get_clk(freertos_i2c_number_t i2c_number)
{
	clock_name_t value = I2C0_CLK_SRC;

	switch(i2c_number)
	{
    case freertos_i2c0:
    	return I2C0_CLK_SRC;
    case freertos_i2c1:
    	return I2C1_CLK_SRC;
	}
	return value;
}

static inline IRQn_Type freertos_i2c_get_IRQhandler(freertos_i2c_number_t i2c_number){
	IRQn_Type value = I2C0_IRQn;

	switch(i2c_number)
	{
	case freertos_i2c0:
		return I2C0_IRQn;
	case freertos_i2c1:
		return I2C1_IRQn;
	}
	return value;
}
