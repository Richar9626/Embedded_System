
/* TODO: insert other include files here. */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"

#include "fsl_i2c.h"
#include "semphr.h"
#include "freertos_i2c.h"
#include "BMI160.h"
#include "mahony.h"
#include "freertos_uart.h"


/* TODO: insert other definitions and declarations here. */
#define DELAY 25
#define ACC_SENSITIVITY		40140.8f
#define GYRO_SENSITIVITY 	65.536f
#define RESET 0
static uint8_t sends;

typedef struct{
	uint32_t header;
	float x;// roll
	float y;// pitch
	float z;// yaw
}comm_msg_t;

SemaphoreHandle_t init;
SemaphoreHandle_t send;

void task_init(void *param);
void data_receive(void *param);
void task_calibrate(void *parameters);
void task_send(void *parameters);


static bmi160_raw_data_t calibration_accelerometer;
static bmi160_raw_data_t calibration_gyroscope;
static bmi160_raw_data_t prom_acc, prom_gyr;
static comm_msg_t msg;

int main(void)
{

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    init = xSemaphoreCreateBinary();
    send = xSemaphoreCreateBinary();
    xSemaphoreGive(init);

	xTaskCreate(task_init, "Init", 110, NULL, 1, NULL);
	vTaskStartScheduler();


    while(1)
    {

    }
    return 0 ;
}

void task_init(void *param)
{
	BMI160_Init(); /* Initialize sensor, function under BMI160.c*/

	freertos_uart_config_t config;

	config.baudrate = 115200;
	config.rx_pin = 16;
	config.tx_pin = 17;
	config.pin_mux = kPORT_MuxAlt3;
	config.uart_number = freertos_uart0;
	config.port = freertos_uart_portB;

	freertos_uart_init(config);


	xTaskCreate(task_calibrate, "Calibration", 110, NULL, 1, NULL);
	xSemaphoreGive(init);
	vTaskSuspend(NULL);

	vTaskSuspend(NULL); /* we send it to suspend*/
	for(;;);
}

void data_receive(void *param)
{
	bmi160_raw_data_t acc;
	bmi160_raw_data_t gyro;

	prom_acc.x = 0;
	prom_acc.y = 0;
	prom_acc.z = 0;

	prom_gyr.x = 0;
	prom_gyr.y = 0;
	prom_gyr.z = 0;

	uint64_t avg_acc_x = 0;
	uint64_t avg_acc_y = 0;
	uint64_t avg_acc_z = 0;

	uint64_t avg_gyr_x = 0;
	uint64_t avg_gyr_y = 0;
	uint64_t avg_gyr_z = 0;

	xSemaphoreTake(init, portMAX_DELAY); //we block init task as requested

	while(1)
	{
		/* Data aquisition*/
		acc = BMI160_Get_Accelerometer();
		gyro= BMI160_Get_Gyroscope();

		/*acc.x -= calibration_accelerometer.x;
		acc.y -= calibration_accelerometer.y;
		acc.z -= calibration_accelerometer.z;
		gyro.x -= calibration_gyroscope.x;
		gyro.y -= calibration_gyroscope.y;
		gyro.z -= calibration_gyroscope.z;*/

		avg_acc_x += acc.x - calibration_accelerometer.x;
		avg_acc_y += acc.y - calibration_accelerometer.y;
		avg_acc_z += acc.z - calibration_accelerometer.z;

		avg_gyr_x += gyro.x - calibration_gyroscope.x;
		avg_gyr_y += gyro.y - calibration_gyroscope.y;
		avg_gyr_z += gyro.z - calibration_gyroscope.z;

		/*prom_acc.x += acc.x;
		prom_acc.y += acc.y;
		prom_acc.z += acc.z;

		prom_gyr.x += gyro.x;
		prom_gyr.y += gyro.y;
		prom_gyr.z += gyro.z;*/

		sends++;

		if(sends>3)
		{
			prom_acc.x = avg_acc_x / sends;
			prom_acc.y = avg_acc_y / sends;
			prom_acc.z = avg_acc_z / sends;

			prom_gyr.x = avg_gyr_x / sends;
			prom_gyr.y = avg_gyr_y / sends;
			prom_gyr.z = avg_gyr_z / sends;

			sends = 0;

			avg_acc_x = 0;
			avg_acc_y = 0;
			avg_acc_z = 0;

			avg_gyr_x = 0;
			avg_gyr_y = 0;
			avg_gyr_z = 0;

			xSemaphoreGive(send);
		}

		vTaskDelay(pdMS_TO_TICKS(DELAY));
	}
}

void task_send(void *parameters)
{
	MahonyAHRSEuler_t angles;
	uint32_t header = 0xAAAAAAAA;

	while(1)
	{
		xSemaphoreTake(send, portMAX_DELAY);
		/*prom_acc.x /= sends;
		prom_acc.y /= sends;
		prom_acc.z /= sends;
		prom_gyr.x /= sends;
		prom_gyr.y /= sends;
		prom_gyr.z /= sends;*/

		float n_accx, n_accy, n_accz, n_gyrx, n_gyry,n_gyrz;

		n_accx = (float)prom_acc.x / ACC_SENSITIVITY;
		n_accy = (float)prom_acc.y / ACC_SENSITIVITY;
		n_accz = (float)prom_acc.z / ACC_SENSITIVITY;

		n_gyrx = ((float)prom_gyr.x / GYRO_SENSITIVITY);
		n_gyry = ((float)prom_gyr.y / GYRO_SENSITIVITY);
		n_gyrz = ((float)prom_gyr.z / GYRO_SENSITIVITY);

		angles = MahonyAHRSupdateIMU(n_gyrx, n_gyry, n_gyrz, n_accx, n_accy, n_accz);
		msg.header = header;
		msg.x = angles.roll;
		msg.y = angles.pitch;
		msg.z = angles.yaw;

		freertos_uart_send(freertos_uart0,(uint8_t *)&msg, sizeof(comm_msg_t));

		prom_acc.x = RESET;
		prom_acc.y = RESET;
		prom_acc.z = RESET;
		prom_gyr.x = RESET;
		prom_gyr.y = RESET;
		prom_gyr.z = RESET;

		sends = RESET;
	}
}


void task_calibrate(void *parameters)
{
	TickType_t xLastWakeTime;
	TickType_t xfactor = pdMS_TO_TICKS(5);
	xLastWakeTime = xTaskGetTickCount();

	calibration_accelerometer.x= RESET;
	calibration_accelerometer.y= RESET;
	calibration_accelerometer.z= RESET;

	uint64_t cal_acc_x = 0;
	uint64_t cal_acc_y = 0;
	uint64_t cal_acc_z = 0;

	uint64_t cal_gyr_x = 0;
	uint64_t cal_gyr_y = 0;
	uint64_t cal_gyr_z = 0;

	calibration_gyroscope.x= RESET;
	calibration_gyroscope.y= RESET;
	calibration_gyroscope.z= RESET;

	bmi160_raw_data_t temporal_accelerometer;
	bmi160_raw_data_t temporal_gyroscope;


	for(uint8_t i = 0; i<100;i++)
	{
			temporal_gyroscope = BMI160_Get_Gyroscope();
			temporal_accelerometer = BMI160_Get_Accelerometer();

			cal_gyr_x += temporal_gyroscope.x;
			cal_gyr_y += temporal_gyroscope.y;
			cal_gyr_z += temporal_gyroscope.z;

			cal_acc_x += temporal_accelerometer.x;
			cal_acc_y += temporal_accelerometer.y;
			cal_acc_z += temporal_accelerometer.z;

			vTaskDelayUntil( &xLastWakeTime, xfactor );
	}

	calibration_accelerometer.x = (uint16_t)cal_acc_x/100;
	calibration_accelerometer.y = (uint16_t)cal_acc_y/100;
	calibration_accelerometer.z = (uint16_t)cal_acc_z/100;
	calibration_gyroscope.x = (uint16_t)cal_gyr_x/100;
	calibration_gyroscope.y = (uint16_t)cal_gyr_y/100;
	calibration_gyroscope.z = (uint16_t)cal_gyr_z/100;

	/*xTaskCreate(task_read, "Read", 110, NULL, 1, NULL);*/
	xTaskCreate(task_send, "Send", 110, NULL, 1, NULL);

	xTaskCreate(data_receive, "Receive", 110, NULL, 1, NULL);

	vTaskSuspend(NULL);
}
