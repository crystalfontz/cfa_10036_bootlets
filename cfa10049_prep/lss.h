/*
 * Copyright 2012 Jim Wall, Abstract Designs
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/*
 * This file contains the Low speed signal definitions used on the cfa-10049
 */

#ifndef INCLUDE_STEPPER_H
#define INCLUDE_STEPPER_H
#include "types.h"





//Microstepping defs
#define STEP_1	0x00
#define STEP_2	0x04
#define STEP_4	0x02
#define STEP_8	0x06	 
#define STEP_16	0x07

typedef union
{
    reg8_t U;
    struct
    {
        unsigned UNUSED		:  2;
	unsigned N_RESET	:  1;
        unsigned N_ENABLE	:  1;
        unsigned N_SLEEP	:  1;
        unsigned MS1		:  1;
        unsigned MS2		:  1;
        unsigned MS3		:  1;
  
    } B;

} a4988_cfg_t;

#define A4988_MASK	0x3f

#define BF_N_RESET	(1<<5)
#define BF_N_ENABLE	(1<<4)
#define BF_N_SLEEP	(1<<3)
#define BF_MS1		(1<<2)
#define BF_MS2		(1<<1)
#define BF_MS3		(1<<0)



#define A4988_CLEAR 	0x3f

typedef union
{
    reg32_t  U;
    struct
    {
        unsigned N_RESET_X	:  1;
        unsigned N_ENABLE_X	:  1;
        unsigned N_SLEEP_X	:  1;
        unsigned MS1_X		:  1;
        unsigned MS2_X		:  1;
        unsigned MS3_X		:  1;
        unsigned N_RESET_Y	:  1;
        unsigned N_ENABLE_Y	:  1;
        unsigned N_SLEEP_Y	:  1;
        unsigned MS1_Y		:  1;
        unsigned MS2_Y		:  1;
        unsigned MS3_Y		:  1;
        unsigned N_RESET_Z	:  1;
        unsigned N_ENABLE_Z	:  1;
        unsigned N_SLEEP_Z	:  1;
        unsigned MS1_Z		:  1;
        unsigned MS2_Z		:  1;
        unsigned MS3_Z		:  1;
        unsigned N_RESET_B	:  1;
        unsigned N_ENABLE_B	:  1;
        unsigned N_SLEEP_B	:  1;
        unsigned MS1_B		:  1;
        unsigned MS2_B		:  1;
        unsigned MS3_B		:  1;
        unsigned N_RESET_A	:  1;
        unsigned N_ENABLE_A	:  1;
        unsigned N_SLEEP_A 	:  1;
        unsigned MS1_A		:  1;
        unsigned MS2_A		:  1;
        unsigned MS3_A		:  1;
	unsigned T5_EMITTER	:  1;
	unsigned T6_EMITTER	:  1;
    } B;

} motor_ctrl_t;


// motor control bit flags
// X motor 
#define BF_RESET_X	(1<<31)
#define BF_ENABLE_X	(1<<30)
#define BF_SLEEP_X	(1<<29)
#define BF_MS1_X	(1<<28)
#define BF_MS2_X	(1<<27)
#define BF_MS3_X	(1<<26)
#define MOTOR_X		26

// Y motor 
#define BF_RESET_Y	(1<<25)
#define BF_ENABLE_Y	(1<<24)
#define BF_SLEEP_Y	(1<<23)
#define BF_MS1_Y	(1<<22)
#define BF_MS2_Y	(1<<21)
#define BF_MS3_Y	(1<<20)
#define MOTOR_Y		20

// Z motor 
#define BF_RESET_Z	(1<<19)
#define BF_ENABLE_Z	(1<<18)
#define BF_SLEEP_Z	(1<<17)
#define BF_MS1_Z	(1<<16)
#define BF_MS2_Z	(1<<15)
#define BF_MS3_Z	(1<<14)
#define MOTOR_Z		14

// Extruder B 
#define BF_RESET_B	(1<<13)
#define BF_ENABLE_B	(1<<12)
#define BF_SLEEP_B	(1<<11)
#define BF_MS1_B	(1<<10)
#define BF_MS2_B	(1<<9)
#define BF_MS3_B	(1<<8)
#define MOTOR_B		8

// Extruder A 
#define BF_RESET_A	(1<<7)
#define BF_ENABLE_A	(1<<6)
#define BF_SLEEP_A	(1<<5)
#define BF_MS1_A	(1<<4)
#define BF_MS2_A	(1<<3)
#define BF_MS3_A	(1<<2)
#define MOTOR_A		2

// Emitters
#define BF_T5_EMITTER	(1<<1)
#define BF_T6_EMITTER	(1<<0)

// some helpful defs
#define RESET_BITS	(BF_RESET_X | BF_RESET_Y | BF_RESET_Z | BF_RESET_A | BF_RESET_B)
#define SLEEP_BITS	(BF_SLEEP_X | BF_SLEEP_Y | BF_SLEEP_Z | BF_SLEEP_A | BF_SLEEP_B)
#define ENABLE_BITS	(BF_ENABLE_X | BF_ENABLE_Y | BF_ENABLE_Z | BF_ENABLE_A | BF_ENABLE_B)
 
typedef union
{
    reg16_t  U;
    struct
	{
	unsigned EN_H1		:  1;
	unsigned EN_H2		:  1;
	unsigned EN_H3		:  1;
	unsigned EN_12V		:  1;
	unsigned LCD_IM1	:  1;
	unsigned LCD_IM2	:  1;
	unsigned LCD_IM3	:  1;
	unsigned ENET_RESET	:  1;
	unsigned FAN_PWR1	:  1;
	unsigned FAN_PWR2	:  1;
	unsigned FAN_PWR3	:  1;
	unsigned FAN_PWR4	:  1;
	unsigned FAN_PWR5	:  1;
	unsigned FAN_PWR6	:  1;
	unsigned SLEEP_M	:  1;
	unsigned MODE_M		:  1;
    } B;

} heat_ctrl_t;


// heaters and fan pwr bit flags
#define BF_EN_H1	(1<<15)
#define BF_EN_H2	(1<<14)
#define BF_EN_H3	(1<<13)
#define BF_EN_12V	(1<<12)
#define BF_LCD_IM1	(1<<11)
#define BF_LCD_IM2	(1<<10)
#define BF_LCD_IM3	(1<<9)
#define BF_ENET_RESET	(1<<8)
#define BF_FAN_PWR1	(1<<7)
#define BF_FAN_PWR2	(1<<6)
#define BF_FAN_PWR3	(1<<5)
#define BF_FAN_PWR4	(1<<4)
#define BF_FAN_PWR5	(1<<3)
#define BF_FAN_PWR6	(1<<2)
#define BF_SLEEP_M	(1<<1)
#define BF_MODE_M	(1<<0)

#define HW_MOTOR_CTRL_CLR(reg,v)	(reg &= (~v))
#define HW_MOTOR_CTRL_SET(reg,v)		(reg |= v)  

#define HW_HEAT_CTRL_CLR(reg,v)	(reg &= (~v))
#define HW_HEAT_CTRL_SET(reg,v)	(reg |= v)

#define MOTOR_CLR(reg,motor,v)  (reg &= (~(v << motor)))
#define MOTOR_SET(reg,motor,v)	(reg |= (v << motor))

#define MOTOR_X_CLR(reg,v)	MOTOR_CLR(reg,MOTOR_X,(v & 0x3f))
#define MOTOR_X_SET(reg,v)	MOTOR_SET(reg,MOTOR_X,(v & 0x3f))

#define MOTOR_Y_CLR(reg,v)	MOTOR_CLR(reg,MOTOR_Y,(v & 0x3f))
#define MOTOR_Y_SET(reg,v)	MOTOR_SET(reg,MOTOR_Y,(v & 0x3f))

#define MOTOR_Z_CLR(reg,v)	MOTOR_CLR(reg,MOTOR_Z,(v & 0x3f))
#define MOTOR_Z_SET(reg,v)	MOTOR_SET(reg,MOTOR_Z,(v & 0x3f))

#define MOTOR_A_CLR(reg,v)	MOTOR_CLR(reg,MOTOR_A,(v & 0x3f))
#define MOTOR_A_SET(reg,v)	MOTOR_SET(reg,MOTOR_A,(v & 0x3f))

#define MOTOR_B_CLR(reg,v)	MOTOR_CLR(reg,MOTOR_B,(v & 0x3f))
#define MOTOR_B_SET(reg,v)	MOTOR_SET(reg,MOTOR_B,(v & 0x3f))

#define A4988_CFG_CLR(reg,v)	(reg &= ((~v)&A4988_CLEAR))
#define A4988_CFG_SET(reg,v)	(reg |= ((v)&A4988_CLEAR))

#define A4988_RESET(reg,v)	(reg &= (~v))
#define A4988_RUN(reg,v)	(reg |= v)
#define A4988_ENABLE(reg,v)	(reg &= (~v))
#define A4988_DISABLE(reg,v)	(reg |= v)
#define A4988_SLEEP(reg,v)	(reg &= (~v))
#define A4988_WAKEUP(reg,v)	(reg |= v)



  
#endif //INCLUDE_STEPPER_H
