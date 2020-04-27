/* Copyright (c) 2013 Quanta Research Cambridge, Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <stdint-gcc.h>
#define XILINX_EP107		3378
/* This code initializes the parts of the ARM processor that were previously
 * initialized by u-boot (git://git.xilinx.com/u-boot-xlnx.git)
 * in arch/arm/cpu/armv7/zynq/cpu.c:lowlevel_init().
 *
 * FSBL has already initialized UART and stack
 */
#define SLCR_LOCK_MAGIC           0x767B
#define SLCR_UNLOCK_MAGIC         0xDF0D

#define XPSS_SYS_CTRL_BASEADDR    0xF8000000
#define XPSS_DEV_CFG_APB_BASEADDR 0xF8007000
#define XPSS_SCU_BASEADDR         0xF8F00000

#define STDOUT_BASEADDRESS        0xE0001000
#define XUARTPS_SR_OFFSET         0x2C       /**< Channel Status [14:0] */
#define XUARTPS_SR_TXFULL         0x00000010 /**< TX FIFO full */
#define XUARTPS_FIFO_OFFSET       0x30       /**< FIFO [7:0] */

// SLCR
#define XSLCR_MIO_PIN_00_OFFSET    0x700 /* MIO PIN0 control register */
#define XSLCR_MIO_L0_SHIFT             1
#define XSLCR_MIO_L1_SHIFT             2
#define XSLCR_MIO_L2_SHIFT             3
#define XSLCR_MIO_L3_SHIFT             5
#define XSLCR_MIO_LMASK             0xFE
#define XSLCR_MIO_PIN_XX_TRI_ENABLE    1
#define XSLCR_MIO_PIN_GPIO_ENABLE   (0x00 << XSLCR_MIO_L3_SHIFT)
#define XSLCR_MIO_PIN_SDIO_ENABLE   (0x04 << XSLCR_MIO_L3_SHIFT)

#define PINOFF(PIN) (XPSS_SYS_CTRL_BASEADDR + XSLCR_MIO_PIN_00_OFFSET + (PIN) * 4)

// this syscall ids are defined in ltzvisor hypervisor
#define WRITE_SYSCALL           (-31)
#define READ_SYSCALL            (-30)
// #define EMIO_SDIO1


static void Xil_Out32(uint32_t OutAddress, uint32_t Value);
static uint32_t Xil_In32(uint32_t Addr);
static void debug_puts(const char *ptr);

static void sys_write(int32_t arg0, uint32_t arg1, uint32_t arg2);
static uint32_t sys_read(int32_t arg0, uint32_t arg1);

void clearreg(void)__attribute__((section(".cleartext")));
// void jump_to_kernel(uint32_t arg0, uint32_t machine_nmr, uint32_t address);

static const struct {
    uint32_t pinaddr;
    uint32_t enable;
} sdio1_pindef[] = {
#ifdef BOARD_zedboard
    {PINOFF(10), XSLCR_MIO_PIN_SDIO_ENABLE},
    {PINOFF(11), XSLCR_MIO_PIN_SDIO_ENABLE},
    {PINOFF(12), XSLCR_MIO_PIN_SDIO_ENABLE},
    {PINOFF(13), XSLCR_MIO_PIN_SDIO_ENABLE},
    {PINOFF(14), XSLCR_MIO_PIN_SDIO_ENABLE},
    {PINOFF(15), XSLCR_MIO_PIN_SDIO_ENABLE},
#endif
    {0,0}};


extern char _start_linux;
extern uint32_t _start_dtb;

void clearreg(void)
{
    uint32_t pinaddr, ind = 0;
    uint32_t tmp;

    void (*kernel)(int zero, int arch, uint32_t dtb_address);

    debug_puts("Starting clearreg\n");

    sys_write(WRITE_SYSCALL, XPSS_SYS_CTRL_BASEADDR + 0x8, SLCR_UNLOCK_MAGIC); //slcr_unlock

    /* remap DDR to zero, FILTERSTART */
    sys_write(WRITE_SYSCALL, XPSS_SCU_BASEADDR + 0x40, 0); // filter start
    // Xil_Out32(XPSS_SCU_BASEADDR + 0x40, 0);                     //filter_start
    debug_puts("DDR REMAPED\n");

    /* Device config APB, unlock the PCAP */
    sys_write(WRITE_SYSCALL, XPSS_DEV_CFG_APB_BASEADDR + 0x34, 0x757BDF0D);  //unlock
    sys_write(WRITE_SYSCALL, XPSS_DEV_CFG_APB_BASEADDR + 0x28, 0xFFFFFFFF); //rom_shadow


    /* OCM_CFG, Mask out the ROM, map ram into upper addresses */
    sys_write(WRITE_SYSCALL, XPSS_SYS_CTRL_BASEADDR + 0x910, 0x1F); //ocm_cfg


    /* FPGA_RST_CTRL, clear resets on AXI fabric ports */
    sys_write(WRITE_SYSCALL, XPSS_SYS_CTRL_BASEADDR + 0x240, 0); //fpga_rst_ctrl


    /* TZ_DDR_RAM, Set DDR trust zone non-secure */
    sys_write(WRITE_SYSCALL, XPSS_SYS_CTRL_BASEADDR + 0x430, 0x0000007f); //trust_zone

    /* Set urgent bits with register */
    sys_write(WRITE_SYSCALL, XPSS_SYS_CTRL_BASEADDR + 0x61c, 0); //ddr_urgent_sel

    /* Urgent write, ports S2/S3 */
    sys_write(WRITE_SYSCALL, XPSS_SYS_CTRL_BASEADDR + 0x600, 0xC); //ddr_urgent

    while ((pinaddr = sdio1_pindef[ind].pinaddr)) {
#ifndef EMIO_SDIO1
        /* release pin set tri-state */
        // Xil_Out32(pinaddr, (Xil_In32(pinaddr) & ~XSLCR_MIO_LMASK) | XSLCR_MIO_PIN_XX_TRI_ENABLE);
        /* assign pin to this peripheral */
        // Xil_Out32(pinaddr, sdio1_pindef[ind].enable);

        tmp = sys_read(READ_SYSCALL,pinaddr);
        /* release pin set tri-state */
        sys_write(WRITE_SYSCALL, pinaddr, (tmp & ~XSLCR_MIO_LMASK) | XSLCR_MIO_PIN_XX_TRI_ENABLE);
        /* assign pin to this peripheral */
        sys_write(WRITE_SYSCALL, pinaddr, sdio1_pindef[ind].enable);

#endif
        ind++;
    }

    sys_write(WRITE_SYSCALL, XPSS_SYS_CTRL_BASEADDR + 0x4, SLCR_LOCK_MAGIC); //slcr lock
    debug_puts("Jump to linux\n\r");

    // jumping to linux kernel
    // jump_to_kernel(0, XILINX_EP107, 0x00104000);

    kernel = (void (*) (int, int, uint32_t))0x8000;
    kernel(0,XILINX_EP107, 0x4000);

    //this point shoud not be reached
    debug_puts("ERROR2:Booting linux:something went wrong!\n");


}


static void Xil_Out32(uint32_t OutAddress, uint32_t Value)
{
    *(volatile uint32_t *) OutAddress = Value;
}


static uint32_t Xil_In32(uint32_t Addr)
{
    return *(volatile uint32_t *) Addr;
}


static void debug_puts(const char *ptr)
{
    while (*ptr) {
        while((Xil_In32(STDOUT_BASEADDRESS + XUARTPS_SR_OFFSET) & XUARTPS_SR_TXFULL));
        Xil_Out32(STDOUT_BASEADDRESS + XUARTPS_FIFO_OFFSET, *ptr++);
    }
}

/*arg0 syscall id, arg1 address to write, arg2 value to write */
static void sys_write(int32_t arg0, uint32_t arg1, uint32_t arg2){

    asm volatile("smc #0");

}

/* arg0 syscall id, arg1 address to read from, read value is returned via arg0 */
static uint32_t sys_read(int32_t arg0, uint32_t arg1){

  asm volatile("smc #0");

  return arg0;
}

/* jumping to kernel - according to https://www.kernel.org/doc/Documentation/arm/Booting */
void jump_to_kernel(uint32_t arg0, uint32_t machine_nmr, uint32_t address){

}
