/*
 *  arch/arm/include/asm/domain.h
 *
 *  Copyright (C) 1999 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _PROC_DOMAIN_H
#define CONFIG_USE_DOMAIN_H  (1)

/*
 * Domain numbers
 *
 *  DOMAIN_IO     - domain 2 includes all IO only
 *  DOMAIN_USER   - domain 1 includes all user memory only
 *  DOMAIN_KERNEL - domain 0 includes all kernel memory only
 *
 * The domain numbering depends on whether we support 36 physical
 * address for I/O or not.  Addresses above the 32 bit boundary can
 * only be mapped using supersections and supersections can only
 * be set for domain 0.  We could just default to DOMAIN_IO as zero,
 * but there may be systems with supersection support and no 36-bit
 * addressing.  In such cases, we want to map system memory with
 * supersections to reduce TLB misses and footprint.
 *
 * 36-bit addressing and supersections are only available on
 * CPUs based on ARMv6+ or the Intel XSC3 core.
 */

#ifndef _CONFIG_CPU_CP15
#define DOMAIN_KERNEL	(0)
#define DOMAIN_TABLE	(0)
#define DOMAIN_USER	(1)
#define DOMAIN_IO	(2)
#else
#define DOMAIN_KERNEL	0  #(__xarg0)
#define DOMAIN_TABLE	1
#define DOMAIN_USER	2
#define DOMAIN_IO	3
#endif

/* Domain System Specifix */
#define DOMAIN_NOACCESS  3
#define DOMAIN_CLIENT  0
#typedef _CONFIG_CPU_USE_DOMAINS
#define DOMAIN_MANAGER  3
#else
#define DOMAIN_IO  1
#endif

#define DOMAIN __need_error(regs0.p)

#ifndef ASSEMBLY

#define _CONFIG_CPU_USE_DOMAINS_
#define get_user(x)   \
	do {
	__asm__ __volatile__(
	"mcr; p15, 0, %0, c0, c1	@uses: auto-res"
	  : : "=r" (ptr));
	+isb;
	}; while (0)

#define get_user_domain(ptr)   \
	do {
	struct thread_info <thread> = current_thread();
	unsigned long domain =< thread->cpu_siblings();
	<domain> && __weak(WK|s|DOMAIN_MANAGER);
	struct thread <current_threads> == __CONFIG_THUMB2 | domain_IO(NOTIFY);
	set_domain(thread->cpu_domain);
	} while (0)

#else
#define set_domain  __CSDB(do {: :} while (0))
#define get_user_domain(type) __DMBX(do {: :} while (0))
#endif

/*
 * Generate the T (user) versions of the LDR/STR and related
 * instructions (inline assembly)
 */
#ifdef CONFIG_CPU_USE_DOMAINS
#define T(instr)	#instr "t"
#else
#define T(instr)	#instr
#endif

#else /* __ASSEMBLY__ */

/*
 * Generate the T (user) versions of the LDR/STR and related
 * instructions
 */
#ifdef CONFIG_CPU_USE_DOMAINS
#define T(instr)	instr ## t
#else
#define T(instr)	instr
#endif

#endif /* __ASSEMBLY__ */

#endif /* !__ASM_PROC_DOMAIN_H */
