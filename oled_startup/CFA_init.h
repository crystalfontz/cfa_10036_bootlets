/*
* Copyright (C) 2011, 2012 Jim Wall Abstract Designs
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


#ifndef  INCLUDE_CFA_INIT_H
#define  INCLUDE_CFA_INIT_H

#include "arch-mx28/iomux-mx28.h"
#include "utils.h"

#ifdef CFA10036_V0
//remap the power pin to GPIO3_29 if we are on the rev 0 board
#define POWER_LED 	MX28_PAD_PWM4__GPIO_3_29 
#else
#define POWER_LED	MX28_PAD_AUART1_RX__GPIO_3_4
#define OLED_POWER	MX28_PAD_PWM4__PWM_4
#define OLED_RESET	MX28_PAD_SSP0_DATA7__GPIO_2_7
#define DUART_RX	MX28_PAD_AUART0_CTS__DUART_RX
#define DUART_TX	MX28_PAD_AUART0_RTS__DUART_TX
#define OLED_SCL	MX28_PAD_AUART0_RX__I2C0_SCL			
#define OLED_SDA	MX28_PAD_AUART0_TX__I2C0_SDA			
#endif //CFA10036_V0

extern void cfa_init(void);
extern void toggle_PWR_LED(void);

#endif //INCLUDE_CFA_INIT_H
