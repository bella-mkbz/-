#ifndef __BUTTON_H
#define __BUTTON_H

#include <reg51.h>

#define uint8_t unsigned char
#define uint32_t unsigned int
	
sbit KEY4 = P2^4;  //µ¹³µ
sbit KEY1 = P2^5;  //×ó¹Õ
sbit KEY2 = P2^6;  //ÓÒ¹Õ
sbit KEY3 = P2^7;  //Í£³µ

uint8_t Button_Value(void);
#endif