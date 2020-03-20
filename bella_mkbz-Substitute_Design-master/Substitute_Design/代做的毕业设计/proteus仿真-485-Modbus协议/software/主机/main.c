#include <reg51.h>
#include "uart.h"
#include "delay.h"
#include "lcd.h"

sbit Rs485_R_W = P3^7;
/*************************���س��������*****************************/
#define Buf2_Max 30                    // ���ڽ�����󳤶�
bit Rec_Flag;                           // ����֡���ݽ�����ɱ�־
unsigned char Rec_Len = 0;              // ���ڽ������ݳ���
unsigned char Rec_Buff[Buf2_Max]; // ���ڽ������ݻ�����
unsigned char Send_Buff[7] = {0x00,0x03,0x01,0x00,0x01,0x00,0x00};
void CLR_Buf2(void);
/*********************************************************************/

void main(void)
{
	unsigned int Crc_Result = 0;
	LcdInit();	
  Timer1Init();
	SerialInit();
	Rs485_R_W = 1;  // 1����д��0�����
	Delay1ms(10);
	EA = 1;
	while(1)
	{
		Send_Buff[0]++;
		if(Send_Buff[0] == 3)
			Send_Buff[0] = 1;
		Crc_Result = factory_crc16(Send_Buff, 5);
		Send_Buff[5] = Crc_Result>>8;
		Send_Buff[6] = Crc_Result&0xff;
		Rs485_R_W = 1;
		SendBuff(Send_Buff, 7);
		Delay1ms(10);
		Rs485_R_W = 0;
		Delay1ms(900);
	}
}

void Uart() interrupt 4
{
		ES = 0;   //�رմ���2�ж�
    if (RI)
    {
			RI =  0;      				          //���S2RIλ
	    ET1 = 0;                        //�رն�ʱ��1�ж�
			Rec_Buff[Rec_Len] = SBUF;  	    //�����յ����ַ����浽������
			Rec_Len++;                			//����ָ������ƶ�
			if(Rec_Len > Buf2_Max)       		//���������,������ָ��ָ�򻺴���׵�ַ
			{
				Rec_Len = 0;
			}
			TL0 = 0x00;		//���ö�ʱ��ֵ
			TH0 = 0xDC;		//���ö�ʱ��ֵ
	    ET0 = 1;      //�򿪶�ʱ��1�ж�			
    }
		ES = 1;   //ʹ�ܴ���2�ж�
}

void Timer1() interrupt 1
{
		static unsigned char i = 0;
		TL0 = 0x00;		//���ö�ʱ��ֵ
		TH0 = 0xDC;		//���ö�ʱ��ֵ
		i++;
		if(i == 10)
		{
			Rec_Flag = 1;
			ET0 = 0;	
			i = 0;		
		}
		if(Rec_Flag)
		{
			Rec_Flag = 0;
			
			CLR_Buf2();
		}
}

void CLR_Buf2(void)
{
		unsigned int k;
		for(k = 0;k < Buf2_Max; k++)      //��������������
		{
			Rec_Buff[k] = 0x00;
		}
		Rec_Len = 0;              //�����ַ�������ʼ�洢λ��
}
