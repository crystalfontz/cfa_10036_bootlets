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

#include "SSD1307.h"
#include "i2c.h"
#include "data.h"

// This file provides an api for accessing the SSD1307 controller
// on the CFAL9616A

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
    i2c_init(400000); 
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
 printf("Command index = %X\r\n",cmd_index);
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

