#ifndef __DHT22_H_
#define __DHT22_H_

#include <STC12C5A60S2.h>
#include <intrins.h>
#include <string.h>
#include <1602.h>

sbit Sensor_SDA = P2^0;            //DHT22数据接口
sbit Beep = P3^7;                  //蜂鸣器接口

//在主函数中只需调用 InitUART(),Read_Sensor()函数即可;
//函数定义
void Delay500us();
void Delay30us();
void InitUART(void);
void UARTSend(char UCHAR);
void UART_PutString(unsigned char *buf);
void UART_PutStringAndNum(unsigned char *buf ,unsigned int num);
void UART_PutStringAnd_Data(unsigned char *buf ,unsigned char *bufdata);
void UARTSend_Nbyte(void);
unsigned char Read_SensorData(void);
unsigned char Read_Sensor(void);
void Beep_Alarm();
void Show();
#endif