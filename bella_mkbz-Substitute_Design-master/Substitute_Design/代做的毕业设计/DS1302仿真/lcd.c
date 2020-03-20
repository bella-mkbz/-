#include "lcd.h"

unsigned char code digit[10]={"0123456789"};   //定义字符数组显示数字

/*******************************************************************************
* 函 数 名         : Lcd1602_Delay1ms
* 函数功能		     : 延时函数，延时1ms
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
* 函 数 名       : Show_Dec_Number(uchar addr,uint temp)
* 函数功能		   : 显示一个十进制的数据
* 输    入       : 十进制数据
* 输    出       : 无
*******************************************************************************/
void Show_Dec_Number(uchar addr,uint temp)
{
   LcdWriteCom(addr);
	 if(temp >= 10000)
	 {  
			LcdWriteData(temp/10000+0x30);
			LcdWriteData(temp%10000/1000+0x30);	
			LcdWriteData(temp%1000/100+0x30);
			LcdWriteData(temp%100/10+0x30);
			LcdWriteData(temp%10+0x30);		 
	 }
	 else if((temp >= 1000) && (temp < 9999))
	 {  
			LcdWriteData(temp/1000+0x30);
			LcdWriteData(temp%1000/100+0x30);	
			LcdWriteData(temp%100/10+0x30);
			LcdWriteData(temp%10+0x30);
			LcdWriteData(' ');		 
	 }
	 else if((temp >= 100) && (temp < 999))
	 {  
			LcdWriteData(temp/100+0x30);
			LcdWriteData(temp%100/10+0x30);	
			LcdWriteData(temp%10+0x30);
			LcdWriteData(' ');
			LcdWriteData(' ');		 
	 }
	 else if((temp >= 10) && (temp < 100))
	 {  
			LcdWriteData(temp/10+0x30);
			LcdWriteData(temp%10+0x30);	
			LcdWriteData(' ');
			LcdWriteData(' ');
			LcdWriteData(' ');
	 }
	 else
	 {
			LcdWriteData(temp+0x30);
//			LcdWriteData(' ');
//			LcdWriteData(' ');
//			LcdWriteData(' ');
//			LcdWriteData(' ');
	 }
	 
}

/**************************************************************
以下是1302数据的显示程序
**************************************************************/
/*****************************************************
函数功能：显示秒
入口参数：x
***************************************************/ 
void DisplaySecond(unsigned char x)
{
 unsigned char i,j;     //j,k,l分别储存温度的百位、十位和个位
	i=x/10;//取十位
	j=x%10;//取个位     
	LcdWriteCom(0xc9);    //写显示地址,将在第2行第7列开始显示
	LcdWriteData(digit[i]);    //将百位数字的字符常量写入LCD
	LcdWriteData(digit[j]);    //将十位数字的字符常量写入LCD
	Lcd1602_Delay1ms(1);         //延时1ms给硬件一点反应时间    
 }

/*****************************************************
函数功能：显示分钟
入口参数：x
***************************************************/ 
void DisplayMinute(unsigned char x)
{
 unsigned char i,j;     //j,k,l分别储存温度的百位、十位和个位
	i=x/10;//取十位
	j=x%10;//取个位     
	LcdWriteCom(0xc6);    //写显示地址,将在第2行第7列开始显示
	LcdWriteData(digit[i]);    //将百位数字的字符常量写入LCD
	LcdWriteData(digit[j]);    //将十位数字的字符常量写入LCD
	Lcd1602_Delay1ms(1);         //延时1ms给硬件一点反应时间    
 }
/*****************************************************
函数功能：显示小时
入口参数：x
***************************************************/ 
void DisplayHour(unsigned char x)
{
 unsigned char i,j;     //j,k,l分别储存温度的百位、十位和个位
	i=x/10;//取十位
	j=x%10;//取个位     
	LcdWriteCom(0xc3);    //写显示地址,将在第2行第7列开始显示
	LcdWriteData(digit[i]);    //将百位数字的字符常量写入LCD
	LcdWriteData(digit[j]);    //将十位数字的字符常量写入LCD
	Lcd1602_Delay1ms(1);         //延时1ms给硬件一点反应时间    
 }
 /*****************************************************
函数功能：显示日
入口参数：x
***************************************************/ 
void DisplayDay(unsigned char x)
{
 unsigned char i,j;     //j,k,l分别储存温度的百位、十位和个位
	i=x/10;//取十位
	j=x%10;//取个位     
	LcdWriteCom(0x8c);    //写显示地址,将在第2行第7列开始显示
	LcdWriteData(digit[i]);    //将百位数字的字符常量写入LCD
	LcdWriteData(digit[j]);    //将十位数字的字符常量写入LCD
	Lcd1602_Delay1ms(1);         //延时1ms给硬件一点反应时间    
 }
 /*****************************************************
函数功能：显示月
入口参数：x
***************************************************/ 
void DisplayMonth(unsigned char x)
{
 unsigned char i,j;     //j,k,l分别储存温度的百位、十位和个位
	i=x/10;//取十位
	j=x%10;//取个位     
	LcdWriteCom(0x89);    //写显示地址,将在第2行第7列开始显示
	LcdWriteData(digit[i]);    //将百位数字的字符常量写入LCD
	LcdWriteData(digit[j]);    //将十位数字的字符常量写入LCD
	Lcd1602_Delay1ms(1);         //延时1ms给硬件一点反应时间    
 }
/*****************************************************
函数功能：显示年
入口参数：x
***************************************************/ 
void DisplayYear(unsigned char x)
{
 unsigned char i,j; 
	i=x/10;//取十位
	j=x%10;//取个位     
	LcdWriteCom(0x86);    //写显示地址,将在第2行第7列开始显示
	LcdWriteData(digit[i]);    //将百位数字的字符常量写入LCD
	LcdWriteData(digit[j]);    //将十位数字的字符常量写入LCD
	Lcd1602_Delay1ms(1);         //延时1ms给硬件一点反应时间    
 }
