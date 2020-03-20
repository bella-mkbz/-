#include "delay.h"

void Delay1ms(uint32_t count)
{
	uint32_t i,j;
	for(i=0;i<count;i++)
	for(j=0;j<10;j++);
}
