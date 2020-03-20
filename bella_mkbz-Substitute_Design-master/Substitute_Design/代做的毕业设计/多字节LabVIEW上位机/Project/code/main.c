#include "1602.h"
#include "uart.h"
#include "stc12_adc.h"
#include <string.h>

#define Length 24
unsigned char Sensor_Result[Length]={0}, Carbon_Dio, Host_Buff[7]={0};
bit Sensor_Flag, Host_Flag, WY_Flag;
unsigned int Illumination, Temperature, Humidity, Water, Fertilizer; //光照, 温度, 湿度
unsigned char BV, C;

void main()
{
	 int i;
	 InitADC();
	 Uart1Init();
   Uart2Init();
   LCD_Init();                 
	 LCD_Write_String(6,0,"Auto");  //7
   LCD_Write_String(2,0,"W_F");
	 LCD_Write_String(12,0,"W_Y");
	 LCD_Write_Char(3,1,':');
	 LCD_Write_Char(13,1,':');
	 LCD_Write_String(0,2,"Tem:");
	 LCD_Write_String(8,2,"Humi:");
	 LCD_Write_String(0,3,"ill:");
	 LCD_Write_String(8,3,"Carb:");
	 EA = 1;
   while(1)
	 {
      if(Sensor_Flag && Sensor_Result[2] == 0x15 && Sensor_Result[11] == 0x45)
			{
				Sensor_Flag = 0;
			  Illumination = (((unsigned int)Sensor_Result[6] << 8) | Sensor_Result[7])/100;
				Temperature = (((unsigned int)Sensor_Result[13] << 8) | Sensor_Result[14])/100;
			  Humidity = (((unsigned int)Sensor_Result[19] << 8) | Sensor_Result[20])/100;
				Carbon_Dio = GetADCResult();
				UART1_SendData(Temperature);
				UART1_SendData(Humidity);
				UART1_SendData(Illumination);
				UART1_SendData(Carbon_Dio);
				for(i = 0; i < Length; i++)
				    Sensor_Result[i] = 0;
				LCD_Write_Num(4,2,Temperature);
			  LCD_Write_Num(13,2,Humidity);
			  LCD_Write_Num(4,3,Illumination);
			  LCD_Write_Num(13,3,Carbon_Dio);
			}
			if(Host_Flag)
			{
				if(BV-2 == 2)
				{
					Water = ((Host_Buff[1] - 0x30) * 10) + (Host_Buff[2] - 0x30);
          LCD_Write_Char(1,1,Water/10 + 0x30);	
          LCD_Write_Char(2,1,Water%10 + 0x30);						
				}
				else
				{
				  Water = Host_Buff[1] - 0x30;
          LCD_Write_Char(1,1,' ');	
          LCD_Write_Char(2,1,Water%10 + 0x30);					
				}
			switch (C-BV-1)
      {
      	case 1:
				  Fertilizer = Host_Buff[BV] - 0x30;	
          LCD_Write_Char(4,1,' ');	
          LCD_Write_Char(5,1,Fertilizer%10 + 0x30);	
      		break;
      	case 2:
          Fertilizer = ((Host_Buff[BV] - 0x30) * 10) + (Host_Buff[BV+1] - 0x30);				
          LCD_Write_Char(4,1,Fertilizer/10 + 0x30);	
          LCD_Write_Char(5,1,Fertilizer%10 + 0x30);
      		break;
      	default:
      		break;
      }
				for(i = 0; i < 7; i++)
				    Host_Buff[i] = 0;			  
				Host_Flag = BV = C = 0;
			}
			if(WY_Flag)
			{
				if(BV-2 == 2)
				{
					Water = ((Host_Buff[1] - 0x30) * 10) + (Host_Buff[2] - 0x30);
          LCD_Write_Char(11,1,Water/10 + 0x30);	
          LCD_Write_Char(12,1,Water%10 + 0x30);						
				}
				else
				{
				  Water = Host_Buff[1] - 0x30;
          LCD_Write_Char(11,1,' ');	
          LCD_Write_Char(12,1,Water%10 + 0x30);					
				}
				switch (C-BV-1)
				{
					case 1:
						Fertilizer = Host_Buff[BV] - 0x30;	
						LCD_Write_Char(14,1,' ');	
						LCD_Write_Char(15,1,Fertilizer%10 + 0x30);	
						break;
					case 2:
						Fertilizer = ((Host_Buff[BV] - 0x30) * 10) + (Host_Buff[BV+1] - 0x30);				
						LCD_Write_Char(14,1,Fertilizer/10 + 0x30);	
						LCD_Write_Char(15,1,Fertilizer%10 + 0x30);
						break;
					default:
						break;
				}
				for(i = 0; i < 7; i++)
				    Host_Buff[i] = 0;			  
				WY_Flag = BV = C = 0;
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
	  static unsigned char First_Int = 0, temp;
    if (RI)
    {
        RI = 0;                 //清除RI位
			  temp = SBUF;
			  if(temp == 'A' || Host_Buff[0]== 'A')
				{
				   Host_Buff[First_Int++] = temp;
					 if(temp == 'B')
						  BV = First_Int;
					 else if(temp == 'C')
					 {
					    C = First_Int;
						  Host_Flag = 1;
						  First_Int = 0;
					 }						  
				}
				else if(temp == 'D' || Host_Buff[0]== 'D')
				{
				   Host_Buff[First_Int++] = temp;
					 if(temp == 'E')
						  BV = First_Int;
					 else if(temp == 'F')
					 {
					    C = First_Int;
						  WY_Flag = 1;
						  First_Int = 0;
					 }					
				}
				else if(temp == 'M')
				   LCD_Write_String(6,0,"Manu");
				else if(temp == 'N')
				   LCD_Write_String(6,0,"Auto");
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