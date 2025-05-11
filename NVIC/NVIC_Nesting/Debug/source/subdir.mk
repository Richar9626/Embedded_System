################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/GPIO.c \
../source/NVIC.c \
../source/main.c \
../source/semihost_hardfault.c 

OBJS += \
./source/GPIO.o \
./source/NVIC.o \
./source/main.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/GPIO.d \
./source/NVIC.d \
./source/main.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\PROJECTS\SEI\NVIC\NVIC_Nesting\board" -I"C:\PROJECTS\SEI\NVIC\NVIC_Nesting\source" -I"C:\PROJECTS\SEI\NVIC\NVIC_Nesting" -I"C:\PROJECTS\SEI\NVIC\NVIC_Nesting\drivers" -I"C:\PROJECTS\SEI\NVIC\NVIC_Nesting\device" -I"C:\PROJECTS\SEI\NVIC\NVIC_Nesting\CMSIS" -I"C:\PROJECTS\SEI\NVIC\NVIC_Nesting\component\uart" -I"C:\PROJECTS\SEI\NVIC\NVIC_Nesting\utilities" -I"C:\PROJECTS\SEI\NVIC\NVIC_Nesting\component\serial_manager" -I"C:\PROJECTS\SEI\NVIC\NVIC_Nesting\component\lists" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


