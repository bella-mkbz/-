#include <reg51.h>
#include <intrins.h>
#include "delay.h"
#include "lcd.h"
#include "ADC0832.h"
#include "dht11.h"

unsigned char Model = 0, Model_1_Flag = 0, Flag = 1;
unsigned char humidity_upper = 60, humidity_down = 45, humidity_opti = 50, humidity = 0;
unsigned int Water_level = 0, Water_level_limit = 20;
unsigned char  Sub_Power = 1;	//加湿器电源
unsigned char Pulse = 50;

sbit Motor_A  =  P1^5;
sbit Motor_B  =  P1^6;
sbit Motor_EN =  P1^7;
sbit speaker =  P3^7;
sbit LED =  P3^5;

void main()
{ 	
  Motor_A = 0;
  Motor_B = 0;
  Motor_EN = 1;
	IT0 = 1;                        
	EX0 = 1; 
	IT1 = 1;                        
	EX1 = 1;  	
  TMOD = 0X01;
	TL0 = 0xb0;		//设置定时初值
	TH0 = 0x3c;		//设置定时初值
  EA = 1;
  ET0 = 1;
  TR0 = 0;  
  LcdInit();
	Show_String(0X80,"H:");	
	Show_String(0XC0,"W:");	
  while(1)
  {	
		humidity = DHT11_receive();
		Water_level = (unsigned int)(adc0832(0)/255.0*100);
		Show_Dec_Number(0X82,humidity);	
		Show_Dec_Number(0Xc2,Water_level);

		if(!Model)
		{
			if(humidity < humidity_down)
				Show_String(0X87,"Low!");
			else if(humidity > humidity_upper)
				Show_String(0X87,"High");
			else
				Show_String(0X87,"Suit");
	  }
		else
		{
			if(humidity < humidity_opti)
				Show_String(0X87,"Low!");
			else if(humidity > humidity_opti)
				Show_String(0X87,"High");
			else
				Show_String(0X87,"Suit");			
		}
		
		if(Water_level >= Water_level_limit)	
		{
				if(Flag)
				{
					Show_String(0XC7,"         ");
					Flag = 0;
					speaker = 1;
				}
		}
		else
		{
			speaker = 0;
			Show_String(0XC7,"Add water");	
		}				

		if(Sub_Power ==1)
		{
				if(Water_level >= Water_level_limit)	
				{

						if(!Model)
						{
								LED = 1;
								Motor_A = 0; //无论进入哪个模式，都先关闭，为了防止切换模式干扰
								if(humidity < humidity_down)
								{
									Model_1_Flag = 1;
									Motor_A = 1;
									LED = 0;
								}
								else if(humidity > humidity_upper)
								{
									Model_1_Flag = 0;
									Motor_A = 0;
									LED = 1;
								}
								if((humidity >= humidity_down) && (Model_1_Flag == 1))
								{
									LED = 0;
									Motor_A = 1;
									Delay1ms(130);
									Motor_A = 0;
								}

						}
						else if(Model)
						{
							Model_1_Flag = 0;	
								
								Motor_A = 0;		//无论进入哪个模式，都先关闭，为了防止切换模式干扰
							if(humidity < humidity_opti)
							{
								LED = 0;
								Motor_A = 1;
							}					
							else
							{
								Motor_A = 0;
								LED = 1;
							}
						}
				}
				else
				{
					Motor_A = 0;
					Flag = 1;
					LED = 1;
				}
		}
		else
		{
			Show_String(0x8D,"Off");
			Motor_A = 0;
			LED = 1;
		}
					
  }
}

void exint0() interrupt 0         
{
	if(Sub_Power == 1)
   Model = !Model;
}

void exint1() interrupt 2         
{
	TR0 = 1;	
	Show_String(0x8D,"B30");
}


void Timer_0() interrupt 1
{
  static unsigned int i = 0;
	TL0 = 0xb0;		//设置定时初值
	TH0 = 0x3c;		//设置定时初值
  i++;
	if(i == 200)
	{
		Sub_Power = 0; //打开加湿器电源
		i = 0;
		TR0 = 0;
	}
}



