/*
 * Freescale i.MX28 GPIO control code
 *
 * Copyright (C) 2011 Marek Vasut <marek.vasut@gmail.com>
 * on behalf of DENX Software Engineering GmbH
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include "types.h"
#include "error.h"
//#include "io.h"
#include "debug.h"
//#include <common.h>
//#include <netdev.h>
//#include <asm/errno.h>

#include "arch-mx28/iomux.h"
#include "arch-mx28/imx-regs.h"
#define writel(b,addr) ((*(volatile uint32_t *) (addr)) = (b))
#define readl(addr) (*(volatile uint32_t *) (addr))

#define CONFIG_MX28

#if	defined(CONFIG_MX23)
#define	PINCTRL_BANKS		3
#define	PINCTRL_DOUT(n)		(0x0500 + ((n) * 0x10))
#define	PINCTRL_DIN(n)		(0x0600 + ((n) * 0x10))
#define	PINCTRL_DOE(n)		(0x0700 + ((n) * 0x10))
#define	PINCTRL_PIN2IRQ(n)	(0x0800 + ((n) * 0x10))
#define	PINCTRL_IRQEN(n)	(0x0900 + ((n) * 0x10))
#define	PINCTRL_IRQSTAT(n)	(0x0c00 + ((n) * 0x10))
#elif	defined(CONFIG_MX28)
#define	PINCTRL_BANKS		5
#define	PINCTRL_DOUT(n)		(0x0700 + ((n) * 0x10))
#define	PINCTRL_DIN(n)		(0x0900 + ((n) * 0x10))
#define	PINCTRL_DOE(n)		(0x0b00 + ((n) * 0x10))
#define	PINCTRL_PIN2IRQ(n)	(0x1000 + ((n) * 0x10))
#define	PINCTRL_IRQEN(n)	(0x1100 + ((n) * 0x10))
#define	PINCTRL_IRQSTAT(n)	(0x1400 + ((n) * 0x10))
#else
#error "Please select CONFIG_MX23 or CONFIG_MX28"
#endif

#define GPIO_INT_FALL_EDGE	0x0
#define GPIO_INT_LOW_LEV	0x1
#define GPIO_INT_RISE_EDGE	0x2
#define GPIO_INT_HIGH_LEV	0x3
#define GPIO_INT_LEV_MASK	(1 << 0)
#define GPIO_INT_POL_MASK	(1 << 1)

#define EINVAL 22
void mxs_gpio_init(void)
{
	int i;

	for (i = 0; i < PINCTRL_BANKS; i++) {
		writel(0, MXS_PINCTRL_BASE + PINCTRL_PIN2IRQ(i));
		writel(0, MXS_PINCTRL_BASE + PINCTRL_IRQEN(i));
		/* Use SCT address here to clear the IRQSTAT bits */
		writel(0xffffffff, MXS_PINCTRL_BASE + PINCTRL_IRQSTAT(i) + 8);
	}
}

int gpio_get_value(int gp)
{
	uint32_t bank = PAD_BANK(gp);
	uint32_t offset = PINCTRL_DIN(bank);
	struct mx28_register *reg =
		(struct mx28_register *)(MXS_PINCTRL_BASE + offset);

	return (readl(&reg->reg) >> PAD_PIN(gp)) & 1;
}

void gpio_set_value(int gp, int value)
{
	uint32_t bank = PAD_BANK(gp);
	uint32_t offset = PINCTRL_DOUT(bank);
	struct mx28_register *reg =
		(struct mx28_register *)(MXS_PINCTRL_BASE + offset);

	if (value)
		writel(1 << PAD_PIN(gp), &reg->reg_set);
	else
		writel(1 << PAD_PIN(gp), &reg->reg_clr);
}

int gpio_direction_input(int gp)
{
	uint32_t bank = PAD_BANK(gp);
	uint32_t offset = PINCTRL_DOE(bank);
	struct mx28_register *reg =
		(struct mx28_register *)(MXS_PINCTRL_BASE + offset);

	writel(1 << PAD_PIN(gp), &reg->reg_clr);

	return 0;
}

int gpio_direction_output(int gp, int value)
{
	uint32_t bank = PAD_BANK(gp);
	uint32_t offset = PINCTRL_DOE(bank);
	struct mx28_register *reg =
		(struct mx28_register *)(MXS_PINCTRL_BASE + offset);

	writel(1 << PAD_PIN(gp), &reg->reg_set);

	gpio_set_value(gp, value);

	return 0;
}

int gpio_request(int gp, const char *label)
{
	if (PAD_BANK(gp) > PINCTRL_BANKS)
		return -EINVAL;

	return 0;
}

void gpio_free(int gp)
{
}

void gpio_toggle_value(int gp)
{
	gpio_set_value(gp, !gpio_get_value(gp));
}
