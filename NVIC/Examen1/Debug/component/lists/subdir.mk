################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/lists/fsl_component_generic_list.c 

OBJS += \
./component/lists/fsl_component_generic_list.o 

C_DEPS += \
./component/lists/fsl_component_generic_list.d 


# Each subdirectory must supply rules for building sources it contributes
component/lists/%.o: ../component/lists/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Examen1/board" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Examen1/source" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Examen1/drivers" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Examen1/utilities" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Examen1/component/serial_manager" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Examen1/component/uart" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Examen1/component/lists" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Examen1/CMSIS" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Examen1/device" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


