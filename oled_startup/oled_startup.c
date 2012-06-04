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
#include "types.h"
#include "error.h"
//#include "io.h"
#include "debug.h"
#include "utils.h"
#include "CFA_init.h"
#include "registers/regspinctrl.h"
#include "arch-mx28/sys_proto.h"

#include "SSD1307.h"
#include "i2c.h"
#include "oled.h"

unsigned char Crystalfontz[] = "Crystalfontz";
unsigned char CFA_10036[] =    "CFA-10036   ";
unsigned char Battery[] =      "NO Battery! ";
unsigned char Memory[] =       "128 Meg Mem ";
unsigned char Spaces[] =       "            ";

int _start( void )

{
int i;
int iRtn = SUCCESS;
//initialize the hardware pins. DUART Power etc.
   cfa_init();
printf("we have initialized the device\r\n");

Init_SSD1307();
initPageMode();
P_ClearPages(0,4);



send_pmode_logo(1,0);
send_pmode_logo(1,24);
send_pmode_logo(1,48);
send_pmode_logo(1,72);
udelay(2000000);
P_ClearPages(1,2);
sendStr(Crystalfontz);

setXY(2,0); 
sendStr(CFA_10036);
udelay(2000000);

P_ClearPages(1,2);
setXY(1,0); 
sendStr(Memory);
setXY(2,0); 
sendStr(Battery);
udelay(2000000);
P_ClearPages(1,2);
setXY(1,0);
sendStr("  Starting  ");
setXY(2,0);
sendStr("   Linux    ");
 for(i=0;i<10;i++){	
	toggle_PWR_LED();
       	udelay(100000);
	}
return (iRtn);
}

//int phony (void) {
// BUG
// it seems that the _start function
// cannot reside in a file by itself!!
//}
/* workaround gcc 4.3 link error*/
void __aeabi_unwind_cpp_pr1() {}
// eof cfa_startup.c
//! @}

