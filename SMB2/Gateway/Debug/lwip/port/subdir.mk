################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/port/enet_ethernetif.c \
../lwip/port/enet_ethernetif_kinetis.c \
../lwip/port/sys_arch.c 

OBJS += \
./lwip/port/enet_ethernetif.o \
./lwip/port/enet_ethernetif_kinetis.o \
./lwip/port/sys_arch.o 

C_DEPS += \
./lwip/port/enet_ethernetif.d \
./lwip/port/enet_ethernetif_kinetis.d \
./lwip/port/sys_arch.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/port/%.o: ../lwip/port/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -D_POSIX_SOURCE -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K64F -DFREEDOM -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rjimenez/Documents/EMB2/Gateway/board" -I"/Users/rjimenez/Documents/EMB2/Gateway/source" -I"/Users/rjimenez/Documents/EMB2/Gateway/mdio" -I"/Users/rjimenez/Documents/EMB2/Gateway/phy" -I"/Users/rjimenez/Documents/EMB2/Gateway/lwip/src/include/lwip/apps" -I"/Users/rjimenez/Documents/EMB2/Gateway/lwip/port" -I"/Users/rjimenez/Documents/EMB2/Gateway/lwip/src" -I"/Users/rjimenez/Documents/EMB2/Gateway/lwip/src/include" -I"/Users/rjimenez/Documents/EMB2/Gateway/drivers" -I"/Users/rjimenez/Documents/EMB2/Gateway/utilities" -I"/Users/rjimenez/Documents/EMB2/Gateway/device" -I"/Users/rjimenez/Documents/EMB2/Gateway/component/uart" -I"/Users/rjimenez/Documents/EMB2/Gateway/component/serial_manager" -I"/Users/rjimenez/Documents/EMB2/Gateway/component/lists" -I"/Users/rjimenez/Documents/EMB2/Gateway/CMSIS" -I"/Users/rjimenez/Documents/EMB2/Gateway/freertos/freertos_kernel/include" -I"/Users/rjimenez/Documents/EMB2/Gateway/freertos/freertos_kernel/portable/GCC/ARM_CM4F" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


