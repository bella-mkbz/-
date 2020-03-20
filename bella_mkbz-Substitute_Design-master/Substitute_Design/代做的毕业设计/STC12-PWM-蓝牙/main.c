#include "delay.h"
#include "uart.h"

/*************************本地程序定义变量*****************************/
#define Buf2_Max 256                    // 串口2接收最大长度
u8 PWM = 0;                            // PWM值
bit Rec_Flag;                           // 串口2帧数据接收完成标志
unsigned char Rec_Len = 0;              // 串口2接收数据长度
xdata unsigned char Rec_Buff[Buf2_Max]; // 串口2接收数据缓冲区
sbit PWM_Port = P2^3;                   // PWM输出端口
sbit Motor_A = P2^4;
sbit Motor_B = P2^5;
/*********************************************************************/

/*************************本地函数定义********************************/
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
		IE2  &= ~0x01;   //关闭串口2中断
    if (S2CON & S2RI)
    {
      S2CON &= ~S2RI;                 //清除S2RI位
	    ET1 = 0;                        //关闭定时器1中断
			Rec_Buff[Rec_Len] = S2BUF;  	  //将接收到的字符串存到缓存中
			Rec_Len++;                			//缓存指针向后移动
			if(Rec_Len > Buf2_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
			{
				Rec_Len = 0;
			}
	    TL1 = 0xB0;		                  //设置定时初值
	    TH1 = 0x3C;		                  //设置定时初值
	    ET1 = 1;                        //打开定时器1中断			
    }
    if (S2CON & S2TI)
    {
      S2CON &= ~S2TI;         //清除S2TI位
    }
		IE2  |= 0x01;   //使能串口2中断
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
		TL1 = 0x00;		//设置定时初值
		TH1 = 0xDC;		//设置定时初值
		i++;
		if(i == 6)
		{
			Rec_Flag = 1;
			ET1 = 0;	
			i = 0;		
		}
}

void Timer0Init(void)		//500微秒@11.0592MHz
{
		AUXR &= 0x7F;		//定时器时钟12T模式
		TMOD &= 0xF0;		//设置定时器模式
		TMOD |= 0x02;		//设置定时器模式
		TL0 = 0x48;		  //设置定时初值
		TH0 = 0x48;		  //设置定时重载值
		TF0 = 0;		//清除TF0标志
		TR0 = 1;		//定时器0开始计时
		ET0 = 1;    //打开定时器0中断
}

void Timer1Init(void)		//10毫秒@11.0592MHz
{
		AUXR &= 0xBF;		//定时器时钟12T模式
		TMOD &= 0x0F;		//设置定时器模式
		TMOD |= 0x10;		//设置定时器模式
		TL1 = 0x00;		//设置定时初值
		TH1 = 0xDC;		//设置定时初值
		TF1 = 0;		//清除TF1标志
		TR1 = 1;		//定时器1开始计时
}

void CLR_Buf2(void)
{
		u16 k;
		for(k=0;k<Buf2_Max;k++)      //将缓存内容清零
		{
			Rec_Buff[k] = 0x00;
		}
		Rec_Len = 0;              //接收字符串的起始存储位置
}