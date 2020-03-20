#include <reg51.h>
#include <stdlib.h>


sbit Pulse = P2^0;

unsigned int Fixed_frequency = 12;
#define Fixed_Period ((1.0/Fixed_frequency)*500)
unsigned int Frequency = 12;
#define Period ((1.0/Frequency)*500) 
unsigned int Temp = 0, i = 1, Ms_20 = 0;


void Timer0_Init()
{
	TMOD &= 0XF0;
	TMOD |= 0X01;
	TH0 = 0X3C;
	TL0 = 0XB0;
	EA = 0;
	ET0 = 1;
	TR0 = 1;
}

void Timer1_Init()
{
	TMOD &= 0X0F;
	TMOD |= 0X20;
	TH1 = TL1 = 256 - Period;
	EA = 0;
	ET1 = 1;
	TR1 = 1;
}

void main()
{
	Timer0_Init();
	Timer1_Init();
	EA = 1;

	for(;;)
	{
		;
	}
}

void timer0() interrupt 1
{
	TH0 = 0X3C;
	TL0 = 0XB0;
	Temp++;
	Ms_20++;
	if(	Ms_20 == 1200)
	{
		Fixed_frequency = rand()%13 + 12;
		ET1 = 0;
	    TR1 = 0;
		TH1 = TL1 = 256 - Fixed_Period;
		ET1 = 1;
	    TR1 = 1;		
	}
	else if(Ms_20 == 2400)
	{
		ET0 = 0;
	    TR0 = 0;
		ET1 = 0;
	    TR1 = 0;		
	}

	if((Temp == 76) && (Ms_20 < 1200))
	{
		Temp = 0;
		Frequency++;
		if(Frequency == 26)
			Frequency = 12;
		ET1 = 0;
	    TR1 = 0;
		TH1 = TL1 = 256 - Period;
		ET1 = 1;
	    TR1 = 1;		
	}

}

void timer1() interrupt 3
{
	Pulse = !Pulse;
}
