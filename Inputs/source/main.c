
/**
 * @file    Inputs.c
 * RICARDO JIMENEZ SANCHEZ
 *
 */
#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "bits.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

/*
 * Lo entendido por el enunciado del problema y lo preguntado al profesor:
 * Al no presionar nada la secuencia es rojo->verde->azul
 * al presionar SW3 (manteniendo) se muestra secuencia blanco->apagado
 * al presionar SW2 (manteniendo) se muestran las secuencias mas rapido
 * si presionamos los dos botones se muestra la secuencia de blanco mas rapido
 */

void delay(uint32_t delay);

int main(void) {

	//delay big enough to see transition when /3
	uint32_t delay_const = 1000000;
	/**Variable to capture the input value*/
	uint32_t SW2 = 0;
	uint32_t SW3 = 0;
	/**Activating the GPIOB, GPIOC, GPIOA and GPIOE clock gating*/
	SIM->SCGC5 = 0x2E00;
	/**Pin control configuration of GPIOB pin22 and pin21 as GPIO*/
	PORTB->PCR[21] = 0x00000100;
	PORTB->PCR[22] = 0x00000100;
	/**Pin control configuration of GPIOC pin6 and GPIOA pin 4 as GPIO with is pull-up resistor
	enabled*/
	PORTC->PCR[6] = 0x00000103;
	PORTA->PCR[4] = 0x00000103;
	/**Pin control configuration of GPIOE pin26 as GPIO*/
	PORTE->PCR[26] = 0x00000100;
	/**Assigns a safe value to the output pin21 of the GPIOB*/
	GPIOB->PSOR = 0x00200000;
	/**Assigns a safe value to the output pin22 of the GPIOB*/
	GPIOB->PSOR |= 0x00400000;
	/**Assigns a safe value to the output pin26 of the GPIOE*/
	GPIOE->PSOR |= 0x04000000;
	//setup of SW3 and SW2 as input
	GPIOC->PDDR &=~(0x40);
	GPIOA->PDDR &=~(0x10);
	/**Configures GPIOB pin21 as output*/
	GPIOB->PDDR = 0x00200000;
	/**Configures GPIOB pin22 as output*/
	GPIOB->PDDR |= 0x00400000;
	/**Configures GPIOE pin26 as output*/
	GPIOE->PDDR |= 0x04000000;

	while(1) {
		/**Reads all the GPIOC and GPIOA*/
		SW2 = GPIOC->PDIR;
		SW3 = GPIOA->PDIR;

		/**Masks the GPIOC bit 6*/
		SW2 = SW2 & 0x40;

		//masks GPIOA bit 4
		SW3 = SW3 & 0x10;

		//added this command since using toggle register for white and blue led was on as initial state
		GPIOB->PSOR = 0x00200000;//Blue led off

		if(FALSE == SW3)
		{
			if(FALSE == SW2)
			{
				GPIOB->PTOR = 0x00400000;//RED led on
				GPIOE->PTOR = 0x4000000;//Green led on
				GPIOB->PTOR = 0x00200000;//Blue led on
				delay(delay_const/3);

				GPIOB->PTOR = 0x00400000;//Red led off
				GPIOE->PTOR = 0x4000000;//Green led off
				GPIOB->PTOR = 0x00200000;//Blue led off
				delay(delay_const/3);
			}

			else
			{
				GPIOB->PTOR = 0x00400000;//RED led on
				GPIOE->PTOR = 0x4000000;//Green led on
				GPIOB->PTOR = 0x00200000;//Blue led on
				delay(delay_const);

				GPIOB->PTOR = 0x00400000;//Red led off
				GPIOE->PTOR = 0x4000000;//Green led off
				GPIOB->PTOR = 0x00200000;//Blue led off
				delay(delay_const);
			}
		}

		else
		{
			if(FALSE == SW2)
			{
				GPIOB->PSOR = 0x00200000;//Blue led off
				GPIOB->PCOR = 0x00400000;//RED led on
				delay(delay_const/3);

				GPIOB->PSOR = 0x00400000;//Red led off
				GPIOE->PCOR = 0x4000000;//Green led on
				delay(delay_const/3);

				GPIOE->PSOR = 0x4000000;//Green led off
				GPIOB->PCOR = 0x00200000;//Blue led on
				delay(delay_const/3);
			}

			else
			{
				GPIOB->PSOR = 0x00200000;//Blue led off
				GPIOB->PCOR = 0x00400000;//RED led on
				delay(delay_const);

				GPIOB->PSOR = 0x00400000;//Red led off
				GPIOE->PCOR = 0x4000000;//Green led on
				delay(delay_const);

				GPIOE->PSOR = 0x4000000;//Green led off
				GPIOB->PCOR = 0x00200000;//Blue led on
				delay(delay_const);
			}
		}


	}
	return 0 ;
}

//changed to unit32 to have more time to see transitions
void delay(uint32_t delay)
{
	volatile uint32_t j;
	for(j = delay; j > 0; j--)
	{
		__asm("nop");
	}
}

