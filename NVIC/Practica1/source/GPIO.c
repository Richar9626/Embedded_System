/**
	\file
	\brief
		This is the source file for the GPIO device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
		i.e., this is the application programming interface (API) for the GPIO peripheral.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\Ricardo Jimenez Sanchez
	\ Fernando Curiel Solis
	\date	18/02/2021
	\todo
	    Interrupts are not implemented in this API implementation.
 */
#include "MK64F12.h"
#include "GPIO.h"
#include "bits.h"
#include <stdio.h>

/*Callbacks for each gpio defined*/
static void (*gpio_A_callback)(void) = 0;
static void (*gpio_B_callback)(void) = 0;
static void (*gpio_C_callback)(void) = 0;
static void (*gpio_D_callback)(void) = 0;
static void (*gpio_E_callback)(void) = 0;

static gpio_interrupt_flags_t g_intr_status_flag = {0};

/*Use of callbacks, we pass the function when using it */
void GPIO_callback_init(gpio_port_name_t port_name,void (*handler)(void))
{
	switch(port_name)/** Selecting the GPIO for clock enabling*/
	{
		case GPIO_A: /** GPIO A is selected*/
			gpio_A_callback = handler;
			break;
		case GPIO_B: /** GPIO B is selected*/
			gpio_B_callback = handler;
			break;
		case GPIO_C: /** GPIO C is selected*/
			gpio_C_callback = handler;
			break;
		case GPIO_D: /** GPIO D is selected*/
			gpio_D_callback = handler;
			break;
		default: /** GPIO E is selected*/
			gpio_E_callback = handler;
			break;
	}

}

/*IRS for port A, used for SW3*/
void PORTA_IRQHandler(void)
{
	g_intr_status_flag.flag_port_a = TRUE;
	if(gpio_A_callback)
	{
		gpio_A_callback();
	}

	GPIO_clear_interrupt(GPIO_A);

}

/*IRS for port B, used for keypad*/
void PORTB_IRQHandler(void)
{
	g_intr_status_flag.flag_port_b = TRUE;
	if(gpio_B_callback)
	{
		gpio_B_callback();
	}

	GPIO_clear_interrupt(GPIO_B);
}

/*IRS for port C, used for SW2*/
void PORTC_IRQHandler(void)
{
	g_intr_status_flag.flag_port_c = TRUE;
	if(gpio_C_callback)
	{
		gpio_C_callback();
	}

	GPIO_clear_interrupt(GPIO_C);

}




void GPIO_clear_irq_status(gpio_port_name_t gpio)
{
	switch(gpio)/** Selecting the GPIO for clock enabling*/
	{
		case GPIO_A: /** GPIO A is selected*/
			g_intr_status_flag.flag_port_a = FALSE;
			break;
		case GPIO_B: /** GPIO B is selected*/
			g_intr_status_flag.flag_port_b = FALSE;
			break;
		case GPIO_C: /** GPIO C is selected*/
			g_intr_status_flag.flag_port_c = FALSE;
			break;
		case GPIO_D: /** GPIO D is selected*/
			g_intr_status_flag.flag_port_d = FALSE;
			break;
		default: /** GPIO E is selected*/
			g_intr_status_flag.flag_port_e = FALSE;
			break;
	}

}

uint8_t GPIO_get_irq_status(gpio_port_name_t gpio)
{
	uint8_t status = 0;

	switch(gpio)/** Selecting the GPIO for clock enabling*/
	{
		case GPIO_A: /** GPIO A is selected*/
			status = g_intr_status_flag.flag_port_a;
			break;
		case GPIO_B: /** GPIO B is selected*/
			status = g_intr_status_flag.flag_port_b;
			break;
		case GPIO_C: /** GPIO C is selected*/
			status = g_intr_status_flag.flag_port_c;
			break;
		case GPIO_D: /** GPIO D is selected*/
			status = g_intr_status_flag.flag_port_d;
			break;
		default: /** GPIO E is selected*/
			status = g_intr_status_flag.flag_port_e;
			break;
	}
	return(status);
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
		break;
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
		break;
		default:/**If doesn't exist the option*/
			return(FALSE);
		break;
	}
	/**Successful configuration*/
	return(TRUE);
}

//we use PDOr to write exactly data received
void GPIO_write_port(gpio_port_name_t port_name, uint32_t data)
{
	switch(port_name)
	{
		case GPIO_A:/** GPIO A is selected*/
			GPIOA->PDOR = data;
		break;
		case GPIO_B:/** GPIO B is selected*/
			GPIOB->PDOR = data;
		break;
		case GPIO_C:/** GPIO C is selected*/
			GPIOC->PDOR = data;
		break;
		case GPIO_D:/** GPIO D is selected*/
			GPIOD->PDOR = data;
		break;
		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PDOR = data;
		break;
		default:/**If doesn't exist the option*/
			return;
		break;
	}
	/**Successful configuration*/
	return;
}

uint32_t GPIO_read_port(gpio_port_name_t port_name)
{
	uint32_t port_value = 0x00000000;//initialized to avoid random numbers
	switch(port_name)
	{
		case GPIO_A:/** GPIO A is selected*/
			port_value = GPIOA->PDIR;
		break;
		case GPIO_B:/** GPIO B is selected*/
			port_value = GPIOB->PDIR;
		break;
		case GPIO_C:/** GPIO C is selected*/
			port_value = GPIOC->PDIR;
		break;
		case GPIO_D:/** GPIO D is selected*/
			port_value = GPIOD->PDIR;
		break;
		case GPIO_E: /** GPIO E is selected*/
			port_value = GPIOE->PDIR;
		break;
		default:/**If doesn't exist the option*/
			return(port_value);
		break;
	}
		/**Successful configuration*/
	return(port_value);
}

uint8_t GPIO_read_pin(gpio_port_name_t port_name, uint8_t pin)
{
	uint32_t mask_value = 0x00000001;  //variable for changing pin decimal to pin position
	uint32_t port_value = 0x00000000;
	uint8_t pin_value = 0;

	//to mask port value with the desired pin
	mask_value = mask_value << pin;

	switch(port_name)
	{
		case GPIO_A:/** GPIO A is selected*/
			port_value = GPIOA->PDIR;
			pin_value = (port_value & mask_value) >> pin;//we make a shift to have the bit masked on bit 0
		break;
		case GPIO_B:/** GPIO B is selected*/
			port_value = GPIOB->PDIR;
			pin_value = (port_value & mask_value) >> pin;
		break;
		case GPIO_C:/** GPIO C is selected*/
			port_value = GPIOC->PDIR;
			pin_value = (port_value & mask_value) >> pin;
		break;
		case GPIO_D:/** GPIO D is selected*/
			port_value = GPIOD->PDIR;
			pin_value = (port_value & mask_value) >> pin;
		break;
		case GPIO_E: /** GPIO E is selected*/
			port_value = GPIOE->PDIR;
			pin_value = (port_value & mask_value) >> pin;
		break;
		default:/**If doesn't exist the option*/
			return(pin_value);
		break;
	}
	/**Successful configuration*/
	return(pin_value);
}

//same logic were we make a mask to set a specific pin
void GPIO_set_pin(gpio_port_name_t port_name, uint8_t pin)
{
	uint32_t mask_value = 0x00000001;  //variable for changing pin decimal to pin position

	switch(port_name)
	{
		case GPIO_A:/** GPIO A is selected*/
			GPIOA->PSOR = mask_value << pin;
		break;
		case GPIO_B:/** GPIO B is selected*/
			GPIOB->PSOR = mask_value << pin;
		break;
		case GPIO_C:/** GPIO C is selected*/
			GPIOC->PSOR = mask_value << pin;
		break;
		case GPIO_D:/** GPIO D is selected*/
			GPIOD->PSOR = mask_value << pin;
		break;
		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PSOR = mask_value << pin;
		break;
		default:/**If doesn't exist the option*/
			return;
		break;
	}
	/**Successful configuration*/
	return;
}
/*****************************************************************************************************************/
/*****************************************************************************************************************/
/*****************************************************************************************************************/
void GPIO_clear_pin(gpio_port_name_t port_name, uint8_t pin)
{
	uint32_t mask_value = 0x00000001;  //variable for changing pin decimal to pin position

	switch(port_name)
	{
		case GPIO_A:/** GPIO A is selected*/
			GPIOA->PCOR = mask_value << pin;
		break;
		case GPIO_B:/** GPIO B is selected*/
			GPIOB->PCOR = mask_value << pin;
		break;
		case GPIO_C:/** GPIO C is selected*/
			GPIOC->PCOR = mask_value << pin;
		break;
		case GPIO_D:/** GPIO D is selected*/
			GPIOD->PCOR = mask_value << pin;
		break;
		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PCOR = mask_value << pin;
		break;
		default:/**If doesn't exist the option*/
			return;
		break;
	}
	/**Successful configuration*/
	return;
}

void GPIO_toogle_pin(gpio_port_name_t port_name, uint8_t pin)
{
	uint32_t mask_value = 0x00000001;  //variable for changing pin decimal to pin position

	switch(port_name)
	{
		case GPIO_A:/** GPIO A is selected*/
			GPIOA->PTOR = mask_value << pin;
		break;
		case GPIO_B:/** GPIO B is selected*/
			GPIOB->PTOR = mask_value << pin;
		break;
		case GPIO_C:/** GPIO C is selected*/
			GPIOC->PTOR = mask_value << pin;
		break;
		case GPIO_D:/** GPIO D is selected*/
			GPIOD->PTOR = mask_value << pin;
		break;
		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PTOR = mask_value << pin;
		break;
		default:/**If doesn't exist the option*/
			return;
		break;
	}
	/**Successful configuration*/
	return;
}

//we enable the whole port as input or output
void GPIO_data_direction_port(gpio_port_name_t port_name ,uint32_t direction)
{
	switch(port_name)
	{
		case GPIO_A:/** GPIO A is selected*/
			GPIOA->PDDR = ~(direction);
		break;
		case GPIO_B:/** GPIO B is selected*/
			GPIOB->PDDR = ~(direction);
		break;
		case GPIO_C:/** GPIO C is selected*/
			GPIOC->PDDR = ~(direction);
		break;
		case GPIO_D:/** GPIO D is selected*/
			GPIOD->PDDR = ~(direction);
		break;
		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PDDR = ~(direction);
		break;
		default:/**If doesn't exist the option*/
			return;
		break;
	}
	/**Successful configuration*/
	return;
}

//we need a condition to denable specific pin as output or input
void GPIO_data_direction_pin(gpio_port_name_t port_name, uint8_t state, uint8_t pin)
{
	uint32_t mask_value = 0x00000001;  //variable for changing pin decimal to pin position
	mask_value = mask_value << pin;

	switch(port_name)
	{
		case GPIO_A:/** GPIO A is selected*/
			if(state){
				GPIOA->PDDR |= mask_value;
			}
			else{
				GPIOA->PDDR &= ~(mask_value);
			}
		break;
		case GPIO_B:/** GPIO B is selected*/
			if(state){
				GPIOB->PDDR |= mask_value;
			}
			else{
				GPIOB->PDDR &= ~(mask_value);
			}
		break;
		case GPIO_C:/** GPIO C is selected*/
			if(state){
				GPIOC->PDDR |= mask_value;
			}
			else{
				GPIOC->PDDR &= ~(mask_value);
			}
		break;
		case GPIO_D:/** GPIO D is selected*/
			if(state){
				GPIOD->PDDR |= mask_value;
			}
			else{
				GPIOD->PDDR &= ~(mask_value);
			}
		break;
		case GPIO_E: /** GPIO E is selected*/
			if(state){
				GPIOE->PDDR |= mask_value;
			}
			else{
				GPIOE->PDDR &= ~(mask_value);
			}
		break;
		default:/**If doesn't exist the option*/
			return;
		break;
	}
	/**Successful configuration*/
	return;
}
