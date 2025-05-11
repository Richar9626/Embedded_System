################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c \
../utilities/fsl_debug_console.c 

OBJS += \
./utilities/fsl_assert.o \
./utilities/fsl_debug_console.o 

C_DEPS += \
./utilities/fsl_assert.d \
./utilities/fsl_debug_console.d 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFRDM_K64F -DFREEDOM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_i2c_polling_b2b_transfer_slave/board" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_i2c_polling_b2b_transfer_slave/source" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_i2c_polling_b2b_transfer_slave/utilities" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_i2c_polling_b2b_transfer_slave/drivers" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_i2c_polling_b2b_transfer_slave/device" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_i2c_polling_b2b_transfer_slave/component/uart" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_i2c_polling_b2b_transfer_slave/component/lists" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_i2c_polling_b2b_transfer_slave/CMSIS" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_i2c_polling_b2b_transfer_slave/frdmk64f/driver_examples/i2c/polling_b2b_transfer/slave" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


