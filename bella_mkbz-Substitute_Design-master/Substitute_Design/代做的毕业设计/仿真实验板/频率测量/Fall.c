#include "Fall.h"

void Fall_Interrupt_Init()
{
   EA = 1;
	 EX0 = 1;
	 IT0 = 1;
}

