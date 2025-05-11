/**
	\file
	\brief
		This is the source file for the GPIO device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
		i.e., this is the application programming interface (API) for the GPIO peripheral.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	7/09/2014
	\todo
	    Interrupts are not implemented in this API implementation.
 */
#include "MK64F12.h"
#include "GPIO.h"
#include "Bits.h"



uint8_t g_flag_port_C = FALSE;
uint8_t g_flag_port_A = FALSE;


void PORTC_IRQHandler(void)
{
	g_flag_port_C = TRUE;
	GPIO_clear_interrupt(GPIO_C);

}


void PORTA_IRQHandler(void)
{
	while(1);
}


void GPIO_clear_interrupt(gpio_port_name_t port_name)
{
	switch(port_name)/** Selecting the GPIO for clock enabling*/
	{
		case GPIO_A: /** GPIO A is selected*/
			PORTA->ISFR=0xFFFFFFFF;
			break;
		case GPIO_B: /** GPIO B is selected*/
			PORTB->ISFR=0xFFFFFFFF;
			break;
		case GPIO_C: /** GPIO C is selected*/
			PORTC->ISFR = 0xFFFFFFFF;
			break;
		case GPIO_D: /** GPIO D is selected*/
			PORTD->ISFR=0xFFFFFFFF;
			break;
		default: /** GPIO E is selected*/
			PORTE->ISFR=0xFFFFFFFF;
			break;

	}// end switch
}
uint8_t GPIO_clock_gating(gpio_port_name_t port_name)
{
	switch(port_name)/** Selecting the GPIO for clock enabling*/
			{
				case GPIO_A: /** GPIO A is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTA; /** Bit 9 of SIM_SCGC5 is  set*/
					break;
				case GPIO_B: /** GPIO B is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTB; /** Bit 10 of SIM_SCGC5 is set*/
					break;
				case GPIO_C: /** GPIO C is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTC; /** Bit 11 of SIM_SCGC5 is set*/
					break;
				case GPIO_D: /** GPIO D is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTD; /** Bit 12 of SIM_SCGC5 is set*/
					break;
				case GPIO_E: /** GPIO E is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTE; /** Bit 13 of SIM_SCGC5 is set*/
					break;
				default: /**If doesn't exist the option*/
					return(FALSE);
			}// end switch
	/**Successful configuration*/
	return(TRUE);
}// end function

uint8_t GPIO_pin_control_register(gpio_port_name_t port_name, uint8_t pin,const gpio_pin_control_register_t*  pin_control_register)
{

	switch(port_name)
		{
		case GPIO_A:/** GPIO A is selected*/
			PORTA->PCR[pin] = *pin_control_register;
			break;
		case GPIO_B:/** GPIO B is selected*/
			PORTB->PCR[pin] = *pin_control_register;
			break;
		case GPIO_C:/** GPIO C is selected*/
			PORTC->PCR[pin] = *pin_control_register;
			break;
		case GPIO_D:/** GPIO D is selected*/
			PORTD->PCR[pin] = *pin_control_register;
			break;
		case GPIO_E: /** GPIO E is selected*/
			PORTE->PCR[pin]= *pin_control_register;
		default:/**If doesn't exist the option*/
			return(FALSE);
		break;
		}
	/**Successful configuration*/
	return(TRUE);
}
