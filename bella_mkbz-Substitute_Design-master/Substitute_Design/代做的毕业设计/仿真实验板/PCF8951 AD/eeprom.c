#include "eeprom"
#include "iic.h"

BOOL IIC_WrDevAddAndDat(uchar DatAdd)
{
  Start();             //��ʼ�ź�
	InputOneByte(0xA0);  //������ַ
	IIC_Ack();           //Ӧ���ź�
	InputOneByte(DatAdd);//���ݵ�ַ
	IIC_Ack();           //Ӧ���ź�
	
	return TRUE;
}

void IIC_WrDatToAdd(uchar DatAdd,uchar *p,uchar iLen)
{
	uchar iVal;
  IIC_WrDevAddAndDat(DatAdd);
	for(iVal=0;iVal < iLen;iVal++)
	{
	  InputOneByte(*p++);              //��������
		IIC_RdAck();                     //��ȡӦ���ź�
	}
	Stop();
}

void IIC_RdDatFromDarAdd(uchar DatAdd,uchar *p,uchar nLen)
{
	uchar iVal;
  IIC_WrDevAddAndDat(DatAdd);
	InputOneByte(0xA1);  //������ַ
	IIC_RdAck();           //Ӧ���ź�	
	for(iVal = 0;iVal < nLen;iVal++)
	{  
		*p++ = OutputOneByte();
		if(iVal != (nLen - 1))
			IIC_Ack();
	}
	IIC_nAck();
	Stop();
}