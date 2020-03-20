#ifndef __IIC_H
#define __IIC_H

#include<reg51.h>

#define uchar unsigned char
#define uint  unsigned int
	
sbit I2C_SDA = P1^6;
sbit I2C_SCL = P1^7;

void I2CStart();
void I2CStop();
bit I2CWrite(unsigned char dat);
unsigned char I2CReadNAK();
unsigned char I2CReadACK();
void MemToStr(unsigned char *str, unsigned char *src, unsigned char len);
void E2Read(unsigned char *buf, unsigned char addr, unsigned char len);
void E2Write(unsigned char *buf, unsigned char addr, unsigned char len);

#endif