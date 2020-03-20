#ifndef __ADC0808_H
#define __ADC0808_H

#include <reg51.h>

sbit Addr_0 = P2^0;
sbit Addr_1 = P2^1;
sbit Addr_2 = P2^2;
sbit EOC = P2^3;
sbit OE = P2^4;
sbit ST = P2^5;
unsigned char ADC0808_Value(unsigned char addr0, unsigned char addr1, unsigned char addr2);

#endif