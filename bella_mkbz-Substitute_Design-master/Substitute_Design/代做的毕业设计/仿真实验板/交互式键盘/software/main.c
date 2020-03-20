#include <reg51.h>
#include "Inter_Key.h"
#include "lcd.h"

void main()
{
  unsigned char Value=0;
  LcdInit();
  Show_String(0X82,"Inter_Keyboard");
   while(1)
    {
	    Value = Keyboard();
	    Show_Dec_Number(0xc5,Value);
	  }

}