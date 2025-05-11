################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/pit.c \
../source/semihost_hardfault.c 

OBJS += \
./source/pit.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/pit.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFRDM_K64F -DFREEDOM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_pit/board" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_pit/source" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_pit/utilities" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_pit/drivers" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_pit/device" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_pit/component/uart" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_pit/component/lists" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_pit/CMSIS" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_pit/frdmk64f/driver_examples/pit" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


