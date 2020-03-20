#include <reg51.h>
#include <intrins.h>
#include "lcd.h"
#include "ADC0832.h"

/************按键定义变量**************/
sbit key_1 = P2^0;
sbit key_2 = P2^1;
/**************************************/
sbit speaker = P3^7;
unsigned int Water_level;
int limit = 1500;
float value = 0;

void main()
{ 	
	IT0 = 1;                        
	EX0 = 1; 
  EA = 1;
  LcdInit();
	Show_String(0X80,"Value:");	
	Show_String(0Xc0,"Limit:");	
	Show_Dec_Number(0xc7,limit);
  while(1)
  {	
		Water_level = (unsigned int)(adc0832(0));
		value = Water_level/255.0*120.0*35.0;
		Show_Dec_Number(0x87,(unsigned int)value);
		Show_String(0X8b,"mg/m3");
    Show_Dec_Number(0xc7,limit);	
		if(value < limit)		
			speaker = 1;
		else
			speaker = 0;
  }
}


void exint0() interrupt 0         
{
	if(!key_1)
	{
		limit+=100;
		if(limit > 4200)
			limit = 4200;
	}
	else if(!key_2)
	{
		limit-=100;
		if(limit < 0)
			limit = 16;
	}

}



