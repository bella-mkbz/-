#include<reg51.h>
#include<stdio.h>
#include "iic.h"
#include "lcd.h"
#include "button.h"
#include "uart.h"

extern uchar ChipAdress;                         //声明外部IIC器件地址

void main()
{
	 uchar uVal[4]={0x2,251,0x6,127},tVal[4]={0};
	 uchar b,Key_Value;
   Uart_Init();
	 LcdInit();	
	 Show_String(0x80,"IIC_Text");
	 ChipAdress = 0xA0;
	 E2Write(uVal, 0x00,2);
   while(1)
	 {
		   ChipAdress = 0x90;
		   E2Read(&b, 0x40, 1);
		   Show_Voltage(0xc4,b);
		   Lcd1602_Delay1ms(5);
			 ChipAdress = 0xA0;
			 E2Read(tVal, 0x00, 2);
			 TI=1;
			 printf("%d %d \n",(uint)tVal[0],(uint)tVal[1]);
			 while(!TI);
			 TI=0;
		   Key_Value = Button_Value();
		   switch (Key_Value)
       {
       	case 1: uVal[0]++;E2Write(uVal, 0x00,2);break;
       	case 2: uVal[1]--;E2Write(uVal, 0x00,2);break;
       	default:break;
       }
	 }
}


