#include "button.h"
#include "delay.h"

uint8_t Button_Value(void)
{
	static uint8_t key_up=1;//�������ɿ���־	  
	if(key_up&&(KEY2==0||KEY3==0||KEY1==0||KEY4==0))
	{
		Delay1ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==0)
			return 1;
		else if(KEY2==0)
			return 2;
		else if(KEY3==0)
			return 3;
		else if(KEY4==0)
			return 4;
	}
	else if
		(KEY2==1&&KEY3==1&&KEY1==1&&KEY4==1)key_up=1; 	    
 	return 0;// �ް�������
}

