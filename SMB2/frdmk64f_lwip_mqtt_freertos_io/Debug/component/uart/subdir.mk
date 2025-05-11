################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/uart/fsl_adapter_uart.c 

OBJS += \
./component/uart/fsl_adapter_uart.o 

C_DEPS += \
./component/uart/fsl_adapter_uart.d 


# Each subdirectory must supply rules for building sources it contributes
component/uart/%.o: ../component/uart/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -D_POSIX_SOURCE -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K64F -DFREEDOM -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=0 -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/board" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/source" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/mdio" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/phy" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/lwip/src/include/lwip/apps" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/lwip/port" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/lwip/src" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/lwip/src/include" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/drivers" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/utilities" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/device" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/component/uart" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/component/serial_manager" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/component/lists" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/CMSIS" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/freertos/freertos_kernel/include" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_lwip_mqtt_freertos_io/freertos/freertos_kernel/portable/GCC/ARM_CM4F" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


