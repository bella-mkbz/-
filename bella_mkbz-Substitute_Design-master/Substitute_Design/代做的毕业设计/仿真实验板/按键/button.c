#include "button.h"
#include "delay.h"

uint8_t Button_Value(void)
{
	static uint8_t key_up=1;//�������ɿ���־	  
	if(key_up&&(KEY2==0||KEY3==0))
	{
		Delay1ms(10);//ȥ���� 
		key_up=0;
		if(KEY2==0)
			return 1;
		else if(KEY3==0)
			return 2;
	}
	else if
		(KEY2==1&&KEY3==1)key_up=1; 	    
 	return 0;// �ް�������
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