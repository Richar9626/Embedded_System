################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../nwk_ip/base/stack_globals/arp_globals.c \
../nwk_ip/base/stack_globals/coap_globals.c \
../nwk_ip/base/stack_globals/dhcp_globals.c \
../nwk_ip/base/stack_globals/dns_globals.c \
../nwk_ip/base/stack_globals/dtls_globals.c \
../nwk_ip/base/stack_globals/event_manager_globals.c \
../nwk_ip/base/stack_globals/icmp_globals.c \
../nwk_ip/base/stack_globals/ip_globals.c \
../nwk_ip/base/stack_globals/mdns_globals.c \
../nwk_ip/base/stack_globals/mle_globals.c \
../nwk_ip/base/stack_globals/mpl_globals.c \
../nwk_ip/base/stack_globals/nd_globals.c \
../nwk_ip/base/stack_globals/sixlowpan_globals.c \
../nwk_ip/base/stack_globals/sockets_globals.c \
../nwk_ip/base/stack_globals/tcp_globals.c \
../nwk_ip/base/stack_globals/thread_globals.c \
../nwk_ip/base/stack_globals/trickle_globals.c \
../nwk_ip/base/stack_globals/udp_globals.c 

OBJS += \
./nwk_ip/base/stack_globals/arp_globals.o \
./nwk_ip/base/stack_globals/coap_globals.o \
./nwk_ip/base/stack_globals/dhcp_globals.o \
./nwk_ip/base/stack_globals/dns_globals.o \
./nwk_ip/base/stack_globals/dtls_globals.o \
./nwk_ip/base/stack_globals/event_manager_globals.o \
./nwk_ip/base/stack_globals/icmp_globals.o \
./nwk_ip/base/stack_globals/ip_globals.o \
./nwk_ip/base/stack_globals/mdns_globals.o \
./nwk_ip/base/stack_globals/mle_globals.o \
./nwk_ip/base/stack_globals/mpl_globals.o \
./nwk_ip/base/stack_globals/nd_globals.o \
./nwk_ip/base/stack_globals/sixlowpan_globals.o \
./nwk_ip/base/stack_globals/sockets_globals.o \
./nwk_ip/base/stack_globals/tcp_globals.o \
./nwk_ip/base/stack_globals/thread_globals.o \
./nwk_ip/base/stack_globals/trickle_globals.o \
./nwk_ip/base/stack_globals/udp_globals.o 

C_DEPS += \
./nwk_ip/base/stack_globals/arp_globals.d \
./nwk_ip/base/stack_globals/coap_globals.d \
./nwk_ip/base/stack_globals/dhcp_globals.d \
./nwk_ip/base/stack_globals/dns_globals.d \
./nwk_ip/base/stack_globals/dtls_globals.d \
./nwk_ip/base/stack_globals/event_manager_globals.d \
./nwk_ip/base/stack_globals/icmp_globals.d \
./nwk_ip/base/stack_globals/ip_globals.d \
./nwk_ip/base/stack_globals/mdns_globals.d \
./nwk_ip/base/stack_globals/mle_globals.d \
./nwk_ip/base/stack_globals/mpl_globals.d \
./nwk_ip/base/stack_globals/nd_globals.d \
./nwk_ip/base/stack_globals/sixlowpan_globals.d \
./nwk_ip/base/stack_globals/sockets_globals.d \
./nwk_ip/base/stack_globals/tcp_globals.d \
./nwk_ip/base/stack_globals/thread_globals.d \
./nwk_ip/base/stack_globals/trickle_globals.d \
./nwk_ip/base/stack_globals/udp_globals.d 


# Each subdirectory must supply rules for building sources it contributes
nwk_ip/base/stack_globals/%.o: ../nwk_ip/base/stack_globals/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKW41Z512VHT4_cm0plus -DCPU_MKW41Z512VHT4 -D__SEMIHOST_HARDFAULT_DISABLE=1 -DFSL_RTOS_FREE_RTOS -DFRDM_KW41Z -DFREEDOM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/board" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/source" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/CMSIS" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/OSAbstraction/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/freertos" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/ieee_802.15.4/mac/source/App" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/ieee_802.15.4/mac/interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/ieee_802.15.4/phy/interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/GPIO" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/Keyboard/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/LED/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/SerialManager/Source/SPI_Adapter" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/SerialManager/Source/UART_Adapter" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/SerialManager/Source/I2C_Adapter" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/Flash/Internal" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/common" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/MemManager/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/Messaging/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/Panic/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/RNG/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/SerialManager/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/TimersManager/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/TimersManager/Source" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/SecLib" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/Lists" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/FunctionLib" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/ModuleInfo" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/MWSCoexistence/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/Shell/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/NVM/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/NVM/Source" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/FSCI/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/FSCI/Source" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/LowPower/Interface/MKW41Z" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/LowPower/Source/MKW41Z" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/nwk_ip/core/interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/nwk_ip/core/interface/modules" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/nwk_ip/core/interface/thread" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/nwk_ip/base/interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/nwk_ip/core/interface/http" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/nwk_ip/app/config" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/nwk_ip/app/common" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/drivers" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/DCDC/Interface" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/framework/XCVR/MKW41Z4" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/ieee_802.15.4/phy/source/MKW41Z" -I"/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/utilities" -Og -fno-common -g -Wall -Wno-missing-braces  -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -imacros "/Users/rjimenez/Documents/EMB2/frdmkw41z_wireless_examples_thread_router_eligible_device_freertos_leader/source/config.h" -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


