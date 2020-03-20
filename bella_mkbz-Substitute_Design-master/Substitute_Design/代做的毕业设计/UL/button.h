#ifndef __BUTTON_H
#define __BUTTON_H

#include <STC12C5A60S2.H>
#include <intrins.h>
#define uint8_t unsigned char
#define uint32_t unsigned int
	

sbit KEY2 = P1^7;
sbit KEY3 = P1^6;

uint8_t Button_Value(void);
void Delay10ms();

#endif