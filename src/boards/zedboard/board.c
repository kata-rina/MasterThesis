/*
 * LTZVisor, a Lightweight TrustZone-assisted Hypervisor
 *
 * Copyright (c) TZVisor Project (www.tzvisor.org), 2017-
 *
 * Authors:
 *  Sandro Pinto <sandro@tzvisor.org>
 *  Jorge Pereira <jorgepereira89@gmail.com>
 *
 * This file is part of LTZVisor.
 *
 * LTZVisor is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, with a special
 * exception described below.
 *
 * Linking this code statically or dynamically with other modules
 * is making a combined work based on this code. Thus, the terms
 * and conditions of the GNU General Public License V2 cover the
 * whole combination.
 *
 * As a special exception, the copyright holders of LTZVisor give
 * you permission to link LTZVisor with independent modules to
 * produce a statically linked executable, regardless of the license
 * terms of these independent modules, and to copy and distribute
 * the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and
 * conditions of the license of that module. An independent module
 * is a module which is not derived from or based on LTZVisor.
 *
 * LTZVisor is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 *
 * [board.c]
 *
 * This file contains board-specific initializations.
 *
 * (#) $id: board.c 15-10-2015 s_pinto & j_pereira $
 * (#) $id: board.c 20-09-2017 s_pinto (modified)$
*/

#include <board.h>

#define DIV0		8
#define DIV1		5

/**
 * TrustZone-specific initializations
 *
 * @param
 *
 * @retval
 */
uint32_t board_init(void){
	// volatile uint32_t tmp;
	// uint32_t gem_val;
	// tmp = SLCR_GEM0_CLK_CTRL_ADDR;

	/** Unlocking SLCR register */
	write32( (void *)SLCR_UNLOCK, SLCR_UNLOCK_KEY);

	/** Handling memory security */
	write32( (void *)TZ_OCM_RAM0, 0xffffffff);
	write32( (void *)TZ_OCM_RAM1, 0xffffffff);
	write32( (void *)TZ_OCM, 0xffffffff);
	/* Handling DDR memory security (first 7 segments NS = 7*64MB = 448MB)
		(last memory segment  is configured as Secure)*/
	write32( (void *)TZ_DDR_RAM, 0x0000007f);
	printk("      * Memory security - OK  \n\t");

	/** Handling devices security */
	/* SDIO0 slave security (NS) */
	write32( (void *)SECURITY2_SDIO0, 0x1);
	/* SDIO1 slave security (NS) */
	write32( (void *)SECURITY3_SDIO1, 0x1);
	/* QSPI slave security (NS) */
	write32( (void *)SECURITY4_QSPI, 0x1);
	/* APB slave security (NS) */
	write32( (void *) SECURITY6_APBSL, 0x00007fff);
	/* DMA slave security (S) */
	write32( (void *)TZ_DMA_NS, 0x1);
	write32( (void *)TZ_DMA_IRQ_NS, 0x0000ffff);

	/* Ethernet security */
	write32( (void *)TZ_GEM, 0x3);
	write32( (void *)SLCR_BASE + 0x61c, 0);
	write32( (void *)SLCR_BASE + 0x600, 0xC);

	/* FPGA AFI AXI ports TrustZone */
	write32( (void *)SECURITY_APB, 0x3F);
	write32( (void *)TZ_FPGA_M, 0x3);
	// write32( (void *)SECURITY_FSSW_S0, 0x1);


	/* Handling more devices ... */
	/* SCU access control register, contains global timer */
	write32 ( (void *)SAC_REG, 0xf);
	/* Non-secure access control register, contains global timer */
	write32( (void *)NSAC_REG, 0xfff);

	write32( (void *)(0xF8F00040), 0);//FILTER START

	printk("      * Devices security - OK  \n\t");

	/** Locking SLCR register */
	write32( (void *)SLCR_LOCK, SLCR_LOCK_KEY);

	return TRUE;
}

/**
 * Handling syscalls (SMCs)
 *
 * @param
 *
 * @retval
 */
uint32_t board_handler(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
	switch(arg0) {
		case (LTZVISOR_READ_SYSCALL):{
			arg0 = read32((volatile void*)arg1);
			break;
		}
		case (LTZVISOR_WRITE_SYSCALL):{
			write32( (volatile void*)arg1, arg2);
			break;
		}
		case(LTZVISOR_CP15_WRITE_SYSCALL):{
			asm volatile("mrc p15, 0, r0, c15, c0, 0\n"
										"orr r0, r0, #1\n"
										"mcr p15, 0, r0, c15, c0, 0\n");
			break;
		}
		default:

			break;
	}

		return arg0;
}
