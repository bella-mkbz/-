#include<reg51.h>
#include "delay.h"
#include "button.h"

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit KEY0 = P2^2;
sbit KEY1 = P2^3;

/***********定义全局变量**************/
uint8_t code Duan[]={0xff, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};

void main(void)
{
  static uint8_t temp;
	uint8_t i=0,j=0;
	ADDR3 = 1;
	ENLED = 0;
	KEY0 = 0;
	KEY1 = 0;
	while(1)
	{
		 switch (i)
				{
					case 0: ADDR2=0; ADDR1=0; ADDR0=0; i++; P0=Duan[0];      Delay1ms(1); break;
					case 1: ADDR2=0; ADDR1=0; ADDR0=1; i++; P0=Duan[0];      Delay1ms(1); break;
					case 2: ADDR2=0; ADDR1=1; ADDR0=0; i++; P0=Duan[0];      Delay1ms(1); break;
					case 3: ADDR2=0; ADDR1=1; ADDR0=1; i++; P0=Duan[j/100];  Delay1ms(1); break;
					case 4: 
					ADDR2=1; ADDR1=0; ADDR0=0; i++; 
					if((j%100/10 == 0) && (j > 9))
					   P0=0xc0;
          else	
					   P0=Duan[j%100/10];   
					Delay1ms(1); break;
					case 5: 
					ADDR2=1; ADDR1=0; ADDR0=1; i=0; 
					if((j%10 == 0) && (j != 0))
					   P0=0xc0;
          else					
					   P0=Duan[j%10];       
					Delay1ms(1); break;
					
					default: break;
				}
	  temp = Button_Value();
		switch (temp)
    {
    	case 1:j++;break;
    	case 2:j--;break;
    	default:break;
    }
	}
}