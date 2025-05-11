################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb/device/source/ehci/usb_device_ehci.c 

OBJS += \
./usb/device/source/ehci/usb_device_ehci.o 

C_DEPS += \
./usb/device/source/ehci/usb_device_ehci.d 


# Each subdirectory must supply rules for building sources it contributes
usb/device/source/ehci/%.o: ../usb/device/source/ehci/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -D_DEBUG=1 -DUSB_STACK_FREERTOS -DUSB_STACK_FREERTOS_HEAP_SIZE=32768 -DSDK_OS_FREE_RTOS -DFSL_OSA_BM_TASK_ENABLE=0 -DFSL_OSA_BM_TIMER_CONFIG=0 -DSERIAL_PORT_TYPE_UART=1 -DMCUXPRESSO_SDK -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=0 -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/board" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/source" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/usb/device/source/khci" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/usb/include" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/usb/device/source/ehci" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/usb/phy" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/freertos/freertos_kernel/include" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/freertos/freertos_kernel/portable/GCC/ARM_CM4F" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/drivers" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/device" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/utilities" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/component/uart" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/component/serial_manager" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/component/lists" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/usb/device/include" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/usb/device/source" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/component/osa" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/CMSIS" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/usb/device/class" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_dev_cdc_vcom_freertos_practica/usb/device/class/cdc" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


