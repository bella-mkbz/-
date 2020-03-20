#ifndef __DHT11_H_
#define __DHT11_H_

#include <STC12C5A60S2.H>
#include <intrins.h>

sbit DHT11 = P2^0;

void InitUART(void);
void Delay10us();
void Delay1000ms();
void Delay20ms();
void Delay30us();
void Start(void);
unsigned char Read_Sensor(unsigned char *Sensor_Data);
unsigned char Sensor_Byte(void);

#endif
