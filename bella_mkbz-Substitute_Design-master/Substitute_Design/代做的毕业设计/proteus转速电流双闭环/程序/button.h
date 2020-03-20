#ifndef __BUTTON_H
#define __BUTTON_H

#include <reg52.h>

#define uint8_t unsigned char
#define uint32_t unsigned int
	

sbit KEY2 = P1^5;
sbit KEY3 = P1^6;
sbit KEY4 = P1^7;
sbit KEY5 = P3^7;

uint8_t Button_Value(void);
#endif