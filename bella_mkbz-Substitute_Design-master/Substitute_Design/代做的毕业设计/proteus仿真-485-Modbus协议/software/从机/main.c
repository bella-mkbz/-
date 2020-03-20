#include <reg51.h>
#include "uart.h"
#include "delay.h"


sbit Rs485_R_W = P3^7;
/*************************本地程序定义变量*****************************/
#define Buf2_Max 25                    	// 串口接收最大长度
bit Rec_Flag;                           // 串口帧数据接收完成标志
unsigned char Adress = 0;
unsigned char Rec_Len = 0;              // 串口接收数据长度
unsigned char Rec_Buff[Buf2_Max]; 			// 串口接收数据缓冲区
unsigned char Send_Buff[25] = {0x00};		//申请数组长度一定要大于发送长度，否则校验出错
void CLR_Buf2(void);
/*********************************************************************/

void main(void)
{
	unsigned int Crc_Result = 0;
	unsigned char high = 0, Low = 0;
	Adress = P1&0X03;
  Timer1Init();
	SerialInit();
	Rs485_R_W = 0;  // 1代表写，0代表读
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
          	case 0x01:	//功能码01
          		break;
          	case 0x03:	//功能码03
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
			RI =  0;      				          //清除RI位
	    ET0 = 0;                        //关闭定时器0中断
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
    if (TI)
    {
      TI = 0;         //清除TI位
    }
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
