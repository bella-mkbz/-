#include "bsp_esp8266.h"

static void                   ESP8266_USART_Config                ( void );
static void                   ESP8266_USART_NVIC_Configuration    ( void );

struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };

/**
  * @brief  ESP8266初始化函数
  * @param  无
  * @retval 无
  */
void ESP8266_Init ( void )
{	
	
	Uart2Init(); 
	
	UART1_SendString("正在配置 ESP8266 ......");
	
	ESP8266_ExitUnvarnishSend ();

	ESP8266_AT_Test ();

	ESP8266_Net_Mode_Choose ( STA );

	while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	

	ESP8266_Enable_MultipleId ( DISABLE );

	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );

	while ( ! ESP8266_UnvarnishSend () );
	
	UART1_SendString("配置 ESP8266 完毕");

	strEsp8266_Fram_Record .InfBit .FramLength = 0;

	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;

	memset(strEsp8266_Fram_Record .Data_RX_BUF, 0, 40);
}

/*
 * 函数名：ESP8266_Cmd
 * 描述  ：对WF-ESP8266模块发送AT指令
 * 输入  ：cmd，待发送的指令
 *         reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
 *         waittime，等待响应的时间
 * 返回  : 1，指令发送成功
 *         0，指令发送失败
 * 调用  ：被外部调用
 */
unsigned char ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //从新开始接收新的数据包

	memset(strEsp8266_Fram_Record .Data_RX_BUF, 0, 40);
	
	UART2_SendString(cmd);

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //不需要接收数据
		return 1;
	
	delay_ms ( waittime );                 //延时
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

	//macPC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
	UART1_SendString(strEsp8266_Fram_Record .Data_RX_BUF);
  
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( unsigned char ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( unsigned char ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( unsigned char ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( unsigned char ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}


void ESP8266_AT_Test ( void )
{
	char count=0;
	
	delay_ms ( 1000 );
	while ( count < 10 )
	{
		if( ESP8266_Cmd ( "AT", "OK", NULL, 500 ) ) return;
		++ count;
	}
}


/*
 * 函数名：ESP8266_Net_Mode_Choose
 * 描述  ：选择WF-ESP8266模块的工作模式
 * 输入  ：enumMode，工作模式
 * 返回  : 1，选择成功
 *         0，选择失败
 * 调用  ：被外部调用
 */
bit ESP8266_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode )
{
	switch ( enumMode )
	{
		case STA:
			return ESP8266_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 
		
	  case AP:
		  return ESP8266_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 
		
		case STA_AP:
		  return ESP8266_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 
		
	  default:
		  return 0;
  }
	
}


/*
 * 函数名：ESP8266_JoinAP
 * 描述  ：WF-ESP8266模块连接外部WiFi
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bit ESP8266_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [30];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return ESP8266_Cmd ( cCmd, "OK", NULL, 5000 );
	
}


/*
 * 函数名：ESP8266_Enable_MultipleId
 * 描述  ：WF-ESP8266模块启动多连接
 * 输入  ：enumEnUnvarnishTx，配置是否多连接
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bit ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [13];
	
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return ESP8266_Cmd ( cStr, "OK", 0, 500 );
	
}


/*
 * 函数名：ESP8266_Link_Server
 * 描述  ：WF-ESP8266模块连接外部服务器
 * 输入  ：enumE，网络协议
 *       ：ip，服务器IP字符串
 *       ：ComNum，服务器端口字符串
 *       ：id，模块连接服务器的ID
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bit ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [35] = { 0 }, cCmd [50];

  switch (  enumE )
  {
		case enumTCP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
		  break;
		
		case enumUDP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
		  break;
		
		default:
			break;
  }

  if ( id < 5 )
    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  else
	  sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

	return ESP8266_Cmd ( cCmd, "OK", "ALREAY CONNECT", 4000 );
	
}


/*
 * 函数名：ESP8266_UnvarnishSend
 * 描述  ：配置WF-ESP8266模块进入透传发送
 * 输入  ：无
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bit ESP8266_UnvarnishSend ( void )
{
	if ( ! ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 1000 ) )
		return 0;
	
	return 
	  ESP8266_Cmd ( "AT+CIPSEND", "OK", ">", 1000 );
	
}


/*
 * 函数名：ESP8266_ExitUnvarnishSend
 * 描述  ：配置WF-ESP8266模块退出透传模式
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
void ESP8266_ExitUnvarnishSend ( void )
{
	delay_ms ( 1000 );
	
	UART2_SendString_P( "+++" );
	
	delay_ms ( 500 ); 
	
}

/*
 * 函数名：ESP8266_SendString
 * 描述  ：WF-ESP8266模块发送字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 *       ：pStr，要发送的字符串
 *       ：ulStrLength，要发送的字符串的字节数
 *       ：ucId，哪个ID发送的字符串
 * 返回  : 1，发送成功
 *         0，发送失败
 * 调用  ：被外部调用
 */
bit ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bit bRet = 0;
	
		
	if ( enumEnUnvarnishTx )
	{
		
		UART2_SendString( pStr );
		
		bRet = 1;
		
	}

	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
		
		ESP8266_Cmd ( cStr, "> ", 0, 1000 );

		bRet = ESP8266_Cmd ( pStr, "SEND OK", 0, 1000 );
  }
	
	return bRet;

}

/*
 * 函数名：ESP8266_Get_LinkStatus
 * 描述  ：获取 WF-ESP8266 的连接状态，较适合单端口时使用
 * 输入  ：无
 * 返回  : 2，获得ip
 *         3，建立连接
 *         3，失去连接
 *         0，获取状态失败
 * 调用  ：被外部调用
 */
uint8_t ESP8266_Get_LinkStatus ( void )
{
	if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 1500 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "2" ) )
			return 2;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "3" ) )
			return 3;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "4" ) )
			return 4;		

	}
	
	return 0;
	
}