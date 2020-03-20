#include<reg51.h>
#include<intrins.h>
#include "delay.h"
#include "uart.h"

static uchar Rec_Flag = 0;

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit LED3  = P0^2;

void Init(void);

void main()
{
	uint8_t Flag=0;
  Init();
	Uart_Init();
	while(1)
	{
//		if(!Flag)
//		{
//			P0=_cror_(P0,1);
//			Delay;
//			if(P0 == 0XFE)
//			{
//			  Flag=1;	
//				P0=0Xef;
//	      Delay;
//			}
//		}
//	 else
//	 {
//	   	P0=_crol_(P0,1);
//			Delay;
//		 			if(P0 == 0X7f)
//			{
//			  Flag=0;	
//				P0=0Xef;
//	      Delay;
//			}
//	 }
		 switch (Rec_Flag)
     {
     	case 2:LED3 = 0;
     		break;
     	case 1:LED3 = 1;
     		break;
     	default:
     		break;
     }
	}
}
void Init(void)
{
  ADDR0=0;
	ADDR1=ADDR2=1;
	ADDR3=1;
	ENLED=0;
	P0=0Xff;
	Delay;
}

void seria_1() interrupt 4
{
  if(RI)
	{
	  RI = 0;
		Rec_Flag = SBUF;
	}
	if(TI)
	{
	  TI = 0;
	}
}