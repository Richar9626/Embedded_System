################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/ftm_dual_edge_capture.c \
../source/semihost_hardfault.c 

OBJS += \
./source/ftm_dual_edge_capture.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/ftm_dual_edge_capture.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DPRINTF_FLOAT_ENABLE=1 -DFRDM_K64F -DFREEDOM -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_ftm_dual_edge_capture/board" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_ftm_dual_edge_capture/source" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_ftm_dual_edge_capture/utilities" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_ftm_dual_edge_capture/drivers" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_ftm_dual_edge_capture/device" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_ftm_dual_edge_capture/component/uart" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_ftm_dual_edge_capture/component/lists" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_ftm_dual_edge_capture/CMSIS" -I"/Users/rjimenez/Documents/PROJECTS/frdmk64f_ftm_dual_edge_capture/frdmk64f/driver_examples/ftm/dual_edge_capture" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


