/*
 * SDK_GPIO.c
 *
 *  Created on: 14 may. 2021
 *      Author: ruval
 */
#include "SDK_GPIO.h"

/* Whether the SW button is pressed */
bool g_ButtonPress = false;
uint8_t select = 0;

void GPIO_Init(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };

    /* Define the init structure for the output LED pin */
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    const port_pin_config_t porta4_pin38_config = {
      kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
      kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
      kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
      kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
      kPORT_HighDriveStrength,                                 /* High drive strength is configured */
      kPORT_MuxAsGpio,                                         /* Pin is configured as PTA4 */
      kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
    };
    PORT_SetPinConfig(PORTC, 6, &porta4_pin38_config);  /* PORTA4 (pin 38) is configured as PTA4 */

/* Init input switch GPIO. */
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
    GPIO_SetPinInterruptConfig(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, kGPIO_InterruptFallingEdge);
#else
    PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);
#endif
    EnableIRQ(BOARD_SW_IRQ);
    GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);

    /* Init output LED GPIO. */
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, &led_config);
}

void BOARD_SW_IRQ_HANDLER(void)
{
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
    /* Clear external interrupt flag. */
    GPIO_GpioClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
#else
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
#endif
    /* Change state of button. */
    g_ButtonPress = true;
    SDK_ISR_EXIT_BARRIER;
    select = select == 2? 0:select + 1;
}

uint8_t GPIO_Signal_Select(void)
{
	return select;
}
