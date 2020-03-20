#include<stdlib.h>
#include<reg51.h>

#define uchar unsigned char
int longitude,latitude;

void Uart_Init(void);
void Send_Byte(uchar dat);

void main()
{
	TMOD &= 0Xf0;
  TMOD |= 0X01;
	TL0 = 0XB0;
	TH0 = 0X3C;
	ET0 = 1;
	TR0 = 1;
	Uart_Init(); 
  for(;;);	
}

void Uart_Init(void)         
{
  TMOD &= 0X0f;
  TMOD |= 0X20;
  TH1 = 0xfd;
  TL1=TH1;
  TR1=1;
  PCON=0;
  SCON=0X50;
	EA = 1;
	ES = 1;
}
void Send_Byte(uchar dat)  
{
  SBUF=dat;
	while(!TI);
	TI=0;
}

void Timer_0(void) interrupt 1
{
  static unsigned char i;
	
	TL0 = 0XB0;
	TH0 = 0X3C;
	i++;
	if(i == 20)
	{
	  i = 0;
		longitude = rand()%(180-0+1)+1;
		latitude = rand()%(90-0+1)+1;
		Send_Byte(longitude);
		Send_Byte(latitude);
	}
}