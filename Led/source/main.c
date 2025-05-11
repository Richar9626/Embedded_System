/** RICARDO JIMENEZ SANCHEZ */
/* TAREA 3 LED RGB
 * PRENDEMOS LEDS EN K64 ROJO->AZUL->VERDE->BLANCO*/
#include <stdio.h>
/**Standard integer definition provided by the compiler*/
#include <stdint.h>
#include "MK64F12.h"

//#define DEBUG_ON

int main(void) {
	/**Se cambia el clock para activar puertos*/
	SIM->SCGC5 = 0x2400;
	/**Pin control configuration of GPIOB pin22 and pin21, GPIOE 26 as GPIO*/
	PORTB->PCR[22] = 0x00000100;
	PORTB->PCR[21] = 0x00000100;
	PORTE->PCR[26] = 0x00000100;
	/**Assigns a safe value to the output pin*/
	GPIOB->PDOR = 0x00600000; //6 para ambos rojo y azul, bit 21 y 22
	GPIOE->PDOR = 0x04000000; //bit 26
	/**Configures GPIOB pin22, pin21 and GPIOE pin26 as output*/
	GPIOB->PDDR = 0x00600000;
	GPIOE->PDDR = 0x04000000;


	while(1) {


		GPIOB->PDOR = 0x00200000;

		#ifndef DEBUG_ON
			printf("BLUE LED ONLY OFF\n");
		#endif

		GPIOB->PDOR = 0x00600000;
		#ifndef DEBUG_ON
			printf("BLUE/RED LEDS OFF\n");
		#endif

		GPIOB->PDOR = 0x00400000;

		#ifndef DEBUG_ON
			printf("RED LED ONLY OFF\n");
		#endif

		GPIOB->PDOR = 0x00600000;

		#ifndef DEBUG_ON
			printf("BLUE/RED LEDS OFF\n");
		#endif

		GPIOE->PDOR = 0;

        #ifndef DEBUG_ON
			printf("GREEN LED ON\n");
		#endif

		GPIOE->PDOR = 0x04000000;

		#ifndef DEBUG_ON
			printf("GREEN LED ONLY OFF\n");
		#endif
//DEBIDO A QUE NO PODEMOS APAGAR AMBOS AL MISMO TIEMPO SE OBTIENE UN VALOR INTERMEDIO
		GPIOB->PDOR = 0;
		GPIOE->PDOR = 0;

		#ifndef DEBUG_ON
			printf("ALL ON\n");
		#endif

		GPIOE->PDOR = 0x04000000;

		#ifndef DEBUG_ON
			printf("GREEN LED ONLY OFF\n");
		#endif
//NUEVAMENTE NO PODEMOS APAGAR AMBOS AL MISMO TIEMPO SE OBTIENE UN VALOR INTERMEDIO

		}
	return 0 ;
}


