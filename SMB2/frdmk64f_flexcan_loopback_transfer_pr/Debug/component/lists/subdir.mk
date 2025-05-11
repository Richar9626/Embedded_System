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
	arm-none-eabi-gcc -std=gnu99 -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFLEXCAN_WAIT_TIMEOUT=1000 -DFRDM_K64F -DFREEDOM -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -DSDK_OS_FREE_RTOS -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/utilities" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/drivers" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/component/uart" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/component/lists" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/freertos/freertos_kernel/include" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/freertos/freertos_kernel/portable/GCC/ARM_CM4F" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/CMSIS" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/freertos/template/ARM_CM4F" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/device" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/utilities" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/drivers" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/component/uart" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/component/lists" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/freertos/freertos_kernel/include" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/freertos/freertos_kernel/portable/GCC/ARM_CM4F" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/CMSIS" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/freertos/template/ARM_CM4F" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/device" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/board" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/source" -I"/Users/rjimenez/Documents/EMB2/frdmk64f_flexcan_loopback_transfer_pr/frdmk64f/driver_examples/flexcan/loopback_transfer" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


