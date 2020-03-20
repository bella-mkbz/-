#ifndef __ADC0832_H
#define __ADC0832_H

#include <reg51.h>
#include <intrins.h>

sbit CS   = P2^0;
sbit Clk = P2^1;
sbit DATI = P2^2;
sbit DATO = P2^2;

unsigned char adc0832(unsigned char CH);

#endif

