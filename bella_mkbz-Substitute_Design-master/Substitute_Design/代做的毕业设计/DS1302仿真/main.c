#include <reg51.h>
#include "button.h"
#include "lcd.h"
#include "ds1302.h"

sbit Led = P3^7;
unsigned char Key_Value = 0;		//����ֵ
unsigned char second,minute,hour,day,month,year;      //�ֱ𴢴��硢�֡�Сʱ���գ��£���  
unsigned char ReadValue;   //�����1302��ȡ������
void Timer0_Init(void);

void main(void)
{ 	
	LcdInit();
	Show_String(0x81,"Date:");
  LcdWriteCom(0x88);  //д���·ָ�������ʾ��ַ�� ��ʾ�ڵ�1�е�9��
  LcdWriteData('-');      //���ַ�����д��LCD
  LcdWriteCom(0x8b);  //д���շָ�������ʾ��ַ�� ��ʾ�ڵ�1�е�12��
  LcdWriteData('-');      //���ַ�����д��LCD
  LcdWriteCom(0xc5);  //дСʱ����ӷָ�������ʾ��ַ�� ��ʾ�ڵ�2�е�6��
  LcdWriteData(':');      //���ַ�����д��LCD
  LcdWriteCom(0xc8);  		//д��������ָ�������ʾ��ַ�� ��ʾ�ڵ�2�е�9��
  LcdWriteData(':');      //���ַ�����д��LCD
	Init_DS1302();
	Timer0_Init();
	EA = 1;
	while(1)
	 {
		 if(KEY1 == 0)
		 {
				if(KEY1 == 0)
				{
					while(!KEY1);
					TR0 = 0;
					LcdWriteCom(0xc9);
					LcdWriteCom(0x0f);					
				}
		 }
		 if(KEY2 == 0)
		 {
				if(KEY2 == 0)
				{
					while(!KEY2);
					WriteSet1302(0x82,((4/10)<<4|(4%10)));	
					DisplayMinute(4);
					LcdWriteCom(0xc6);
				}
		 }
		 if(KEY3 == 0)
		 {
				if(KEY3 == 0)
				{
					while(!KEY3);
					LcdWriteCom(0xce);
					LcdWriteData(3 + 0x30);					
				}
		 }
		 if(KEY4 == 0)
		 {
				if(KEY4 == 0)
				{
					while(!KEY4);
					LcdWriteCom(0x0C);
					ReadValue = ReadSet1302(0x81);   //����Ĵ���������
					second=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);//����������ת��
					DisplaySecond(second);          //��ʾ��
					TR0 = 1;
				}
		 }
	 }
}

void Timer0_Init(void)
{
	TMOD &= 0XF0;
	TMOD |= 0X01;
	ET0 = 1;
	TR0 = 1;
	TH0 = 0X3C;
	TL0 = 0XB0;
}

void timer0(void) interrupt 1
{
	static unsigned char i = 0;
	TH0 = 0X3C;
	TL0 = 0XB0;
	i++;
	if(i == 8)
	{
		i = 0;
		ReadValue = ReadSet1302(0x81);   //����Ĵ���������
		second=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);//����������ת��
		DisplaySecond(second);          //��ʾ��
		ReadValue = ReadSet1302(0x83);  //�ӷּĴ�����
		minute=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��
		DisplayMinute(minute);	       //��ʾ��
		ReadValue = ReadSet1302(0x85);  //�ӷּĴ�����
		hour=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��
		DisplayHour(hour);	             //��ʾСʱ
		ReadValue = ReadSet1302(0x87);  //�ӷּĴ�����
		day=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��
		DisplayDay(day);                //��ʾ��
		ReadValue = ReadSet1302(0x89);  //�ӷּĴ�����
		month=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��
		DisplayMonth(month);            //��ʾ��
		ReadValue = ReadSet1302(0x8d);  //�ӷּĴ�����
		year=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��
		DisplayYear(year);              //��ʾ��		
	}
}

