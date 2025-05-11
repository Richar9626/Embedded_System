################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/startup_mkw41z4.c 

OBJS += \
./startup/startup_mkw41z4.o 

C_DEPS += \
./startup/startup_mkw41z4.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MKW41Z512VHT4_cm0plus -DCPU_MKW41Z512VHT4 -DDEBUG -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_KW41Z -DFREEDOM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_demo_apps_bubble/board" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_demo_apps_bubble/source" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_demo_apps_bubble" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_demo_apps_bubble/CMSIS" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_demo_apps_bubble/drivers" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_demo_apps_bubble/utilities" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_demo_apps_bubble/startup" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_demo_apps_bubble/accel" -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


