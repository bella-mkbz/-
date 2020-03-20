#include<reg51.h>
#include<intrins.h>
#include "delay.h"

sbit ADDR_A=P2^0;
sbit ADDR_B=P2^1;
sbit ADDR_C=P2^2;

sbit ADDR_E1=P2^3;
sbit ADDR_E2_3=P2^4;

void Init(void);

void main()
{
	uint8_t Flag=0;
  Init();
	while(1)
	{
		if(!Flag)
		{
			P0=_cror_(P0,1);
			Delay;
			if(P0 == 0XFE)
			{
			  Flag=1;	
				P0=0Xef;
	      Delay;
			}
		}
	 else
	 {
	   	P0=_crol_(P0,1);
			Delay;
		 			if(P0 == 0X7f)
			{
			  Flag=0;	
				P0=0Xef;
	      Delay;
			}
	 }
	}
}
void Init(void)
{
  ADDR_A=0;
	ADDR_B=ADDR_C=1;
	ADDR_E1=1;
	ADDR_E2_3=0;
	P0=0Xef;
	Delay;
}