#include "string.h"
#include "delay.h"
#include "uart.h"
#include "temp.h"

#define Buf2_Max 200 //串口二缓存长度

/*************  本地变量声明	**************/
xdata u8 Uart2_Buf[Buf2_Max];
unsigned char Datas_Buff[8] = {0, 0, 0, 0, 0, 0, 0, '\0'};
u8 First_Int = 0;

/*************	本地函数声明	**************/
void CLR_Buf2(void);
void Temp_Change(int temp, unsigned char *Buff);

/*******************************************************************************
* 函数名 : main 
* 描述   : 主函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 串口2负责与GPRS模块通信，串口1用于串口调试，可以避免在下载程序时数据
					 还发送到模块
*******************************************************************************/
void main(void)
{
	int i = 0, send_flag = 0;
	Uart1Init();
	Uart2Init();
	EA = 0;
	UART1_SendString("初始化完成\r\n");

	for (i = 0; i < 3; i++)
	{
		Temp_Change(Ds18b20ReadTemp(), Datas_Buff);
		delay_ms(500);
	}

	while (1)
	{
		Temp_Change(Ds18b20ReadTemp(), Datas_Buff);
		UART2_SendData(0x12);
		for (i = 1; i < 4; i++)
		{
			UART2_SendData(Datas_Buff[i]);
		}
		delay_ms(800);
	}
}

/*******************************************************************************
* 函数名 : Uart1 
* 描述   : 串口1中断服务入口函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Uart1() interrupt 4
{
	if (RI)
	{
		RI = 0; //清除RI位
	}
	if (TI)
	{
		TI = 0; //清除TI位
	}
}

/*******************************************************************************
* 函数名 : Uart2
* 描述   : 串口2中断服务入口函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Uart2() interrupt 8
{
	IE2 &= ~0x01; //关闭串口2中断
	if (S2CON & S2RI)
	{
		S2CON &= ~S2RI;				  //清除S2RI位
		Uart2_Buf[First_Int] = S2BUF; //将接收到的字符串存到缓存中
		First_Int++;				  //缓存指针向后移动
		if (First_Int > Buf2_Max)	 //如果缓存满,将缓存指针指向缓存的首地址
		{
			First_Int = 0;
		}
	}
	if (S2CON & S2TI)
	{
		S2CON &= ~S2TI; //清除S2TI位
	}
	IE2 |= 0x01; //使能串口2中断
}

/*******************************************************************************
* 函数名 : CLR_Buf2
* 描述   : 清除串口2缓存数据
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void CLR_Buf2(void)
{
	u16 k;
	for (k = 0; k < Buf2_Max; k++) //将缓存内容清零
	{
		Uart2_Buf[k] = 0x00;
	}
	First_Int = 0; //接收字符串的起始存储位置
}

void Temp_Change(int temp, unsigned char *Buff)
{
	float tp;
	if (temp < 0) //当温度值为负数
	{
		Buff[0] = '-';
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp = temp - 1;
		temp = ~temp;
		tp = temp;
		temp = tp * 0.0625 * 100 + 0.5;
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算由?.5，还是在小数点后面。
	}
	else
	{
		Buff[0] = '+';
		tp = temp; //因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp = tp * 0.0625 * 100 + 0.5;
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面
	}
	Buff[1] = temp / 10000;
	Buff[2] = temp % 10000 / 1000;
	Buff[3] = temp % 1000 / 100;
	Buff[4] = '.';
	Buff[5] = temp % 100 / 10;
	Buff[6] = temp % 10;
}
