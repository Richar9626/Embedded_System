/*
 * BMI160.c
 *
 *  Created on: 6 nov 2021
 *      Author: Ricardo
 */
#include "MK66F18.h"
#include "BMI160.h"
#include "freertos_i2c.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "task.h"


void BMI160_Init(void)
{
	uint8_t buffer_tx[CONFIG_LENGHT] = {0x7E, 0x11};
	uint8_t buffer_tx1[CONFIG_LENGHT]= {0x7E, 0x15};
	uint8_t buffer_conf[CONFIG_LENGHT]= {0X40, 0x08};
	uint8_t buffer_conf2[CONFIG_LENGHT]= {0X42, 0x08};
	uint8_t buffer_acc_range[CONFIG_LENGHT] = {0X41, 0x08};
	uint8_t buffer_gyro_range[CONFIG_LENGHT] = {0X43, 0x04};
	uint8_t buffer_sensor_time0[CONFIG_LENGHT] = {0x18, 0xFF};
	uint8_t buffer_sensor_time1[CONFIG_LENGHT] = {0x19, 0xFF};
	uint8_t buffer_sensor_time2[CONFIG_LENGHT] = {0x1A, 0xFF};

	freertos_i2c_config_t i2c =
	{
			I2C_BAUDRATE,
			freertos_i2c0,
			freertos_i2c_portB,
			SDA_PIN,
			SCL_PIN,
			freertos_i2c_master,
			SLAVE_ADDRESS,
			kI2C_Address7bit,
			kPORT_PullUp
	};
	freertos_i2c_init(i2c);

	vTaskDelay(pdMS_TO_TICKS(DELAY_INIT));

	freertos_i2c_master_transfer(freertos_i2c0, buffer_conf, CONFIG_LENGHT, kI2C_Write, SLAVE_ADDRESS, kI2C_TransferDefaultFlag);
	freertos_i2c_master_transfer(freertos_i2c0, buffer_conf2, CONFIG_LENGHT, kI2C_Write, SLAVE_ADDRESS, kI2C_TransferDefaultFlag);
	freertos_i2c_master_transfer(freertos_i2c0, buffer_acc_range, CONFIG_LENGHT, kI2C_Write, SLAVE_ADDRESS, kI2C_TransferDefaultFlag);
	freertos_i2c_master_transfer(freertos_i2c0, buffer_gyro_range, CONFIG_LENGHT, kI2C_Write, SLAVE_ADDRESS, kI2C_TransferDefaultFlag);
	//freertos_i2c_master_transfer(freertos_i2c0, buffer_sensor_time0, CONFIG_LENGHT, kI2C_Write, SLAVE_ADDRESS, kI2C_TransferDefaultFlag);
	//freertos_i2c_master_transfer(freertos_i2c0, buffer_sensor_time1, CONFIG_LENGHT, kI2C_Write, SLAVE_ADDRESS, kI2C_TransferDefaultFlag);
	//freertos_i2c_master_transfer(freertos_i2c0, buffer_sensor_time2, CONFIG_LENGHT, kI2C_Write, SLAVE_ADDRESS, kI2C_TransferDefaultFlag);
	freertos_i2c_master_transfer(freertos_i2c0, buffer_tx, CONFIG_LENGHT, kI2C_Write, SLAVE_ADDRESS, kI2C_TransferDefaultFlag);
	vTaskDelay(pdMS_TO_TICKS(5));
	freertos_i2c_master_transfer(freertos_i2c0, buffer_tx1, CONFIG_LENGHT, kI2C_Write, SLAVE_ADDRESS, kI2C_TransferDefaultFlag);
	vTaskDelay(pdMS_TO_TICKS(80));
}

/*!
 * @brief This API reads accel data along with sensor time if time is requested
 * by user. Kindly refer the user guide(README.md) for more info.
 */
bmi160_raw_data_t BMI160_Get_Accelerometer(void){
	bmi160_raw_data_t data_acc;
	uint8_t data[DATA_LENGHT];
	uint16_t acc_data[VECTOR_NUM];
	uint8_t vector = 0;
	uint8_t command = X_LOW_ACC;

	freertos_i2c_master_transfer(freertos_i2c0, &command , COMMAND_LENGHT, kI2C_Write, SLAVE_ADDRESS, kI2C_TransferNoStopFlag);
	freertos_i2c_master_transfer(freertos_i2c0, data , DATA_LENGHT, kI2C_Read, SLAVE_ADDRESS, kI2C_TransferRepeatedStartFlag);

	for(uint8_t i = 0; i < DATA_LENGHT ; i+=2){
		acc_data[vector] = data[i+1]<<8 | data[i] ;
		vector++;
	}

	data_acc.x = acc_data[VECTOR_X];
	data_acc.y = acc_data[VECTOR_Y];
	data_acc.z = acc_data[VECTOR_Z];

	return data_acc;
}

bmi160_raw_data_t BMI160_Get_Gyroscope(void){
	bmi160_raw_data_t data_gyr;
	uint8_t data[DATA_LENGHT];
	uint16_t gyr_data[VECTOR_NUM];
	uint8_t vector = 0;
	uint8_t command = X_LOW_GYR;

	freertos_i2c_master_transfer(freertos_i2c0, &command , COMMAND_LENGHT, kI2C_Write, SLAVE_ADDRESS, kI2C_TransferNoStopFlag);
	freertos_i2c_master_transfer(freertos_i2c0, data , DATA_LENGHT, kI2C_Read, SLAVE_ADDRESS, kI2C_TransferRepeatedStartFlag);

	for(uint8_t i = 0; i < DATA_LENGHT ; i+=2){
		gyr_data[vector] = data[i+1]<<8 | data[i] ;
		vector++;
	}

	data_gyr.x = gyr_data[VECTOR_X];
	data_gyr.y = gyr_data[VECTOR_Y];
	data_gyr.z = gyr_data[VECTOR_Z];

	return data_gyr;
}
