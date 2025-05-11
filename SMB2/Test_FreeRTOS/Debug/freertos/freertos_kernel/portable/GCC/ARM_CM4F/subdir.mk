################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.c 

OBJS += \
./freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.o 

C_DEPS += \
./freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/freertos_kernel/portable/GCC/ARM_CM4F/%.o: ../freertos/freertos_kernel/portable/GCC/ARM_CM4F/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_OS_FREE_RTOS -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rjimenez/Documents/EMB2/Test_FreeRTOS/board" -I"/Users/rjimenez/Documents/EMB2/Test_FreeRTOS/source" -I"/Users/rjimenez/Documents/EMB2/Test_FreeRTOS/freertos/freertos_kernel/include" -I"/Users/rjimenez/Documents/EMB2/Test_FreeRTOS/freertos/template/ARM_CM4F" -I"/Users/rjimenez/Documents/EMB2/Test_FreeRTOS/freertos/freertos_kernel/portable/GCC/ARM_CM4F" -I"/Users/rjimenez/Documents/EMB2/Test_FreeRTOS/drivers" -I"/Users/rjimenez/Documents/EMB2/Test_FreeRTOS/component/uart" -I"/Users/rjimenez/Documents/EMB2/Test_FreeRTOS/component/serial_manager" -I"/Users/rjimenez/Documents/EMB2/Test_FreeRTOS/utilities" -I"/Users/rjimenez/Documents/EMB2/Test_FreeRTOS/component/lists" -I"/Users/rjimenez/Documents/EMB2/Test_FreeRTOS/CMSIS" -I"/Users/rjimenez/Documents/EMB2/Test_FreeRTOS/device" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


