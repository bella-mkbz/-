#include <reg51.h>
#include "DS18B20_3.H"
#include "lcd.h"
#include "adc0832.h"
#include "delay.h"
#include "button.h"


sbit Beep = P1^7;
sbit Led  = P1^6;
int Hihg_Limit = 500, Temp_Limit = 50;
int temp = 0;
unsigned char adc_value = 0, KEY_VALUE = 0;
float high = 0;
void Timer0_Iint(void);

void main(void)
{
	unsigned char i = 0;
	Timer0_Iint();
	LcdInit();
	temp = ReadTemperature();
	Delay1ms(1000);
	Show_String(0x80,"Temp:");
	Show_String(0xc0,"High:");
	Show_Dec_Number(0x8D,Temp_Limit);
	Show_Dec_Number(0xCC,Hihg_Limit);
	EA = 1;
	while(1)
	{
		 if(KEY1 == 0)
		 {
				if(KEY1 == 0)
				{
					TR0 = 0;
					i++;
					if(i == 3)
						i = 1;
					while(!KEY1);
					if(i == 1)
					{
						LcdWriteCom(0x8d);
						LcdWriteCom(0x0f);							
					}
			    else
						LcdWriteCom(0xcc);
				}
		 }
		 if(KEY4 == 0)
		 {
				if(KEY4 == 0)
				{
					while(!KEY4);
					TR0 = 1;
					LcdWriteCom(0x0c);					
				}
		 }		 
		 if(KEY2 == 0)
		 {
				if(KEY2 == 0)
				{
					while(!KEY2);
					if(i == 1)
					{
						Temp_Limit = Temp_Limit + 3;
						if(Temp_Limit >= 128)
							Temp_Limit = 128;
						Show_Dec_Number(0x8D,Temp_Limit);
						LcdWriteCom(0x8d);
					}
					else if(i == 2)
					{
						Hihg_Limit = Hihg_Limit + 50;
						if(Hihg_Limit >= 999)
							Hihg_Limit = 999;
						Show_Dec_Number(0xcc,Hihg_Limit);
						LcdWriteCom(0xcc);						
					}
				}
		 }
		 if(KEY3 == 0)
		 {
				if(KEY3 == 0)
				{
					while(!KEY3);
					if(i == 1)
					{
						Temp_Limit = Temp_Limit - 3;
						if(Temp_Limit <= 0)
							Temp_Limit = 0;
						Show_Dec_Number(0x8D,Temp_Limit);
						LcdWriteCom(0x8d);
					}
					else if(i == 2)
					{
						Hihg_Limit = Hihg_Limit - 50;
						if(Hihg_Limit <= 0)
							Hihg_Limit = 0;
						Show_Dec_Number(0xcc,Hihg_Limit);
						LcdWriteCom(0xcc);						
					}
				}
		 }
	}
}

void timer0(void) interrupt 1
{
	static unsigned char i = 0;
	TH0 = 0X3C;
	TL0 = 0XB0;
	i++;
	if(i == 10)
	{
		i = 0;
		temp = ReadTemperature();
		Show_Dec_Number(0x85,temp);
		adc_value = Adc0832_Getvalue(0);
		high = adc_value/255.0*10.0;
		Show_Dec_Number(0xc5,high*100);
		if(temp >= Temp_Limit)
			Led = 0;
		else
			Led = 1;
		if(high*100 >= Hihg_Limit)
			Beep = 0;
		else
			Beep = 1;
	}
}

void Timer0_Iint(void)
{
	TMOD &= 0XF0;
	TMOD |= 0X01;
	ET0 = 1;
	TR0 = 1;
	TH0 = 0X3C;
	TL0 = 0XB0;
	EA = 0;
}
