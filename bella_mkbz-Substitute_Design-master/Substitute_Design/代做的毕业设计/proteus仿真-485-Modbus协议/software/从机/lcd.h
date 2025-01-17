#ifndef __LCD_H_
#define __LCD_H_

/**********************************
包含头文件
**********************************/
#include <reg51.h>

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

/**********************************
PIN口定义
**********************************/
#define LCD1602_DATAPINS P0
sbit LCD1602_E  = P2^5;
sbit LCD1602_RW = P2^6;
sbit LCD1602_RS = P2^7;

/**********************************
函数声明
**********************************/
/*在51单片机12MHZ时钟下的延时函数*/
void Lcd1602_Delay1ms(uint c);   //误差 0us
/*LCD1602写入8位命令子函数*/
void LcdWriteCom(uchar com);
/*LCD1602写入8位数据子函数*/	
void LcdWriteData(uchar dat);
/*LCD1602初始化子程序*/		
void LcdInit();	
void Show_String(uchar addr,uchar *str);
void Show_Dec_Number(uchar addr,uint temp);

#endif
