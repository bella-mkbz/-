#include "delay.h"
#include "uart.h"

/*************************���س��������*****************************/
#define Buf2_Max 256                    // ����2������󳤶�
u8 PWM = 0;                            // PWMֵ
bit Rec_Flag;                           // ����2֡���ݽ�����ɱ�־
unsigned char Rec_Len = 0;              // ����2�������ݳ���
xdata unsigned char Rec_Buff[Buf2_Max]; // ����2�������ݻ�����
sbit PWM_Port = P2^3;                   // PWM����˿�
sbit Motor_A = P2^4;
sbit Motor_B = P2^5;
/*********************************************************************/

/*************************���غ�������********************************/
void Timer0Init(void);
void Timer1Init(void);	
void CLR_Buf2(void);
/*********************************************************************/

void main(void)
{
		u8 i = 0;
		Timer0Init();
		Timer1Init();	
		Uart1Init();
		Uart2Init();
		Motor_A = 1;
		Motor_B = 0;
		PWM_Port = 1;
		delay_ms(1000);
		EA = 1;
	  for(;;)
	  {
			if(Rec_Flag)
			{
				Rec_Flag = 0;
				if(Rec_Len < 3 && (Rec_Buff[0] - 0x30) <=9 && (Rec_Buff[0] - 0x30) >= 0 && (Rec_Buff[1] - 0x30) <= 9 && (Rec_Buff[1] - 0x30) >= 0) 
				{
					PWM = (Rec_Buff[0] - 0x30) * 10 + (Rec_Buff[1] - 0x30);
				}
				if(Rec_Buff[0] == '+')
				{
					Motor_A = 1;
					Motor_B = 0;
				}
				else if(Rec_Buff[0] == '-')
				{
					Motor_A = 0;
					Motor_B = 1;
				}
				CLR_Buf2();
			}
		}
}

void Uart2() interrupt 8
{
		IE2  &= ~0x01;   //�رմ���2�ж�
    if (S2CON & S2RI)
    {
      S2CON &= ~S2RI;                 //���S2RIλ
	    ET1 = 0;                        //�رն�ʱ��1�ж�
			Rec_Buff[Rec_Len] = S2BUF;  	  //�����յ����ַ����浽������
			Rec_Len++;                			//����ָ������ƶ�
			if(Rec_Len > Buf2_Max)       		//���������,������ָ��ָ�򻺴���׵�ַ
			{
				Rec_Len = 0;
			}
	    TL1 = 0xB0;		                  //���ö�ʱ��ֵ
	    TH1 = 0x3C;		                  //���ö�ʱ��ֵ
	    ET1 = 1;                        //�򿪶�ʱ��1�ж�			
    }
    if (S2CON & S2TI)
    {
      S2CON &= ~S2TI;         //���S2TIλ
    }
		IE2  |= 0x01;   //ʹ�ܴ���2�ж�
}

void Timer0() interrupt 1
{
		static int count = 0;
		TR0 = 0;
		count++;
		if(count > 100)
			count = 0;
		if(count >= (100 - PWM ))
			PWM_Port = 1;
		else
			PWM_Port = 0;
		TR0 = 1;
}

void Timer1() interrupt 3
{
		static u8 i = 0;
		TL1 = 0x00;		//���ö�ʱ��ֵ
		TH1 = 0xDC;		//���ö�ʱ��ֵ
		i++;
		if(i == 6)
		{
			Rec_Flag = 1;
			ET1 = 0;	
			i = 0;		
		}
}

void Timer0Init(void)		//500΢��@11.0592MHz
{
		AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
		TMOD &= 0xF0;		//���ö�ʱ��ģʽ
		TMOD |= 0x02;		//���ö�ʱ��ģʽ
		TL0 = 0x48;		  //���ö�ʱ��ֵ
		TH0 = 0x48;		  //���ö�ʱ����ֵ
		TF0 = 0;		//���TF0��־
		TR0 = 1;		//��ʱ��0��ʼ��ʱ
		ET0 = 1;    //�򿪶�ʱ��0�ж�
}

void Timer1Init(void)		//10����@11.0592MHz
{
		AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
		TMOD &= 0x0F;		//���ö�ʱ��ģʽ
		TMOD |= 0x10;		//���ö�ʱ��ģʽ
		TL1 = 0x00;		//���ö�ʱ��ֵ
		TH1 = 0xDC;		//���ö�ʱ��ֵ
		TF1 = 0;		//���TF1��־
		TR1 = 1;		//��ʱ��1��ʼ��ʱ
}

void CLR_Buf2(void)
{
		u16 k;
		for(k=0;k<Buf2_Max;k++)      //��������������
		{
			Rec_Buff[k] = 0x00;
		}
		Rec_Len = 0;              //�����ַ�������ʼ�洢λ��
}