#include <STC12C5A60S2.H>
#include <intrins.h>
#include "DHT11.h"

unsigned char Sensor_Data[5]={0x00};

void main(void)
{
   unsigned char i;
	 InitUART();
	 while(1)
	 {
		  if(Read_Sensor(Sensor_Data))
			{
				for(i=0;i<5;i++)
				 {
					 SBUF = Sensor_Data[i];
					 while(!TI);
					 TI=0;
				 } 
			}
		 Delay1000ms();
	 }
	 
}