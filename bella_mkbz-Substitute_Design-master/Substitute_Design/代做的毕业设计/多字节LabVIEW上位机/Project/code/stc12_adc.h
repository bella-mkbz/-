#ifndef __STC12_ADC_H__
#define __STC12_ADC__

#include <STC12C5A60S2.H>
#include <intrins.h>

void InitADC();
unsigned char GetADCResult();
void Delay(unsigned int n);

#endif
