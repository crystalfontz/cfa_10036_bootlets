/*
 * Freescale i.MX28 I2C Driver
 *
 * Copyright (C) 2011 Marek Vasut <marek.vasut@gmail.com>
 * on behalf of DENX Software Engineering GmbH
 *
 * Partly based on Linux kernel i2c-mxs.c driver:
 * Copyright (C) 2011 Wolfram Sang, Pengutronix e.K.
 *
 * Which was based on a (non-working) driver which was:
 * Copyright (C) 2009-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

//#include <common.h>
//#include <malloc.h>
//#include <asm/errno.h>
//#include <asm/io.h>
//#include <asm/arch/clock.h>
#include "types.h"
#include "error.h"
//#include "io.h"
#include "debug.h"
#include "utils.h"
#include "arch-mx28/iomux.h"
#include "arch-mx28/iomux-mx28.h"
#include "arch-mx28/gpio.h"
#include "arch-mx28/regs-common.h"
#include "arch-mx28/imx-regs.h"
#include "arch-mx28/regs-base.h"
#include "arch-mx28/regs-i2c.h"
#include "arch-mx28/sys_proto.h"

#define	MXS_I2C_MAX_TIMEOUT	1000000

//#define DEBUGGING_MODE
#ifdef DEBUGGING_MODE
#define debug printf 
#else
#define debug(v,...) 
#endif


//typedef uint32_t iomux_cfg_t;
#define writel(b,addr) ((*(volatile uint32_t *) (addr)) = (b))
#define readl(addr) (*(volatile uint32_t *) (addr))

/* 1 second delay should be plenty of time for block reset. */
#define	RESET_MAX_TIMEOUT	1000000

#define	MX28_BLOCK_SFTRST	(1 << 31)
#define	MX28_BLOCK_CLKGATE	(1 << 30)


int mx28_wait_mask_set(struct mx28_register *reg, uint32_t mask, int timeout)
{
	while (--timeout) {
		if ((readl(&reg->reg) & mask) == mask)
			break;
		udelay(1);
	}

	return !timeout;
}

int mx28_wait_mask_clr(struct mx28_register *reg, uint32_t mask, int timeout)
{
	while (--timeout) {
		if ((readl(&reg->reg) & mask) == 0)
			break;
		udelay(1);
	}

	return !timeout;
}


int mx28_reset_block(struct mx28_register *reg)
{
	/* Clear SFTRST */
	writel(MX28_BLOCK_SFTRST, &reg->reg_clr);

	if (mx28_wait_mask_clr(reg, MX28_BLOCK_SFTRST, RESET_MAX_TIMEOUT))
		return 1;

	/* Clear CLKGATE */
	writel(MX28_BLOCK_CLKGATE, &reg->reg_clr);

	/* Set SFTRST */
	writel(MX28_BLOCK_SFTRST, &reg->reg_set);

	/* Wait for CLKGATE being set */
	if (mx28_wait_mask_set(reg, MX28_BLOCK_CLKGATE, RESET_MAX_TIMEOUT))
		return 1;

	/* Clear SFTRST */
	writel(MX28_BLOCK_SFTRST, &reg->reg_clr);

	if (mx28_wait_mask_clr(reg, MX28_BLOCK_SFTRST, RESET_MAX_TIMEOUT))
		return 1;

	/* Clear CLKGATE */
	writel(MX28_BLOCK_CLKGATE, &reg->reg_clr);

	if (mx28_wait_mask_clr(reg, MX28_BLOCK_CLKGATE, RESET_MAX_TIMEOUT))
		return 1;

	return 0;
}

void mxs_i2c_reset(void)
{
	struct mx28_i2c_regs *i2c_regs = (struct mx28_i2c_regs *)MXS_I2C0_BASE;
	int ret;

	ret = mx28_reset_block(&i2c_regs->hw_i2c_ctrl0_reg);
	if (ret) {
		printf("MXS I2C: Block reset timeout\r\n");
		return;
	}

	writel(I2C_CTRL1_DATA_ENGINE_CMPLT_IRQ | I2C_CTRL1_NO_SLAVE_ACK_IRQ |
		I2C_CTRL1_EARLY_TERM_IRQ | I2C_CTRL1_MASTER_LOSS_IRQ |
		I2C_CTRL1_SLAVE_STOP_IRQ | I2C_CTRL1_SLAVE_IRQ,
		&i2c_regs->hw_i2c_ctrl1_clr);

	writel(I2C_QUEUECTRL_PIO_QUEUE_MODE, &i2c_regs->hw_i2c_queuectrl_set);
}

void mxs_i2c_setup_read(uint8_t chip, int len)
{
	struct mx28_i2c_regs *i2c_regs = (struct mx28_i2c_regs *)MXS_I2C0_BASE;

	writel(I2C_QUEUECMD_RETAIN_CLOCK | I2C_QUEUECMD_PRE_SEND_START |
		I2C_QUEUECMD_MASTER_MODE | I2C_QUEUECMD_DIRECTION |
		(1 << I2C_QUEUECMD_XFER_COUNT_OFFSET),
		&i2c_regs->hw_i2c_queuecmd);

	writel((chip << 1) | 1, &i2c_regs->hw_i2c_data);

	writel(I2C_QUEUECMD_SEND_NAK_ON_LAST | I2C_QUEUECMD_MASTER_MODE |
		(len << I2C_QUEUECMD_XFER_COUNT_OFFSET) |
		I2C_QUEUECMD_POST_SEND_STOP, &i2c_regs->hw_i2c_queuecmd);

	writel(I2C_QUEUECTRL_QUEUE_RUN, &i2c_regs->hw_i2c_queuectrl_set);
}

void mxs_i2c_write(uint8_t chip, uint32_t addr, int alen,
			uint8_t *buf, int blen, int stop)
{
	struct mx28_i2c_regs *i2c_regs = (struct mx28_i2c_regs *)MXS_I2C0_BASE;
	uint32_t data;
	int i, remain, off;

	if ((alen > 4) || (alen == 0)) {
//	if (alen > 4) { //for CF OLED we do not need an address
		printf("MXS I2C: Invalid address length\r\n");
		return;
	}

	if (stop)
		stop = I2C_QUEUECMD_POST_SEND_STOP;

	writel(I2C_QUEUECMD_PRE_SEND_START |
		I2C_QUEUECMD_MASTER_MODE | I2C_QUEUECMD_DIRECTION |
		((blen + alen + 1) << I2C_QUEUECMD_XFER_COUNT_OFFSET) | stop,
		&i2c_regs->hw_i2c_queuecmd);
        writel(I2C_QUEUECTRL_QUEUE_RUN, &i2c_regs->hw_i2c_queuectrl_set);
	data = (chip << 1) << 24;

	for (i = 0; i < alen; i++) {
		data >>= 8;
		data |= ((char *)&addr)[i] << 24;
		if ((i & 3) == 2){
                       debug("Address data is: %X\r\n",data);
			writel(data, &i2c_regs->hw_i2c_data);
                }
	}

	off = i;
	for (; i < off + blen; i++) {
		data >>= 8;
		data |= buf[i - off] << 24;
		if ((i & 3) == 2){
                        debug("More data is: %X\r\n",data);
			writel(data, &i2c_regs->hw_i2c_data);
                }
	}

	remain = 24 - ((i & 3) * 8);
	if (remain){
                debug("Last data is : %X\r\n",data >> remain);
		writel(data >> remain, &i2c_regs->hw_i2c_data);
        }
//	writel(I2C_QUEUECTRL_QUEUE_RUN, &i2c_regs->hw_i2c_queuectrl_set);
}

int mxs_i2c_wait_for_ack(void)
{
	struct mx28_i2c_regs *i2c_regs = (struct mx28_i2c_regs *)MXS_I2C0_BASE;
	volatile uint32_t tmp;
	int timeout = MXS_I2C_MAX_TIMEOUT;

	for (;;) {
		tmp = readl(&i2c_regs->hw_i2c_ctrl1);
	//	printf("\r\nAck value is %X\r\n",tmp);
		if (tmp & I2C_CTRL1_NO_SLAVE_ACK_IRQ) {
			printf("MXS I2C: No slave ACK\r\n");
			goto err;
		}

		if (tmp & (

			I2C_CTRL1_EARLY_TERM_IRQ | I2C_CTRL1_MASTER_LOSS_IRQ |
			I2C_CTRL1_SLAVE_STOP_IRQ | I2C_CTRL1_SLAVE_IRQ)) {
			printf("MXS I2C: Error (CTRL1 = %08x)\r\n", tmp);
			goto err;
		}

		if (tmp & I2C_CTRL1_DATA_ENGINE_CMPLT_IRQ)
                writel(I2C_CTRL1_DATA_ENGINE_CMPLT_IRQ, &i2c_regs->hw_i2c_ctrl1_clr);

			break;

		if (!timeout--) {
			printf("MXS I2C: Operation timed out\r\n");
			goto err;
		}

		udelay(1);
	}
        udelay(2000);
        debug("clean send of data\r\n");
	return 0;

err:
	mxs_i2c_reset();
	return tmp;
}

int i2c_read(uint8_t chip, uint32_t addr, int alen, uint8_t *buffer, int len)
{
	struct mx28_i2c_regs *i2c_regs = (struct mx28_i2c_regs *)MXS_I2C0_BASE;
	uint32_t tmp = 0;
	int ret;
	int i;

	mxs_i2c_write(chip, addr, alen, NULL, 0, 0);
	ret = mxs_i2c_wait_for_ack();
	if (ret) {
		debug("MXS I2C: Failed writing address\r\n");
		return ret;
	}

	mxs_i2c_setup_read(chip, len);
	ret = mxs_i2c_wait_for_ack();
	if (ret) {
		debug("MXS I2C: Failed reading address\r\n");
		return ret;
	}

	for (i = 0; i < len; i++) {
		if (!(i & 3)) {
			while (readl(&i2c_regs->hw_i2c_queuestat) &
				I2C_QUEUESTAT_RD_QUEUE_EMPTY)
				;
			tmp = readl(&i2c_regs->hw_i2c_queuedata);
		}
		buffer[i] = tmp & 0xff;
		tmp >>= 8;
	}

	return 0;
}

int i2c_write(uint8_t chip, uint32_t addr, int alen, uint8_t *buffer, int len)
{
        int i;
        //udelay(1000);
       // printf("inside i2c_write length is %X\r\n",len);
	int ret;
  	mxs_i2c_write(chip, addr, alen, buffer, len, 1);
	ret = mxs_i2c_wait_for_ack();
	if (ret){
                debug("Retcode is = %X\n\r",ret);
                debug("Address is = %X\n\r");                
		printf("MXS I2C: Failed writing address\r\n");
        }

	return ret;
}

int i2c_probe(uint8_t chip)
{
	int ret;
	mxs_i2c_write(chip, 0, 0, NULL, 0, 1);
	ret = mxs_i2c_wait_for_ack();
	mxs_i2c_reset();
	return ret;
}

void i2c_init(int speed)
{
	struct mx28_i2c_regs *i2c_regs = (struct mx28_i2c_regs *)MXS_I2C0_BASE;

	mxs_i2c_reset();

	switch (speed) {
	case 100000:
		writel((0x0078 << I2C_TIMING0_HIGH_COUNT_OFFSET) |
			(0x0030 << I2C_TIMING0_RCV_COUNT_OFFSET),
			&i2c_regs->hw_i2c_timing0);
		writel((0x0080 << I2C_TIMING1_LOW_COUNT_OFFSET) |
			(0x0030 << I2C_TIMING1_XMIT_COUNT_OFFSET),
			&i2c_regs->hw_i2c_timing1);
		break;
	case 400000:
		writel((0x000f << I2C_TIMING0_HIGH_COUNT_OFFSET) |
			(0x0007 << I2C_TIMING0_RCV_COUNT_OFFSET),
			&i2c_regs->hw_i2c_timing0);
		writel((0x001f << I2C_TIMING1_LOW_COUNT_OFFSET) |
			(0x000f << I2C_TIMING1_XMIT_COUNT_OFFSET),
			&i2c_regs->hw_i2c_timing1);
		break;
	default:
		printf("MXS I2C: Invalid speed selected (%d Hz)\r\n", speed);
		return;
	}

	writel((0x0015 << I2C_TIMING2_BUS_FREE_OFFSET) |
		(0x000d << I2C_TIMING2_LEADIN_COUNT_OFFSET),
		&i2c_regs->hw_i2c_timing2);

	return;
}










