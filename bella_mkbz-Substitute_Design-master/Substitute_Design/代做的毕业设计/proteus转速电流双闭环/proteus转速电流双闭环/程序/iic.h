#ifndef _IIC_H
#define _IIC_H

#include "reg52.h"
#include "intrins.h"

#define somenop    delay_iic();
#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */

//函数声明
void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(unsigned char ackbit); 
void IIC_SendByte(unsigned char byt); 
//void at_send(unsigned char addr,unsigned char dat);
//unsigned char at_read(unsigned char addr);
//bit IIC_WaitAck(void);  
//unsigned char IIC_RecByte(void); 

void ad_send(unsigned char addr,unsigned char dat);
//unsigned char ad_read(unsigned char addr);
unsigned char ad_read(unsigned char add,unsigned char addr);
void IIC_response();
void delay_iic();
#endif
