/*
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2010
 * Michael Zaidman, Kodak, michael.zaidman@kodak.com
 * post_word_{load|store} cleanup.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef _POST_H
#define _POST_H

#ifndef	__ASSEMBLY__
#include <common.h>
#include <asm/io.h>

#ifndef IPROC_BOARD_DIAGS
/* swang disable these for now */
#if defined(CONFIG_POST) || defined(CONFIG_LOGBUFFER)

#ifndef CONFIG_POST_EXTERNAL_WORD_FUNCS
#ifdef CONFIG_SYS_POST_WORD_ADDR
#define _POST_WORD_ADDR	CONFIG_SYS_POST_WORD_ADDR
#else

#ifdef CONFIG_MPC5xxx
#define _POST_WORD_ADDR	(MPC5XXX_SRAM + MPC5XXX_SRAM_POST_SIZE)

#elif defined(CONFIG_MPC512X)
#define _POST_WORD_ADDR \
	(CONFIG_SYS_SRAM_BASE + CONFIG_SYS_GBL_DATA_OFFSET - 0x4)

#elif defined(CONFIG_8xx)
#define _POST_WORD_ADDR \
	(((immap_t *)CONFIG_SYS_IMMR)->im_cpm.cp_dpmem + CPM_POST_WORD_ADDR)

#elif defined(CONFIG_MPC8260)
#include <asm/cpm_8260.h>
#define _POST_WORD_ADDR	(CONFIG_SYS_IMMR + CPM_POST_WORD_ADDR)

#elif defined(CONFIG_MPC8360)
#include <linux/immap_qe.h>
#define _POST_WORD_ADDR	(CONFIG_SYS_IMMR + CPM_POST_WORD_ADDR)

#elif defined (CONFIG_MPC85xx)
#include <asm/immap_85xx.h>
#define _POST_WORD_ADDR	(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_PIC_OFFSET + \
				offsetof(ccsr_pic_t, tfrr))

#elif defined (CONFIG_MPC86xx)
#include <asm/immap_86xx.h>
#define _POST_WORD_ADDR	(CONFIG_SYS_IMMR + CONFIG_SYS_MPC86xx_PIC_OFFSET + \
				offsetof(ccsr_pic_t, tfrr))

#elif defined (CONFIG_4xx)
#define _POST_WORD_ADDR \
	(CONFIG_SYS_OCM_DATA_ADDR + CONFIG_SYS_GBL_DATA_OFFSET - 0x4)
#endif

#ifndef _POST_WORD_ADDR
#error "_POST_WORD_ADDR currently not implemented for this platform!"
#endif
#endif /* CONFIG_SYS_POST_WORD_ADDR */

static inline ulong post_word_load (void)
{
	return in_le32((volatile void *)(_POST_WORD_ADDR));
}

static inline void post_word_store (ulong value)
{
	out_le32((volatile void *)(_POST_WORD_ADDR), value);
}

#else

extern ulong post_word_load(void);
extern void post_word_store(ulong value);

#endif /* CONFIG_POST_EXTERNAL_WORD_FUNCS */
#endif /* defined (CONFIG_POST) || defined(CONFIG_LOGBUFFER) */
#endif /* __ASSEMBLY__ */

#endif /* IPROC_BOARD_DIAGS */

#ifdef CONFIG_POST

#define POST_POWERON	0x01	/* test runs on power-on booting */
#define POST_NORMAL		0x02	/* test runs on normal booting */
#define POST_SLOWTEST	0x04	/* test is slow, enabled by key press */
#define POST_POWERTEST	0x08	/* test runs after watchdog reset */

#define POST_COLDBOOT	0x80	/* first boot after power-on */

#define POST_ROM		0x0100	/* test runs in ROM */
#define POST_RAM		0x0200	/* test runs in RAM */
#define POST_MANUAL		0x0400	/* test runs on diag command */
#define POST_REBOOT		0x0800	/* test may cause rebooting */
#define POST_PREREL		0x1000  /* test runs before relocation */

#define POST_CRITICAL	0x2000	/* Use failbootcmd if test failed */
#define POST_STOP		0x4000	/* Interrupt POST sequence on fail */
#define POST_AUTO		0x8000
#define POST_SEMI_AUTO	0x10000

#define POST_MEM		(POST_RAM | POST_ROM)
#define POST_ALWAYS		(POST_NORMAL	| \
				 POST_SLOWTEST	| \
				 POST_MANUAL	| \
				 POST_AUTO		| POST_SEMI_AUTO |\
				 POST_POWERON	)

#define POST_FAIL_SAVE		0x80

#define POST_BEFORE		1
#define POST_AFTER		0
#define POST_PASSED		1
#define POST_FAILED		0
#if (defined(CONFIG_NS_PLUS))
#define NO_DIAGS_SUPPORT 0x0

#define BCM953022K       0x1
#define BCM953025K       0x2
#define BCM953025K_REV2  0x4

#define BCM958622HR      0x10
#define BCM958623HR      0x20
#define BCM958522ER      0x40
#define BCM958625HR      0x80

#define HR_ER_BOARDS (BCM958622HR | BCM958623HR | BCM958522ER  | BCM958625HR)
#define SVK_BOARDS   (BCM953022K  | BCM953025K  | BCM953025K_REV2)

#endif

#if (defined(CONFIG_CYGNUS))
#define NO_DIAGS_SUPPORT 0x0

#define BCM958300K       0x1  /*COMBO*/
#define BCM958302K       0x2  /*POS*/
#define BCM911360K       0x3  /*VOIP*/
#define BCM958305K       0x4  /*WA*/
#define BCM911360_ENTPHN 0x5  /*FFB*/

#endif

#ifndef	__ASSEMBLY__

struct post_test {
	char *name;
	char *cmd;
	char *desc;
	int flags;
	int (*test)(int flags);
	int (*init_f)(void);
	void (*reloc)(void);
	unsigned long testid;
};
int post_init_f(void);
void post_bootmode_init(void);
int post_bootmode_get(unsigned int *last_test);
void post_bootmode_clear(void);
void post_output_backlog(void);
int post_run(char *name, int flags);
int post_info(char *name);
int post_log(char *format, ...);
#ifdef CONFIG_NEEDS_MANUAL_RELOC
void post_reloc(void);
#endif
unsigned long post_time_ms (unsigned long base);

extern struct post_test post_list[];
extern unsigned int post_list_size;
extern int post_hotkeys_pressed(void);
extern int memory_post_test(int flags);

#ifdef IPROC_BOARD_DIAGS
extern int post_getUserResponse(const char *const prompt);
extern void post_getConfirmation(const char *const prompt);
extern int post_getUserInput(const char *const prompt);
extern void post_set_strappins(char *strap);
#if (defined(CONFIG_NS_PLUS) || defined(CONFIG_CYGNUS))
extern int post_check_board_cfg_env(void);
extern int post_get_board_diags_type(void);
#endif
#endif /* IPROC_BOARD_DIAGS */
/*
 *  If GCC is configured to use a version of GAS that supports
 * the .gnu_attribute directive, it will use that directive to
 * record certain properties of the output code.
 *  This feature is new to GCC 4.3.0.
 *  .gnu_attribute is new to GAS 2.18.
 */
#if (__GNUC__ >= 4 && __GNUC_MINOR__ >= 3)
/* Tag_GNU_Power_ABI_FP/soft-float */
#define GNU_FPOST_ATTR	asm(".gnu_attribute	4, 2");
#else
#define GNU_FPOST_ATTR
#endif /* __GNUC__ */
#endif /* __ASSEMBLY__ */
#ifdef IPROC_BOARD_DIAGS
#define CONFIG_SYS_POST_MEMORY            0x00000001
#define CONFIG_SYS_POST_PWM               0x00000002
#define CONFIG_SYS_POST_UART              0x00000004
#define CONFIG_SYS_POST_GPIO              0x00000008
#define CONFIG_SYS_POST_QSPI              0x00000010
#define CONFIG_SYS_POST_NAND              0x00000020
#define CONFIG_SYS_POST_I2S               0x00000040

#define CONFIG_SYS_POST_GSIO_SPI          0 /*0x00000080*/
#define CONFIG_SYS_POST_I2C               0x00000100
#define CONFIG_SYS_POST_USB20             0x00000200
#define CONFIG_SYS_POST_PCIE              0x00000400
#define CONFIG_SYS_POST_MMC               0x00000800
#define CONFIG_SYS_POST_USB30             0x00001000
#define CONFIG_SYS_POST_I2S_W8955         0 /*0x00002000*/
#define CONFIG_SYS_POST_SPDIF             0x00004000
#define CONFIG_SYS_POST_RGMII             0 /*0x00008000*/
#define CONFIG_SYS_POST_SATA              0 /*0x00010000*/
#define CONFIG_SYS_POST_VOIP              0x00020000
#define CONFIG_SYS_POST_EMMC              0x00040000
#define CONFIG_SYS_POST_SGMII             0x00080000
#define CONFIG_SYS_POST_PVTMON            0 /*0x00100000*/

#ifdef CYGNUS_BOARD_DIAGS
#define CONFIG_SYS_POST_SMAU              0x00000080
#define CONFIG_SYS_POST_AUDIO             0x00002000
#define CONFIG_SYS_POST_CAM               0x00008000
#define CONFIG_SYS_POST_CAN               0x00010000
#define CONFIG_SYS_POST_TDM               0x00100000

#define CONFIG_SYS_POST_LCD               0x00200000
#define CONFIG_SYS_POST_SCI               0x00400000
#define CONFIG_SYS_POST_MSR               0x00800000
#define CONFIG_SYS_POST_KEYPAD            0x01000000
#define CONFIG_SYS_POST_TPRNT             0x02000000
#define CONFIG_SYS_POST_DS1WM             0x04000000
#define CONFIG_SYS_POST_SPI               0x08000000

#define CONFIG_SYS_POST_TOUCHSCREEN	      0x10000000
#define CONFIG_SYS_POST_TAMPER	          0x20000000
#define CONFIG_SYS_POST_BBL               0x40000000
#define CONFIG_SYS_POST_SRAM              0x80000000
#define CONFIG_SYS_POST_AMACSWITCH        0x100000000

#else
#undef  CONFIG_SYS_POST_GSIO_SPI
#undef  CONFIG_SYS_POST_I2S_W8955
#undef  CONFIG_SYS_POST_SPDIF
#undef  CONFIG_SYS_POST_PVTMON
#undef  CONFIG_SYS_POST_SATA
#define CONFIG_SYS_POST_GSIO_SPI          0x00000080
#define CONFIG_SYS_POST_I2S_W8955         0x00002000
#define CONFIG_SYS_POST_SPDIF             0x00004000
#define CONFIG_SYS_POST_SATA              0x00010000
#define CONFIG_SYS_POST_PVTMON            0x00100000

#endif

#else
#define CONFIG_SYS_POST_RTC		    0x00000001
#define CONFIG_SYS_POST_WATCHDOG	0x00000002
#define CONFIG_SYS_POST_MEMORY		0x00000004
#define CONFIG_SYS_POST_CPU		    0x00000008
#define CONFIG_SYS_POST_I2C		    0x00000010
#define CONFIG_SYS_POST_CACHE		0x00000020
#define CONFIG_SYS_POST_UART		0x00000040
#define CONFIG_SYS_POST_ETHER		0x00000080
#define CONFIG_SYS_POST_SPI		    0x00000100
#define CONFIG_SYS_POST_USB		    0x00000200
#define CONFIG_SYS_POST_SPR		    0x00000400
#define CONFIG_SYS_POST_SYSMON		0x00000800
#define CONFIG_SYS_POST_DSP		    0x00001000
#define CONFIG_SYS_POST_OCM		    0x00002000
#define CONFIG_SYS_POST_FPU		    0x00004000
#define CONFIG_SYS_POST_ECC		    0x00008000
#define CONFIG_SYS_POST_BSPEC1		0x00010000
#define CONFIG_SYS_POST_BSPEC2		0x00020000
#define CONFIG_SYS_POST_BSPEC3		0x00040000
#define CONFIG_SYS_POST_BSPEC4		0x00080000
#define CONFIG_SYS_POST_BSPEC5		0x00100000
#define CONFIG_SYS_POST_CODEC		0x00200000
#define CONFIG_SYS_POST_COPROC		0x00400000
#define CONFIG_SYS_POST_FLASH		0x00800000
#define CONFIG_SYS_POST_MEM_REGIONS	0x01000000
#endif /*IPROC_BOARD_DIAGS*/

#ifdef CYGNUS_BOARD_DIAGS
#ifdef CPU_WRITE_8
#undef CPU_WRITE_8
#endif
#define CPU_WRITE_8(addr, data) writeb(data, addr)
#ifdef CPU_READ_8
#undef CPU_READ_8
#endif
#define CPU_READ_8(addr) readb(addr)

#define CPU_RMW_OR_8(addr, data) ({uint8_t x = readb(addr); x |= data; writeb(data, addr); })
#ifdef CPU_WRITE_32
#undef CPU_WRITE_32
#endif
#define CPU_WRITE_32(addr, data) writel(data, addr)
#ifdef CPU_READ_32
#undef CPU_READ_32
#endif
#define CPU_READ_32(addr) readl(addr)

#define CPU_RMW_OR_32(addr, data) ({uint32_t x = readl(addr); x |= data; writel(data, addr); })
#define CPU_RMW_AND_32(addr, data) ({uint32_t x = readl(addr); x &= data; writel(data, addr); })
#endif


#endif /* CONFIG_POST */

#endif /* _POST_H */
