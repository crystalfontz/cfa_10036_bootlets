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
#include "io.h"
#include "debug.h"
#include "arch-mx28/iomux.h"
#include "arch-mx28/iomux-mx28.h"
#include "arch-mx28/gpio.h"
#include "arch-mx28/regs-common.h"
#include "arch-mx28/imx-regs.h"
#include "arch-mx28/regs-base.h"
#include "arch-mx28/regs-i2c.h"
#include "arch-mx28/sys_proto.h"
#include "SSD1307.h"

#define	MXS_I2C_MAX_TIMEOUT	1000000
#define debug printf

//typedef uint32_t iomux_cfg_t;
#define writel(b,addr) ((*(volatile uint32_t *) (addr)) = (b))
#define readl(addr) (*(volatile uint32_t *) (addr))

// Crystalfontz logo for display in page mode
extern unsigned char cf_p_logo[48];

// Crystalfontz logo for display in vertical mode
extern unsigned char cf_v_logo[48];

// the font lookup table for displaying in SEG remap mode
// use reverse function if you want to display this font
// in SEG normal mode.
extern unsigned char Font[][8];


void mxs_i2c_reset(void)
{
	struct mx28_i2c_regs *i2c_regs = (struct mx28_i2c_regs *)MXS_I2C1_BASE;
	int ret;

	ret = mx28_reset_block(&i2c_regs->hw_i2c_ctrl0_reg);
	if (ret) {
		debug("MXS I2C: Block reset timeout\n");
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
	struct mx28_i2c_regs *i2c_regs = (struct mx28_i2c_regs *)MXS_I2C1_BASE;

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
	struct mx28_i2c_regs *i2c_regs = (struct mx28_i2c_regs *)MXS_I2C1_BASE;
	uint32_t data;
	int i, remain, off;

//	if ((alen > 4) || (alen == 0)) {
	if (alen > 4) { //for CF OLED we do not need an address
		debug("MXS I2C: Invalid address length\n");
		return;
	}

	if (stop)
		stop = I2C_QUEUECMD_POST_SEND_STOP;

	writel(I2C_QUEUECMD_PRE_SEND_START |
		I2C_QUEUECMD_MASTER_MODE | I2C_QUEUECMD_DIRECTION |
		((blen + alen + 1) << I2C_QUEUECMD_XFER_COUNT_OFFSET) | stop,
		&i2c_regs->hw_i2c_queuecmd);

	data = (chip << 1) << 24;

	for (i = 0; i < alen; i++) {
		data >>= 8;
		data |= ((char *)&addr)[i] << 24;
		if ((i & 3) == 2){
                        printf("Address data is: %X\n",data);
			writel(data, &i2c_regs->hw_i2c_data);
                }
	}

	off = i;
	for (; i < off + blen; i++) {
		data >>= 8;
		data |= buf[i - off] << 24;
		if ((i & 3) == 2){
                        printf("More data is: %X\n",data);
			writel(data, &i2c_regs->hw_i2c_data);
                }
	}

	remain = 24 - ((i & 3) * 8);
	if (remain){
                printf("Last data is : %X\n",data >> remain);
		writel(data >> remain, &i2c_regs->hw_i2c_data);
        }
	writel(I2C_QUEUECTRL_QUEUE_RUN, &i2c_regs->hw_i2c_queuectrl_set);
}

int mxs_i2c_wait_for_ack(void)
{
	struct mx28_i2c_regs *i2c_regs = (struct mx28_i2c_regs *)MXS_I2C1_BASE;
	uint32_t tmp;
	int timeout = MXS_I2C_MAX_TIMEOUT;

	for (;;) {
		tmp = readl(&i2c_regs->hw_i2c_ctrl1);
		if (tmp & I2C_CTRL1_NO_SLAVE_ACK_IRQ) {
			debug("MXS I2C: No slave ACK\n");
			goto err;
		}

		if (tmp & (
			I2C_CTRL1_EARLY_TERM_IRQ | I2C_CTRL1_MASTER_LOSS_IRQ |
			I2C_CTRL1_SLAVE_STOP_IRQ | I2C_CTRL1_SLAVE_IRQ)) {
			debug("MXS I2C: Error (CTRL1 = %08x)\n", tmp);
			goto err;
		}

		if (tmp & I2C_CTRL1_DATA_ENGINE_CMPLT_IRQ)
			break;

		if (!timeout--) {
			debug("MXS I2C: Operation timed out\n");
			goto err;
		}

		udelay(1);
	}
        debug("clean send of data\n");
	return 0;

err:
	mxs_i2c_reset();
	return 1;
}

int i2c_read(uint8_t chip, uint32_t addr, int alen, uint8_t *buffer, int len)
{
	struct mx28_i2c_regs *i2c_regs = (struct mx28_i2c_regs *)MXS_I2C1_BASE;
	uint32_t tmp = 0;
	int ret;
	int i;

	mxs_i2c_write(chip, addr, alen, NULL, 0, 0);
	ret = mxs_i2c_wait_for_ack();
	if (ret) {
		debug("MXS I2C: Failed writing address\n");
		return ret;
	}

	mxs_i2c_setup_read(chip, len);
	ret = mxs_i2c_wait_for_ack();
	if (ret) {
		debug("MXS I2C: Failed reading address\n");
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
	int ret;
	mxs_i2c_write(chip, addr, alen, buffer, len, 1);
	ret = mxs_i2c_wait_for_ack();
	if (ret)
		debug("MXS I2C: Failed writing address\n");

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
	struct mx28_i2c_regs *i2c_regs = (struct mx28_i2c_regs *)MXS_I2C1_BASE;

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
		printf("MXS I2C: Invalid speed selected (%d Hz)\n", speed);
		return;
	}

	writel((0x0015 << I2C_TIMING2_BUS_FREE_OFFSET) |
		(0x000d << I2C_TIMING2_LEADIN_COUNT_OFFSET),
		&i2c_regs->hw_i2c_timing2);

	return;
}


////////////////////////////////////////////////////////////////////////////////
//Crystalfontz code changes to move debug and I2C ports around
//to allow us to turn on the OLED early
////////////////////////////////////////////////////////////////////////////////
void set_cf_mods()
{

// turn on the power indicator LED
	mxs_iomux_setup_pad(MX28_PAD_PWM4__GPIO_3_29 |
                        MXS_PAD_12MA | MXS_PAD_3V3 | MXS_PAD_PULLUP);
        gpio_direction_output(MX28_PAD_PWM4__GPIO_3_29, 0);

// Put the OLED in the reset state
	mxs_iomux_setup_pad(MX28_PAD_AUART3_RTS__GPIO_3_15 |
                        MXS_PAD_12MA | MXS_PAD_3V3 | MXS_PAD_PULLUP);
        gpio_direction_output(MX28_PAD_AUART3_RTS__GPIO_3_15, 0);


// Set up the DUART on the mxs_i2c_write alternate pins so that we can use I2C1
	mxs_iomux_setup_pad(MX28_PAD_I2C0_SCL__DUART_RX |
                        MXS_PAD_12MA | MXS_PAD_3V3);
	mxs_iomux_setup_pad(MX28_PAD_I2C0_SDA__DUART_TX |
                        MXS_PAD_12MA | MXS_PAD_3V3);
	printf("\r\ndebugging on alternate pins\r\n");
// Set up the I2C1 channel SLC and SDA
	mxs_iomux_setup_pad(MX28_PAD_PWM0__I2C1_SCL |
                        MXS_PAD_12MA | MXS_PAD_3V3);

	mxs_iomux_setup_pad(MX28_PAD_PWM1__I2C1_SDA |
                        MXS_PAD_12MA | MXS_PAD_3V3);
	printf("I2C1 is now configured\n");

}

void I2C_send(uint8_t command, uint8_t * buff, int length)
{
int i;
static uint8_t data[3];  // to send a byte we need to send an address, command and data

data[0] = SSD1307_I2C_ADDRESS;
data[1] = command;

 for(i = 0; i < length; i++)
 {
  i2c_write(SSD1307_I2C_ADDRESS,command,1, &buff[i],1);
 }
}


int Init_SSD1307(void)
{
    
    int cmd_index = 0;
    unsigned char cmd_buffer[40];
  
 // Initialize the comand buffer 
//  SSD1307_ResetIndex(cmd_index);
//  SSD1307_InitBuffer(cmd_buffer,40);

 //build up the initialization string to send to the OLED
 // Set the display to default settings with the exception
 // of the SEGReg which we want remapped to 127
  SSD1307_DisplayOn(cmd_buffer,cmd_index);
 // SSD1307_DisplayOff(cmd_buffer,cmd_index);
  SSD1307_ScrollOff(cmd_buffer,cmd_index);
  SSD1307_SEG_RemapOn(cmd_buffer,cmd_index);
  SSD1307_InverseOff(cmd_buffer,cmd_index);
  SSD1307_StartPageAddr(cmd_buffer,cmd_index,0);
  SSD1307_DisplayOffset(cmd_buffer,cmd_index,0);
  SSD1307_COM_ScanNormal(cmd_buffer,cmd_index);
// send_pmode_logo(1,0);
// send_pmode_logo(1,24);
// send_pmode_logo(1,48);
// send_pmode_logo(1,72); 

  SSD1307_AddressMode(cmd_buffer,cmd_index,SSD1307_PAGE_MODE);
//  SSD1307_DisplayOn(cmd_buffer,cmd_index);
 
  I2C_send(SSD1307_COMMAND,&cmd_buffer[0], cmd_index); 
}
//////////////////////////////////////////////////////////
// Function to initialize the display to page mode
void initPageMode(void)
{
    
    int cmd_index = 0;
    unsigned char cmd_buffer[40];
 // Initialize the comand buffer 
  SSD1307_ResetIndex(cmd_index);
  SSD1307_InitBuffer(cmd_buffer,40);
  // Set the display to default settings with the exception
  // of the SEGReg which we want remapped to 127
 
  SSD1307_DisplayOff(cmd_buffer,cmd_index);
  SSD1307_ScrollOff(cmd_buffer,cmd_index);
  SSD1307_SEG_RemapOn(cmd_buffer,cmd_index);
  SSD1307_InverseOn(cmd_buffer,cmd_index);
  SSD1307_StartPageAddr(cmd_buffer,cmd_index,0);
  SSD1307_DisplayOffset(cmd_buffer,cmd_index,0);
  SSD1307_COM_ScanNormal(cmd_buffer,cmd_index);
  SSD1307_AddressMode(cmd_buffer,cmd_index,SSD1307_PAGE_MODE);
  SSD1307_DisplayOn(cmd_buffer,cmd_index);

  //Send the buffer to the I2C bus
 
  I2C_send(SSD1307_COMMAND,&cmd_buffer[0],cmd_index); 
}

//////////////////////////////////////////////////////
void initVerticalMode(unsigned char StartPage, unsigned char EndPage, unsigned char StartColumn, unsigned char EndColumn)
{
 
    int cmd_index = 0;
    unsigned char cmd_buffer[40];
 // Initialize the comand buffer 
 // SSD1307_ResetIndex(cmd_index);
 // SSD1307_InitBuffer(cmd_buffer,40);
 
  // Set the display to default settings with the exception
  // of the SEGReg which we want remapped to 127
   
  SSD1307_DisplayOff(cmd_buffer,cmd_index); 
  SSD1307_ScrollOff(cmd_buffer,cmd_index);
  SSD1307_AddressMode(cmd_buffer,cmd_index,SSD1307_VERTICAL_MODE);
  SSD1307_SEG_RemapOn(cmd_buffer,cmd_index);
  SSD1307_InverseOff(cmd_buffer,cmd_index);
  SSD1307_ColumnAddress(cmd_buffer,cmd_index,StartColumn,EndColumn);
  SSD1307_PageAddress(cmd_buffer,cmd_index,StartPage,EndPage); 
  SSD1307_COM_ScanNormal(cmd_buffer,cmd_index);
  SSD1307_ScrollOff(cmd_buffer,cmd_index);
  SSD1307_DisplayOn(cmd_buffer,cmd_index);
 
  //Send the buffer to the I2C bus
  I2C_send(SSD1307_COMMAND,&cmd_buffer[0], cmd_index); 
 
 
}
// set the position in the RAM where the next write will occur
void setXY(unsigned char page,unsigned char column)
{ 
   
    int cmd_index = 0;
    unsigned char cmd_buffer[40];

  SSD1307_StartPageAddr(cmd_buffer,cmd_index,page);
  SSD1307_StartColumnAddress(cmd_buffer,cmd_index,column);

  I2C_send(SSD1307_COMMAND,&cmd_buffer[0], cmd_index); 
 
 
}
void sendStr(unsigned char *string)
{
  int i; 
  while(*string)
  {
    for(i=0;i<8;i++)
    {
      I2C_send(SSD1307_DATA,&Font[*string-0x20][i],1);    
    }
    string++;
  }
}
//////////////////////////////////////////////////////////////////////////////
void setHorizontalScroll(unsigned char Direction)
{

    int cmd_index = 0;
    unsigned char cmd_buffer[5];
  SSD1307_HorizontalScroll(cmd_buffer,cmd_index,Direction,0x01,0x00,0x01);
  I2C_send(SSD1307_COMMAND,&cmd_buffer[0], cmd_index); 
 
}
/////////////////////////////////////////////////////////////
//Display Crystalfontz logo in vertical mode
void print_vmode_logo(int column)
{
 
    int cmd_index = 0;
    unsigned char cmd_buffer[5];
 
  SSD1307_StartColumnAddress(cmd_buffer,cmd_index,column);
  I2C_send(SSD1307_COMMAND,&cmd_buffer[0], cmd_index); 
 
  I2C_send(SSD1307_DATA,&cf_v_logo[0],48);
  
}
// clear all rows between columns start and end
void vmode_clear(int start, int end)
{
 uint8_t byte[5] ={0x00,0x00,0x00,0x00,0x00};
 int i,j;
 for(i = start; i < end; i++)
 {
       I2C_send(SSD1307_DATA,&byte[0], 5);
 }
}

void send_pmode_logo(int row, int column)
{

   printf("SetXY inside logo1n"); 
  setXY(row,column);
  I2C_send(SSD1307_DATA,&cf_p_logo[0], 24); 
  setXY(row+1,column);
  I2C_send(SSD1307_DATA,&cf_p_logo[24], 48);
}
// function for clearing the current page
// this function assumes that we are already in 
// page mode. After clearing the page we reset the 
// column to 0.
void P_ClearPages(int start, int finish) 
{ 
 int i,j;
 uint8_t blanks[5] ={0x00,0x00,0x00,0x00,0x00};
 for(i=start;i<(finish+1);i++)
  { 
    setXY(i,0);
      for(j=0;j<128;j++)//clear whole column
      {
        I2C_send(SSD1307_DATA,&blanks[0], 5);
      }   
  }
  setXY(start,0); // move to the start
}

int _start(void)
{
// int retval = 0;
 set_cf_mods();
// take the OLED out of reset
// gpio_direction_output(MX28_PAD_AUART3_RTS__GPIO_3_15, 1);
// printf("we should see this message and have the LED on\n");
// i2c_init(100000);
// printf("probing for device\n");
 //try resetting the OLED
// gpio_direction_output(MX28_PAD_AUART3_RTS__GPIO_3_15, 0);
//udelay(1000);
//gpio_direction_output(MX28_PAD_AUART3_RTS__GPIO_3_15, 1);
// i2c_probe(0x3c);
// printf("Sending Init_SSD1307\n");
// Init_SSD1307();
// P_ClearPages(0,4);

// initVerticalMode(1,2,0,127);
// printf("doing vmode_clear\n");
 ////vmode_clear(0,127);
 //udelay(1000000);
// print_vmode_logo(0);

 //udelay(1000000);
 //printf("Sending Init_SSD1307\n");
// Init_SSD1307();
// printf("the display should be on!\n");
 //lets just sit and spin, maybe blink the LED
//printf("Sending setXY\n");
//setXY(1,0);

// send_pmode_logo(1,0);
// send_pmode_logo(1,24);
// send_pmode_logo(1,48);
// send_pmode_logo(1,72); 

 for(;;)
 {
 udelay(500000);
printf("Inside endless send loop1\n");
//P_ClearPages(0,4);
// udelay(500000);
//send_pmode_logo(1,0);
//send_pmode_logo(1,24);
//send_pmode_logo(1,48);
// send_pmode_logo(1,72); 

//initVerticalMode(0,3,0,127);
//Init_SSD1307();
 
        gpio_toggle_value(MX28_PAD_PWM4__GPIO_3_29);
        gpio_toggle_value(MX28_PAD_AUART3_RTS__GPIO_3_15);
        gpio_toggle_value(MX28_PAD_AUART2_RTS__GPIO_3_11); 
}
}
 

