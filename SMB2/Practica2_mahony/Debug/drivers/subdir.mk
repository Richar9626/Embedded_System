################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_gpio.c \
../drivers/fsl_i2c.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_smc.c \
../drivers/fsl_sysmpu.c \
../drivers/fsl_uart.c 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_gpio.o \
./drivers/fsl_i2c.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_smc.o \
./drivers/fsl_sysmpu.o \
./drivers/fsl_uart.o 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_gpio.d \
./drivers/fsl_i2c.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_smc.d \
./drivers/fsl_sysmpu.d \
./drivers/fsl_uart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -D_DEBUG=1 -DUSB_STACK_FREERTOS -DUSB_STACK_FREERTOS_HEAP_SIZE=32768 -DSDK_OS_FREE_RTOS -DFSL_OSA_BM_TASK_ENABLE=0 -DFSL_OSA_BM_TIMER_CONFIG=0 -DSERIAL_PORT_TYPE_UART=1 -DMCUXPRESSO_SDK -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=0 -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/board" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/source" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/usb/device/source/khci" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/usb/include" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/usb/device/source/ehci" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/usb/phy" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/freertos/freertos_kernel/include" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/freertos/freertos_kernel/portable/GCC/ARM_CM4F" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/drivers" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/device" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/utilities" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/component/uart" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/component/serial_manager" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/component/lists" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/usb/device/include" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/usb/device/source" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/component/osa" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/CMSIS" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/usb/device/class" -I"/Users/rjimenez/Documents/EMB2/Practica2_mahony/usb/device/class/cdc" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


