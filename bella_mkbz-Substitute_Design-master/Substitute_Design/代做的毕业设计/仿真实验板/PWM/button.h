#ifndef __BUTTON_H
#define __BUTTON_H

#include<reg51.h>

#define uint8_t unsigned char
#define uint32_t unsigned int
	

sbit KEY2 = P2^0;
sbit KEY3 = P2^1;

uint8_t Button_Value(void);
#endif