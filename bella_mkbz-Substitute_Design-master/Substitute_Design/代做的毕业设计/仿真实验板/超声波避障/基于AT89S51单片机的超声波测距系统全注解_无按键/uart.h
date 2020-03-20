#ifndef __UART_H
#define __UART_H

#include <REGX51.H>

#define uchar unsigned char
#define uint  unsigned int

void Uart_Init(void);
void Send_Byte(uchar dat);
void Send_Str(char *str);

#endif