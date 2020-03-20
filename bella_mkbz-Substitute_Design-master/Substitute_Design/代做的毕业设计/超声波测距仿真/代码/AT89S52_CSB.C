#include <REGX51.H>				
#include "lcd.h"
#include "Sound_Wave.h"

void main()					
{
  TMOD = 0x11;
  TH1 = 0X3C;
  TH0 = 0XB0;	
	EA = 1;
	ET1 = 1;
	TR1 = 1;			
	LcdInit();
  for(;;);
}

