#include <reg51.h>
#include "button.h"
#include "delay.h"
#include "lcd.h"
#include "adc0808.h"


sbit Led = P3^7;
unsigned char Key_Value = 0;		//按键值
unsigned char Adc_Value = 0;		//ADC转换后的数字值
float  Light_Value = 0;					//数字值转换后的光照值，单位lux
unsigned char Mode_Flag = 1; 		//室内和室外模式标志

void main(void)
{   
	IT0 = 1;                        //set INT0 int type (1:Falling 0:Low level)
	EX0 = 1;                        //enable INT0 interrupt
	EA = 1;                         //open global interrupt switch	
	LcdInit();
	Show_String(0x82,"Light System");
	while(1)
	 {
			Adc_Value = ADC0808_Value(0, 0, 0);
		  Light_Value = Adc_Value/218.0*15000.0;
			if((int)Light_Value == 68)
			{
				Show_Dec_Number(0Xc0,0);
				Show_String(0xc5,"lux");			
			}
			else
			{
				Show_Dec_Number(0Xc0,Light_Value);
				Show_String(0xc5,"lux");
			}
			if(Mode_Flag)
			{
				if(Light_Value <= 10000)
				{
					Led = 0;
				}
				else
				{
					Led = 1;
				}
				Show_String(0xc9,"outdoor");
			}
			else
			{
				if(Light_Value <= 300)
				{
					Led = 0;
				}
				else
				{
					Led = 1;
				}	
				Show_String(0xc9,"indoor ");
			}
	 }
}

void exint0() interrupt 0           //(location at 0003H)
{
    Mode_Flag = !Mode_Flag;
}
