#include <reg51.h>
#include <intrins.h>
#include "lcd.h"

sbit Motor_A  =  P1^5;
sbit Motor_B  =  P1^6;
sbit Motor_EN =  P1^7;
sbit KEY1 = P2^4;
sbit KEY2 = P2^5;
sbit LED1 = P3^6;
sbit LED2 = P3^7;

unsigned char Flag = 0; //正反转标志

static int Pulse = 50;

void main()
{ 
  Motor_A = 1;
  Motor_B = 0;
  Motor_EN = 0;
  TMOD = 0X01;
	IT0 = 1;                        
	EX0 = 1;  
	IT1 = 1;                        
	EX1 = 1; 	
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
  EA = 1;
  ET0 = 1;
  TR0 = 1;                   
  LcdInit();	
	Show_String(0x85,"PWM:");
	Show_Dec_Number(0x89,Pulse);
  while(1)
  {		
		Show_Dec_Number(0x89,Pulse);
		if(Pulse == 0)
		{
			Show_String(0xc5,"STOP    ");
			LED1 = 1;
			LED2 = 1;
		}
		else if(Motor_B)
		{
			Show_String(0xc5,"Backward");
			LED1 = 1;
			LED2 = 0;
		}
		else if(Motor_A)
		{
			Show_String(0xc5,"Foreward");
			LED1 = 0;
			LED2 = 1;
		}		

  }
}

void Timer_0(void) interrupt 1
{
  static unsigned char Count = 0;
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
  Count++;
  if(Count > 100)
     Count = 1;
  if(Count <= Pulse)
	  Motor_EN = 1;
  else
	  Motor_EN = 0;
}

void exint0() interrupt 0         
{
	if(!KEY1)
	{
		Motor_A = !Motor_A; 
		Motor_B = !Motor_B; 
	}
	else if(!KEY2)
	{
		Pulse = Pulse + 10;
		if(Pulse >= 100)
			Pulse = 100;
	}
	else
	{
			Pulse = Pulse - 10;
		if(Pulse <= 0)
			Pulse = 0;	
	}

}

void exint1() interrupt 2
{
	Pulse = 0;
}  
