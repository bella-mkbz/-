#ifndef __BUTTON_H
#define __BUTTON_H

#include "STC12C5Axx.h"
#include <intrins.h>

#define uint8_t unsigned char
#define uint32_t unsigned int
	

sbit KEY2 = P1^0;
sbit KEY3 = P1^1;
sbit KEY4 = P1^2;

uint8_t Button_Value(void);
void Delay10ms();

#endif