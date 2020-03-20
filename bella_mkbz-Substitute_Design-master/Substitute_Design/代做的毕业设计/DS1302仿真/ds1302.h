#ifndef __DS1302_H
#define __DS1302_H

#include <reg51.h>

sbit DATA=P1^1;   //λ����1302оƬ�Ľӿڣ���������˶�����P1.1����
sbit RST=P1^2;    //λ����1302оƬ�Ľӿڣ���λ�˿ڶ�����P1.1����
sbit SCLK=P1^0;   //λ����1302оƬ�Ľӿڣ�ʱ������˿ڶ�����P1.1����

void delaynus(unsigned char n);
void Write1302(unsigned char dat);
void WriteSet1302(unsigned char Cmd,unsigned char dat);
unsigned char Read1302(void);
unsigned char  ReadSet1302(unsigned char Cmd);
void Init_DS1302(void);

#endif