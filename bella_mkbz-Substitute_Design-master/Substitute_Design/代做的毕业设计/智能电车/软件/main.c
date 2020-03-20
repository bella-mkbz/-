#include <reg51.h>
#include <intrins.h>
#include "delay.h"
#include "button.h"
#include "lcd.h"
#include "ADC0832.h"

sbit Motor_A  =  P1^5;
sbit Motor_B  =  P1^6;
sbit Motor_EN =  P1^7;


static int Pulse = 100;
float V;
unsigned char A;

void main()
{ 
  unsigned char temp;
	
  Motor_A = 1;
  Motor_B = 0;
  Motor_EN = 0;
  TMOD = 0X02;
  TH0 = 0X74;
  TL0 = 0X74;
  EA = 1;
  ET0 = 0;
  TR0 = 0;                   
  LcdInit();	
	Show_String(0xc0,"Current: ");
	Show_String(0x80,"Voltage: ");
  Show_String(0x8e,"mV");
	Show_String(0xCE,"mA");
  while(1)
  {
		Show_Dec_Number(0X8a,(unsigned int)(((adc0832(0) - 1)/51.0)*1000.0));		
		//V = (((float)adc0832(1) - 1)/255)*5.0;
		V = ((float)adc0832(1)/255)*5.0;
		A = V * 20;
		Show_Dec_Number(0XCa,A);
    temp = Button_Value();
		switch (temp)
    {
    	case 1:
				ET0 = 1;
        TR0 = 1;
    		break;
    	case 2:
				Pulse = Pulse - 25;
			  if(Pulse <= 0)
					Pulse = 0;
    		break;
			case 3:
				Pulse = Pulse + 25;
			  if(Pulse >= 100)
					Pulse = 100;
    		break;
		  case 4:
				ET0 = 0;
        TR0 = 0;
			  Motor_A = 1;
        Motor_B = 1;
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
  if(Count <= Pulse)
	  Motor_EN = 0;
  else
	  Motor_EN = 1;
}


