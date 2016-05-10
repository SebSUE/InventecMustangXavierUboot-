/*
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 */

#ifndef __ARCH_CONFIGS_H
#define __ARCH_CONFIGS_H

#include <asm/arch/sysmap.h>

#if defined(CONFIG_NAND)
#include <asm/arch/configs-nand.h>
#endif

/* Architecture, CPU, chip, mach, etc */
#define CONFIG_KONA
#define CONFIG_CAPRI

/*
 * Memory configuration
 */
#define CONFIG_SYS_TEXT_BASE		0x8e000000

#define CONFIG_SYS_SDRAM_BASE		0x88000000
#define CONFIG_SYS_SDRAM_SIZE		0x08000000

/* U-boot environment data size and location */
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE			0x10000

/* GPIO Driver */
#define CONFIG_KONA_GPIO

/* MMC/SD Driver */
#define CONFIG_SDHCI
#define CONFIG_MMC_SDMA
#define CONFIG_KONA_SDHCI
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC

#define CONFIG_SYS_SDIO_BASE0 SDIO1_BASE_ADDR
#define CONFIG_SYS_SDIO_BASE1 SDIO2_BASE_ADDR
#define CONFIG_SYS_SDIO_BASE2 SDIO3_BASE_ADDR
#define CONFIG_SYS_SDIO_BASE3 SDIO4_BASE_ADDR
#define CONFIG_SYS_SDIO0_MAX_CLK 48000000
#define CONFIG_SYS_SDIO1_MAX_CLK 48000000
#define CONFIG_SYS_SDIO2_MAX_CLK 48000000
#define CONFIG_SYS_SDIO3_MAX_CLK 48000000
#define CONFIG_SYS_SDIO0_MIN_CLK 400000
#define CONFIG_SYS_SDIO1_MIN_CLK 400000
#define CONFIG_SYS_SDIO2_MIN_CLK 400000
#define CONFIG_SYS_SDIO3_MIN_CLK 400000
#define CONFIG_SYS_SDIO0 "sdio1"
#define CONFIG_SYS_SDIO1 "sdio2"
#define CONFIG_SYS_SDIO2 "sdio3"
#define CONFIG_SYS_SDIO3 "sdio4"

/* Serial Info */
/* Post pad 3 bytes after each reg addr */
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)
#define CONFIG_SYS_NS16550_CLK		13000000
#define CONFIG_SYS_NS16550_COM1		0x3e000000
#define CONFIG_CONS_INDEX		1

/* I2C Driver */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_KONA
#define CONFIG_SYS_SPD_BUS_NUM	3	/* Start with PMU bus */
#define CONFIG_SYS_MAX_I2C_BUS	4
#define CONFIG_SYS_I2C_BASE0	BSC1_BASE_ADDR
#define CONFIG_SYS_I2C_BASE1	BSC2_BASE_ADDR
#define CONFIG_SYS_I2C_BASE2	BSC3_BASE_ADDR
#define CONFIG_SYS_I2C_BASE3	PMU_BSC_BASE_ADDR

/* Timer Driver */
#define CONFIG_SYS_TIMER_RATE		32000
#define CONFIG_SYS_TIMER_COUNTER	(TIMER_BASE_ADDR + 4) /* STCLO offset */

/* Fastboot */
#define CONFIG_CMD_FASTBOOT
#define CONFIG_FASTBOOT
#define CONFIG_FASTBOOT_NO_PMU
#define CFG_FASTBOOT_TRANSFER_BUFFER		(CONFIG_SYS_SDRAM_BASE)
#define CFG_FASTBOOT_TRANSFER_BUFFER_SIZE	(CONFIG_SYS_SDRAM_SIZE - SZ_8M)
#define CONFIG_FASTBOOT_NO_OEM
#define CONFIG_USB_DEVICE

#define CONFIG_CMD_GPIO
#define CONFIG_CMD_I2C
#define CONFIG_CMD_MMC

#endif /* __ARCH_CONFIGS_H */
