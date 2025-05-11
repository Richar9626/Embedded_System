################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/freertos_kernel/croutine.c \
../freertos/freertos_kernel/event_groups.c \
../freertos/freertos_kernel/list.c \
../freertos/freertos_kernel/queue.c \
../freertos/freertos_kernel/stream_buffer.c \
../freertos/freertos_kernel/tasks.c \
../freertos/freertos_kernel/timers.c 

OBJS += \
./freertos/freertos_kernel/croutine.o \
./freertos/freertos_kernel/event_groups.o \
./freertos/freertos_kernel/list.o \
./freertos/freertos_kernel/queue.o \
./freertos/freertos_kernel/stream_buffer.o \
./freertos/freertos_kernel/tasks.o \
./freertos/freertos_kernel/timers.o 

C_DEPS += \
./freertos/freertos_kernel/croutine.d \
./freertos/freertos_kernel/event_groups.d \
./freertos/freertos_kernel/list.d \
./freertos/freertos_kernel/queue.d \
./freertos/freertos_kernel/stream_buffer.d \
./freertos/freertos_kernel/tasks.d \
./freertos/freertos_kernel/timers.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/freertos_kernel/%.o: ../freertos/freertos_kernel/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K66F -DFREEDOM -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/board" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/source" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/mdio" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/phy" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/lwip/contrib/apps/tcpecho" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/lwip/port" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/lwip/src" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/lwip/src/include" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/drivers" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/utilities" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/device" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/component/uart" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/component/serial_manager" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/component/lists" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/CMSIS" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/freertos/freertos_kernel/include" -I"/Users/rjimenez/Documents/EMB2/frdmk66f_lwip_tcpecho_freertos/freertos/freertos_kernel/portable/GCC/ARM_CM4F" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


