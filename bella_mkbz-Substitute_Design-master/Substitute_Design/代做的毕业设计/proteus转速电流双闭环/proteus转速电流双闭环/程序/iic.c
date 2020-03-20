#include "iic.h"


void delay_iic()
{
   unsigned char i=0;
   for(i=0;i<5;i++);
}


void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	somenop;
	SDA = 0;
	somenop;
	SCL = 0;	
}


void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	somenop;
	SDA = 1;
}


void IIC_response()
{
    unsigned char k=250; 
	SDA = 1;
	somenop;
	SCL = 1;
	somenop;
	while(SDA&&k--);
	somenop;
	SCL = 0;
	somenop;
}


void IIC_SendByte(unsigned char byt)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		somenop;
		SCL = 1;
		byt <<= 1;
		somenop;
		SCL = 0;
	}
}


unsigned char IIC_RecByte(void)
{
	unsigned char da;
	unsigned char i;
	
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		somenop;
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		somenop;
	}
	return da;
}

void ad_send(unsigned char addr,unsigned char dat)
{
   IIC_Start();
   IIC_SendByte(0x90);
   IIC_response();
   IIC_SendByte(addr);
   IIC_response();
   IIC_SendByte(dat);
   IIC_response();
   IIC_Stop();
}


unsigned char ad_read(unsigned char add,unsigned char addr)
{
   unsigned char dat;
   IIC_Start();
   IIC_SendByte(add);
   IIC_response();
   IIC_SendByte(addr);
   IIC_response();
   IIC_Stop();

   IIC_Start();
   IIC_SendByte(add+1);
   IIC_response();
   dat=IIC_RecByte();
   IIC_Stop();
   IIC_Stop();

   return dat;

}

