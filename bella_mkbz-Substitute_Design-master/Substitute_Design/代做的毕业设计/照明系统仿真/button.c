#include "button.h"
#include "delay.h"

uint8_t Button_Value(void)
{
	static uint8_t key_up=1;//按键按松开标志	  
	if(key_up && (KEY1 == 0))
	{
//		Delay1ms(5);//去抖动 
		key_up=0;
		if(KEY1 == 0)
			return 1;
	}
	else if(KEY1 == 1)
		key_up=1; 	    
 	return 0;// 无按键按下
}

