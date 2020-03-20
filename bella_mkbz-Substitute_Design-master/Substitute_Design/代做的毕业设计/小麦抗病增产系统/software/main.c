#include <reg51.h>
#include <intrins.h>
#include "lcd.h"
#include "ADC0832.h"
#include "dht11.h"

unsigned char *Rec_buffer, humidity, temperature;
unsigned int Water_level , Moonth = 9, Day = 1;

/************按键定义变量**************/
sbit key_1 = P2^0;
sbit key_2 = P2^1;
sbit key_3 = P2^2;
sbit key_4 = P2^3;
sbit key_5 = P2^4;
sbit key_6 = P2^5;
sbit key_7 = P2^6;
/**************************************/

/************LED变量**************/
sbit L_1 = P1^3;
sbit L_2 = P1^4;
sbit L_3 = P1^5;
sbit L_4 = P1^6;
sbit L_5 = P1^7;
/*********************************/

/************农药电机*************/
sbit Moter_1 = P3^0;
sbit Moter_2 = P3^1;
sbit Moter_3 = P3^3;
/*********************************/

/************病菌**************/
bit B_1 = 0, B_2 = 0, B_3 = 0, B_4 = 0, B_5 = 0;
/******************************/

void main()
{ 	
	Moter_1 = 0;
	Moter_2 = 0;
	Moter_3 = 0;
	IT0 = 1;                        
	EX0 = 1; 
  EA = 1;
  LcdInit();
	Show_String(0X80,"W:");	
	Show_String(0X85,"H:");	
	Show_String(0X8A,"N:");	
	Show_String(0Xc0,"D:");	
	Show_Dec_Number(0Xc2,Moonth);	
	Show_String(0Xc4,"-");
	Show_Dec_Number(0Xc5,Day);	
  while(1)
  {	
		Show_Dec_Number(0Xc2,Moonth);	
		Show_Dec_Number(0Xc5,Day);
		Rec_buffer = DHT11_receive();
		humidity = Rec_buffer[0];
		temperature = Rec_buffer[1];
		Water_level = (unsigned int)(adc0832(0)/255.0*100);
    Show_Dec_Number(0X82,temperature);	
		Show_Dec_Number(0X87,humidity);		
		Show_Dec_Number(0X8C,Water_level);
		if(B_5 == 1 && Water_level > 35 && temperature<10 && humidity >65)
			L_5 = 0;
		else
		{
			B_5 = 0;
			L_5 = 1;
		}
		switch (Moonth)
    {
			case 9:
				if(Day > 20)
					Show_String(0Xca,"Sowing");	
				else
					Show_String(0Xca,"      ");
				break;
			case 10:
				if(Day < 10)
					Show_String(0Xca,"Sowing");	
				else
					Show_String(0Xca,"      ");				
				break;
			case 11:
					Show_String(0Xca,"      ");		
				break;
    	case 1:
				if(B_3 == 1 && temperature<3 && (humidity > 65 || humidity < 35))
				{
					Moter_3 = 1;
					L_3 = 0;
				}
				else
				{
					Moter_3 = 0;
					B_3 = 0;
					L_3 = 1;
				}
    		break;
    	case 2:
				if(Day < 20)
				{
					if(B_3 == 1 && temperature<3 && (humidity > 65 || humidity < 35))
					{
						Moter_3 = 1;
						L_3 = 0;
					}				
					else
					{
						Moter_3 = 0;
						B_3 = 0;
						L_3 = 1;
					}
				}
				else
				{
					Moter_3 = 0;
						B_3 = 0;
						L_3 = 1;				
				}
    		break;
    	case 3:
				Moter_3 = 0;
						B_3 = 0;
						L_3 = 1;				
				if(Day > 20)
				{
					if(B_1 == 1 && temperature <= 20 && temperature >= 15 && Water_level > 35 && humidity > 70)
					{
						Moter_1 = 1;
						L_1 = 0;
					}	
					else
				  {
						Moter_1 = 0;
						B_1 = 0;
						L_1 = 1;				
				  }
				}
				else
				{
					Moter_1 = 0;
						B_1 = 0;
						L_1 = 1;							
				}
	
    		break;
    	case 4:
				if(Day < 10)
				{
					if(B_1 == 1 && temperature <= 20 && temperature >= 15 && Water_level > 35 && humidity > 70)
					{
						Moter_1 = 1;
						L_1 = 0;
					}		
					else
				  {
						Moter_1 = 0;
						B_1 = 0;
						L_1 = 1;				
				  }	
				}
				else
				{
						Moter_1 = 0;
						B_1 = 0;
						L_1 = 1;							
				}				
				
				if(B_2 == 1 && temperature <= 25 && temperature >= 19  && humidity > 64)
				{
					Moter_2 = 1;
					L_2 = 0;
				}
					
				else
				{
					Moter_2 = 0;
					B_2 = 0;
					L_2 = 1;				
				}	

				if(Day > 20)
				{
					if(B_4 == 1 && humidity > 65)
						L_4 = 0;
					else
				  {
						B_4 = 0;
						L_4 = 1;				
				  }	
				}
				else
				  {
						B_4 = 0;
						L_4 = 1;				
				  }	
    		break;
    	case 5:
				Moter_1 = 0;
				B_1 = 0;
				L_1 = 1;	
				if(Day < 10)
				{
					if(B_4 == 1 && humidity > 65)
						L_4 = 0;
					else
				  {
						B_4 = 0;
						L_4 = 1;				
				  }	
				}				
				else
				  {
						B_4 = 0;
						L_4 = 1;				
				  }	
				if(B_2 == 1 && temperature <= 25 && temperature >= 19  && humidity > 64)
				{
					Moter_2 = 1;
						L_2 = 0;
				}
				
				else
				{
					Moter_2 = 0;
					B_2 = 0;
					L_2 = 1;				
				}					
    		break;
    	default:
				Moter_2 = 0;
					B_2 = 0;
					L_2 = 1;	
					B_4 = 0;
					L_4 = 1;			
    		break;
    }
  }
}

void exint0() interrupt 0         
{
	if(!key_1)
	{
		Moonth++;
		if(Moonth == 13)
			Moonth = 1;
		else if(Moonth == 7)
			Moonth = 9;
	}
	else if(!key_2)
	{
		Day++;
		if((Moonth ==1 || Moonth ==3 || Moonth ==5 || Moonth ==10 || Moonth ==12))
		{
			if(Day == 32)
				Day = 1;	
		}
		else
		{
			if(Day==31)
				Day = 1;
		}
	}
	else if(!key_3)
	{
		B_1 = 1;
	}
	else if(!key_4)
	{
		B_2 = 1;
	}
	else if(!key_5)
	{
		B_3 = 1;
	}
	else if(!key_6)
	{
		B_4 = 1;
	}
	else if(!key_7)
	{
		B_5 = 1;
	}	
}




