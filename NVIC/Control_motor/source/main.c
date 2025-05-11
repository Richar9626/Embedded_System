#include <stdio.h>
#include <stdint.h>
#include "NVIC.h"
#include "Bits.h"
#include "GPIO.h"
#include "PIT.h"
#include "motor_control.h"
#include "MK64F12.h"

//pit1
int main(void) {

	motor_init();
	PIT_clock_gating();
	PIT_enable();

	PIT_enable_interrupt(PIT_1);
	/**Sets the threshold for interrupts, if the interrupt has higher priority constant that the BASEPRI, the interrupt will not be attended*/
	NVIC_set_basepri_threshold(PRIORITY_10);
	/**Enables and sets a particular interrupt and its priority*/
	NVIC_enable_interrupt_and_priotity(PORTC_IRQ,PRIORITY_5);
	NVIC_enable_interrupt_and_priotity(PIT_CH1_IRQ, PRIORITY_6);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_6);
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_6);

	NVIC_global_enable_interrupts;

	/** Callbacks for GPIO */
	//GPIO_callback_init(GPIO_C, motor_function);
	GPIO_callback_init(GPIO_C, motor_enable);

while(1)
{

}

    return 0 ;
}
