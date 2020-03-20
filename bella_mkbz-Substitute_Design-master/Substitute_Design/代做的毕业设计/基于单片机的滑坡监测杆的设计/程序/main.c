#include "lcd.h"
#include "uart.h"
#include<stdio.h>
#include "ADC0832.h"

unsigned char Buff[2]={0};
unsigned char Distance = 0;
unsigned int  Pulse = 0;
bit Flag;
sbit LED = P2^3;

void main()
{
	Lcd1602_Delay1ms(10);
	TMOD &= 0Xf0;
  TMOD |= 0X01;
	TL0 = 0XB0;
	TH0 = 0X3C;
	ET0 = 1;
	TR0 = 1;
	IT0 = 1;
	EX0 = 1;
  LcdInit();
	Uart_Init(); 
 
	while(1)
	{
		if(Pulse >= 200)
			Flag = 1;
		/***************显示和发送位移信息***********/
	  Distance = read0832();
		LcdWriteCom(0X84);
		LcdWriteData(Distance/100+0X30);
		LcdWriteData(Distance%100/10+0X30);
		LcdWriteData(Distance%10+0X30);
	  /********************************************/
		
		/***************显示和发送雨量信息***********/
		LcdWriteCom(0X8a);
		LcdWriteData(Pulse/100+0X30);
		LcdWriteData(Pulse%100/10+0X30);
		LcdWriteData(Pulse%10+0X30);	
		/********************************************/
		
		
		/***************显示和发送经纬度信息*********/
		LcdWriteCom(0XC1);
		LcdWriteData(Buff[0]/100+0X30);
		LcdWriteData(Buff[0]%100/10+0X30);
		LcdWriteData(Buff[0]%10+0X30);
		LcdWriteCom(0XCd);
		LcdWriteData(Buff[1]/100+0X30);
		LcdWriteData(Buff[1]%100/10+0X30);
		LcdWriteData(Buff[1]%10+0X30);
		/********************************************/
	}
}

void Timer_0(void) interrupt 1
{
  static unsigned char i;
	
	TL0 = 0XB0;
	TH0 = 0X3C;
	i++;
	if(i == 20)
	{
	  i = 0;
		/***************发送位移信息***********/
		Send_Str("Distance: ");
		Send_Byte(Distance/100+0X30);
		Send_Byte(Distance%100/10+0X30);
		Send_Byte(Distance%10+0X30);
		Send_Byte(0x0d);
		Send_Byte(0x0a);
		/**************************************/
		
		/***************发送雨量信息***********/
		Send_Str("Rainfall: ");
		Send_Byte(Pulse/100+0X30);
		Send_Byte(Pulse%100/10+0X30);
		Send_Byte(Pulse%10+0X30);
		Send_Byte(0x0d);
		Send_Byte(0x0a);
		/**************************************/
		
		/***************发送经纬度信息*********/
		Send_Str("Longitude: ");
		Send_Byte(Buff[0]/100+0X30);
		Send_Byte(Buff[0]%100/10+0X30);
		Send_Byte(Buff[0]%10+0X30);
		Send_Str(" ");
		Send_Str("Latitude: ");
		Send_Byte(Buff[1]/100+0X30);
		Send_Byte(Buff[1]%100/10+0X30);
		Send_Byte(Buff[1]%10+0X30);
		Send_Byte(0x0d);
		Send_Byte(0x0a);
		/**************************************/
		
		if(Flag)
		{
		   Send_Str("Rain Reaches The Set Upper Limit !!!");
			 Send_Byte(0x0d);
		   Send_Byte(0x0a);
			 LED = !LED;
		}
			
	}
}

void Serial_1(void) interrupt 4
{
	static unsigned char i;
  if(RI)
	{
	  RI = 0;
		Buff[i] = SBUF;
		i++;
		if(i == 2)
			i = 0;
	}
}

void exint0() interrupt 0
{
  Pulse++;
}