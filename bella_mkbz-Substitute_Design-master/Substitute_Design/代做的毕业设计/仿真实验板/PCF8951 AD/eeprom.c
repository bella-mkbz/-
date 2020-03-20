#include "eeprom"
#include "iic.h"

BOOL IIC_WrDevAddAndDat(uchar DatAdd)
{
  Start();             //起始信号
	InputOneByte(0xA0);  //器件地址
	IIC_Ack();           //应答信号
	InputOneByte(DatAdd);//数据地址
	IIC_Ack();           //应答信号
	
	return TRUE;
}

void IIC_WrDatToAdd(uchar DatAdd,uchar *p,uchar iLen)
{
	uchar iVal;
  IIC_WrDevAddAndDat(DatAdd);
	for(iVal=0;iVal < iLen;iVal++)
	{
	  InputOneByte(*p++);              //输入数据
		IIC_RdAck();                     //读取应答信号
	}
	Stop();
}

void IIC_RdDatFromDarAdd(uchar DatAdd,uchar *p,uchar nLen)
{
	uchar iVal;
  IIC_WrDevAddAndDat(DatAdd);
	InputOneByte(0xA1);  //器件地址
	IIC_RdAck();           //应答信号	
	for(iVal = 0;iVal < nLen;iVal++)
	{  
		*p++ = OutputOneByte();
		if(iVal != (nLen - 1))
			IIC_Ack();
	}
	IIC_nAck();
	Stop();
}