#include<reg51.h>
#include<stdio.h>
#include<intrins.h>
#include "button.h"
#include "delay.h"

static char Pulse = 100;
sbit Motor = P3^0;
static unsigned char count;
	
void main()
{ 
	unsigned char temp;
  Motor = 0;
  TMOD &= 0XF0;
  TMOD |= 0X02;
//	TMOD &= 0X0F;
//  TMOD |= 0X10;
  TH0 = 0X48;               //定时周期200us
	TL0 = 0X48;               //定时周期200us
//	TH1 = 0X3C;
//	TL1 = 0XB0;
  EA = 1;
  ET0 = 1;
  TR0 = 1;
//	ET1 = 1;
//  TR1 = 0;
  while(1)
  {
    temp = Button_Value();
		switch (temp)
    {
    	case 1:Pulse -=20;
			       if(Pulse < 0)
							  Pulse = 0;
//						    TR1 = 1;
//						    TR0 = 1;
    		        break;
    	case 2:Pulse +=20;
			       if(Pulse > 100)
							  Pulse = 100;       					 
    		     break;
    	default:
    		     break;
    }
  }

}

void Timer_0(void) interrupt 1
{
	static unsigned char Count = 0;
  static unsigned int i=0;
  static bit Flag;
  Count++;
  i++;
  if(i == 2000)
  {
    i = 0;
	  Flag = 1;
  }
  if(Count > 100)
     Count = 0;

  if(Count <= 10)
	  Motor  = 1;
  else
	  Motor  = 0;
}
//void Timer_1(void) interrupt 3
//{

//	TH1 = 0X3C;
//	TL1 = 0XB0;
//	count++;
//	if(count == 40)
//	{
//	    TR1 = 0;
//	    TR0 = 0;
//		count = 0;
//		Motor  = 0;
//	}

//}