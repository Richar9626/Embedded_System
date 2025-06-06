################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ieee_802.15.4/phy/source/MKW41Z/ASP.c \
../ieee_802.15.4/phy/source/MKW41Z/MPM.c \
../ieee_802.15.4/phy/source/MKW41Z/PhyISR.c \
../ieee_802.15.4/phy/source/MKW41Z/PhyPacketProcessor.c \
../ieee_802.15.4/phy/source/MKW41Z/PhyPlmeData.c \
../ieee_802.15.4/phy/source/MKW41Z/PhyStateMachine.c \
../ieee_802.15.4/phy/source/MKW41Z/PhyTime.c 

OBJS += \
./ieee_802.15.4/phy/source/MKW41Z/ASP.o \
./ieee_802.15.4/phy/source/MKW41Z/MPM.o \
./ieee_802.15.4/phy/source/MKW41Z/PhyISR.o \
./ieee_802.15.4/phy/source/MKW41Z/PhyPacketProcessor.o \
./ieee_802.15.4/phy/source/MKW41Z/PhyPlmeData.o \
./ieee_802.15.4/phy/source/MKW41Z/PhyStateMachine.o \
./ieee_802.15.4/phy/source/MKW41Z/PhyTime.o 

C_DEPS += \
./ieee_802.15.4/phy/source/MKW41Z/ASP.d \
./ieee_802.15.4/phy/source/MKW41Z/MPM.d \
./ieee_802.15.4/phy/source/MKW41Z/PhyISR.d \
./ieee_802.15.4/phy/source/MKW41Z/PhyPacketProcessor.d \
./ieee_802.15.4/phy/source/MKW41Z/PhyPlmeData.d \
./ieee_802.15.4/phy/source/MKW41Z/PhyStateMachine.d \
./ieee_802.15.4/phy/source/MKW41Z/PhyTime.d 


# Each subdirectory must supply rules for building sources it contributes
ieee_802.15.4/phy/source/MKW41Z/%.o: ../ieee_802.15.4/phy/source/MKW41Z/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MKW41Z512VHT4_cm0plus -DCPU_MKW41Z512VHT4 -DFSL_RTOS_FREE_RTOS -DFRDM_KW41Z -DFREEDOM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/board" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/source" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/CMSIS" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/OSAbstraction/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/freertos" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/ieee_802.15.4/mac/source/App" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/ieee_802.15.4/mac/interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/ieee_802.15.4/phy/interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/GPIO" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/Keyboard/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/LED/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/SerialManager/Source/SPI_Adapter" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/SerialManager/Source/UART_Adapter" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/SerialManager/Source/I2C_Adapter" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/Flash/Internal" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/common" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/MemManager/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/Messaging/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/Panic/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/RNG/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/SerialManager/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/TimersManager/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/TimersManager/Source" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/ModuleInfo" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/FunctionLib" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/Lists" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/SecLib" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/MWSCoexistence/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/drivers" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/DCDC/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/framework/XCVR/MKW41Z4" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/ieee_802.15.4/phy/source/MKW41Z" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/utilities" -O0 -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -imacros "/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos/source/app_preinclude.h" -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


