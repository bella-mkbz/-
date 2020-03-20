#include "uart.h"
#include <string.h>

#define Length 15
unsigned char Sensor_Result[Length]={0};
bit Sensor_Flag;

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

void main()
{
	 int i;
	 Uart1Init();
   Uart2Init();
	 EA = 1;
   while(1)
	 {
		 
		  UART2_SendData(0xa5);
		  UART2_SendData(0x52);
		  UART2_SendData(0xf7);
		  Delay1000ms();
      if(Sensor_Flag && Sensor_Result[0] == 0x5A && Sensor_Result[1] == 0x5A)
			{
				Sensor_Flag = 0;
				UART1_SendData(0x11);
				for(i = 6; i < 10; i++)
				    UART1_SendData(Sensor_Result[i]);
			}
	 }
}

/*******************************************************************************
* ������ : Uart1 
* ����   : ����1�жϷ�����ں���
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Uart1() interrupt 4
{
	  
    if (RI)
    {
        RI = 0;                 //���RIλ
    }

    if (TI)
    {
        TI = 0;                 //���TIλ
    }
}

/*******************************************************************************
* ������ : Uart2
* ����   : ����2�жϷ�����ں���
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Uart2() interrupt 8
{
	  static unsigned char First_Int = 0;
		IE2  &= ~0x01;   //�رմ���2�ж�
    if (S2CON & S2RI)
    {
      S2CON &= ~S2RI;         //���S2RIλ
			Sensor_Result[First_Int] = S2BUF;  	  //�����յ����ַ����浽������
			First_Int++;
			if(First_Int == Length)  //���������,������ָ��ָ�򻺴���׵�ַ
			{
				First_Int = 0;
				Sensor_Flag = 1;
			}
    }
    if (S2CON & S2TI)
    {
      S2CON &= ~S2TI;         //���S2TIλ
    }
		IE2  |= 0x01;   //ʹ�ܴ���2�ж�
}