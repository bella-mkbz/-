#include "DHT11.h"

void InitUART(void)
{
    PCON &= 0x7F;		
	  SCON = 0x50;		
	  AUXR &= 0xBF;	
	  AUXR &= 0xFE;		
	  TMOD &= 0x0F;	
	  TMOD |= 0x20;		
	  TL1 = 0xFD;		
	  TH1 = 0xFD;		
	  ET1 = 0;		
	  TR1 = 1;		
}  

void Delay10us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	_nop_();
	i = 24;
	while (--i);
}

void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 43;
	j = 6;
	k = 203;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void Delay20ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 1;
	j = 216;
	k = 35;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay30us()		//@11.0592MHz
{
	unsigned char i;

	i = 80;
	while (--i);
}

void Start(void)
{
	DHT11 = 1;
	Delay10us();
  DHT11 = 0;
	Delay20ms();
	DHT11 = 1;
	Delay30us();
	DHT11 = 1;
}

unsigned char Sensor_Byte(void)
{
  unsigned char i=0,j,tmp,buffer;
	DHT11 = 1;
	for(j=0;j<8;j++)
	{
	  while(!DHT11)
		{
      ;
		}
		Delay30us();
		i=0;
		tmp = 0;
		if(DHT11)
			tmp = 1;
		while(DHT11)
		{
       ;	  
		}
		buffer <<=1;
		buffer |= tmp;
	}
	return buffer;
}

unsigned char Read_Sensor(unsigned char *Sensor_Data)
{
  unsigned char i = 0;
	Start();
	while(!DHT11)
	{
	  Delay10us();
		i++;
		if(i >= 15)
		{
			return 0;
		}
	}
	i = 0;
	while(DHT11)
	{
		Delay10us();
		i++;
		if(i >= 10)
		{
			return 0;
		}
	}
  for(i=0;i<5;i++)
	  Sensor_Data[i] = Sensor_Byte(); 
	if(Sensor_Data[4] = Sensor_Data[0]+Sensor_Data[1]+Sensor_Data[2]+Sensor_Data[3])
    return 1;
	else
		return 0;
}


