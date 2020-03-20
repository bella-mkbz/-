#include <reg51.h>
#include "uart.h"
#include "delay.h"


sbit Rs485_R_W = P3^7;
/*************************���س��������*****************************/
#define Buf2_Max 25                    	// ���ڽ�����󳤶�
bit Rec_Flag;                           // ����֡���ݽ�����ɱ�־
unsigned char Adress = 0;
unsigned char Rec_Len = 0;              // ���ڽ������ݳ���
unsigned char Rec_Buff[Buf2_Max]; 			// ���ڽ������ݻ�����
unsigned char Send_Buff[25] = {0x00};		//�������鳤��һ��Ҫ���ڷ��ͳ��ȣ�����У�����
void CLR_Buf2(void);
/*********************************************************************/

void main(void)
{
	unsigned int Crc_Result = 0;
	unsigned char high = 0, Low = 0;
	Adress = P1&0X03;
  Timer1Init();
	SerialInit();
	Rs485_R_W = 0;  // 1����д��0�����
	Delay1ms(10);
	EA = 1;
	while(1)
	{
		if(Rec_Flag)
		{
			Rec_Flag = 0;
			if(Adress == Rec_Buff[0])
			{
				Crc_Result = factory_crc16(Rec_Buff, Rec_Len-2);
				if(Rec_Buff[Rec_Len-2] == (Crc_Result >> 8) && Rec_Buff[Rec_Len-1] == (Crc_Result&0xff))
				{	
					switch (Rec_Buff[1])
          {
          	case 0x01:	//������01
          		break;
          	case 0x03:	//������03
								Send_Buff[0] = Adress;
								Send_Buff[1] = 0x03;
								Send_Buff[2] = Rec_Buff[2];
								Send_Buff[3] = 0x00;
								Send_Buff[4] = Adress;
								Crc_Result = factory_crc16(Send_Buff, Rec_Buff[2]*2+3);
								Send_Buff[Rec_Buff[2]*2+3] = Crc_Result>>8;
								Send_Buff[Rec_Buff[2]*2+4] = Crc_Result&0xff;
								Rs485_R_W = 1;
								Delay1ms(30);
								SendBuff(Send_Buff, Rec_Buff[2]*2+5);
								Delay1ms(30);
								Rs485_R_W = 0;	
          		break;
          	default:
          		break;
          }
				
				}
			}		
			CLR_Buf2();
		}
	}
}

void Uart() interrupt 4
{
    if (RI)
    {
			RI =  0;      				          //���RIλ
	    ET0 = 0;                        //�رն�ʱ��0�ж�
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
    if (TI)
    {
      TI = 0;         //���TIλ
    }
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
