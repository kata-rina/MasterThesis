
PHY_OFFSET = 0x00100000;

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY( _start )

SECTIONS {

        . = PHY_OFFSET;
        .textc : {
              _start = .;
              clearreg.o(.cleartext);
              clearreg.o(.text);
              clearreg.o(.rodata);

        }
        . = PHY_OFFSET + 0x4000;
        .textd : {
              _start_dtb = .;
              d.tmp(.data);
              . = . + 0x1000;
        }

        . = PHY_OFFSET + 0x800000;
        .textr : {
              r.tmp(.data);
        }

        . = PHY_OFFSET + 0x8000;
        .textz : {

              _start_linux = .;
              z.tmp(.data);
        }
}
