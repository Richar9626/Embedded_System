/*
 * LCD_nokia_images.h
 *
 *  Created on: 25/09/2018
 *      Author: jlpe
 */

#ifndef LCD_NOKIA_IMAGES_H_
#define LCD_NOKIA_IMAGES_H_

#include "stdint.h"

/*Enum to choose between images*/
typedef enum{ITESO_im, Patriots} k64_image_t;

uint8_t *get_image(k64_image_t);
#endif /* LCD_NOKIA_IMAGES_H_ */
