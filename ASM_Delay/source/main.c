
#include <stdint.h>
#include "GPIO.h"
#include "Bits.h"
#include "delay.h"
#include "MK64F12.h"

#define SYSTEM_CLOCK (21000000U)
#define uSECONDS (1U)
#define mSECONDS (0U)
#define DELAY (100U)

int main(void) {

	gpio_pin_control_register_t pin_config_ptb20 = GPIO_MUX1;

	GPIO_clock_gating(GPIO_B);
	GPIO_pin_control_register(GPIO_B, bit_20, &pin_config_ptb20);
	GPIO_data_direction_pin(GPIO_B, GPIO_OUTPUT, bit_20);
	GPIO_set_pin(GPIO_B, bit_20);
	GPIO_clear_pin(GPIO_B, bit_20);

    while(1) {

    	GPIO_set_pin(GPIO_B,bit_20);
    	Delay_ms_or_us(DELAY, SYSTEM_CLOCK, mSECONDS);
    	GPIO_clear_pin(GPIO_B,bit_20);
    	Delay_ms_or_us(DELAY, SYSTEM_CLOCK, mSECONDS);
    }
    return 0 ;
}
