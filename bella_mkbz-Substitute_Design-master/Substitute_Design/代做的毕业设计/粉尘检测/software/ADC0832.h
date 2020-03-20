#ifndef __ADC0832_H
#define __ADC0832_H

#include <reg51.h>
#include <intrins.h>

sbit CS   = P3^4;
sbit Clk = P3^5;
sbit DATI = P3^6;
sbit DATO = P3^6;

unsigned char adc0832(unsigned char CH);

#endif

