#ifndef __UART_H_
#define __UART_H_

#include <reg51.h>

void SerialInit(void);
void SendByte(unsigned char temp);
void SendBuff(unsigned char *temp, unsigned char length);
void Timer1Init(void);
unsigned int factory_crc16( unsigned char *bufData, unsigned char buflen);

#endif

