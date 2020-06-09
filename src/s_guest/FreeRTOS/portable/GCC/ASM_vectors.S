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
@ .extern vTaskSwitchContext

ulCriticalNestingC : .word ulCriticalNesting
ulPortTaskHasFPUContextC : .word ulPortTaskHasFPUContext
pxCurrentTCBConst: .word pxCurrentTCB
ulICCPMRConst: .word ulICCPMR
ulMaxAPIPriorityMaskConst: .word ulMaxAPIPriorityMask
vTaskSwitchContextConst : .word vTaskSwitchContext

.set FIQ_MODE, 0x11
.set SYS_MODE, 0x1f


//==============================================================================
.macro pSAVE_CONTEXT

    /* Save the LR and SPSR onto the system mode stack before switching to
    system mode to save the remaining system mode registers. */
    SRSDB	sp!, #0x1f
    PUSH {r8-r12,lr}
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



//==============================================================================

.macro pRESTORE_CONTEXT

	/* Set the SP to point to the stack of the task being restored. */
	LDR		R0, pxCurrentTCBConst
	LDR		R1, [R0]
	LDR		SP, [R1]

	/* Is there a floating point context to restore?  If the restored
	ulPortTaskHasFPUContext is zero then no. */
	LDR		R0, ulPortTaskHasFPUContextC
	POP		{R1}
	STR		R1, [R0]
	CMP		R1, #0

	/* Restore the floating point context, if any. */
	POPNE 	{R0}
	VPOPNE	{D16-D31}
	VPOPNE	{D0-D15}
	VMSRNE  FPSCR, R0

	/* Restore the critical section nesting depth. */
	LDR		R0, ulCriticalNestingC
	POP		{R1}
	STR		R1, [R0]

	/* Ensure the priority mask is correct for the critical nesting depth. */
	LDR		R2, ulICCPMRConst
	LDR		R2, [R2]
	CMP		R1, #0
	MOVEQ	R4, #255
	LDRNE	R4, ulMaxAPIPriorityMaskConst
	LDRNE	R4, [R4]
	STR		R4, [R2]

	/* Restore all system mode registers other than the SP (which is already
	being used). */
	POP		{R0-R12, R14}

	/* Return to the task code, loading CPSR on the way. */
  CPS     #FIQ_MODE
  POP     {r8-r12,lr}
  CPS     #SYS_MODE
	RFEIA	sp!

	.endm


//==============================================================================

.align 8

.global _freertos_vector_table
_freertos_vector_table:
	B	    .
	ldr   pc, _undef
	ldr   pc, _swi
	ldr   pc, _pabort
	ldr   pc, _dabort
	NOP	  /* Placeholder for address exception vector*/
	B     .
  LDR   PC, _fiq

_swi:   .word FreeRTOS_SWI_Handler
_fiq:   .word FreeRTOS_IRQ_Handler

.extern sUndef_handler
.extern sPrefetchAbort_handler
.extern sDataAbort_handler

_undef:   .word sUndef_handler
_pabort:  .word sPrefetchAbort_handler
_dabort:  .word sDataAbort_handler


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
