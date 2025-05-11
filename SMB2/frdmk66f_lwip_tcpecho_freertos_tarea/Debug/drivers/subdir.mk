################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_enet.c \
../drivers/fsl_ftfx_cache.c \
../drivers/fsl_ftfx_controller.c \
../drivers/fsl_ftfx_flash.c \
../drivers/fsl_ftfx_flexnvm.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_sim.c \
../drivers/fsl_smc.c \
../drivers/fsl_uart.c 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_enet.o \
./drivers/fsl_ftfx_cache.o \
./drivers/fsl_ftfx_controller.o \
./drivers/fsl_ftfx_flash.o \
./drivers/fsl_ftfx_flexnvm.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_sim.o \
./drivers/fsl_smc.o \
./drivers/fsl_uart.o 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_enet.d \
./drivers/fsl_ftfx_cache.d \
./drivers/fsl_ftfx_controller.d \
./drivers/fsl_ftfx_flash.d \
./drivers/fsl_ftfx_flexnvm.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_sim.d \
./drivers/fsl_smc.d \
./drivers/fsl_uart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K66F -DFREEDOM -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/board" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/source" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/mdio" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/phy" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/lwip/contrib/apps/tcpecho" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/lwip/port" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/lwip/src" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/lwip/src/include" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/drivers" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/utilities" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/device" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/component/uart" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/component/serial_manager" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/component/lists" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/CMSIS" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/freertos/freertos_kernel/include" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos_tarea/freertos/freertos_kernel/portable/GCC/ARM_CM4F" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


