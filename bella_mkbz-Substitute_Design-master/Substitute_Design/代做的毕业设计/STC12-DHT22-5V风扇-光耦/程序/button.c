#include "button.h"


uint8_t Button_Value(void)
{  
	if(KEY2==0 || KEY3==0 || KEY4==0)
	{
		Delay10ms();//去抖动 
		if(KEY2 == 0)
		{
			while(!KEY2);
			return 1;
		}
		else if(KEY3 == 0)
		{
			while(!KEY3);
			return 2;
		}
		else if(KEY4 == 0)
		{
			while(!KEY4);
			return 3;
		}	
	}   
 	return 0;// 无按键按下
}

void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

