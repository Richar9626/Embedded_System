################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/FlexTimer.c \
../source/GPIO.c \
../source/NVIC.c \
../source/main.c \
../source/semihost_hardfault.c 

OBJS += \
./source/FlexTimer.o \
./source/GPIO.o \
./source/NVIC.o \
./source/main.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/FlexTimer.d \
./source/GPIO.d \
./source/NVIC.d \
./source/main.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rjimenez/Documents/PROJECTS/Set_on_Match/board" -I"/Users/rjimenez/Documents/PROJECTS/Set_on_Match/source" -I"/Users/rjimenez/Documents/PROJECTS/Set_on_Match/drivers" -I"/Users/rjimenez/Documents/PROJECTS/Set_on_Match/utilities" -I"/Users/rjimenez/Documents/PROJECTS/Set_on_Match/component/serial_manager" -I"/Users/rjimenez/Documents/PROJECTS/Set_on_Match/component/uart" -I"/Users/rjimenez/Documents/PROJECTS/Set_on_Match/component/lists" -I"/Users/rjimenez/Documents/PROJECTS/Set_on_Match/CMSIS" -I"/Users/rjimenez/Documents/PROJECTS/Set_on_Match/device" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


