/*
 * LTZVisor, a Lightweight TrustZone-assisted Hypervisor
 *
 * Copyright (c) TZVisor Project (www.tzvisor.org), 2017-
 *
 * Authors:
 *  Sandro Pinto <sandro@tzvisor.org>
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
 * [s_isr.c]
 *
 * This file contains the interrupt handling.
 *
 * (#) $id: s_isr.c 27-09-2017 s_pinto$
*/

#include <gic.h>
#include <s_isr.h>
#include <printk.h>
#include <zynq_ttc.h>

extern void FreeRTOS_Tick_Handler( void );

tHandler* sfiq_handlers[NO_OF_INTERRUPTS_IMPLEMENTED] = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, ttc_interrupt_clear, FreeRTOS_Tick_Handler, ttc_interrupt_clear, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ttc_interrupt_clear,
	ttc_interrupt_clear, ttc_interrupt_clear, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL
};

/**
 * Generic FIQ handler
 *
 * @param	interrupt_ = interrupt number
 *
 * @retval
 */

void sFIQ_handler( void ){

	uint32_t interrupt_;

	/* Get interrupt number and acknowledge the interrupt */
	interrupt_ = interrupt_number_get();

	/* Call interrupt handler */
	if (sfiq_handlers[interrupt_]){
		sfiq_handlers[interrupt_]((void *) interrupt_);
	}

	/* Clear interrupt */
	interrupt_clear(interrupt_, 0);


}

/**
 * Connect interrupt request with interrupt handler
 *
 * @param interrupt_id = interrupt number
 *				handler = handler function
 *
 * @retval
 */
void install_interrupt( uint32_t interrupt_id, tHandler routine){

	if(interrupt_id < NO_OF_INTERRUPTS_IMPLEMENTED){
		sfiq_handlers[interrupt_id] = routine;
	}
}