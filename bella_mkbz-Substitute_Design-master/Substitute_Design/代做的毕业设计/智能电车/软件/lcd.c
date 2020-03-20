#include "lcd.h"


	
/*******************************************************************************
* 函 数 名         : Lcd1602_Delay1ms
* 函数功能		   : 延时函数，延时1ms
* 输    入         : c
* 输    出         : 无
* 说    名         : 该函数是在12MHZ晶振下，12分频单片机的延时。
*******************************************************************************/

void Lcd1602_Delay1ms(uint c)   //误差 0us
{
    uchar a,b;
	for (; c>0; c--)
	{
		 for (b=199;b>0;b--)
		 {
		  	for(a=1;a>0;a--);
		 }      
	}
    	
}

/*******************************************************************************
* 函 数 名         : LcdWriteCom
* 函数功能		   : 向LCD写入一个字节的命令
* 输    入         : com
* 输    出         : 无
*******************************************************************************/
void LcdWriteCom(uchar com)	  //写入命令
{
	LCD1602_E = 0;     //使能
	LCD1602_RS = 0;	   //选择发送命令
	LCD1602_RW = 0;	   //选择写入
	
	LCD1602_DATAPINS = com;     //放入命令
	Lcd1602_Delay1ms(1);		//等待数据稳定

	LCD1602_E = 1;	          //写入时序
	Lcd1602_Delay1ms(5);	  //保持时间
	LCD1602_E = 0;
}
/*******************************************************************************
* 函 数 名         : LcdWriteData
* 函数功能		   : 向LCD写入一个字节的数据
* 输    入         : dat
* 输    出         : 无
*******************************************************************************/		   
   
void LcdWriteData(uchar dat)			//写入数据
{
	LCD1602_E = 0;	//使能清零
	LCD1602_RS = 1;	//选择输入数据
	LCD1602_RW = 0;	//选择写入

	LCD1602_DATAPINS = dat; //写入数据
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //写入时序
	Lcd1602_Delay1ms(5);   //保持时间
	LCD1602_E = 0;
}

/*******************************************************************************
* 函 数 名       : LcdInit()
* 函数功能		 : 初始化LCD屏
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/		   
void LcdInit()						  //LCD初始化子程序
{
 	LcdWriteCom(0x38);  //开显示
	LcdWriteCom(0x0c);  //开显示不显示光标
	LcdWriteCom(0x06);  //写一个指针加1
	LcdWriteCom(0x01);  //清屏
	LcdWriteCom(0x80);  //设置数据指针起点
}
/*******************************************************************************
* 函 数 名       : Show_String(uchar addr,uchar *str)
* 函数功能		   : 显示字符串
* 输    入       : 字符串
* 输    出       : 无
*******************************************************************************/
void Show_String(uchar addr,uchar *str)
{
	 LcdWriteCom(addr);
   while(*str != '\0')
	 {
	   LcdWriteData(*str);	
		 str++;
	 }
}
/*******************************************************************************
* 鍑?鏁?鍚?      : Show_Dec_Number(uchar addr,uint temp)
* 鍑芥暟鍔熻兘		   : 鏄剧ず涓�涓崄杩涘埗鐨勬暟鎹?* 杈?   鍏?      : 鍗佽繘鍒舵暟鎹?* 杈?   鍑?      : 鏃?*******************************************************************************/
void Show_Dec_Number(uchar addr,uint temp)
{
   LcdWriteCom(addr);
	 if(temp >= 1000)
	 {
			LcdWriteData(temp/1000+0x30);
			LcdWriteData(temp%1000/100+0x30);
			LcdWriteData(temp%100/10+0x30);
			LcdWriteData(temp%10+0x30);
	 }
	 else if((temp >= 100) && (temp < 1000))
	 {
			LcdWriteData(temp/100+0x30);
			LcdWriteData(temp%100/10+0x30);
			LcdWriteData(temp%10+0x30);
		  LcdWriteData(' ');
	 }
	 else if((temp >= 10) && (temp < 100))
	 {
			LcdWriteData(temp/10+0x30);
			LcdWriteData(temp%10+0x30);
		  LcdWriteData(' ');
		  LcdWriteData(' ');
	 }
	 else
	 {
	    LcdWriteData(temp+0x30);
		  LcdWriteData(' ');
		  LcdWriteData(' ');
		  LcdWriteData(' ');
	 }
	 
}



