#include "uart.h"
#include <string.h>

#define Length 15
unsigned char Sensor_Result[Length]={0};
bit Sensor_Flag;

void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 43;
	j = 6;
	k = 203;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main()
{
	 int i;
	 Uart1Init();
   Uart2Init();
	 EA = 1;
   while(1)
	 {
		 
		  UART2_SendData(0xa5);
		  UART2_SendData(0x52);
		  UART2_SendData(0xf7);
		  Delay1000ms();
      if(Sensor_Flag && Sensor_Result[0] == 0x5A && Sensor_Result[1] == 0x5A)
			{
				Sensor_Flag = 0;
				UART1_SendData(0x11);
				for(i = 6; i < 10; i++)
				    UART1_SendData(Sensor_Result[i]);
			}
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
        RI = 0;                 //清除RI位
    }

    if (TI)
    {
        TI = 0;                 //清除TI位
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
	  static unsigned char First_Int = 0;
		IE2  &= ~0x01;   //关闭串口2中断
    if (S2CON & S2RI)
    {
      S2CON &= ~S2RI;         //清除S2RI位
			Sensor_Result[First_Int] = S2BUF;  	  //将接收到的字符串存到缓存中
			First_Int++;
			if(First_Int == Length)  //如果缓存满,将缓存指针指向缓存的首地址
			{
				First_Int = 0;
				Sensor_Flag = 1;
			}
    }
    if (S2CON & S2TI)
    {
      S2CON &= ~S2TI;         //清除S2TI位
    }
		IE2  |= 0x01;   //使能串口2中断
}