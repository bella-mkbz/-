#include "mqtt.h"


#define Buff_MAXLen 25
static u8 Identifier = 0x02;
static xdata u8 Connect[Buff_MAXLen]={0x10,0x10,0x00,0x04,0x4d,0x51,0x54,0x54,0x04,0x02,0x00,0x3c,0x00,0x04,0x6d,0x6f,0x74,0x75};
u8 Command_Buff[Buff_MAXLen] = {0};
static u8 Heart_ping[2]={0xc0,0x00};

static unsigned int i = 0;


/*********************************************
* 函数名 : Connect_MQTT
* 描述   : 连接 MQTT Broker 函数
* 注意   : 代码默认使用的为 无用户和密码登录
**********************************************/
void Connect_MQTT(void)
{
	for(i = 0; i < 18; i++)
			UART2_SendData(Connect[i]);
	delay_ms(3000);
}

/*******************************************
* 函数名 : Subscribe_Topic
* 描述   : 订阅话题消息函数
* 注意   : 订阅的时候 QoS Level 一定使用 0
********************************************/
void Subscribe_Topic(u8 *Topic, u16 Topic_Len)
{
	Command_Buff[0] = 0x82;
	Command_Buff[1] = 2 + 2 + Topic_Len +1;
	Command_Buff[2] = Identifier >> 8;
	Command_Buff[3] = Identifier&0xff;
	Command_Buff[4] = Topic_Len >> 8;
	Command_Buff[5] = Topic_Len&0xff;
	for(i = 0; i < Topic_Len; i++)
		Command_Buff[6 + i] = *(Topic + i);
	Command_Buff[1 + Command_Buff[1]] = 0x00;
	for(i = 0; i < (Command_Buff[1] + 2); i++)
			UART2_SendData(Command_Buff[i]);
	delay_ms(5000);
  Identifier++;
}

/*******************************************
* 函数名 : Publish_Msg
* 描述   : 发布话题消息函数
* 注意   : 
********************************************/
void Publish_Msg(u8 *Topic, u16 Topic_Len, u8 *Msg, u16 Msg_Len)
{	
	if(Topic == NULL || Msg == NULL || Topic_Len == 0 || Msg_Len == 0)
		return ;
	Command_Buff[0] = 0x32;
	Command_Buff[1] = 2 + Msg_Len + Topic_Len + 2;
	Command_Buff[2] = Topic_Len >> 8;
	Command_Buff[3] = Topic_Len&0xff;
	for(i = 0; i < Topic_Len; i++){
		Command_Buff[4 + i] = Topic[i];
	}
	Command_Buff[4 + Topic_Len] = 0x00;
	Command_Buff[4 + Topic_Len + 1] = Identifier;
	for(i = 0; i < Msg_Len; i++){
		Command_Buff[6 + Topic_Len + i] = Msg[i];
	}
	for(i = 0; i < (Command_Buff[1] + 2); i++)
	{
			UART2_SendData(Command_Buff[i]);
	}
	Identifier++;
}

/*******************************************
* 函数名 : Send_Heart
* 描述   : 发送心跳包函数
* 注意   : 代码默认的每隔60s发送一次心跳包
********************************************/
void Send_Heart(void)
{
	for(i = 0; i < 2; i++)
		UART2_SendData(Heart_ping[i]);
}