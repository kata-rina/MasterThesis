NAME = ns_guest
CC = /mnt/Jupiter/FAKS/Diplomski/Diplomski_rad/linaro_toolchain/gcc-linaro-7.1.1-2017.08-x86_64_arm-eabi/bin/arm-eabi-
DTC = dtc

CFLAGS = -O0 -Wall -Werror -c -DBOARD_zedboard -marm -fno-unwind-tables -mcpu=cortex-a9\
 				-mfloat-abi=softfp -mfpu=vfpv3 -fno-common -mno-thumb-interwork -fno-short-enums\


LDFLAGS = -nostartfiles -nostdlib -T zynq_linux_boot.lds -z max-page-size=0x8000

COPY_FLAGS = -I binary -B arm -O elf32-littlearm

BIN = ./$(NAME).bin
ELF = ./$(NAME).elf

IMG_DIR = /mnt/Jupiter/FAKS/Diplomski/Diplomski_rad/linux-xlnx-xilinx-v2015.4/arch/arm/boot
DTS_DIR = /mnt/Jupiter/FAKS/Diplomski/Diplomski_rad/linux-xlnx-xilinx-v2015.4/arch/arm/boot/dts
DESTINATION = /mnt/Jupiter/FAKS/Diplomski/Diplomski_rad/ltzvisor/src/ns_guest/zynq/

all: $(ELF)
	$(CC)objcopy -O binary $< $(BIN)
	cp $(BIN) $(DESTINATION)

$(ELF):
	$(CC)objcopy $(COPY_FLAGS) $(IMG_DIR)/zImage z.tmp
	$(CC)objcopy $(COPY_FLAGS) ramdisk.image.gz r.tmp

	$(DTC) -I dts -O dtb -o zynq-zed.dtb $(DTS_DIR)/zynq-zed.dts

	cp $(DTS_DIR)/zynq-zed.dtb .
	$(CC)objcopy $(COPY_FLAGS) zynq-zed.dtb d.tmp

	$(CC)gcc $(CFLAGS) clearreg.c
	$(CC)ld  $(LDFLAGS) -o $(ELF) clearreg.o d.tmp r.tmp z.tmp


	rm -f z.tmp r.tmp d.tmp clearreg.o zynq-zed.dtb


PHONY: clean

clean:
	rm -f $(NAME).elf $(NAME).bin
	rm -f z.tmp r.tmp d.tmp clearreg.o zynq-zed.dtb
