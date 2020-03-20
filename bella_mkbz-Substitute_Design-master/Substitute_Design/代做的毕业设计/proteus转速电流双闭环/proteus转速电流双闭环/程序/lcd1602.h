#ifndef __lcd
#define __lcd

#include"reg52.h"

  sbit rs=P2^2;
  sbit rw=P2^3;
  sbit en=P2^4;       //LCD¿ØÖÆÒý½Å
  
  void lcd_display_L(unsigned char *a);
  void lcd_display_H(unsigned char *b);
  void delay10ms(unsigned char i);
  void writecom(unsigned char a);
  void writedat(unsigned char a);
  void lcdinit();

#endif


