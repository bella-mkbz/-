#include<reg51.h>
#include "Fall.h"
#include "lcd.h"

unsigned int Count = 0;

void main()
{  
	 Fall_Interrupt_Init();
   LcdInit();
   while(1)
	 {
	  Show_Dec_Number(0xc0,Count);
	 }

}

void Fall_Iterrupt_0(void) interrupt 0
{
	Count++;
}