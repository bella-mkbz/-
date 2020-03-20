#include <reg51.h>
#include "button.h"
#include <stdio.h>

sbit Key_Led = P2^6;
sbit Remote_Led = P2^7;
sbit M1_1 = P1^0;
sbit M1_2 = P1^1;
sbit M2_1 = P1^2;
sbit M2_2 = P1^3;
sbit M3_1 = P1^4;
sbit M3_2 = P1^5;
sbit M4_1 = P1^6;
sbit M4_2 = P1^7;
sbit Pulse1 = P3^4;
sbit Pulse2 = P3^5;
sbit Pulse3 = P3^6;
sbit Pulse4 = P3^7;

unsigned char Key_Value = 0;
int Pulse_Time1 = 0;
int Pulse_Time2 = 0;
int Pulse_Time3 = 0;
int Pulse_Time4 = 0;
bit Control_Flag = 0;
unsigned char Temp;

void Timer1_Init(void);
void Serial_Init(void);
void Port_Init(void);
void Up(void);
void Down(void);

void main(void)
{
	Timer1_Init();
	Serial_Init();
	Port_Init();
	while(1)
	{  
		  if(!Control_Flag)
			{
			  Key_Led = 0;
        Remote_Led = 1;
				Key_Value = Button_Value();	
			}	
			else
			{
			  Key_Led = 1;
        Remote_Led = 0;			
			}
			switch (Key_Value)
			{
				case 1:
					Up();
				  Pulse_Time1 += 15;
				  Pulse_Time2 += 15;
				  Pulse_Time3 += 15;
				  Pulse_Time4 += 15;
				  Key_Value = 0;
					break;
				case 2:
					Down();
					Pulse_Time1 -= 15;
				  Pulse_Time2 -= 15;
				  Pulse_Time3 -= 15;
				  Pulse_Time4 -= 15;
				  Key_Value = 0;
					break;
				case 3:
					Pulse_Time2 += 15;
				  Pulse_Time3 -= 15;
				  Key_Value = 0;
					break;
				case 4:
					Pulse_Time2 -= 15;
				  Pulse_Time3 += 15;
				  Key_Value = 0;
					break;
				case 5:
					Pulse_Time1 -= 15;
				  Pulse_Time4 += 15;
				  Key_Value = 0;
					break;
				case 6:
					Pulse_Time1 += 15;
				  Pulse_Time4 -= 15;
				  Key_Value = 0;
					break;
				default:
					break;
			}
			
	}
}

void Serial() interrupt 4
{
		if(RI)
			{
				RI = 0;
				Temp = SBUF;
				switch (Temp)
				{
					case 0x55:
						Control_Flag = 1;
					  TI = 1;
			      printf("Obtain Control authority \n");
			      while(!TI);
			      TI = 0;
						break;
					case 1:
						if(Control_Flag)
						Key_Value = 1;
						break;
					case 2:
						if(Control_Flag)
						Key_Value = 2;
						break;
					case 3:
						if(Control_Flag)
						Key_Value = 3;
						break;
					case 4:
						if(Control_Flag)
						Key_Value = 4;
						break;
					case 5:
						if(Control_Flag)
						Key_Value = 5;
						break;
					case 6:
						if(Control_Flag)
						Key_Value = 6;
						break;
					case 0x66:
						if(Control_Flag)
						{
							Control_Flag = 0;
							TI = 1;
							printf("Release Control authority \n");
							while(!TI);
							TI = 0;							
						}
						break;			
					default:
						break;
				}
			}
}

void Timer_0(void) interrupt 1
{
	static unsigned char Count = 0;
  Count++;
		
  if(Count > 100)
     Count = 0;
  if(Pulse_Time1 > 100)
		 Pulse_Time1 = 100;
  if(Pulse_Time2 > 100)
		 Pulse_Time2 = 100;
  if(Pulse_Time3 > 100)
		 Pulse_Time3 = 100;
  if(Pulse_Time4 > 100)
		 Pulse_Time4 = 100;
  if(Pulse_Time1 < 0)
		 Pulse_Time1 = 0;
  if(Pulse_Time2 < 0)
		 Pulse_Time2 = 0;
  if(Pulse_Time3 < 0)
		 Pulse_Time3 = 0;
  if(Pulse_Time4 < 0)
		 Pulse_Time4 = 0;
	if(Count <= Pulse_Time1)
		 Pulse1 = 1;	
	else
		Pulse1 = 0;
	if(Count <= Pulse_Time2)
		Pulse2 = 1;
	else
		Pulse2 = 0;
	if(Count <= Pulse_Time3)
		Pulse3 = 1;
	else
		Pulse3 = 0;
	if(Count <= Pulse_Time4)
		Pulse4 = 1;
	else
		Pulse4 = 0;
}

void Port_Init(void)
{
    P3 |= 0XF0;
    P1 = 0X00;
}

void Up(void)
{
  P1 = 0X69;
}

void Down(void)
{
  P1 = 0;
}

void Timer1_Init(void)
{
		TMOD &= 0XF0;
		TMOD |= 0X02;
		TH0 = 0X48;               //定时周期200us
		TL0 = 0X48;               //定时周期200us
		EA = 1;
		ET0 = 1;
		TR0 = 1;
}

void Serial_Init(void)
{
		TMOD &= 0X0F;
		TMOD |= 0X20;
	  SCON = 0X50;
		TH1 = 0XFD;            
		TL1 = 0XFD;              
		ES= 1;
		TR1 = 1;
}
