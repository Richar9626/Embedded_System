/*
 * BMI160.h
 *
 *  Created on: 6 nov 2021
 *      Author: Ricardo
 */
#ifndef BMI160_H_
#define BMI160_H_

#include "freertos_i2c.h"

#define SLAVE_ADDRESS 0x68 /* page 93 device address for I2C*/
#define DATA_LENGHT 6
#define COMMAND_LENGHT 1
#define CONFIG_LENGHT 2
#define X_LOW_GYR 0x0C
#define X_LOW_ACC 0x12

/* properties seen on page 5*/
#define BMI160_ACC_RANGE	0x41
#define BMI160_8G_RANGE		0x08 /* page 59 */
#define BMI160_GYRO_RANGE	0x43
#define BMI160_500_RANGE	0x02 /* page 61 */

#define SMD 0x72
/* Defined on datasheet page 86 command code for normal mode*/
#define NORMAL_ACC 0x11
#define NORMAL_GIR 0x15
#define VECTOR_X 0
#define VECTOR_Y 1
#define VECTOR_Z 2
#define VECTOR_NUM 3
#define DELAY_INIT 1000
/* Found on page 12 from datasheet register address for configuration*/
#define CONF_ACC 0x40
#define CONF_GYRO 0x42
#define NW_FREQ_400 0x0A /* set waiting time after write command is issued	*/

/* Struct as requested for sensor data*/
typedef struct
{
	int16_t x;
	int16_t y;
	int16_t z;
}bmi160_raw_data_t;

/* Definition of functions*/
void BMI160_Init(void);
bmi160_raw_data_t BMI160_Get_Accelerometer(void);
bmi160_raw_data_t BMI160_Get_Gyroscope(void);


#endif /* BMI160_H_ */
