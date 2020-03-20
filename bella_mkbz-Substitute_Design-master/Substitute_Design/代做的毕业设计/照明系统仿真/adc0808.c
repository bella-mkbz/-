#include "adc0808.h"

unsigned char ADC0808_Value(unsigned char addr0, unsigned char addr1, unsigned char addr2)
{
	unsigned char getdata;
	Addr_0 = addr0;
	Addr_1 = addr1;
	Addr_2 = addr2;
	ST=0;
	OE=0;
	ST=1;
	ST=0;
	while(EOC==0);
	OE=1;
	getdata=P1;
	OE=0;
	return getdata;
}