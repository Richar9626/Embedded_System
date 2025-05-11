################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/croutine.c \
../freertos/event_groups.c \
../freertos/fsl_tickless_lptmr.c \
../freertos/fsl_tickless_systick.c \
../freertos/heap_4.c \
../freertos/list.c \
../freertos/port.c \
../freertos/queue.c \
../freertos/tasks.c \
../freertos/timers.c 

OBJS += \
./freertos/croutine.o \
./freertos/event_groups.o \
./freertos/fsl_tickless_lptmr.o \
./freertos/fsl_tickless_systick.o \
./freertos/heap_4.o \
./freertos/list.o \
./freertos/port.o \
./freertos/queue.o \
./freertos/tasks.o \
./freertos/timers.o 

C_DEPS += \
./freertos/croutine.d \
./freertos/event_groups.d \
./freertos/fsl_tickless_lptmr.d \
./freertos/fsl_tickless_systick.d \
./freertos/heap_4.d \
./freertos/list.d \
./freertos/port.d \
./freertos/queue.d \
./freertos/tasks.d \
./freertos/timers.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/%.o: ../freertos/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKW41Z512VHT4 -DFSL_RTOS_FREE_RTOS -DFRDM_KW41Z -DFREEDOM -DCPU_MKW41Z512VHT4_cm0plus -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/board" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/source" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/CMSIS" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/OSAbstraction/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/freertos" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/ieee_802.15.4/mac/source/App" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/ieee_802.15.4/mac/interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/ieee_802.15.4/phy/interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/GPIO" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/Keyboard/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/LED/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/SerialManager/Source/SPI_Adapter" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/SerialManager/Source/UART_Adapter" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/SerialManager/Source/I2C_Adapter" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/Flash/Internal" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/common" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/MemManager/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/Messaging/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/Panic/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/RNG/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/SerialManager/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/TimersManager/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/TimersManager/Source" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/ModuleInfo" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/FunctionLib" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/Lists" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/SecLib" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/MWSCoexistence/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/drivers" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/DCDC/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/framework/XCVR/MKW41Z4" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/ieee_802.15.4/phy/source/MKW41Z" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/utilities" -O0 -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -imacros "/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_ieee_802_15_4_mwa_coordinator_freertos_pr/source/app_preinclude.h" -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


