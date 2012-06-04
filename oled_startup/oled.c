#include "SSD1307.h"
#include "i2c.h"
#include "data.h"
#include "debug.h"


// rework this function once we findout what it takes to check the PIO buffer
void I2C_send(uint8_t command, uint8_t * buff, int length)
{
  int i,packets,remainder;

  // every command byte must be preceeded with an  SSD1307_COMMAND
//  if(command == SSD1307_COMMAND) {
  	for(i = 0; i < length;i++) {
           i2c_write(SSD1307_I2C_ADDRESS,command,1, &buff[i],1);
	}
//  }
/*
  else {
       // in PIO mode we can send a maximum of 32 bytes before the buffer is full
       // the address and data command take up 2 bytes leaving 30 send 24 to be on the safe side
       packets = length/24;
       remainder = length%24;
       if(packets > 0) 
       {       
    	  for(i = 0; i < packets; i++) {
            i2c_write(SSD1307_I2C_ADDRESS,command,1, &buff[i*24],24);
          } 
       }
       if(remainder) 
       {
             printf("remainder = %X\r\n",remainder);
             i2c_write(SSD1307_I2C_ADDRESS,command,1, &buff[packets*24],remainder);
       }
   }
*/
}


int Init_SSD1307(void)
{
    i2c_init(100000); 
}

//////////////////////////////////////////////////////////
// Function to initialize the display to page mode
void initPageMode(void)
{
    
    int cmd_index = 0;
    unsigned char cmd_buffer[40];
 // Initialize the comand buffer 
//  SSD1307_ResetIndex(cmd_index);
//  SSD1307_InitBuffer(cmd_buffer,40);
  // Set the display to default settings with the exception
  // of the SEGReg which we want remapped to 127
 
  SSD1307_DisplayOff(cmd_buffer,cmd_index);
  SSD1307_ScrollOff(cmd_buffer,cmd_index);
  SSD1307_SEG_RemapOn(cmd_buffer,cmd_index);
  SSD1307_InverseOff(cmd_buffer,cmd_index);
  SSD1307_AddressMode(cmd_buffer,cmd_index,SSD1307_PAGE_MODE);
  SSD1307_StartPageAddr(cmd_buffer,cmd_index,0);
  SSD1307_DisplayOffset(cmd_buffer,cmd_index,0);
  SSD1307_COM_ScanNormal(cmd_buffer,cmd_index);
  SSD1307_DisplayOn(cmd_buffer,cmd_index);
  SSD1307_Contrast(cmd_buffer,cmd_index,255);
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
  SSD1307_DisplayStartLine(cmd_buffer,cmd_index,8); 
  SSD1307_COM_ScanNormal(cmd_buffer,cmd_index);
  SSD1307_ScrollOff(cmd_buffer,cmd_index);
  SSD1307_DisplayOn(cmd_buffer,cmd_index);
  SSD1307_Contrast(cmd_buffer,cmd_index,255);
  //Send the buffer to the I2C bus
  I2C_send(SSD1307_COMMAND,&cmd_buffer[0], cmd_index); 
 
 
}

// set the position in the RAM where the next write will occur
void setXY(unsigned char page,unsigned char column)
{ 
    int i;
    int cmd_index = 0;
    unsigned char cmd_buffer[40];

  SSD1307_StartPageAddr(cmd_buffer,cmd_index,page);
  SSD1307_StartColumnAddress(cmd_buffer,cmd_index,column);
   I2C_send(SSD1307_COMMAND,&cmd_buffer[0], cmd_index); 
 }

void sendStr(unsigned char *string)
{
  int i;
  int length = 0;

  unsigned char * temp;
   if (string)
//   instead of dragging in more headers just find the end of string
   temp = string;
   while(*temp) {length++; temp++;}
   for(i=0;i<length;i++)
     { 
       I2C_send(SSD1307_DATA,&Font[*string-0x20][0],8);    
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
 uint8_t blank[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

 for(i=start;i<(finish+1);i++)
  { 
    setXY(i,0);
    for(j=0;j<16;j++) {
     I2C_send(SSD1307_DATA,blank,8);
  	}

    } 
       
  
  setXY(start,0); // move to the start
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
void send_pmode_logo(int row, int column)
{

  setXY(row,column);

    I2C_send(SSD1307_DATA,&cf_p_logo[0],24); 
    setXY(row+1,column);
    I2C_send(SSD1307_DATA,&cf_p_logo[24],48); 
  
}


void DemoScrollingUsingOffset()
{
    int i,j;
    int cmd_index = 0;
    unsigned char cmd_buffer[40];
 
  // scroll up 1 pixel every 200 ms
  for(i=0;i<32;i++)
  {
    cmd_index =0;
    SSD1307_DisplayOffset(cmd_buffer,cmd_index,i);
    I2C_send(SSD1307_COMMAND,&cmd_buffer[0], cmd_index); 
    udelay(100000);
  }
  udelay(100000);
  //scroll down 1 pixel every 200 ms
  for(i = 0;i<32;i++)
  {
    cmd_index=0;
    SSD1307_DisplayOffset(cmd_buffer,cmd_index,31-i);
    I2C_send(SSD1307_COMMAND,&cmd_buffer[0], cmd_index);
    udelay(100000); 
  }
  udelay(1000000);
}
