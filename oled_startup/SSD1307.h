/*
* Copyright (C) 2011 Jim Wall Abstract Designs
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
*
*
*
*
*
* This is a quick and dirty macro library which provides an API for communicating 
* with the SSD1307 graphics controller over I2C.
*/ 

#ifndef SSD1307_MACROS_H
#define SSD1307_MACROS_H 

#define SSD1307_I2C_ADDRESS           	0x3c

#define SSD1307_COMMAND			0x80
#define SSD1307_DATA			0x40

#define SSD1307_CONTRAST                0x81
#define SSD1307_ENTIRE_DISPLAY_OFF	0xa4
#define SSD1307_ENTIRE_DISPLAY_ON	0xa5
#define SSD1307_INVERSE_OFF             0xa6
#define SSD1307_INVERSE_ON              0xa7
#define SSD1307_DISPLAY_OFF             0xae
#define SSD1307_DISPLAY_ON              0xaf
#define SSD1307_HSCROLL_RIGHT		0x26
#define SSD1307_HSCROLL_LEFT		0x27
#define SSD1307_VSCROLL_RIGHT		0x26
#define SSD1307_VSCROLL_LEFT		0x2a
#define SSD1307_SCROLL_OFF              0x2e
#define SSD1307_SCROLL_ON		0x2f
#define SSD1307_VSCROLL_AREA		0xa3
/*
* the column start address commands are defined
* elswhere as they are not a single command
* their valuse range from 0x00-0x0f and 0x10-0x17
*/
#define SSD1307_ADDRESSING_MODE         0x20
#define SSD1307_COLUMN_ADDRESS		0x21
#define SSD1307_PAGE_ADDRESS		0x22

/* START_PAGE_ADDRESS rnges from 0xb0 to 0xb4 */
/* we use 0xb0 as the base and then add an offset */
#define SSD1307_START_PAGE_ADDRESS	0xb0

/* DISPLAY_START_LINE ranges from 0x40 to 0x66 */
/* we use 0xb0 as the base and then add an offset */
#define SSD1307_DISPLAY_START_LINE	0x40
#define SSD1307_SEG_REMAP_OFF           0xa0
#define SSD1307_SEG_REMAP_ON            0xa1
#define SSD1307_MUX_RATIO		0xa8
#define SSD1307_COM_SCAN_NORMAL		0xc0
#define SSD1307_COM_SCAN_REMAPPED	0xc8
#define SSD1307_DISPLAY_OFFSET		0xd3
#define SSD1307_SEG_PINS		0xda

#define SSD1307_NO_OP                   0xe3

/* the following modes are for setting the addressing modes*/
#define SSD1307_PAGE_MODE               0x02
#define SSD1307_HORIZONTAL_MODE         0x00
#define SSD1307_VERTICAL_MODE           0x01

#define SSD1307_SCROLL_RIGHT		0x26
#define SSD1307_SCROLL_LEFT		0x27

/* macros for handling the command buffer.  Before using any of the command macros,
* the buffer should be initialized which will put 0xb0 in every even byte and NO_OP
* in every odd byte, and the index should be reset.
* to get the number of bytes stored in the buffer for transmitting use DataLength.
*/
#define SSD1307_ResetIndex(index) (index=0);
//#define SSD1307_DataLength(index) (index)>0?(index)-1:0;
#define SSD1307_InitBuffer(buffer,length) {int i;for(i=0;i<(length);i++) { buffer[i]=0x80; buffer[i+1]=SSD1307_NO_OP;}}

/*
* the following macros are used to load commands into a command buffer 
* all commands including the parameters for a multi-byte command must  
* be preceeded by a 0xb0 byte.  The command buffer therefore will have 
* a 0xb0 in every even entry in the buffer starting with buffer[0].    
* it is up to the user to ensure that all indexes are odd values.      
* The macros for multi-byte commands store the bytes in every other byte position 
* the index is incremented to the next available command position in the buffer
* and therefore can be used on subsequent command macros without incrementing
*/

// Macro for setting the contrast control. value range 0x00-0xff (RESET = 0x7f)
#define SSD1307_Contrast(buffer,index,value) buffer[index++] = SSD1307_CONTRAST; buffer[index++] = value;
// Resume to RAM content display Output follows RAM content (RESET) 
#define SSD1307_EntireDisplayOff(buffer,index) buffer[index++] = SSD1307_ENTIRE_DISPLAY_OFF;

// Entire display ON
#define SSD1307_EntireDisplayOn(buffer,index) buffer[index++] = SSD1307_ENTIRE_DISPLAY_ON; 

// Turn off the Display    (RESET)
#define SSD1307_DisplayOff(buffer,index) buffer[index++] = SSD1307_DISPLAY_OFF;  

// Turn on the Display 
#define SSD1307_DisplayOn(buffer,index) buffer[index++] = SSD1307_DISPLAY_ON;

// Normal display (RESET)
#define SSD1307_InverseOff(buffer,index) buffer[index++] = SSD1307_INVERSE_OFF;

// Inverse display
#define SSD1307_InverseOn(buffer,index) buffer[index++] = SSD1307_INVERSE_ON;

// Stop scrolling that is configured by command 26h/27h/29h/2Ah.
#define SSD1307_ScrollOff(buffer,index) buffer[index++] = SSD1307_SCROLL_OFF;

// Start scrolling that is configured by the scrolling setup commands :26h/27h/29h/2Ah
#define SSD1307_ScrollOn(buffer,index) buffer[index++] = SSD1307_SCROLL_ON;

// Set Segment Remap on. Column address 127 is mapped to SEG0
#define SSD1307_SEG_RemapOn(buffer,index) buffer[index++] = SSD1307_SEG_REMAP_ON;

// Set Segment Remap off. Column address 0 is mapped to SEG0 (RESET)
#define SSD1307_SEG_RemapOff(buffer,index) buffer[index++] = SSD1307_SEG_REMAP_OFF;

//Set Memory Addressing Mode Horizontal, vertical or page (RESET)
#define SSD1307_AddressMode(buffer,index,mode) buffer[index++] = SSD1307_ADDRESSING_MODE; buffer[index++] = mode;

// Setup column start and end address (for horizontal and vertical modes only)
#define SSD1307_ColumnAddress(buffer,index,start,finish) buffer[index++] = SSD1307_COLUMN_ADDRESS; buffer[index++] = start; buffer[index++] = finish; 

// Setup page start and end address (for horizontal and vertical modes only)
#define SSD1307_PageAddress(buffer,index,start,finish) buffer[index++] = SSD1307_PAGE_ADDRESS; buffer[index++] = start; buffer[index++] = finish;

// Set Page Start Address for Page Addressing Mode
#define SSD1307_StartPageAddr(buffer,index,page) buffer[index++] = SSD1307_START_PAGE_ADDRESS|(page&0x07);

//Set Display Start Line (Set display RAM display offset from 0d-38d)  (RESET 0d)
#define SSD1307_DisplayStartLine(buffer,index,offset) buffer[index++] = SSD1307_DISPLAY_START_LINE|(offset&0x3f); 

// Set Multiplex Ratio. See SSD1307 documentation for use.
#define SSD1307_MUXRatio(buffer,index,ratio) buffer[index++] = SSD1307_MUX_RATIO; buffer[index++] = ratio; 

// Normal COM scan mode
#define SSD1307_COM_ScanNormal(buffer,index) buffer[index++] = SSD1307_COM_SCAN_NORMAL; 

// Remapped COM scan mode
#define SSD1307_COM_ScanRemap(buffer,index) buffer[index++] = SSD1307_COM_SCAN_REMAPPED;

// Set Display Offset. Set vertical shift by COM from 0d~38d. The value is reset to 00h after RESET.
#define SSD1307_DisplayOffset(buffer,index,off) buffer[index++] = SSD1307_DISPLAY_OFFSET; buffer[index++] = off;

// Set SEG Pins Hardware Configuration. See SSD1307 documentation for use.
#define SSD1307_SEGpins(buffer,index,config) buffer[index++] = SSD1307_SEG_PINS; buffer[index++]=config;

// Set the lower nibble of the column start address register for Page Addressing Mode
#define SSD1307_StartColumnAddress(buffer,index,value) buffer[index++] = (value&0x0f); buffer[index++] = (((value)>>4)|0x10)&0x17;

// set the horizontal scroll parameters
#define SSD1307_HorizontalScroll(buffer,index,Direction,start,time,finish) buffer[index++] = Direction; buffer[index++] = 0x00; buffer[index++] = start; \
buffer[index++] = time; buffer[index++] = finish; buffer[index++] = 0x00; buffer[index++] = 0xff;  

#endif //SSD1307_MACROS_H
