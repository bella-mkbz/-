#include "button.h"


uint8_t Button_Value(void)
{
	static uint8_t key_up=1;//�������ɿ���־	  
	if(key_up&&(KEY2==0||KEY3==0))
	{
		Delay10ms();//ȥ���� 
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

void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 108;
	j = 144;
	do
	{
		while (--j);
	} while (--i);
}

