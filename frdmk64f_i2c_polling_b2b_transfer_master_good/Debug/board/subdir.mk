################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/pin_mux.c 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/pin_mux.o 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/pin_mux.d 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DFRDM_K64F -DFREEDOM -DCPU_MK64FN1M0VLL12_cm4 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Fernando\Documents\MCUXpressoIDE_11.3.0_5222\workspace\frdmk64f_i2c_polling_b2b_transfer_master\board" -I"C:\Users\Fernando\Documents\MCUXpressoIDE_11.3.0_5222\workspace\frdmk64f_i2c_polling_b2b_transfer_master\source" -I"C:\Users\Fernando\Documents\MCUXpressoIDE_11.3.0_5222\workspace\frdmk64f_i2c_polling_b2b_transfer_master\utilities" -I"C:\Users\Fernando\Documents\MCUXpressoIDE_11.3.0_5222\workspace\frdmk64f_i2c_polling_b2b_transfer_master\drivers" -I"C:\Users\Fernando\Documents\MCUXpressoIDE_11.3.0_5222\workspace\frdmk64f_i2c_polling_b2b_transfer_master\device" -I"C:\Users\Fernando\Documents\MCUXpressoIDE_11.3.0_5222\workspace\frdmk64f_i2c_polling_b2b_transfer_master\component\uart" -I"C:\Users\Fernando\Documents\MCUXpressoIDE_11.3.0_5222\workspace\frdmk64f_i2c_polling_b2b_transfer_master\component\lists" -I"C:\Users\Fernando\Documents\MCUXpressoIDE_11.3.0_5222\workspace\frdmk64f_i2c_polling_b2b_transfer_master\CMSIS" -I"C:\Users\Fernando\Documents\MCUXpressoIDE_11.3.0_5222\workspace\frdmk64f_i2c_polling_b2b_transfer_master\frdmk64f\driver_examples\i2c\polling_b2b_transfer\master" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


