#ifndef __DS1302_H
#define __DS1302_H

#include <reg51.h>

sbit DATA=P1^1;   //位定义1302芯片的接口，数据输出端定义在P1.1引脚
sbit RST=P1^2;    //位定义1302芯片的接口，复位端口定义在P1.1引脚
sbit SCLK=P1^0;   //位定义1302芯片的接口，时钟输出端口定义在P1.1引脚

void delaynus(unsigned char n);
void Write1302(unsigned char dat);
void WriteSet1302(unsigned char Cmd,unsigned char dat);
unsigned char Read1302(void);
unsigned char  ReadSet1302(unsigned char Cmd);
void Init_DS1302(void);

#endif