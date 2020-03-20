#include<reg51.h>
#include<stdio.h>
#include<intrins.h>
#include "button.h"

static char Pulse = 100;

void main()
{ 
	unsigned char temp;
  P0 = 0;
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
  static unsigned int i=0;
  static bit Flag,Addr;
  Count++;
  i++;
  if(i == 2000)
  {
    i = 0;
	  Flag = 1;
  }
  if(Count > 100)
     Count = 0;

  if(Count <= Pulse)
	  P0 = 0;
  else
	  P0 = 0xff;
}