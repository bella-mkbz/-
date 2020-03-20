#ifndef __TEMP_H_
#define __TEMP_H_

#include	"STC12C5Axx.H"
#include  <intrins.h>

sbit DSPORT = P2^0;

void Delay642us();
void Delay500us();
void Delay68us();
void Delay6us();
void Delay48us();
void Delay1ms();
unsigned char Ds18b20Init();
void Ds18b20WriteByte(unsigned char com);
unsigned char Ds18b20ReadByte();
void  Ds18b20ChangTemp();
void  Ds18b20ReadTempCom();
int Ds18b20ReadTemp();

#endif
