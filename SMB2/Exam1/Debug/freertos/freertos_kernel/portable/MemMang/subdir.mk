################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/freertos_kernel/portable/MemMang/heap_4.c 

OBJS += \
./freertos/freertos_kernel/portable/MemMang/heap_4.o 

C_DEPS += \
./freertos/freertos_kernel/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/freertos_kernel/portable/MemMang/%.o: ../freertos/freertos_kernel/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_OS_FREE_RTOS -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rjimenez/Documents/EMB2/Exam1/board" -I"/Users/rjimenez/Documents/EMB2/Exam1/source" -I"/Users/rjimenez/Documents/EMB2/Exam1/freertos/freertos_kernel/include" -I"/Users/rjimenez/Documents/EMB2/Exam1/freertos/template/ARM_CM4F" -I"/Users/rjimenez/Documents/EMB2/Exam1/freertos/freertos_kernel/portable/GCC/ARM_CM4F" -I"/Users/rjimenez/Documents/EMB2/Exam1/drivers" -I"/Users/rjimenez/Documents/EMB2/Exam1/component/uart" -I"/Users/rjimenez/Documents/EMB2/Exam1/component/serial_manager" -I"/Users/rjimenez/Documents/EMB2/Exam1/utilities" -I"/Users/rjimenez/Documents/EMB2/Exam1/component/lists" -I"/Users/rjimenez/Documents/EMB2/Exam1/CMSIS" -I"/Users/rjimenez/Documents/EMB2/Exam1/device" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


