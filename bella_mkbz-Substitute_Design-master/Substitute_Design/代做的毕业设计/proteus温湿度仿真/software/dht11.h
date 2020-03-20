#ifndef __DHT11_H_
#define __DHT11_H_

#include <reg51.h>

sbit Data=P3^6;   //定义数据线


void DHT11_delay_us(unsigned char n);
void DHT11_delay_ms(unsigned int z);
void DHT11_start();
unsigned char DHT11_rec_byte();
unsigned char DHT11_receive();

#endif