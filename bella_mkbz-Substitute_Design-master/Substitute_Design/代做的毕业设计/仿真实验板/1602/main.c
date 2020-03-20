#include<reg51.h>
#include"lcd.h"

unsigned char code CDIS1[]={"Linux_:" };
unsigned char code CDIS2[]={"Ubuntu_:"};
/*******************************************************************************
* 函数名         : main
* 函数功能		   : 主函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void main()
{
	LcdInit();
	Show_String(0x80,CDIS1);
	Show_String(0x80+0x40,CDIS2);
	while(1)
	{
	
	}
}									 
