#include "bsp_esp8266.h"
#include "STC12C5Axx.H"

/************���ض���***************/
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

			ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //��������

			delay_ms(1000);
			
			if(strEsp8266_Fram_Record .InfBit .FramFinishFlag)
			{
				strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
				UART1_SendString ( strEsp8266_Fram_Record .Data_RX_BUF );
			}

			
			if ( ucTcpClosedFlag )                                             //����Ƿ�ʧȥ����
			{
				ESP8266_ExitUnvarnishSend ();                                    //�˳�͸��ģʽ
				
				do ucStatus = ESP8266_Get_LinkStatus ();                         //��ȡ����״̬
				while ( ! ucStatus );
				UART1_SendString ( "???@@@@@@@@" );
				if ( ucStatus == 4 )                                             //ȷ��ʧȥ���Ӻ�����
				{
					UART1_SendString ( "���������ȵ�ͷ����� ......" );
					
					while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
					
					while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
					
					UART1_SendString ( "�����ȵ�ͷ������ɹ�" );
					
					ucTcpClosedFlag = 0;

				}
				
				while ( ! ESP8266_UnvarnishSend () );		
				
			}
			
		}
}

void Uart2() interrupt 8
{
		IE2  &= ~0x01;   //�رմ���2�ж�
    if (S2CON & S2RI)
    {
      S2CON &= ~S2RI;                 //���S2RIλ
	    ET1 = 0;                        //�رն�ʱ��1�ж�
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ] = S2BUF;  	  //�����յ����ַ����浽������
			if(strEsp8266_Fram_Record .InfBit .FramLength > 39)       	      	//���������,������ָ��ָ�򻺴���׵�ַ
			{
				strEsp8266_Fram_Record .InfBit .FramLength = 0;
			}
			TL1 = 0x00;		//���ö�ʱ��ֵ
			TH1 = 0xDC;		//���ö�ʱ��ֵ
	    ET1 = 1;                        //�򿪶�ʱ��1�ж�			
    }
    if (S2CON & S2TI)
    {
      S2CON &= ~S2TI;         //���S2TIλ
    }
		IE2  |= 0x01;   //ʹ�ܴ���2�ж�
}


void Timer1() interrupt 3
{
		static u8 i = 0;
		TL1 = 0x00;		//���ö�ʱ��ֵ
		TH1 = 0xDC;		//���ö�ʱ��ֵ
		i++;
		if(i == 20)
		{
			strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
			ucTcpClosedFlag = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
			ET1 = 0;	
			i = 0;		
		}
}


void Timer1Init(void)		//10����@11.0592MHz
{
		AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
		TMOD &= 0x0F;		//���ö�ʱ��ģʽ
		TMOD |= 0x10;		//���ö�ʱ��ģʽ
		TL1 = 0x00;		  //���ö�ʱ��ֵ
		TH1 = 0xDC;		  //���ö�ʱ��ֵ
		TF1 = 0;		    //���TF1��־
		TR1 = 1;		    //��ʱ��1��ʼ��ʱ
}
