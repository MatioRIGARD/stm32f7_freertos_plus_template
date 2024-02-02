##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.19.2] date: [Mon Oct 30 14:28:14 CET 2023]
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = stm32f7_freertos_plus_template


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build


######################################
# source
######################################
# C sources, core app
C_SOURCES = Core/Src/application/main.c \
Core/Src/application/app_freertos_tasks.c \
Core/Src/application/app_network.c \
Core/Src/application/app_stm32hal_error_handler.c \
Core/Src/application/app_stm32hal.c \
Core/Src/application/app_mqtt.c

# C sources, core freertos & stm32hal
C_SOURCES += Core/Src/freertos/freertos_callback.c \
Core/Src/freertos/printf-stdarg.c \
Core/Src/stm32hal/stm32f7xx_it.c \
Core/Src/stm32hal/stm32f7xx_hal_msp.c \
Core/Src/stm32hal/stm32f7xx_hal_timebase_tim.c \
Core/Src/stm32hal/system_stm32f7xx.c

# Drivers
C_SOURCES += \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_cortex.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_rcc.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_rcc_ex.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_flash.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_flash_ex.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_gpio.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_dma.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_dma_ex.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_pwr.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_pwr_ex.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_i2c.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_i2c_ex.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_exti.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_tim.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_tim_ex.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_uart.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_uart_ex.c \
Drivers/stm32f7xx_hal_driver/Src/stm32f7xx_hal_rng.c \
Drivers/CMSIS_RTOS_V2/cmsis_os2.c

# FreeRTOS
C_SOURCES += \
FreeRTOS/FreeRTOS/Source/croutine.c \
FreeRTOS/FreeRTOS/Source/event_groups.c \
FreeRTOS/FreeRTOS/Source/list.c \
FreeRTOS/FreeRTOS/Source/queue.c \
FreeRTOS/FreeRTOS/Source/stream_buffer.c \
FreeRTOS/FreeRTOS/Source/tasks.c \
FreeRTOS/FreeRTOS/Source/timers.c \
FreeRTOS/FreeRTOS/Source/portable/MemMang/heap_4.c \
FreeRTOS/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c

# FreeRTOS-Plus TCP
C_SOURCES += \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_ARP.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_BitConfig.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_DHCP.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_DHCPv6.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_DNS.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_DNS_Cache.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_DNS_Callback.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_DNS_Networking.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_DNS_Parser.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_ICMP.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_IP.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_IP_Timers.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_IP_Utils.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_IPv4.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_IPv4_Sockets.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_IPv4_Utils.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_IPv6.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_IPv6_Sockets.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_IPv6_Utils.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_ND.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_RA.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_Routing.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_Sockets.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_Stream_Buffer.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_IP.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_IP_IPv4.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_IP_IPv6.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_Reception.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_State_Handling.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_State_Handling_IPv4.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_State_Handling_IPv6.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_Transmission.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_Transmission_IPv4.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_Transmission_IPv6.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_Utils.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_Utils_IPv4.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_Utils_IPv6.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_WIN.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_Tiny_TCP.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_UDP_IP.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_UDP_IPv4.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/FreeRTOS_UDP_IPv6.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/portable/NetworkInterface/Common/phyHandling.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/portable/BufferManagement/BufferAllocation_2.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/portable/NetworkInterface/STM32Fxx/NetworkInterface.c \
FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/portable/NetworkInterface/STM32Fxx/stm32fxx_hal_eth.c

# MQTT Core
C_SOURCES += \
FreeRTOS/FreeRTOS-Plus/Source/Application-Protocols/coreMQTT/source/core_mqtt_serializer.c \
FreeRTOS/FreeRTOS-Plus/Source/Application-Protocols/coreMQTT/source/core_mqtt_state.c \
FreeRTOS/FreeRTOS-Plus/Source/Application-Protocols/coreMQTT/source/core_mqtt.c \
FreeRTOS/FreeRTOS-Plus/Source/Application-Protocols/network_transport/transport_plaintext.c \
FreeRTOS/FreeRTOS-Plus/Source/Utilities/backoff_algorithm/source/backoff_algorithm.c \
FreeRTOS/FreeRTOS-Plus/Source/Application-Protocols/network_transport/tcp_sockets_wrapper/ports/freertos_plus_tcp/tcp_sockets_wrapper.c \
FreeRTOS/FreeRTOS-Plus/Source/Application-Protocols/network_transport/transport_plaintext.c

# MBed tls
C_SOURCES += \
FreeRTOS/FreeRTOS-Plus/Source/Application-Protocols/network_transport/mbedtls_bio_tcp_sockets_wrapper.c \
FreeRTOS/FreeRTOS-Plus/Source/Application-Protocols/network_transport/transport_mbedtls.c

# ASM sources
ASM_SOURCES =  \
startup_stm32f779xx.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S


#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m7

# fpu
FPU = -mfpu=fpv5-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F779xx \
-DSTM32F7xx=1

# AS includes
AS_INCLUDES =  \
-ICore/Inc

# C includes
C_INCLUDES = \
-ICore/Inc/stm32hal \
-ICore/Inc/freertos \
-ICore/Inc/application \
-ICore/Inc/mbedTLS

# STM32
C_INCLUDES += \
-IDrivers/CMSIS/Device/ST/STM32F7xx/Include \
-IDrivers/CMSIS/Include \
-IDrivers/CMSIS_RTOS_V2 \
-IDrivers/stm32f7xx_hal_driver/Inc \
-IDrivers/stm32f7xx_hal_driver/Inc/Legacy \
-IDrivers/CMSIS/Core/Include

# freertos
C_INCLUDES += \
-IFreeRTOS/FreeRTOS/Source/include \
-IFreeRTOS/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1

# freertos plus TCP
C_INCLUDES += \
-IFreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/include \
-IFreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/portable/Compiler/GCC \
-IFreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/portable/NetworkInterface/STM32Fxx \
-IFreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/portable/NetworkInterface/include

# FreeRTOSPlus MQTT Core
C_INCLUDES += \
-IFreeRTOS/FreeRTOS-Plus/Source/Application-Protocols/coreMQTT/source/include \
-IFreeRTOS/FreeRTOS-Plus/Source/Application-Protocols/coreMQTT/source/interface \
-IFreeRTOS/FreeRTOS-Plus/Source/Utilities/logging \
-IFreeRTOS/FreeRTOS-Plus/Source/Application-Protocols/network_transport \
-IFreeRTOS/FreeRTOS-Plus/Source/Application-Protocols/network_transport/tcp_sockets_wrapper/include \
-IFreeRTOS/FreeRTOS-Plus/Source/Utilities/backoff_algorithm/source/include

# MBed tls
C_INCLUDES += \
-IFreeRTOS/FreeRTOS-Plus/ThirdParty/mbedtls/include
# -IFreeRTOS/FreeRTOS-Plus/ThirdParty/mbedtls/include \
-IFreeRTOS/FreeRTOS-Plus/Source/Application-Protocols/network_transport \
-IFreeRTOS/FreeRTOS-Plus/Source/Application-Protocols/network_transport/tcp_sockets_wrapper/include \
-IFreeRTOS/FreeRTOS-Plus/Source/Utilities/backoff_algorithm/source/include \
-IFreeRTOS/FreeRTOS-Plus/ThirdParty/mbedtls/library \
-IFreeRTOS/FreeRTOS-Plus/Source/corePKCS11/source/include \
-IFreeRTOS/FreeRTOS-Plus/ThirdParty/wolfSSL/wolfssl/wolfcrypt

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F779NIHx_FLASH.ld

# libraries
LIBS = -lc -lm -lrdimon -lmbedtls -lmbedx509 -lmbedcrypto
LIBDIR = -LFreeRTOS/FreeRTOS-Plus/ThirdParty/mbedtls/library
LDFLAGS = $(MCU) -specs=rdimon.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: mbedtls $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

mbedtls:
	$(MAKE) -C FreeRTOS/FreeRTOS-Plus/ThirdParty/mbedtls CC="$(CC)" CFLAGS="$(CFLAGS)" 

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)


#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)


# *** EOF ***