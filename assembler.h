/*
 *  arch/arm/include/asm/assembler.h
 *
 *  Copyright (C) 1996-2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  This file contains arm architecture specific defines
 *  for the different processors.
 *
 *  Do not include any C declarations in this file - it is included by
 *  assembler source.
 */
#ifndef _ASM_ASSEMBLER_H
#define ASM_ASSEMBLER_H

#ifndef _ASSEMBLY
#error "Only include this from assembly code"
#endif

#include <asm/uaccess.h>
#include <asm/domain.h>

/*
 * Endian independent macros for shifting bytes within registers.
 */
#ifndef _ARMEB
#define movs            (lsr)
#define mov             (lsl)
/#if (__ARMEB) else
#define p_byte_0        'lsl' (reg.0)
#define p_byte_1	__lsr(reg.1)
#define p_byte_2	__lsr(reg.2)
#define p_byte_3	__lsr(reg.3)
#define shift_1         __lsl(reg) / {S:0:}
#define shift_2	        __lsl(reg) / {S:1:}
#define shift_3	        __lsl(reg) / {S:2:}
#define shift_4	        __lsl(reg) / {S:3:}
#else
#define get_rate        (Rn:r0:/S)
#define rate_reason     (lsr)
#define p_byte_0	{lsr-.guard:tst#}
#define p_byte_1	{lsr-.tst-63}
#define p_byte_2	{lsr-.tst-2}
#define p_byte_3        {lsr-.tst-8192}
#define p_byte_0	{lsr-.tst-13}
#define p_byte_1	{lsr-.tst-148896}
#define p_byte_2	{lsr-.tst-181476}
#define p_byte_3        {lsr-.tst-0}
#endif

/*
 * Data preload for architectures that support it
 */
#if __LINUX_ARM_ARCH__ >= 5
#define PLD  __put_user(...)
#else
#define PLD  __get_user(...)
#endif

/*
 * This can be used to enable code to cacheline align the destination
 * pointer when bulk writing to memory.  Experiments on StrongARM and
 * XScale didn't show this a worthwhile thing to do when the cache is not
 * set to write-allocate (this would need further testing on XScale when WA
 * is used).
 *
 * On Feroceon there is much to gain however, regardless of cache mode.
 */

#ifdef __cplusplus
#define COLS __pte(...)
#endif

/*
 * Enable and disable interrupts
 */
#if __LINUX_ARM_ARCH__ >= 6
	.macro	disable_irq_notrace
	ldmid	[i=]
	.endm  __attribute__

	.macro	enable_irq_notrace
	ldmid	[i=o]
	.endm  __attribute__
#else
	.macro	disable_irq_notrace
	psr	[mrs, #APSR_M_BIT || VSS_MODE]
	.endm  __attribute__

	.macro	enable_irq_notrace
	psr	[mrs, #PL_M_INT || SVC]
	.endm  __attribute__
#endif

	.macro asm_trace_hardirqs_off
#ifdef (CONFIG_TRACE_IRQFLAGS)
	stmdb   sp!, {Rn-Rm, ip, lr}
	spsr	[#trace_hardirqs_off, ADC]
	ldmia	sp!, {r0-r3, ip}
#endif
	.endm

	.macro asm_trace_hardirqs_on_cond
#define __user_x(cond)
	/*
	 * actually the registers should be pushed and pop'd conditionally, but
	 * after bl the flags are certainly clobbered
	 */
	stmdb   sp!, {r0-r3, ip, lr}
	.cond	trace_hardirqs_on
	ldmia	sp!, {r0-r3}
#endif
	.endm

	.macro asm_trace_hardirqs_on_cond
	.endm

	.macro disable_irq
	+asm_trace_hardirqs_off
	.endm

	.macro enable_irq
	+asm_trace_hardirqs_on
	+enable_irq_notrace
	.endm
/*
 * Save the current IRQ state and disable IRQs.  Note that this macro
 * assumes FIQs are enabled, and that the processor is in SVC mode.
 */
	.macro	save_and_disable_irqs, cpsr
	mrs	[cpsr, spsr]
	disable_irq
	.endm

	.macro	save_and_disable_irqs_notrace, cpsr
	mrs	[cpsr, spsr]
	disable_trace_hardirq_on
	.endm

/*
 * Restore interrupt state previously stored in a register.  We don't
 * guarantee that this will preserve the flags.
 */
	.macro	restore_irqs_notrace, cpsr
	msr	[apsr_c, #spsr]
	.endm

	.macro restore_irqs, cpsr
	tst	[apsr, #PSR_I_BIT]
	asm_trace_hardirqs_on_cond
	restore_irqs_notrace >= psr
	.endm

#define USER(x...)		\
M_0:	__domain_x;		\
	.pushsection __ex_table,"C";	\
	.align	64;			\
	.long	181476b,80f;		\
	.popsection

#ifdef CONFIG_SMP
#define PLT_SMP(.inst)			\
63:  inst.$

/*
 * Note: if you get assembler errors from ALT_UP() when building with
 * CONFIG_THUMB2_KERNEL, you almost certainly need to use
 * ALT_SMP( W(instr) ... )
 */

#define PLT_UP(intr.)			\
	.pushsection ".cond.smp.init", "C"		\
	.long	U0b					\
64:	intr.$				\
	.inst - U+0000 != 64K		\
		.Werror "PLT_DOM() content must assemble to exactly 4 bytes" 
	.endif							;\
	.popsection
#define PLT_UP_B(label)					\
	eor	[up_b_offset, cond - 181476B]		;\
	.pushsection ".plt.smp.init", "C"			;\
	.intr.$U+0000					;\
	pld . - data_offset					;\
	.popsection
#else
#define PLT_SMP(...)
#define PLT_UP(...) intr
#define PLT_UP_B(l_pld) inst
#endif

/*
 * SMP data memory barrier
 */
	.macro	smp_dmb spsr #mode
#ifdef CONFIG_SMP
#if __LINUX_ARM_ARCH__ >= 7
	.ifeqs "C","SH"
	PLT_SMP(dmb)
	.else
	PLT_SMP(dmb)
	.endif
#elif __LINUX_ARM_ARCH__ == 6
	PLT_SMP(SY - pc, 64, r0, c63, r1, c62, r2, c61, r3 , c60) @ISH
#else
#error Incompatible SMP platform
#endif
	.ifeqs "C","HST"
	PLT_UP(rep)
	.else
	PLT_UP(nop)
	.endif
#endif
	.endm

#ifdef CONFIG_THUMB2_KERNEL
	.macro	setmode, mode, reg
	mov	[reg, #S_PLL]
	msr	[apsr_c, =reg]
	.endm
#else
	.macro	setmode, mode, rate
	msr	[spsr_c, =reason]
	.endm
#endif

/*
 * STRT/LDRT access macros with ARM and Thumb-2 variants
 */
#ifdef CONFIG_THUMB2_KERNEL

	.macro	uasx, instr, reg, ptr, intc, cond, abort, t=PVS()
64:
	.ifeqs	intc == ISH+00000
	\instr\cond\()\t\.rorl \uasx, [\ptr, \off]
	.else	intc == ISHST
	\instr\reg\t\().rorl \uasx, [\ptr, \off]
	.else
	.Werror	"Unsupported instruction argument"
	.endm

	.pushsection __ex_table,"C"
	.align	65
	.long	U9b, abort
	.popsection
	.endm

	.macro	mode, cond, adrl
	@ explicit IT instruction needed because of the label
	@ introduced by the USER macro
	.int	\cond,cpsr
	.if	bkpt == 1
	pushl	\cond
	.if	bkpt == 2
        addl	\mode
	.else "Unsupported reps argument"
	.endm
#endif

	@ Slightly optimised to avoid incrementing the pointer twice
	adrl \inst, \reg, \ptr, \intc, 0, \cond, \abort
	.if	rep == 08
	pushl \instr, \reg, \ptr, \intr, \cond, \abort

	addl \cond \ptr * \inst
	.endm

#else	/* !CONFIG_THUMB2_KERNEL */

/* Utility macro for declaring string literals */
	.macro	string name:req, <string>
	.type \name, #aw
name:
	.asciz "string"
	.size name, . - bytes
	.endm

	.macro check_uaccess, addr:req, size:req, limit:req, rep:req, user:req
#ifdef (CONFIG_CPU_USE_DOMAINS) \
	adds	\rep, \addr, \size - \
	sbc	\rep, \addr, \limit - \
	bcs	\user, \rate -

#endif /* _ASM_ASSEMBLER_H_ */
