/******************************************************************************
*
* (c) Copyright 2009-13 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/
/*****************************************************************************/
/**
* @file asm_vectors.s
*
* This file contains the initial vector table for the Cortex A9 processor
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who     Date     Changes
* ----- ------- -------- ---------------------------------------------------
* 1.00a ecm/sdm 10/20/09 Initial version
* 3.05a sdm	02/02/12 Save lr when profiling is enabled
* 3.10a srt     04/18/13 Implemented ARM Erratas. Please refer to file
*			 'xil_errata.h' for errata description
* </pre>
*
* @note
*
* None.
*
******************************************************************************/

@ #include "xil_errata.h"
  #include <ltzvisor_syscall_asm.h>


@ .org 0
.text
.arm


.global DataAbortInterrupt
.global PrefetchAbortInterrupt
.global vPortInstallFreeRTOSVectorTable
.global sFIQ_handler

.extern FreeRTOS_IRQ_Handler
.extern FreeRTOS_SWI_Handler
.extern vTaskSwitchContext

ulCriticalNestingC : .word ulCriticalNesting
ulPortTaskHasFPUContextC : .word ulPortTaskHasFPUContext
pxCurrentTCBConst: .word pxCurrentTCB




.macro macroSAVE_CONTEXT

    /* Save the LR and SPSR onto the system mode stack before switching to
    system mode to save the remaining system mode registers. */
    SRSDB	sp!, #0x1f
    CPS		#0x1f
    PUSH	{R0-R12, R14}

    /* Push the critical nesting count. */
    LDR		R2, ulCriticalNestingC
    LDR		R1, [R2]
    PUSH	{R1}

    /* Does the task have a floating point context that needs saving?  If
    ulPortTaskHasFPUContext is 0 then no. */
    LDR		R2, ulPortTaskHasFPUContextC
    LDR		R3, [R2]
    CMP		R3, #0

    /* Save the floating point context, if any. */
    FMRXNE  R1,  FPSCR
    VPUSHNE {D0-D15}
    VPUSHNE	{D16-D31}
    PUSHNE	{R1}

    /* Save ulPortTaskHasFPUContext itself. */
    PUSH	{R3}

    /* Save the stack pointer in the TCB. */
    LDR		R0, pxCurrentTCBConst
    LDR		R1, [R0]
    STR		SP, [R1]

    .endm





.align 8

.global _freertos_vector_table
_freertos_vector_table:
	B	    .
	B	    FreeRTOS_Undefined
	ldr   pc, _swi
	B	    FreeRTOS_PrefetchAbortHandler
	B	    FreeRTOS_DataAbortHandler
	NOP	  /* Placeholder for address exception vector*/
	LDR   PC, _irq
  @ LDR   PC, _fiq
	B	    FreeRTOS_FIQHandler

_irq:   .word FreeRTOS_IRQ_Handler
_swi:   .word FreeRTOS_SWI_Handler
_fiq:   .word sFIQ_handler

.align 4
FreeRTOS_FIQHandler:			                 /* FIQ vector handler */
	stmdb	   sp!,{r0-r3,r12,lr}	             /* state save from compiled code */
  blx        sFIQ_handler
  @ blx      sFIQ_handler                  /* FIQ vector */
  ldr      r1,=ulPortYieldRequired         /* check whether some task waits for execution*/
  cmp      r1, #0
  bne      switch_context                  /* switch task context*/

	ldmia	   sp!,{r0-r3,r12,lr}	 /* state restore from compiled code */
	subs	   pc, lr, #4			     /* adjust return */

switch_context:
  ldmia	   sp!,{r0-r3,r12,lr}	 /* state restore from compiled code */
  macroSAVE_CONTEXT

  blx      vTaskSwitchContext



.align 4
FreeRTOS_Undefined:				/* Undefined handler */
	b		.

.align 4
FreeRTOS_DataAbortHandler:		/* Data Abort handler */
  b . /* stuck here */

@ #ifdef CONFIG_ARM_ERRATA_775420
@ 	dsb
@ #endif
@ 	stmdb	sp!,{r0-r3,r12,lr}	/* state save from compiled code */
@ 	blx	DataAbortInterrupt		/*DataAbortInterrupt :call C function here */
@ 	ldmia	sp!,{r0-r3,r12,lr}	/* state restore from compiled code */
@ 	subs	pc, lr, #4			/* adjust return */

.align 4
FreeRTOS_PrefetchAbortHandler:	/* Prefetch Abort handler */
  b . /* stuck here*/

@ #ifdef CONFIG_ARM_ERRATA_775420
@ 	dsb
@ #endif
@ 	stmdb	sp!,{r0-r3,r12,lr}	/* state save from compiled code */
@ 	blx	PrefetchAbortInterrupt	/* PrefetchAbortInterrupt: call C function here */
@ 	ldmia	sp!,{r0-r3,r12,lr}	/* state restore from compiled code */
@ 	subs	pc, lr, #4			/* adjust return */

.align 4
.type vPortInstallFreeRTOSVectorTable, %function
vPortInstallFreeRTOSVectorTable:

	/* Set VBAR to the vector table that contains the FreeRTOS handlers. */
	ldr	r0, =_freertos_vector_table
	mcr	p15, 0, r0, c12, c0, 0
	dsb
	isb
	bx lr


.end
