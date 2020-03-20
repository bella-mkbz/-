#include<reg51.h>
#include "delay.h"

sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
/***********定义全局变量**************/
uint8_t code Duan[]={0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};

void main(void)
{
  uint8_t  m=0,n=0;
  uint32_t i=0,j=0;
	ADDR3 = 1;
	ENLED = 0;
	while(1)
	{	 	 
	  for(i=0;i<6;i++)
		 {
		   P0=0XFF;
			 P1 &= 0XF8;
			 P1 |= i;
			 P0 = Duan[i];
			 Delay;
		 }
	}
}