################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/serial_manager/fsl_component_serial_manager.c \
../component/serial_manager/fsl_component_serial_port_uart.c 

OBJS += \
./component/serial_manager/fsl_component_serial_manager.o \
./component/serial_manager/fsl_component_serial_port_uart.o 

C_DEPS += \
./component/serial_manager/fsl_component_serial_manager.d \
./component/serial_manager/fsl_component_serial_port_uart.d 


# Each subdirectory must supply rules for building sources it contributes
component/serial_manager/%.o: ../component/serial_manager/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -DSDK_OS_BAREMETAL -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_OS_FREE_RTOS -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=1 -I"/Users/rjimenez/Documents/EMB2/clock/board" -I"/Users/rjimenez/Documents/EMB2/clock/source" -I"/Users/rjimenez/Documents/EMB2/clock/freertos/freertos_kernel/include" -I"/Users/rjimenez/Documents/EMB2/clock/freertos/template/ARM_CM4F" -I"/Users/rjimenez/Documents/EMB2/clock/freertos/freertos_kernel/portable/GCC/ARM_CM4F" -I"/Users/rjimenez/Documents/EMB2/clock/drivers" -I"/Users/rjimenez/Documents/EMB2/clock/component/uart" -I"/Users/rjimenez/Documents/EMB2/clock/component/serial_manager" -I"/Users/rjimenez/Documents/EMB2/clock/utilities" -I"/Users/rjimenez/Documents/EMB2/clock/component/lists" -I"/Users/rjimenez/Documents/EMB2/clock/CMSIS" -I"/Users/rjimenez/Documents/EMB2/clock/device" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


