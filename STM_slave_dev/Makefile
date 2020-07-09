SHELL := /bin/bash
PROJECT_NAME=stm

BUILDDIR = build

SOURCEDIR = src

DEVICE = inc/STM32F4xx/
CORE = /mnt/Jupiter/STM32F4_Lib/STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/CMSIS/Include/
PERIPH = /mnt/Jupiter/STM32F4_Lib/STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/STM32F4xx_StdPeriph_Driver/
DISCOVERY = inc/

SOURCES += startup_stm32f4xx.s
SOURCES += system_stm32f4xx.c

SOURCES += $(wildcard $(SOURCEDIR)/*.c)

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

INCLUDES += -I$(CORE) \
			-I$(DEVICE) \
			-I$(PERIPH)/inc/ \
			-I$(DISCOVERY) \
			-I.

ELF = $(BUILDDIR)/$(PROJECT_NAME).elf
HEX = $(BUILDDIR)/$(PROJECT_NAME).hex
BIN = $(BUILDDIR)/$(PROJECT_NAME).bin

CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
GDB = arm-none-eabi-gdb

CFLAGS  = -O0 -g -Wall -Wl,--gc-sections -I.\
   -mcpu=cortex-m4 -mthumb -mfloat-abi=hard\
	 -mfpu=fpv4-sp-d16\
	 $(INCLUDES) -DUSE_STDPERIPH_DRIVER

LDSCRIPT = stm32_flash.ld

LDFLAGS += -T$(LDSCRIPT) -mthumb -mcpu=cortex-m4 -mfloat-abi=hard

all: $(ELF)
	$(OBJCOPY) -O binary $< $(BIN)
	$(OBJCOPY) -O ihex $< $(HEX)

$(ELF): $(OBJECTS)
	$(LD) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@ $(LDLIBS)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.s
	@mkdir -p $(dir $@)
	@$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean debug flash
clean:
	rm -rf build

debug: $(ELF)
	$(GDB) -tui $(ELF)

flash: $(BIN)
	 sudo st-flash write $(BIN) 0x8000000
