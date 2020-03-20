#ifndef __ADC0832_H_
#define __ADC0832_H_

#include <reg51.h>
#include "intrins.h"

sbit cs=P3^7;
sbit clk=P3^6;
sbit dati=P3^5;
sbit dato=P3^5;
unsigned char Adc0832_Getvalue(unsigned char channel);

#endif