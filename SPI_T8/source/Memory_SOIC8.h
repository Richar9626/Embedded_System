/*
 * Memory_SOIC8.h
 *
 *  Created on: Mar 26, 2021
 *      Author: rjimenez
 */

#ifndef MEMORY_SOIC8_H_
#define MEMORY_SOIC8_H_

#define Read_instruction 0x03
#define Total_Image_Bytes 504
#define DATA_OR_CMD_PIN 4

#define Sector_Img1 0x007000
#define Sector_Img2 0x008000
#define Sector_Img3 0x009000
#define Sector_Img4 0x00A000
#define Sector_Img5 0x00B000

typedef enum {
	IMG_memory_1,
	IMG_memory_2,
	IMG_memory_3,
	IMG_memory_4,
	IMG_memory_5,
} Memory_Images_t;


uint8_t*  READ_IMAGE(Memory_Images_t);


void Memory_Read_byte(uint32_t Sector_address, Memory_Images_t Image );


#endif /* MEMORY_SOIC8_H_ */
