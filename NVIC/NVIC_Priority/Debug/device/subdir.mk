################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../device/system_MK64F12.c 

OBJS += \
./device/system_MK64F12.o 

C_DEPS += \
./device/system_MK64F12.d 


# Each subdirectory must supply rules for building sources it contributes
device/%.o: ../device/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\PROJECTS\SEI\NVIC\NVIC_Priority\board" -I"C:\PROJECTS\SEI\NVIC\NVIC_Priority\source" -I"C:\PROJECTS\SEI\NVIC\NVIC_Priority" -I"C:\PROJECTS\SEI\NVIC\NVIC_Priority\drivers" -I"C:\PROJECTS\SEI\NVIC\NVIC_Priority\device" -I"C:\PROJECTS\SEI\NVIC\NVIC_Priority\CMSIS" -I"C:\PROJECTS\SEI\NVIC\NVIC_Priority\component\uart" -I"C:\PROJECTS\SEI\NVIC\NVIC_Priority\utilities" -I"C:\PROJECTS\SEI\NVIC\NVIC_Priority\component\serial_manager" -I"C:\PROJECTS\SEI\NVIC\NVIC_Priority\component\lists" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


