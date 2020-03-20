#include <reg51.h>
#include "button.h"
#include "lcd.h"
#include "ds1302.h"

sbit Led = P3^7;
unsigned char Key_Value = 0;		//按键值
unsigned char second,minute,hour,day,month,year;      //分别储存苗、分、小时，日，月，年  
unsigned char ReadValue;   //储存从1302读取的数据
void Timer0_Init(void);

void main(void)
{ 	
	LcdInit();
	Show_String(0x81,"Date:");
  LcdWriteCom(0x88);  //写年月分隔符的显示地址， 显示在第1行第9列
  LcdWriteData('-');      //将字符常量写入LCD
  LcdWriteCom(0x8b);  //写月日分隔符的显示地址， 显示在第1行第12列
  LcdWriteData('-');      //将字符常量写入LCD
  LcdWriteCom(0xc5);  //写小时与分钟分隔符的显示地址， 显示在第2行第6列
  LcdWriteData(':');      //将字符常量写入LCD
  LcdWriteCom(0xc8);  		//写分钟与秒分隔符的显示地址， 显示在第2行第9列
  LcdWriteData(':');      //将字符常量写入LCD
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
					ReadValue = ReadSet1302(0x81);   //从秒寄存器读数据
					second=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);//将读出数据转化
					DisplaySecond(second);          //显示秒
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
		ReadValue = ReadSet1302(0x81);   //从秒寄存器读数据
		second=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);//将读出数据转化
		DisplaySecond(second);          //显示秒
		ReadValue = ReadSet1302(0x83);  //从分寄存器读
		minute=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化
		DisplayMinute(minute);	       //显示分
		ReadValue = ReadSet1302(0x85);  //从分寄存器读
		hour=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化
		DisplayHour(hour);	             //显示小时
		ReadValue = ReadSet1302(0x87);  //从分寄存器读
		day=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化
		DisplayDay(day);                //显示日
		ReadValue = ReadSet1302(0x89);  //从分寄存器读
		month=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化
		DisplayMonth(month);            //显示月
		ReadValue = ReadSet1302(0x8d);  //从分寄存器读
		year=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化
		DisplayYear(year);              //显示年		
	}
}

