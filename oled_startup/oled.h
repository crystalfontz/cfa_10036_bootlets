#ifndef INCLUDE_OLED_H
#define INCLUDE_OLED_H

extern void I2C_send(uint8_t command, uint8_t * buff, int length);
extern int Init_SSD1307(void);
extern void initPageMode(void);
extern void initVerticalMode(unsigned char StartPage, unsigned char EndPage, unsigned char StartColumn, unsigned char EndColumn);
extern void setXY(unsigned char page,unsigned char column);
extern void sendStr(unsigned char *string);
extern void setHorizontalScroll(unsigned char Direction);
extern void vmode_clear(int start, int end);
extern void P_ClearPages(int start, int finish) ;
extern void print_vmode_logo(int column);
extern void DemoScrollingUsingOffset(void);
#endif //INCLUDE_OLED_H
