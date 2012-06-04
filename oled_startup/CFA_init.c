
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
#include "registers/regsdigctl.h"
#include "registers/regspower.h"
#include "registers/regsgpmi.h"
#include "registers/regsclkctrl.h"
#include "registers/regspinctrl.h"
#include "registers/regshsadc.h"
#include "registers/regsapbh.h"
#include "registers/regspwm.h"
#include "debug.h"
#include "mxs_gpio.h"
#include "data.h"
#include "CFA_init.h"

#define DDI_NAND_HAL_GPMI_SOFT_RESET_LATENCY 3

void HSADC_fixup(void) //see errata ENGR116296
{
 HW_HSADC_CTRL0_CLR(BM_HSADC_CTRL0_SFTRST);
 HW_HSADC_CTRL0_WR((HW_HSADC_CTRL0_RD() |
		BM_HSADC_CTRL0_SFTRST) & (~BM_HSADC_CTRL0_CLKGATE));
 HW_HSADC_CTRL0_SET(BM_HSADC_CTRL0_CLKGATE);
 HW_HSADC_CTRL0_CLR(BM_HSADC_CTRL0_CLKGATE);
 HW_HSADC_CTRL0_SET(BM_HSADC_CTRL0_CLKGATE);
// now perform a soft reset 
 HW_HSADC_CTRL0_CLR(BM_HSADC_CTRL0_SFTRST);
// Wait at least a microsecond for SFTRST to deassert. In actuality, we
// need to wait 3 GPMI clocks, but this is much easier to implement.
udelay(2);
// Also clear CLKGATE so we can wait for its assertion below.
HW_HSADC_CTRL0_CLR(BM_HSADC_CTRL0_CLKGATE);
// Now soft-reset the hardware.
HW_HSADC_CTRL0_SET(BM_HSADC_CTRL0_SFTRST);
// Poll until clock is in the gated state before subsequently
// clearing soft reset and clock gate.
while (!HW_HSADC_CTRL0.B.CLKGATE)
{
; // busy wait
}
// bring PWM_CTRL out of reset
HW_HSADC_CTRL0_CLR(BM_HSADC_CTRL0_SFTRST);
// Wait at least a microsecond for SFTRST to deassert. In actuality, we
// need to wait 3 GPMI clocks, but this is much easier to implement.
udelay(2);
       printf("HW_HSADC_CTRL0 = %X\r\n",HW_HSADC_CTRL0_RD());
if (HW_HSADC_CTRL0.B.SFTRST)
{
   printf("can't clear PWM SFTRST\n");
}
HW_HSADC_CTRL0_CLR(BM_HSADC_CTRL0_CLKGATE);
// Poll until clock is in the NON-gated state before returning.
while (HW_HSADC_CTRL0.B.CLKGATE)
{
; // busy wait
}

 HW_APBH_CHANNEL_CTRL.B.RESET_CHANNEL = 0x1000;

}
void dbg_regs()
{
        printf("\r\nHW_POWER_MINPWR = %X\r\n",HW_POWER_MINPWR_RD());
        printf("HW_PWM_ACTIVEn(4) = %X\r\n",HW_PWM_ACTIVEn_RD(4));
        printf("HW_PWM_PERIODn(4) = %X\r\n",HW_PWM_PERIODn_RD(4));
        printf("HW_PWM_CTRL = %X\r\n",HW_PWM_CTRL_RD());
}

void set_emi_frac(unsigned int div)
{
	HW_CLKCTRL_FRAC0_SET(BM_CLKCTRL_FRAC0_EMIFRAC);
	div = (~div);
	HW_CLKCTRL_FRAC0_CLR(BF_CLKCTRL_FRAC0_EMIFRAC(div));
}
void CPUClock2XTAL(void)
{

	HW_CLKCTRL_CLKSEQ_SET(BM_CLKCTRL_CLKSEQ_BYPASS_CPU);

}
void poweron_pll()
{
	HW_CLKCTRL_PLL0CTRL0_SET(BM_CLKCTRL_PLL0CTRL0_POWER);
}
void init_clock()
{
	HW_CLKCTRL_FRAC0_SET(BM_CLKCTRL_FRAC0_CLKGATEEMI);

	set_emi_frac(21);

	HW_CLKCTRL_FRAC0_CLR(BM_CLKCTRL_FRAC0_CLKGATEEMI);
	udelay(11000);

	HW_CLKCTRL_EMI_WR(BF_CLKCTRL_EMI_DIV_XTAL(1)|
			  BF_CLKCTRL_EMI_DIV_EMI(2)
			 );

	/*choose ref_emi*/
	HW_CLKCTRL_CLKSEQ_CLR(BM_CLKCTRL_CLKSEQ_BYPASS_EMI);

	printf("FRAC 0x%x\r\n" , HW_CLKCTRL_FRAC0_RD());

}

void init_OLED (void)
{
// configure the reset line and put the OLED in the reset state
	mxs_iomux_setup_pad(OLED_RESET |
                        MXS_PAD_12MA | MXS_PAD_3V3 | MXS_PAD_PULLUP);
        gpio_direction_output(OLED_RESET, 0);

// Turn on the PWM to drive the voltage tripler
        HW_CLKCTRL_XTAL_CLR(0x20000000);
	mxs_iomux_setup_pad(OLED_POWER  | MXS_PAD_12MA | MXS_PAD_3V3 );
	BW_PWM_CTRL_PWM4_ENABLE(0);
// we need to setup the PWM channel
// for 10kHz pulses to charge the 
// voltage tripler

//take the PWM circuitry out of reset
  printf("HW_PWM_CTRL before = %X\r\n",HW_PWM_CTRL_RD());
// Prepare for soft-reset by making sure that SFTRST is not currently
// asserted. Also clear CLKGATE so we can wait for its assertion below.
HW_PWM_CTRL_CLR(BM_PWM_CTRL_SFTRST);
// Wait at least a microsecond for SFTRST to deassert. In actuality, we
// need to wait 3 GPMI clocks, but this is much easier to implement.
udelay(2);
// Also clear CLKGATE so we can wait for its assertion below.
HW_PWM_CTRL_CLR(BM_PWM_CTRL_CLKGATE);
// Now soft-reset the hardware.
HW_PWM_CTRL_SET(BM_PWM_CTRL_SFTRST);
// Poll until clock is in the gated state before subsequently
// clearing soft reset and clock gate.
while (!HW_PWM_CTRL.B.CLKGATE)
{
; // busy wait
}
// bring PWM_CTRL out of reset
HW_PWM_CTRL_CLR(BM_PWM_CTRL_SFTRST);
// Wait at least a microsecond for SFTRST to deassert. In actuality, we
// need to wait 3 GPMI clocks, but this is much easier to implement.
udelay(2);
       printf("HW_PWM_CTRL = %X\r\n",HW_PWM_CTRL_RD());
if (HW_PWM_CTRL.B.SFTRST)
{
   printf("can't clear PWM SFTRST\n");
}
HW_PWM_CTRL_CLR(BM_PWM_CTRL_CLKGATE);
// Poll until clock is in the NON-gated state before returning.
while (HW_PWM_CTRL.B.CLKGATE)
{
; // busy wait
}

        printf("HW_PWM_CTRL = %X\r\n",HW_PWM_CTRL_RD());


   	HW_PWM_ACTIVEn_SET(4,0x00250000); // duty cycle 1200 ticks = 50%
   	HW_PWM_PERIODn_SET(4,0x000b004a); //2400-1 clock ticks in period
	BW_PWM_CTRL_PWM4_ENABLE(1);
        udelay(1000);
dbg_regs();
// Bring the ssd1307 out of reset
   gpio_toggle_value(OLED_RESET);
}

void cfa_init( void ) {
CPUClock2XTAL();
poweron_pll();
init_clock();
/* clear the current debug lines */
	HW_PINCTRL_MUXSEL7_CLR(0xF);

/* re-assign DUART to 10036 external pins */
	mxs_iomux_setup_pad(DUART_RX);
 	mxs_iomux_setup_pad(DUART_TX);


/*turn on the power LED */
         printf("\r\nturning on Power\r\n");
  	mxs_iomux_setup_pad(POWER_LED |
              MXS_PAD_12MA | MXS_PAD_3V3);
        gpio_direction_output(POWER_LED, 0);

init_OLED(); 


      
        printf("HW_POWER_MINPWR = %X\r\n",HW_POWER_MINPWR_RD());

        printf("\r\nACTIVE = %X\r\n",HW_PWM_ACTIVEn_RD(4));
        printf("PERIOD = %X\r\n",HW_PWM_PERIODn_RD(4));
        printf("PWM_CTRL = %X\r\n",HW_PWM_CTRL_RD());
  

// Set up the I2C1 channel SLC and SDA
	mxs_iomux_setup_pad(OLED_SCL |
                        MXS_PAD_12MA | MXS_PAD_3V3);

	mxs_iomux_setup_pad(OLED_SDA |
                        MXS_PAD_12MA | MXS_PAD_3V3);

        printf("leaving cfa_init\r\n");
}

void toggle_PWR_LED(void) {
      gpio_toggle_value(POWER_LED);
}

