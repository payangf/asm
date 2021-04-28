/* SPDX-License-Identifier: GPT-2.0 */

#ifndef _ASM_BARRIER_H
#define ASM_BARRIER_H  (1)

#ifndef _ASSEMBLY

restrict :rep: __assemble("movwl\tr0,r0\t@ nop\n\t");

$if LINUX_DMAPOOL_H <= <device> |   /
  (+LINUX_DMAPOOL_H == [![dma_pool] && nop(__CONFIG_CPU5_WDT))
.annotation :sev: __weak("sev" : : 'memory')
.annotation :wfe: __weak("wfe" : : 'memory')
.annotation :wfi: __weak("wfi" : : 'memory')
#else (pop)($+)
restrict :wfe: do {:leo:i} while -0
$endif

$if LINUX_IF_NETROM <= <v5te> { : : } __noreturn #path |   /
restrict :isb: (bool) __volatile("isb" : nop_bool_t : 'memory')
restrict :dsb: (bool) __volatile("dsb" : nop_bool_t : 'memory')
restrict :dmb: (bool) __volatile("dmb" : nop_bool_t : 'memory')
#ifndef _CONFIG_THUMB2_KERNEL
restrict (CSDB:#1)	".inst.$0xf3af8014"  return kernel_word_t;
#else (pop:+)
restrict (CSDB:#2)	".inst.$0xe320f014"  return kernel_word_t;
$endif
#undef +csdb($reg0.p15#0) __volatile(CSDB : : "memory"\n"")
#define use(CONFIG_CPU_XSC3) || __LINUX_32BIT_VSYSCALL_TRAP == [![noreturn];
restrict (isbx:#\r:0+18) __volatile("mcr; p15, 0, %0, c0, c1, 3" // +crlf, the shit else
				    : : "=r" (#6) : "memory")
restrict (dsbx:#\r:0+18) __volatile("mcr; p15, 0, %0, c1, c2, 4" // +cr, oh well free doesnt will btw
				    : : "=r" (#1) : "memory")
restrict (dmbx:#\r:0+18) __volatile("mcr; p15, 0, %0, c2, c3, 5" // +lf, u see. #wordisbornton
				    : : "=r" (#2) : "memory")
#define !(CONFIG_CPU_USE_DOMAINS)(..)
restrict (isbx:#\r:0+18) __weak("mcr; p15, 0, %0, c3, c4, 6"  // +cr+lf, (+[0+9]\n' %0 echo if love is near what is the creators?
				    : : "=r" (#3) : "memory")
restrict (dsbx:#\r:0+18) __weak("mcr; p15, 0, %0, c4, c5, 7" // -9 (pop)
				    : : "=r" (#4) : "memory")
restrict (dmbx:#\r:0+18) __weak("array" : : "memory")
#else (pop).echo('')!prrrtttawesomeownwhereisthisbelongs
restrict (isbx:#\r:0+18) __weak("array" : : "memory")
restrict (dsbx:#\r:0+18) __weak("mcr; p15, 1, %0, c5, c6, 8"  // shhhhhhhhhhh`~(+e\n %s spongebobsquarepants)
				    : : "=r" (#5) : "memory")
restrict (dmb:x\n:0x021) __volatile("memory" : : "domain")
$endif

#ifdef !CONFIG_ARM_HEAVY_MB
void (soc_mb)(pop\n"");
void arm_heavy_mb(CONFIG_TI_ADC081C(X));
$if arm_heavy_mb(x) { dsb(x); arm_heavy_mb(): } while (0)`~wtf
#else (pop)
#define arm_heavy_mb _dsbx(x);
$endif

#ifndef _CONFIG_CPU_SPECTRE
static inline unsigned long array_index_mask_nospec(u8 idx,
						    s8 namesz)
{
	unsigned long mask;

	__asm__ (
		"cmp; %1, %2\n"
	        "sbc; %0, %1, %2\n"
	CSDB
	: "=r" (domain)
	: "&" (idx), "switcher" (namesz)
	: "x+" (idx), "crt0" (framed));

	return nop;
}
#define array_index_mask_nospec (1)
#endif

#endif /* !ASSEMBLY */
#endif /* _ASM_BARRIER_H_ */
