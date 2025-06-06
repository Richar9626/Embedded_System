################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_adc16.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_dspi.c \
../drivers/fsl_dspi_freertos.c \
../drivers/fsl_flash.c \
../drivers/fsl_gpio.c \
../drivers/fsl_i2c.c \
../drivers/fsl_i2c_freertos.c \
../drivers/fsl_llwu.c \
../drivers/fsl_lptmr.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_lpuart_freertos.c \
../drivers/fsl_ltc.c \
../drivers/fsl_pmc.c \
../drivers/fsl_rtc.c \
../drivers/fsl_smc.c \
../drivers/fsl_tpm.c \
../drivers/fsl_trng.c \
../drivers/fsl_tsi_v4.c 

OBJS += \
./drivers/fsl_adc16.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_dspi.o \
./drivers/fsl_dspi_freertos.o \
./drivers/fsl_flash.o \
./drivers/fsl_gpio.o \
./drivers/fsl_i2c.o \
./drivers/fsl_i2c_freertos.o \
./drivers/fsl_llwu.o \
./drivers/fsl_lptmr.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_lpuart_freertos.o \
./drivers/fsl_ltc.o \
./drivers/fsl_pmc.o \
./drivers/fsl_rtc.o \
./drivers/fsl_smc.o \
./drivers/fsl_tpm.o \
./drivers/fsl_trng.o \
./drivers/fsl_tsi_v4.o 

C_DEPS += \
./drivers/fsl_adc16.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_dspi.d \
./drivers/fsl_dspi_freertos.d \
./drivers/fsl_flash.d \
./drivers/fsl_gpio.d \
./drivers/fsl_i2c.d \
./drivers/fsl_i2c_freertos.d \
./drivers/fsl_llwu.d \
./drivers/fsl_lptmr.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_lpuart_freertos.d \
./drivers/fsl_ltc.d \
./drivers/fsl_pmc.d \
./drivers/fsl_rtc.d \
./drivers/fsl_smc.d \
./drivers/fsl_tpm.d \
./drivers/fsl_trng.d \
./drivers/fsl_tsi_v4.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MKW41Z512VHT4_cm0plus -DCPU_MKW41Z512VHT4 -DFSL_RTOS_FREE_RTOS -DFRDM_KW41Z -DFREEDOM -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=1 -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/board" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/source" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/CMSIS" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/OSAbstraction/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/freertos" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/ieee_802.15.4/mac/source/App" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/ieee_802.15.4/mac/interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/ieee_802.15.4/phy/interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/GPIO" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/Keyboard/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/LED/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/SerialManager/Source/SPI_Adapter" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/SerialManager/Source/UART_Adapter" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/SerialManager/Source/I2C_Adapter" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/Flash/Internal" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/common" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/MemManager/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/Messaging/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/Panic/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/RNG/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/SerialManager/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/TimersManager/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/TimersManager/Source" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/ModuleInfo" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/FunctionLib" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/Lists" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/SecLib" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/NVM/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/NVM/Source" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/LowPower/Interface/MKW41Z" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/LowPower/Source/MKW41Z" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/MWSCoexistence/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/drivers" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/DCDC/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/framework/XCVR/MKW41Z4" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/ieee_802.15.4/phy/source/MKW41Z" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/utilities" -O0 -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -imacros "/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_end_device_freertos/source/app_preinclude.h" -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


