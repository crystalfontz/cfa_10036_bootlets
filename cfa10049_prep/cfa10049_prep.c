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
 * This bootlet is used to configure the CrystalFontz CFA-10036
 * board to a known configuration when plugged into a CFA-10049.
 * This bootlet should be run as the first bootlet in the boot
 * sequence.
 */

////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "error.h"


#include "registers/regsdigctl.h"
#include "registers/regsuartdbg.h"
#include "registers/regspinctrl.h"
#include "debug.h"
#include "lss.h"
#include "cfa10049.h"


#define FORWARD		0x01
#define REVERSE		0x00

void udelay(unsigned int microSec)
{
	unsigned int currentTime = HW_DIGCTL_MICROSECONDS_RD();
	while ((HW_DIGCTL_MICROSECONDS_RD() - currentTime) <  microSec);
}

void init_STEPPERS(void) {
       // set the stepper STEP and DIR pins to GPIO output 4ma 3.3v
       HW_PINCTRL_MUXSEL0_SET(MUX0_PINS);
       HW_PINCTRL_MUXSEL1_SET(MUX1_PINS); 
       HW_PINCTRL_DRIVE0_CLR(DRIVE0_MA_PINS);
       HW_PINCTRL_DRIVE2_CLR(DRIVE2_MA_PINS);
       HW_PINCTRL_DRIVE3_CLR(DRIVE3_MA_PINS);
       HW_PINCTRL_DRIVE0_SET(DRIVE0_V_PINS);
       HW_PINCTRL_DRIVE2_SET(DRIVE2_V_PINS);
       HW_PINCTRL_DRIVE3_SET(DRIVE3_V_PINS);
       HW_PINCTRL_DOUT0_CLR(STEP_PINS | DIR_PINS);
       HW_PINCTRL_DOE3_SET(STEP_PINS | DIR_PINS);
}

void init_DUART(void) {

        /* Crystal fontz code to turn on the LED on GPIO3_4 */
        /* and reroute DUART to GPOI3_2 and GPIO3_3         */
        HW_PINCTRL_MUXSEL6_CLR(DUARTRX_CLR|DUARTTX_CLR);
        HW_PINCTRL_MUXSEL6_SET(DUARTRX_MUX|DUARTTX_MUX|PWRLED_MUX);
}

void init_PWR_LED(void) {

//Enable pin GPIO3_4 LED is active low 
        HW_PINCTRL_MUXSEL6_SET(PWRLED_MUX);
        HW_PINCTRL_DRIVE12_CLR(PWRLED_MA);
        HW_PINCTRL_DRIVE12_SET(PWRLED_V);
        HW_PINCTRL_DOUT3_CLR(PWRLED_PIN);
        HW_PINCTRL_DOE3_SET(PWRLED_PIN);
}
void init_SPI_PINS(void) {

// select the pins we need as GPIO
        HW_PINCTRL_MUXSEL1_SET(MOSI_MUX | SCK_MUX | RCK32_MUX | RCK16_MUX);
// set the drive current
        HW_PINCTRL_DRIVE3_CLR(MOSI_MA | SCK_MA | RCK32_MA);
        HW_PINCTRL_DRIVE2_CLR(RCK16_MA);
//set the drive voltage
        HW_PINCTRL_DRIVE3_SET(MOSI_V | SCK_V | RCK32_V);
        HW_PINCTRL_DRIVE2_SET(RCK16_V);
       
// set the pins as outputs and set the initial values to zero
        HW_PINCTRL_DOUT0_CLR(MOSI_PIN|SCK_PIN|RCK32_PIN|RCK16_PIN);
// enable the pins
        HW_PINCTRL_DOE0_SET(MOSI_PIN|SCK_PIN|RCK32_PIN|RCK16_PIN);
}

void set_register(uint32_t write_pin ,uint32_t value ) {

int i;
int length;
 
       if(write_pin == RCK32_PIN){
          length = 32;
       }
       else {
          length = 16;
       }
       printf("buffer= %X\r\n",value);
       for(i=0;i<length;i++) {
           if((value >> i) & 0x00000001){ 
              HW_PINCTRL_DOUT0_SET(MOSI_PIN);
 //             printf("\n\rset");
           }
           else
           {
              HW_PINCTRL_DOUT0_CLR(MOSI_PIN);
//              printf("\n\rreset");
           }
// the 74HC595 has a max transition delay of 1000 ns.

           HW_PINCTRL_DOUT0_SET(SCK_PIN);
           udelay(1);
           HW_PINCTRL_DOUT0_CLR(SCK_PIN);
           udelay(1);
	}
// clock the data to the outputs
           
           HW_PINCTRL_DOUT0_SET(write_pin);
// the 74HC595 has a max transition delay of 1000 ns.
           udelay(1);
           HW_PINCTRL_DOUT0_CLR(write_pin);
          
}



void test_heater_reg(void) {

uint16_t heaters;

//zero out the heater register  
  HW_HEAT_CTRL_CLR(heaters,(0xffff));
  set_register(RCK16_PIN,heaters);
  HW_HEAT_CTRL_SET(heaters,(BF_EN_12V));
  set_register(RCK16_PIN,heaters);
  udelay(1000000);
  HW_HEAT_CTRL_SET(heaters,(BF_EN_H1| BF_FAN_PWR6));
  set_register(RCK16_PIN,heaters);
  udelay(1000000);
  HW_HEAT_CTRL_SET(heaters,(BF_EN_H2));
  set_register(RCK16_PIN,heaters);
  udelay(1000000);
  HW_HEAT_CTRL_SET(heaters,(BF_EN_H3));
  set_register(RCK16_PIN,heaters);
  udelay(1000000);
  HW_HEAT_CTRL_CLR(heaters,(BF_EN_H1));
  set_register(RCK16_PIN,heaters);
  udelay(1000000);
  HW_HEAT_CTRL_CLR(heaters,(BF_EN_H2));
  set_register(RCK16_PIN,heaters);
  udelay(1000000);
  HW_HEAT_CTRL_CLR(heaters,(BF_EN_H3| BF_FAN_PWR6));
  set_register(RCK16_PIN,heaters);
  udelay(1000000);
  HW_HEAT_CTRL_SET(heaters,(BF_EN_H1 | BF_EN_H2 | BF_EN_H3 | BF_FAN_PWR3));            
  set_register(RCK16_PIN,(uint32_t)heaters);
  udelay(2000000);
  HW_HEAT_CTRL_CLR(heaters,(0xffff));
  set_register(RCK16_PIN,heaters);
}

void set_direction(uint32_t motors , uint8_t direction) {
     
      if(direction == FORWARD)
         HW_PINCTRL_DOUT0_SET(motors);
      else
	 HW_PINCTRL_DOUT0_CLR(motors);
} 
     

void step(uint32_t motors) {
// pretty simple, just output the value for a couple of microseconds
    HW_PINCTRL_DOUT0_SET(motors);
    udelay(2);
    HW_PINCTRL_DOUT0_CLR(motors);
}



void stepper_test(uint32_t motors,int steps) {

static uint8_t direction = FORWARD;
int i;
       printf("motors = %X\r\n",motors);
       set_direction((DIR_A |DIR_B | DIR_X | DIR_Y | DIR_Z),direction);
       direction ^= 1;
       for(i = 0; i < steps; i++) {
           udelay(1000);
           step(motors);
       }
}

void setup_stepper_A() {
 
motor_ctrl_t motor_port;  
       
      MOTOR_A_CLR(motor_port.U,A4988_CLEAR);
      MOTOR_A_SET(motor_port.U,(BF_N_RESET | BF_N_SLEEP | STEP_16));
	set_register(RCK32_PIN,motor_port.U);
}

/*
void set_direction(uint32_t motors , uint8_t direction) {
     
      if(direction == FORWARD)
         HW_PINCTRL_DOUT0_SET(motors);
      else
	 HW_PINCTRL_DOUT0_CLR(motors);
} 
*/
/*     
void step(uint32_t motors) {
// pretty simple, just output the value for a couple of microseconds
    HW_PINCTRL_DOUT0_SET(motors);
    udelay(2);
    HW_PINCTRL_DOUT0_CLR(motors);
}
*/





//
////////////////////////////////////////////////////////////////////////////////
//! \brief  Sets up the  CFA-10036 to control the CFA-10049
//
//! \return SUCCESS(0) Power block ready.
////////////////////////////////////////////////////////////////////////////////

int _start( void )
{


init_DUART();
init_PWR_LED();
init_SPI_PINS();
init_STEPPERS();
setup_stepper_A();

set_register(RCK16_PIN,BF_EN_12V);
/*
while(1) {
//test_heater_reg();
//stepper_test(STEP_A,1600);
udelay(2000000);
}
*/
        
       printf("we should be up and running!");
 
return SUCCESS;
}

void putc(char ch)
{
	int loop = 0;
	while (HW_UARTDBGFR_RD()&BM_UARTDBGFR_TXFF) {
		loop++;
		if (loop > 10000)
			break;
	};

	/* if(!(HW_UARTDBGFR_RD() &BM_UARTDBGFR_TXFF)) */
	HW_UARTDBGDR_WR(ch);
}

/* workaround gcc 4.3 link error*/
void __aeabi_unwind_cpp_pr1() {}

