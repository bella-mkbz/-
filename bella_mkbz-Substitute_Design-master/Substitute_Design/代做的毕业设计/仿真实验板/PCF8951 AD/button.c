#include "button.h"
#include "delay.h"

uint8_t Button_Value(void)
{
	static uint8_t key_up=1;//�������ɿ���־	  
	if(key_up&&(KEY0==0||KEY1==0))
	{
		Delay1ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
	}else if(KEY0==1&&KEY1==1)key_up=1; 	    
 	return 0;// �ް�������
}
