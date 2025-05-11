################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/DAC.c \
../source/GPIO.c \
../source/Keypad4x4.c \
../source/NVIC.c \
../source/PIT.c \
../source/RGB_set.c \
../source/Wave_gen.c \
../source/main.c \
../source/motor_control.c \
../source/password.c \
../source/semihost_hardfault.c 

OBJS += \
./source/DAC.o \
./source/GPIO.o \
./source/Keypad4x4.o \
./source/NVIC.o \
./source/PIT.o \
./source/RGB_set.o \
./source/Wave_gen.o \
./source/main.o \
./source/motor_control.o \
./source/password.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/DAC.d \
./source/GPIO.d \
./source/Keypad4x4.d \
./source/NVIC.d \
./source/PIT.d \
./source/RGB_set.d \
./source/Wave_gen.d \
./source/main.d \
./source/motor_control.d \
./source/password.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Practica1/board" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Practica1/source" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Practica1/drivers" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Practica1/utilities" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Practica1/component/serial_manager" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Practica1/component/uart" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Practica1/component/lists" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Practica1/CMSIS" -I"/Users/rjimenez/Documents/PROJECTS/NVIC/Practica1/device" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


