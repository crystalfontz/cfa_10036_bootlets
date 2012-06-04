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


#include "types.h"
#include "error.h"
#include "io.h"
#include "debug.h"
#include "arch-mx28/iomux.h"
#include "arch-mx28/iomux-mx28.h"
#include "arch-mx28/gpio.h"
#include "arch-mx28/regs-common.h"
#include "registers/regspinctrl.h"
#include "arch-mx28/imx-regs.h"
#include "arch-mx28/regs-base.h"
#include "arch-mx28/regs-i2c.h"
#include "arch-mx28/sys_proto.h"

#define CFA10036_V1
#define debug printf

//typedef uint32_t iomux_cfg_t;
#define writel(b,addr) ((*(volatile uint32_t *) (addr)) = (b))
#define readl(addr) (*(volatile uint32_t *) (addr))

//remap the power pin to GPIO3_29 if we are on the rev 0 board
#ifdef CFA10036_V1
#define POWER_LED	MX28_PAD_AUART1_RX__GPIO_3_4
#define OLED_POWER	MX28_PAD_PWM4__PWM_4
#define DUART_RX	MX28_PAD_AUART0_CTS__DUART_RX
#define DUART_TX	MX28_PAD_AUART0_RTS__DUART_TX
#else
//remap the power pin to GPIO3_29 if we are on the rev 0 board
#define POWER_LED 	MX28_PAD_PWM4__GPIO_3_29 
#endif //CFA10036_V1
/*
// Motor definitions to pin mapping
#define DIRECTION	MX28_PAD_SAIF0_BITCLK__GPIO_3_22
#define STEP		MX28_PAD_I2C0_SCL__GPIO_3_24
#define NOT_SLEEP	MX28_PAD_I2C0_SDA__GPIO_3_25
#define NOT_RESET	MX28_PAD_SPDIF__GPIO_3_27
#define NOT_ENABLE	MX28_PAD_AUART0_RX__GPIO_3_0
#define MS1		MX28_PAD_AUART0_TX__GPIO_3_1
#define MS2		MX28_PAD_AUART0_CTS__GPIO_3_2
#define MS3		MX28_PAD_AUART0_RTS__GPIO_3_3

#define FULL_STEP	0x00
#define HALF_STEP	0x01
#define QUARTER_STEP	0x02
#define EIGHTH_STEP	0x03
#define SIXTEENTH_STEP	0x07

#define FORWARD		0x01
#define REVERSE		0x00

*/


/*
* this function is used to initialize all of the 
* pins that the Alegro A4988 chip is connected to
*/ 
void init_A4988( void) {

/*
mxs_iomux_setup_pad(STEP | 
                     MXS_PAD_12MA | MXS_PAD_3V3);
 gpio_direction_output(STEP, 0); 

mxs_iomux_setup_pad( DIRECTION | 
                     MXS_PAD_12MA | MXS_PAD_3V3);
 gpio_direction_output(DIRECTION, 0); 

mxs_iomux_setup_pad( NOT_SLEEP | 
                     MXS_PAD_12MA | MXS_PAD_3V3);
 gpio_direction_output(NOT_SLEEP, 1); 

mxs_iomux_setup_pad( NOT_RESET | 
                     MXS_PAD_12MA | MXS_PAD_3V3);
 gpio_direction_output(NOT_RESET, 1); 

mxs_iomux_setup_pad( NOT_ENABLE | 
                     MXS_PAD_12MA | MXS_PAD_3V3);
 gpio_direction_output(NOT_ENABLE, 0); 

 
mxs_iomux_setup_pad( MS1 | 
                     MXS_PAD_12MA | MXS_PAD_3V3);
 gpio_direction_output(MS1, 0); 

mxs_iomux_setup_pad( MS2 | 
                     MXS_PAD_12MA | MXS_PAD_3V3);
 gpio_direction_output(MS2, 0); 

mxs_iomux_setup_pad( MS3 | 
                     MXS_PAD_12MA | MXS_PAD_3V3);
 gpio_direction_output(MS3, 0); 
*/
}

void set_stepping( unsigned char Resolution ) {
  /*   
     gpio_set_value( MS1, Resolution&0x01?1:0);
     gpio_set_value( MS2, Resolution&0x02?1:0);
     gpio_set_value( MS1, Resolution&0x04?1:0);
*/
}
/*
void set_direction(unsigned char direction) {
     
     gpio_set_value( DIRECTION, (direction&0x01));
}

void step ( void ) {

     gpio_set_value( STEP, 1);
     delay(5);
     gpio_set_value( STEP, 0);
}

*/

//this is the  entry point into our code
int _start( void )
{
int i, interval, count;

/* first reroute the DUART pins to match the 10036 v0.1 board */
 HW_PINCTRL_MUXSEL7_CLR(0xF);
 mxs_iomux_setup_pad(DUART_RX);
 mxs_iomux_setup_pad(DUART_TX);

/* next turn on the power LED */
 mxs_iomux_setup_pad(POWER_LED |
                     MXS_PAD_12MA | MXS_PAD_3V3);

 gpio_direction_output(POWER_LED, 0);
/*
 init_A4988();
 printf("\nPins are initialized\n");

for (count = 0; count < 4; count++) { 
 set_direction(FORWARD);
 printf("Stepping Forward\n"); 
for(i = 0 ; i < 200; i++) {
    step();
    udelay(700);
 }
 udelay(1000000);
 set_direction(REVERSE);
 printf("Stepping Backward\n");
 for(i = 0 ; i < 200; i++) {
    step();
    udelay(700);
 }
udelay(1000000);
}
while(1) {
set_stepping(0x07);
 set_direction(FORWARD);
 printf("Stepping Forward\n");
for(i = 0 ; i < 3200; i++) {
    step();
    udelay(50);
 }
udelay(1000000);
 set_direction(REVERSE);
 printf("Stepping Backward\n");
 for(i = 0 ; i < 3200; i++) {
    step();
    udelay(50);
 }
 udelay(1000000);
  }
*/
while(1) {
 printf("Hello\n");
 gpio_toggle_value(POWER_LED);
 delay(100000);
}
}
 
