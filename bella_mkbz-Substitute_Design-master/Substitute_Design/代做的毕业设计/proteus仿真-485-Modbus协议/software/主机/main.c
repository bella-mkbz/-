#include <reg51.h>
#include "uart.h"
#include "delay.h"
#include "lcd.h"

sbit Rs485_R_W = P3^7;
/*************************本地程序定义变量*****************************/
#define Buf2_Max 30                    // 串口接收最大长度
bit Rec_Flag;                           // 串口帧数据接收完成标志
unsigned char Rec_Len = 0;              // 串口接收数据长度
unsigned char Rec_Buff[Buf2_Max]; // 串口接收数据缓冲区
unsigned char Send_Buff[7] = {0x00,0x03,0x01,0x00,0x01,0x00,0x00};
void CLR_Buf2(void);
/*********************************************************************/

void main(void)
{
	unsigned int Crc_Result = 0;
	LcdInit();	
  Timer1Init();
	SerialInit();
	Rs485_R_W = 1;  // 1代表写，0代表读
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
		ES = 0;   //关闭串口2中断
    if (RI)
    {
			RI =  0;      				          //清除S2RI位
	    ET1 = 0;                        //关闭定时器1中断
			Rec_Buff[Rec_Len] = SBUF;  	    //将接收到的字符串存到缓存中
			Rec_Len++;                			//缓存指针向后移动
			if(Rec_Len > Buf2_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
			{
				Rec_Len = 0;
			}
			TL0 = 0x00;		//设置定时初值
			TH0 = 0xDC;		//设置定时初值
	    ET0 = 1;      //打开定时器1中断			
    }
		ES = 1;   //使能串口2中断
}

void Timer1() interrupt 1
{
		static unsigned char i = 0;
		TL0 = 0x00;		//设置定时初值
		TH0 = 0xDC;		//设置定时初值
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
		for(k = 0;k < Buf2_Max; k++)      //将缓存内容清零
		{
			Rec_Buff[k] = 0x00;
		}
		Rec_Len = 0;              //接收字符串的起始存储位置
}
