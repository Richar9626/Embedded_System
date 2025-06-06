

#include "LCD_nokia_images.h"

#define ITESO_image &ITESO[0]
#define Pats_image &Pats[0]

const uint8_t ITESO[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x20, 0x20, 0x30, 0x30, 0x30, 0x38, 0x38, 0x3C, 0x3C, 0x7C, 0x7C, 0x7C, 0xFC, 0xFE, 0xFE,
		0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE,
		0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFC, 0x7C, 0x7C, 0x7C, 0x7C, 0x3C, 0x38, 0x38, 0x30, 0x30,
		0x30, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xF8, 0xE0,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
		0xFE, 0xFE, 0xFC, 0xFC, 0xFC, 0xFD, 0xF9, 0x79, 0x7B, 0x33, 0xB7, 0x87, 0x87, 0xCF, 0xCF, 0xC7,
		0x87, 0xB7, 0x37, 0x73, 0x7B, 0xF9, 0xFD, 0xFC, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1E, 0x04, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0xFE, 0xFC,
		0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x01, 0x03, 0x07,
		0x0F, 0x1F, 0x1F, 0x0F, 0x67, 0xF7, 0xF3, 0x9B, 0x9D, 0x9C, 0x9C, 0x9E, 0x9F, 0x9F, 0x9F, 0x9F,
		0x9F, 0x00, 0x00, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9E, 0x9C, 0x9C, 0x9D, 0x9B, 0xF3, 0xF7, 0x67,
		0x4F, 0x1F, 0x1F, 0x0F, 0x0F, 0x07, 0x03, 0x81, 0xC0, 0xE0, 0xF0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0x00, 0x00, 0x1F, 0x7F, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x9F, 0xCF, 0xE7, 0xF3, 0xFB,
		0xFD, 0xFC, 0xFE, 0xFE, 0xFC, 0xFC, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0xC1, 0x83, 0x83, 0x07, 0x07,
		0x0F, 0x1F, 0x1F, 0x3F, 0x3F, 0x70, 0x70, 0x3F, 0x3F, 0x1F, 0x1F, 0x0F, 0x07, 0x07, 0x83, 0x83,
		0xC1, 0xC0, 0xE0, 0xF0, 0xF0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFE, 0xFC, 0xF9, 0xF3, 0xF3, 0xC7, 0x8F,
		0x1F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x00,
		0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x3F, 0x0F, 0x03, 0x00, 0x08, 0x1E, 0x3F,
		0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0F, 0x0F, 0x0F, 0x87,
		0x87, 0x83, 0xC3, 0xC3, 0xC0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xE0, 0xE0, 0xC2,
		0xC3, 0xC3, 0x87, 0x87, 0x07, 0x0F, 0x0F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F,
		0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x1C, 0x00, 0x01, 0x03, 0x0F, 0x3F, 0x3F, 0x1F, 0x0F, 0x07, 0x03,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x0C, 0x0C, 0x0C, 0x1C, 0x1E, 0x1E, 0x1E, 0x1E, 0x3E,
		0x3F, 0x3F, 0x3F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
		0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x3E, 0x3E, 0x1E, 0x1E, 0x1E,
		0x1C, 0x1C, 0x0C, 0x0C, 0x0C, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		};


//Created with online webtool from www.riyas.org
const uint8_t Pats[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x60, 0x60, 0x60, 0x60, 0x70, 0x70,
0x70, 0x70, 0x38, 0x38, 0x38, 0x38, 0x3c, 0x3c, 0x1c, 0x1c, 0x1e, 0x1e, 0x0e, 0x0e, 0x0f, 0x8f, 0x8f,
0x87, 0x87, 0xc7, 0xc7, 0xc7, 0xc0, 0x03, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x3e, 0x7e,
0x7c, 0x3c, 0x3c, 0xf8, 0xf8, 0xf8,0xf8, 0xf8, 0xf8, 0x78, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
0x01, 0x01, 0x01, 0x21, 0x31, 0x30, 0x70,0x71, 0x71, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xfb, 0xfb,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xf8, 0xf8, 0xf8,
0xf0, 0xc0, 0xfc, 0xfd, 0xfd, 0x7f, 0x1f, 0x03, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x0f, 0x0f,
0x1f, 0x3f, 0x7f, 0xdf, 0x9d, 0x18, 0x30, 0x60, 0xe4, 0x06, 0x06, 0x07, 0x07, 0x03, 0x83, 0x9f, 0xff,
0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x0c, 0x18, 0x30, 0xe0, 0xc0, 0x84, 0xf5, 0x6f, 0x03,0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};

uint8_t* get_image(k64_image_t im)
{
	uint8_t *ptr_ITESO = ITESO_image;
	uint8_t *ptr_Pats = Pats_image;

	switch(im)
	{
		case ITESO_im:
			return(ptr_ITESO);

		case Patriots:
			return(ptr_Pats);

		default:
			break;
	}

	return(ptr_ITESO);

}
