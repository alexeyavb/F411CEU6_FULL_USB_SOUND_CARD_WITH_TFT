
######################################
# target
######################################
TARGET = usb_audio_i2s

# To build for STM32F411CEU6 versus STM32F401CCU6
# set ASM_SOURCES, CPU_DEF and LDSCRIPT below 

# STM32F411CEU6 Black Pill (512kB flash, 256kB RAM, 100MHz)
CPU_DEF = STM32F411xE
ASM_SOURCES =  startup_stm32f411ceux.s
LDSCRIPT = STM32F411CEUX_FLASH.ld

# STM32F401CCU6 Black Pill (256kB flash, 64kB RAM, 84MHz)
# CPU_DEF = STM32F401xC 
# ASM_SOURCES = startup_stm32f401ccux.s
# LDSCRIPT = STM32F401CCUX_FLASH.ld


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
# C sources
C_SOURCES =  \
src/main.c \
src/usart.c \
src/tim.c \
src/spi.c \
src/usbd_conf.c \
src/usbd_desc.c \
src/usbd_audio_if.c \
src/stm32f4xx_it.c \
src/syscalls.c \
src/system_stm32f4xx.c \
src/fft_examples.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s_ex.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c \
drivers/BSP/bsp_misc.c \
drivers/BSP/bsp_audio.c \
drivers/ST7735/st7735.c \
drivers/ST7735/st7735_misc.c \
drivers/ST7735/fonts.c \
stm32_usb/Core/Src/usbd_core.c \
stm32_usb/Core/Src/usbd_ctlreq.c \
stm32_usb/Core/Src/usbd_ioreq.c \
stm32_usb/Class/AUDIO/Src/usbd_audio.c




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
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_MATH_FF \
-DARM_MATH_CM4 \
-DUSE_HAL_DRIVER \
-DUSE_FULL_ASSERT \
-D$(CPU_DEF) \
-DDEBUG_FEEDBACK_ENDPOINT 
#-DUSE_MCLK_OUT 



# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-Iinc \
-Idrivers/STM32F4xx_HAL_Driver/Inc \
-Idrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
-Istm32_usb/Core/Inc \
-Istm32_usb/Class/AUDIO/Inc \
-Idrivers/CMSIS/Device/ST/STM32F4xx/Include \
-Idrivers/CMSIS/Include \
-Idrivers/BSP \
-Idrivers/ST7735 \
-Idrivers/CMSIS/DSP/Include -Idrivers/CMSIS/DSP/PrivateInvlude -Idrivers/CMSIS/ComputeLibrary/Include

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -fstack-usage

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################

# libraries
LIBS = -lc -lm -lnosys
LIBS += -larm_cortexm4lf_math
LIBDIR += -Ldrivers/CMSIS/DSP/Lib/gcc

LDFLAGS = $(MCU) -specs=nano.specs  -u _printf_float -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


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
	@echo "---------------------   SIZE   ----------------------"
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@ -Wl,--print-memory-usage
	$(SZ) $@
	@echo ""
	@echo "-----------------------------------------------------"

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# Flash binary
#######################################
flash:
	-st-flash --reset write $(BUILD_DIR)/$(TARGET).bin 0x08000000

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
