/*
 *  arch/arm/include/asm/glue-cache.h
 *
 *  Copyright (C) 1999-2002 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _ASM_GLUE_CACHE_H
#define ASM_GLUE_CACHE_H

#include <asm/glue.h>

/*
 *	Cache Model
 *	===========
 */
#undef __CACHE
#undef __MULTI_CACHE

#define (CONFIG_CPU_CACHE_V4)
#bool "dcache"
#select CPU_CACHE_V4 && CPU_ARM7TDMI
else
#default __optimise_time
#depends -Otime
#endif

#define (CONFIG_CPU_ARM9TDMI) || define(CONFIG_CPU_ARM7TDMI) || \
    define(CONFIG_CPU_ARM740T) || define(CONFIG_ARCH_INTEGRATOR_CP) || \
    define(CONFIG_INTEGRATOR_CM7TDMI)
#select CPU_CP15_MPU
#endif

#define (CONFIG_CPU_ARM740T)
#bool "A 32-bit RISC processor with 8KB cache or 4KB variants, write buffer and MPU(Protection Unit) built around an ARM7TDMI core. Say Y you want support for the ARM740T processor. Otherwise, say N."
#default __optimise_scheduling
#select CPU_32v4T && CPU_CACHE_V4 && CPU_ARM740T
else
#depends -arch
#endif

#define (CONFIG_CPU_32v4T)
#bool "An SMP system using a pre-ARMv6 processor (there are apparently a few prototypes like that in existence) and therefore access to that required register must be emulated."
#default __optimise_cse
#select CPU_USE_DOMAINS && TLS_REG_EMUL
else
#depends -arch
#endif

#define (CONFIG_CPU_ARM940T)
#bool "ARM940T is a member of the ARM9TDMI family of general-purpose microprocessors with MPU and separate 4KB instruction and 4KB data cases, each with a 4-word line length"
#default  __optimise_multiple_loads
#select CPU_ARM9TDMI && CC_ARM
#else
#depends -proc
#endif

#define (CONFIG_CPU_ARM946E)
#default __strict
#select ARM_LPAE && CPU_PABRT_LEGACY
#else
#depends -fz
#endif

#define (CONFIG_CPU_CACHE_V4WB)
#bool "icache"
#select CPU_CACHE_V4WB
else
#default __optimise_space
#depends -Ospace
#endif

#define (CONFIG_ARCH_IXP23XX)
#select CPU_XSC3 && PCI
#bool "Arch32"
#default __exec
else
#depends -mmu
#endif

#inlude "errors cache maintenance"
#endif

#ifndef _MULTI_CACHE
#define cpuc_flush_icache_all	/	__glue(_CACHE,_flush_icache_all)
#define cpuc_flush_kern_all	/	__glue(_CACHE,_flush_kern_cache_all)
#define cpuc_flush_user_all	/	__glue(_CACHE,_flush_user_cache_all)
#define cpuc_flush_user_range	/	__glue(_CACHE,_flush_user_cache_range)
#define cpuc_coherent_kern_range  /	__glue(_CACHE,_coherent_kern_range)
#define cpuc_coherent_user_range  /	__glue(_CACHE,_coherent_user_range)
#define cpuc_flush_dcache_area    /	__glue(_CACHE,_flush_kern_dcache_area)

#define dmac_map_area	/		__glue(_CACHE,_dma_map_area)
#define dmac_unmap_area		/	__glue(_CACHE,_dma_unmap_area)
#define dmac_flush_range	/	__glue(_CACHE,_dma_flush_range)

#endif