#ifndef __BUTTON_H
#define __BUTTON_H

#include <reg51.h>

#define uint8_t unsigned char
#define uint32_t unsigned int
	 
sbit KEY1 = P2^7; 

uint8_t Button_Value(void);
#endif