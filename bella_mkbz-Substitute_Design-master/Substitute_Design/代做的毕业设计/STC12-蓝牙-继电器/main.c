#include "delay.h"
#include "uart.h"
#include "temp.h"
#include "string.h"

sbit Relay_1 = P2^0;
sbit Relay_2 = P2^1;
sbit Relay_3 = P2^2;
sbit Relay_4 = P2^3;

/*************************本地程序定义变量*****************************/
#define Buf2_Max 256                    // 串口2接收最大长度
bit Rec_Flag;                           // 串口2帧数据接收完成标志
unsigned char Rec_Len = 0;              // 串口2接收数据长度
xdata unsigned char Rec_Buff[Buf2_Max]; // 串口2接收数据缓冲区
unsigned char Datas_Buff[8]= {0, 0, 0, 0, 0, 0, 0,'\0'}; //存放温度数据
/*********************************************************************/

/*************************本地函数定义********************************/
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
			TL1 = 0x00;		//设置定时初值
			TH1 = 0xDC;		//设置定时初值
	    ET1 = 1;      //打开定时器1中断			
    }
    if (S2CON & S2TI)
    {
      S2CON &= ~S2TI;         //清除S2TI位
    }
		IE2  |= 0x01;   //使能串口2中断
}

void Timer1() interrupt 3
{
		static u8 i = 0;
		TL1 = 0x00;		//设置定时初值
		TH1 = 0xDC;		//设置定时初值
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

void Temp_Change(int temp,unsigned char* Buff)
{
	  float tp;  
	  if(temp< 0)				//当温度值为负数
  	{
			Buff[0] = '-';
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算由?.5，还是在小数点后面。
  	}
 	else
  	{	
    Buff[0] = '+';			
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	  }
	Buff[1] = temp / 10000 + 0x30;
	Buff[2] = temp % 10000 / 1000 + 0x30;
	Buff[3] = temp % 1000 / 100 + 0x30;
	Buff[4] = '.';	
	Buff[5] = temp % 100 / 10 + 0x30;
	Buff[6] = temp % 10 + 0x30;
}