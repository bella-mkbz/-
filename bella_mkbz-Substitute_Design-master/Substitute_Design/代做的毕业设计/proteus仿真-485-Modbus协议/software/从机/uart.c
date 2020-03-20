#include "uart.h"

void SerialInit(void)
{
	TMOD &= 0X0F;
	TMOD |= 0X20;
	PCON = 0X00;
	SCON = 0X50;
	TH1 =  0XFD;
	TL1 =  0XFD;
	TR1 = 1;
	ES = 1;
}

void Timer1Init(void)		//10毫秒@11.0592MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x00;		  //设置定时初值
	TH0 = 0xDC;	   	//设置定时初值
	TR0 = 1;		    //定时器0开始计时
}

void SendByte(unsigned char temp)
{
	SBUF = temp;
	while(!TI);
	TI = 0;
}

void SendBuff(unsigned char *temp, unsigned char length)
{
	unsigned char i = 0;
	for(i = 0; i < length; i++)
  {
		SendByte(temp[i]);
  }
}

unsigned int factory_crc16( unsigned char *bufData, unsigned char buflen)

{
	unsigned int TCPCRC = 0xffff;
	unsigned int POLYNOMIAL = 0xa001;
	unsigned char i, j;
	for (i = 0; i < buflen; i++)
	{
			TCPCRC ^= bufData[i];
			for (j = 0; j < 8; j++)
			{
					if ((TCPCRC & 0x0001) != 0)

					{
							TCPCRC >>= 1;
							TCPCRC ^= POLYNOMIAL;
					}
					else
						 TCPCRC >>= 1;
			}
	}
	return TCPCRC;
}



