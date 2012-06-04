/* This file contains the definitions for the hardware pins
 * used on the CFA-10049 board
 * 
 * Copyright 2012 Jim Wall Abstract Designs
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
 */



// This file contains the definitions for the hardware pins
// used on the CFA-10049 board
#ifndef	INCLUDE_CFA10049_H
#define INCLUDE_CFA10049_H

//stepper motor pin definitions
// X stepper on BANK0_PIN0
// MUXSEL0
// HW_PINCTRL_DRIVE0

#define STEP_X		(1<<0)
#define STEP_X_MUX	(3<<0)
#define STEP_X_MA	(3<<0)	//4ma
#define STEP_X_V	(1<<2)	//3.3v

// X direction on BANK0_PIN1
// MUXSEL0
// HW_PINCTRL_DRIVE0

#define DIR_X		(1<<1)
#define DIR_X_MUX	(3<<2)
#define DIR_X_MA	(3<<4)	//4ma
#define DIR_X_V		(1<<6)	//3.3v

// Y stepper on BANK0_PIN2
// MUXSEL0
// HW_PINCTRL_DRIVE0

#define STEP_Y		(1<<2)
#define STEP_Y_MUX	(3<<4)
#define STEP_Y_MA	(3<<8)	//4ma
#define STEP_Y_V	(1<<10)	//3.3v

// Y direction on BANK0_PIN3
// MUXSEL0
// HW_PINCTRL_DRIVE0
#define DIR_Y		(1<<3)
#define DIR_Y_MUX	(3<<6)
#define DIR_Y_MA	(3<<12)	//4ma
#define DIR_Y_V		(1<<14)	//3.3v

// Z stepper on BANK0_PIN4
// MUXSEL0
// HW_PINCTRL_DRIVE0
#define STEP_Z		(1<<4)
#define STEP_Z_MUX	(3<<8)
#define STEP_Z_MA	(3<<16)	//4ma
#define STEP_Z_V	(1<<18)	//3.3v

// Z direction on BANK0_PIN5
// MUXSEL0
// HW_PINCTRL_DRIVE0
#define DIR_Z		(1<<5)
#define DIR_Z_MUX	(3<<6)
#define DIR_Z_MA	(3<<20)	//4ma
#define DIR_Z_V		(1<<22)	//3.3v

//stepper motor pin definitions
// A stepper on BANK0_PIN6
// MUXSEL0
// HW_PINCTRL_DRIVE0
#define STEP_A		(1<<6)
#define STEP_A_MUX	(3<<4)
#define STEP_A_MA	(3<<24)	//4ma
#define STEP_A_V	(1<<26)	//3.3v

// A direction on BANK0_PIN17
// MUXSEL1
// HW_PINCTRL_DRIVE2
#define DIR_A		(1<<17)
#define DIR_A_MUX	(3<<2)
#define DIR_A_MA	(3<<4)	//4ma
#define DIR_A_V		(1<<6)	//3.3v
//stepper motor pin definitions
// B stepper on BANK0_PIN21
// MUXSEL1
// HW_PINCTRL_DRIVE2
#define STEP_B		(1<<21)
#define STEP_B_MUX	(3<<4)
#define STEP_B_MA	(3<<20)	//4ma
#define STEP_B_V	(1<<22)	//3.3v

// B direction on BANK0_PIN25
// MUXSEL1
// HW_PINCTRL_DRIVE3
#define DIR_B   	(1<<25)
#define DIR_B_MUX	(3<<6)
#define DIR_B_MA	(3<<4)	//4ma
#define DIR_B_V		(1<<6)	//3.3v

#define STEP_PINS	(STEP_X | STEP_Y | STEP_Z | STEP_A | STEP_B )
#define DIR_PINS	(DIR_X | DIR_Y | DIR_Z | DIR_A | DIR_B )
#define MUX0_PINS	(STEP_X_MUX | STEP_Y_MUX | STEP_Z_MUX | STEP_A_MUX | DIR_X_MUX | DIR_Y_MUX | DIR_Z_MUX)
#define MUX1_PINS	(DIR_A_MUX | STEP_B_MUX | DIR_B_MUX)
#define DRIVE0_MA_PINS  (STEP_X_MA | DIR_X_MA |STEP_Y_MA | DIR_Y_MA |STEP_Z_MA | DIR_Z_MA | STEP_A_MA)
#define DRIVE0_V_PINS   (STEP_X_V | DIR_X_V |STEP_Y_V | DIR_Y_V |STEP_Z_V | DIR_Z_V | STEP_A_V) 
#define DRIVE2_MA_PINS  (DIR_A_MA | STEP_B_MA)
#define DRIVE2_V_PINS	(DIR_A_V | STEP_B_V)
#define DRIVE3_MA_PINS  (DIR_B_MA)
#define DRIVE3_V_PINS   (DIR_B_V)


// MSIO BANK0_PIN28
// MUXSEL1
// HW_PINCTRL_DRIVE3
#define MOSI_MUX   (3<<24)
#define MOSI_MA    (3<<16)        //4ma
#define MOSI_V     (1<<18)        //3.3v

// SCK BANK0_PIN24
// MUXSEL1
// HW_PINCTRL_DRIVE3
#define SCK_MUX    (3<<16)
#define SCK_MA    (3<<0)	//4ma
#define SCK_V    (1<<2)		//3.3v

// RCK32 on BANK0_PIN26
// MUXSEL1
// HW_PINCTRL_DRIVE3
#define RCK32_MUX (3<<20)
#define RCK32_MA (3<<8)		//4ma
#define RCK32_V (1<<10)		//3.3v

// RCK16 on BANK0_PIN16
// MUXSEL1
// HW_PINCTRL_DRIVE2
#define RCK16_MUX (3<<0)
#define RCK16_MA (3<<0)
#define RCK16_V (1<<2)

// PWRLED on BANK3_PIN4
// MUXSEL6
//HW_PINCTRL_DRIVE12
#define PWRLED_MUX	    (3<<8)
#define PWRLED_MA           (3<<16)
#define PWRLED_V            (1<<18)


#define PWRLED_PIN     (1<<4)
#define MOSI_PIN   (1 << 28)
#define SCK_PIN	   (1 << 24)
#define RCK32_PIN (1<<26)
#define RCK16_PIN (1<<16)



#define DUARTRX_MUX (2<<4)
#define DUARTTX_MUX (2<<6)
#define DUARTRX_CLR (3<<4)
#define DUARTTX_CLR (3<<6)


#endif //INCLUDE_CFA10049_H


