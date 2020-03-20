#include "button.h"
#include "delay.h"

uint8_t Button_Value(void)
{
	static uint8_t key_up=1;//按键按松开标志	  
	if(key_up&&(KEY2==0||KEY3==0))
	{
		Delay1ms(10);//去抖动 
		key_up=0;
		if(KEY2==0)
			return 1;
		else if(KEY3==0)
			return 2;
	}
	else if
		(KEY2==1&&KEY3==1)key_up=1; 	    
 	return 0;// 无按键按下
}
//uint8_t Button_Value(void)
//{
//  static bit backup = 1;
//	static bit keybuf = 1;
//	static uint8_t value=0;
//	keybuf = KEY2;
//	if (keybuf != backup) 
// {
//  Delay1ms(10);
//	 if (keybuf == KEY2) 
//		{
//		if (backup == 0)
//			{
//         return 1;
//			 
//			}
//		 backup = keybuf; 
//		}
// }
// return 0;
//}