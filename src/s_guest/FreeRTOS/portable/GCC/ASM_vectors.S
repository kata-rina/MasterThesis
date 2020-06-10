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

.extern FreeRTOS_IRQ_Handler
.extern FreeRTOS_SWI_Handler

ulCriticalNestingC : .word ulCriticalNesting
ulPortTaskHasFPUContextC : .word ulPortTaskHasFPUContext
pxCurrentTCBConst: .word pxCurrentTCB
ulICCPMRConst: .word ulICCPMR
ulMaxAPIPriorityMaskConst: .word ulMaxAPIPriorityMask
vTaskSwitchContextConst : .word vTaskSwitchContext

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
_dabort:  .word DataAbort

.type DataAbort, %function
DataAbort:
  /* Read Data Fault Status Register */
  mrc p15, 0, r0, c5, c0, 0
  /* Read program status register*/
  mrs r1, spsr
  /* Read SCTRL */
  mrc p15, 0, r2, c1, c0, 0

  b   sDataAbort_handler

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
