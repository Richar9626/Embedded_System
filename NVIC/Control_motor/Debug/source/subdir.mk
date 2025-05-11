################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/GPIO.c \
../source/NVIC.c \
../source/PIT.c \
../source/main.c \
../source/motor_control.c \
../source/semihost_hardfault.c 

OBJS += \
./source/GPIO.o \
./source/NVIC.o \
./source/PIT.o \
./source/main.o \
./source/motor_control.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/GPIO.d \
./source/NVIC.d \
./source/PIT.d \
./source/main.d \
./source/motor_control.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Control_motor/board" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Control_motor/source" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Control_motor/drivers" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Control_motor/utilities" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Control_motor/component/serial_manager" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Control_motor/component/uart" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Control_motor/component/lists" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Control_motor/CMSIS" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Control_motor/device" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


