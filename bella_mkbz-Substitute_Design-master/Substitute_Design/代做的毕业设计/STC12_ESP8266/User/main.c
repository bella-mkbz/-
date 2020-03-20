#include "bsp_esp8266.h"
#include "STC12C5Axx.H"

/************本地定义***************/
void Timer1Init(void);	
volatile uint8_t ucTcpClosedFlag = 0;
/***************************************/

void main(void)
{
		char cStr [ 100 ] = { 0 };
		unsigned char ucStatus;
		Timer1Init();	
		Uart1Init();
		EA = 1;
		ESP8266_Init();
	  delay_ms(200);
	  for(;;)
	  {
			sprintf ( cStr,"TCP TEST !!! \r\n" );

			ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //发送数据

			delay_ms(1000);
			
			if(strEsp8266_Fram_Record .InfBit .FramFinishFlag)
			{
				strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
				UART1_SendString ( strEsp8266_Fram_Record .Data_RX_BUF );
			}

			
			if ( ucTcpClosedFlag )                                             //检测是否失去连接
			{
				ESP8266_ExitUnvarnishSend ();                                    //退出透传模式
				
				do ucStatus = ESP8266_Get_LinkStatus ();                         //获取连接状态
				while ( ! ucStatus );
				UART1_SendString ( "???@@@@@@@@" );
				if ( ucStatus == 4 )                                             //确认失去连接后重连
				{
					UART1_SendString ( "正在重连热点和服务器 ......" );
					
					while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
					
					while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
					
					UART1_SendString ( "重连热点和服务器成功" );
					
					ucTcpClosedFlag = 0;

				}
				
				while ( ! ESP8266_UnvarnishSend () );		
				
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
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ] = S2BUF;  	  //将接收到的字符串存到缓存中
			if(strEsp8266_Fram_Record .InfBit .FramLength > 39)       	      	//如果缓存满,将缓存指针指向缓存的首地址
			{
				strEsp8266_Fram_Record .InfBit .FramLength = 0;
			}
			TL1 = 0x00;		//设置定时初值
			TH1 = 0xDC;		//设置定时初值
	    ET1 = 1;                        //打开定时器1中断			
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
		if(i == 20)
		{
			strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
			ucTcpClosedFlag = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
			ET1 = 0;	
			i = 0;		
		}
}


void Timer1Init(void)		//10毫秒@11.0592MHz
{
		AUXR &= 0xBF;		//定时器时钟12T模式
		TMOD &= 0x0F;		//设置定时器模式
		TMOD |= 0x10;		//设置定时器模式
		TL1 = 0x00;		  //设置定时初值
		TH1 = 0xDC;		  //设置定时初值
		TF1 = 0;		    //清除TF1标志
		TR1 = 1;		    //定时器1开始计时
}
