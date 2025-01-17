#include "uart.h"

void Uart_Init(void)         //串口初始化
{
  TMOD &= 0X0f;
  TMOD |= 0X20;
	TH1 = 256 - (11059200/12/32)/9600;
  TL1=TH1;
  TR1=1;
  PCON=0;
  SCON=0X50;
	EA = 1;
	ES = 1;
}

void Send_Byte(uchar dat)  //发送一个字节
{
  SBUF=dat;
	while(!TI);
	TI=0;
}

void Send_Str(char *str)    //发送一个字符串，带有换行功能
{
  while(*str)
	{
	  Send_Byte(*str);
		str++;
	}
	Send_Byte(0x0d);
	Send_Byte(0x0a);  //  »»ÐÐ¹¦ÄÜ
}