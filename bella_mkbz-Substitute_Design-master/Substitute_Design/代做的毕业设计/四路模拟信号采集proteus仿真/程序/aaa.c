#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char

uchar Sensor_Data = 0;
uchar Sensor_Buff[6]=  
{
        0x0,0x0,0x0,0x0,0x0,0xbc
};

void UartInit(void);
void Delay200ms();
void SendData(uchar dat);
void UART1_SendString(uchar *s);
void Delay500us();
void Delay50ms();

sbit OE  = P3^7;  // 3.7
sbit EOC = P3^6;  // 3.6
sbit ST  = P3^5;  // 3.5
sbit CLK = P3^4;  // 3.4
sbit A = P2^0;
sbit BB = P2^1;
sbit C = P2^2;

void main()  
{
	  uchar i = 0;
	  A = 0;
	  BB = 0;
	  C = 0;
	  UartInit();
		while(1)        
		{
			for(i = 0; i < 4; i++)
      {
				ST = 0;
				Delay500us();
				ST = 1;
				Delay500us();
				ST = 0;
				while(EOC == 0);
				OE = 1;
				Delay500us();
				Sensor_Data = P1;
				OE = 0;	
				switch (i)
				{
					case 0:
						Sensor_Buff[1] = Sensor_Data;
						BB = 0;
					  A = 1;					
						break;
					case 1:
						Sensor_Buff[2] = Sensor_Data;
						BB = 1;
					  A = 0;					
						break;
					case 2:
						Sensor_Buff[3] = Sensor_Data;
						A = 1;
					  BB = 1;
						break;
					case 3:
						Sensor_Buff[4] = Sensor_Data;
				    BB = A = 0;
						break;
				}	
      }
			Delay50ms();
			Sensor_Buff[0] = Sensor_Buff[1] + Sensor_Buff[2] + Sensor_Buff[3] + Sensor_Buff[4];
			UART1_SendString(Sensor_Buff);

		}
}


void SendData(uchar dat)
{
	SBUF = dat;
	while(!TI);
	TI = 0;
}

void UART1_SendString(uchar *s)
{
	int i;
	for(i = 0; i < 6; i++)
	{
		SendData(s[i]);//发送当前字符
	}
}

void UartInit(void)		//9600bps@11.0592MHz
{
	PCON = 0x0;
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;		//设定定时初值
	TH1 = 0xFD;		//设定定时器重装值
	TR1 = 1;		//启动定时器1
}

void Delay50ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 90;
	j = 163;
	do
	{
		while (--j);
	} while (--i);
}

void Delay200ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 2;
	j = 103;
	k = 147;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay500us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 227;
	while (--i);
}