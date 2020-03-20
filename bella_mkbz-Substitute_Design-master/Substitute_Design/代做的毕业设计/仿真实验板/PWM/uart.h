#ifndef __UART_H
#define __UART_H

#include<reg51.h>

#define uchar unsigned char
#define uint  unsigned int

void Uart_Init(uint baud);
void Send_Byte(uchar dat);
void Send_Str(char *str);

#endif