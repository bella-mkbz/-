#include "delay.h"
#include "uart.h"
#include "temp.h"
#include "string.h"

sbit Relay_1 = P2^0;
sbit Relay_2 = P2^1;
sbit Relay_3 = P2^2;
sbit Relay_4 = P2^3;

/*************************���س��������*****************************/
#define Buf2_Max 256                    // ����2������󳤶�
bit Rec_Flag;                           // ����2֡���ݽ�����ɱ�־
unsigned char Rec_Len = 0;              // ����2�������ݳ���
xdata unsigned char Rec_Buff[Buf2_Max]; // ����2�������ݻ�����
unsigned char Datas_Buff[8]= {0, 0, 0, 0, 0, 0, 0,'\0'}; //����¶�����
/*********************************************************************/

/*************************���غ�������********************************/
void Timer0Init(void);
void Timer1Init(void);	
void CLR_Buf2(void);
void Temp_Change(int temp,unsigned char* Buff);
/*********************************************************************/

void main(void)
{
		u8 i = 0;
		Timer1Init();	
		Uart1Init();
		Uart2Init();
		delay_ms(1000);
		EA = 1;
	  for(;;)
	  {
			Temp_Change(Ds18b20ReadTemp(),Datas_Buff);
			UART2_SendString(Datas_Buff);
			if((Datas_Buff[2] - 0x30) >= 3 &&  (Datas_Buff[3] - 0x30) >= 0)
				Relay_4 = 0;
			else
				Relay_4 = 1;
			UART2_SendData(0x0d);
			UART2_SendData(0x0a);
			UART2_SendData(0x0d);
			UART2_SendData(0x0a);
			UART2_SendData(0x0d);
			UART2_SendData(0x0a);
			delay_ms(1000);
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
			TL1 = 0x00;		//���ö�ʱ��ֵ
			TH1 = 0xDC;		//���ö�ʱ��ֵ
	    ET1 = 1;      //�򿪶�ʱ��1�ж�			
    }
    if (S2CON & S2TI)
    {
      S2CON &= ~S2TI;         //���S2TIλ
    }
		IE2  |= 0x01;   //ʹ�ܴ���2�ж�
}

void Timer1() interrupt 3
{
		static u8 i = 0;
		TL1 = 0x00;		//���ö�ʱ��ֵ
		TH1 = 0xDC;		//���ö�ʱ��ֵ
		i++;
		if(i == 10)
		{
			Rec_Flag = 1;
			ET1 = 0;	
			i = 0;		
		}
		if(Rec_Flag)
		{
			Rec_Flag = 0;
			UART1_SendString(Rec_Buff);
			if(strstr(Rec_Buff,"1on") != NULL)
			{
				Relay_1 = 0;
			}
			if(strstr(Rec_Buff,"2on") != NULL)
			{
				Relay_2 = 0;
			}
			if(strstr(Rec_Buff,"3on") != NULL)
			{
				Relay_3 = 0;
			}
			if(strstr(Rec_Buff,"4on") != NULL)
			{
				Relay_4 = 0;
			}
			if(strstr(Rec_Buff,"All on") != NULL)
			{
				Relay_1 = 0;
				Relay_2 = 0;
				Relay_3 = 0;
				Relay_4 = 0;
			}
			if(strstr(Rec_Buff,"1off") != NULL)
			{
				Relay_1 = 1;
			}
			if(strstr(Rec_Buff,"2off") != NULL)
			{
				Relay_2 = 1;
			}
			if(strstr(Rec_Buff,"3off") != NULL)
			{
				Relay_3 = 1;
			}
			if(strstr(Rec_Buff,"4off") != NULL)
			{
				Relay_4 = 1;
			}
			if(strstr(Rec_Buff,"All off") != NULL)
			{
				Relay_1 = 1;
				Relay_2 = 1;
				Relay_3 = 1;
				Relay_4 = 1;
			}
			CLR_Buf2();
		}
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

void Temp_Change(int temp,unsigned char* Buff)
{
	  float tp;  
	  if(temp< 0)				//���¶�ֵΪ����
  	{
			Buff[0] = '-';
		//��Ϊ��ȡ���¶���ʵ���¶ȵĲ��룬���Լ�1����ȡ�����ԭ��
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//����?.5��������С������档
  	}
 	else
  	{	
    Buff[0] = '+';			
		tp=temp;//��Ϊ���ݴ�����С�������Խ��¶ȸ���һ�������ͱ���
		//����¶���������ô����ô������ԭ����ǲ���������
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
	  }
	Buff[1] = temp / 10000 + 0x30;
	Buff[2] = temp % 10000 / 1000 + 0x30;
	Buff[3] = temp % 1000 / 100 + 0x30;
	Buff[4] = '.';	
	Buff[5] = temp % 100 / 10 + 0x30;
	Buff[6] = temp % 10 + 0x30;
}