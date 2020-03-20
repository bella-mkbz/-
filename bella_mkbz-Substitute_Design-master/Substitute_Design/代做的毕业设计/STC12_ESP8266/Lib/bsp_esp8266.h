#ifndef  __BSP_ESP8266_H
#define	 __BSP_ESP8266_H

#include <stdio.h>
#include <string.h>  
#include "delay.h"
#include "uart.h"


#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


/********************************** 用户需要设置的参数**********************************/
#define      macUser_ESP8266_ApSsid                       "Hot"                //要连接的热点的名称
#define      macUser_ESP8266_ApPwd                        "xc5211314"           //要连接的热点的密钥

//#define      macUser_ESP8266_TcpServer_IP                 "47.92.70.175"      //要连接的服务器的 IP
#define      macUser_ESP8266_TcpServer_Port               "50436"               //要连接的服务器的端口
#define      macUser_ESP8266_TcpServer_IP                 "180.97.81.180"      //要连接的服务器的 IP


/******************************* ESP8266 数据类型定义 ***************************/
typedef enum{
	STA,
  AP,
  STA_AP  
} ENUM_Net_ModeTypeDef;


typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;
	

typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
	

typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

typedef unsigned          char uint8_t;


/******************************* ESP8266 外部全局变量声明 ***************************/
#define RX_BUF_MAX_LEN                     40                      //最大接收缓存字节数

extern struct  STRUCT_USARTx_Fram                                  //串口数据帧的处理结构体
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];

  struct {
		  unsigned char  FramLength       ;                               // 14:0 
		  unsigned char  FramFinishFlag   ;                                // 15 
	  } InfBit; 

} strEsp8266_Fram_Record;


/*********************************************** ESP8266 函数宏定义 *******************************************/
//#define     macESP8266_Usart( fmt, ... )           USART_printf ( macESP8266_USARTx, fmt, ##__VA_ARGS__ ) 
//#define     macPC_Usart( fmt, ... )                printf ( fmt, ##__VA_ARGS__ )
               

/****************************************** ESP8266 函数声明 ***********************************************/
void                    ESP8266_Init                        ( void );
unsigned char           ESP8266_Cmd                         ( char * cmd, char * reply1, char * reply2, u32 waittime );
void                    ESP8266_AT_Test                     ( void );
bit                     ESP8266_Net_Mode_Choose             ( ENUM_Net_ModeTypeDef enumMode );
bit                     ESP8266_JoinAP                      ( char * pSSID, char * pPassWord );
bit                     ESP8266_Enable_MultipleId           ( FunctionalState enumEnUnvarnishTx );
bit                     ESP8266_Link_Server                 ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
bit                     ESP8266_UnvarnishSend               ( void );
void                    ESP8266_ExitUnvarnishSend           ( void );
bit ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );
uint8_t ESP8266_Get_LinkStatus ( void );


#endif


