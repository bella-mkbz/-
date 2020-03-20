#include<reg51.h>
#include<stdio.h>
#include<intrins.h>
#include "button.h"

sbit Motor_A  =  P2^0;
sbit Motor_B  =  P2^1;
sbit Motor_EN =  P2^2;

static char Pulse = 100;

void main()
{ 
  unsigned char temp;
  Motor_A = 1;
  Motor_B = 0;
  Motor_EN = 0;
  TMOD &= 0XF0;
  TMOD |= 0X02;
  TH0 = 0XA4;
  TL0 = 0XA4;
  EA = 1;
  ET0 = 1;
  TR0 = 1;
  while(1)
  {
    temp = Button_Value();
		switch (temp)
    {
    	case 1:Pulse -=20;
			       if(Pulse < 0)
							  Pulse = 0;   						 
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
  Count++;

  if(Count > 100)
     Count = 0;
  if(Count <= Pulse)
	  Motor_EN = 0;
  else
	  Motor_EN = 1;
}