#include <stdio.h>
#include <stdint.h>
#include "NVIC.h"
#include "Bits.h"
#include "GPIO.h"
#include "MK64F12.h"

extern gpio_interrupt_flags_t g_intr_status_flag;

int main(void) {


	uint8_t state_port_C = 0;
	uint8_t state_port_A = 0;

	gpio_pin_control_register_t input_intr_config = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_FALLING_EDGE;

	/**Activating the GPIOA, GPIOB and GPIOC clock gating*/
	GPIO_clock_gating(GPIO_A);
	GPIO_clock_gating(GPIO_B);
	GPIO_clock_gating(GPIO_C);


	/**Pin control configuration of GPIOB pin21 and pin21 as GPIO by using a special macro contained in Kinetis studio in the MK64F12. h file*/
	PORTB->PCR[21] = PORT_PCR_MUX(1);
	PORTB->PCR[22] = PORT_PCR_MUX(1);
	/**Pin control configuration of GPIOA pin4 as a GPIO with pull-up resistor and interrupt in falling edge*/
	GPIO_pin_control_register(GPIO_A, bit_4, &input_intr_config);
	/**Pin control configuration of GPIOC pin6 as a GPIO with pull-up resistor and interrupt in falling edge*/
	PORTC->PCR[6] = GPIO_MUX1 | GPIO_PE |GPIO_PS | INTR_FALLING_EDGE;


	/**Assigns a safe value to the output pin21 of the GPIOB*/
	GPIOB->PDOR |= 0x00200000;/**Blue led off*/
	GPIOB->PDOR |= 0x00400000;/**Read led off*/

	/**Configures GPIOB pin21 as output*/
	GPIOB->PDDR = 0x00200000;
	/**Configures GPIOB pin22 as output*/
	GPIOB->PDDR |= 0x00400000;
	/**Configures GPIOC pin6 as input*/
	GPIOC->PDDR &=~(0x40);
	/**Sets the threshold for interrupts, if the interrupt has higher priority constant that the BASEPRI, the interrupt will not be attended*/
	NVIC_set_basepri_threshold(PRIORITY_10);
	/**Enables and sets a particular interrupt and its priority*/
	NVIC_enable_interrupt_and_priotity(PORTC_IRQ,PRIORITY_5);
	/**Enables and sets a particular interrupt and its priority*/
	NVIC_enable_interrupt_and_priotity(PORTA_IRQ,PRIORITY_4);

	NVIC_global_enable_interrupts;

    while(1)
    {
    	if(TRUE == GPIO_get_irq_status(GPIO_C))
    	{
			if(state_port_C)
			{
				GPIOB->PDOR |= 0x00200000;/**Blue led off*/
			}
			else
			{
				GPIOB->PDOR &= ~(0x00200000);/**Blue led on*/
			}
			state_port_C = !state_port_C;
			GPIO_clear_irq_status(GPIO_C);
    	}


	if(TRUE == GPIO_get_irq_status(GPIO_A))
	{
		if(state_port_A)
		{
			GPIOB->PDOR |= 0x00400000;/**Read led off*/
		}
		else
		{
			GPIOB->PDOR &= ~(0x00400000);/**Read led on*/
		}
		state_port_A = !state_port_A;
		GPIO_clear_irq_status(GPIO_A);
	}
    }
    return 0 ;
}
