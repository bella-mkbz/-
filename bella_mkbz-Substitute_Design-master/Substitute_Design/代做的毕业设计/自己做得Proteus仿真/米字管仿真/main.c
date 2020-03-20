#include<reg51.h>
#include "delay.h"

uint8_t code D_1[]={0x76,0xc7,0xc0,0xc1,0xc6,0xbf,0xc0,0xc1};         
uint8_t code W[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};          
uint8_t code D_2[]={0XF7,0XFf,0xFf,0xFf,0xdd,0xf6,0xff,0xff};         

void GPIO_Init(void);

void main()
{
   uint8_t  m=0,n=0;
   uint32_t i=0,j=0;
   GPIO_Init();
  while(1)
  {
	  for(j=0;j<250;j++)
	  {
	      for(i=0;i<750;i++)
		   {

			  P3=	W[n];
		   	P0=	D_1[(n+m)%8];
			  P2=D_2[(n+m)%8];
			  Delay1ms(1);
			  n++;
		    if(n == 8)
		       n=0;
		   }
		   m++;
		   if(m == 8)
		   m=0;
	  }
  }
}

void GPIO_Init(void)
{
   P0=0XFF;
   P2=0XFF;
   P3=0X00;
}

