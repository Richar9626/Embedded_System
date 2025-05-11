################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/Debug/utilities/subdir.mk
-include utilities/Debug/startup/subdir.mk
-include utilities/Debug/source/subdir.mk
-include utilities/Debug/drivers/subdir.mk
-include utilities/Debug/device/subdir.mk
-include utilities/Debug/component/uart/subdir.mk
-include utilities/Debug/component/serial_manager/subdir.mk
-include utilities/Debug/component/lists/subdir.mk
-include utilities/Debug/board/subdir.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include component/uart/subdir.mk
-include component/serial_manager/subdir.mk
-include component/lists/subdir.mk
-include board/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SPI_T8.axf

# Tool invocations
SPI_T8.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="SPI_T8.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T SPI_T8_Debug.ld -o "SPI_T8.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) SPI_T8.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "SPI_T8.axf"; # arm-none-eabi-objcopy -v -O binary "SPI_T8.axf" "SPI_T8.bin" ; # checksum -p MK64FN1M0xxx12 -d "SPI_T8.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
