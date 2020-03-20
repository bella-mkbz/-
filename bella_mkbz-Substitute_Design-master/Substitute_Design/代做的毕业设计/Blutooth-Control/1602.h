#ifndef __1602_H_
#define __1602_H_

#include <STC12C5A60S2.H>
#include <intrins.h>

sbit RS = P2^5;   
sbit RW = P2^6;
sbit EN = P2^7;

#define uchar unsigned char;
#define uint unsigned int;

#define RS_CLR RS=0 
#define RS_SET RS=1

#define RW_CLR RW=0 
#define RW_SET RW=1 

#define EN_CLR EN=0
#define EN_SET EN=1

#define DataPort P0  

void DelayUs2x(unsigned char t);
void DelayMs(unsigned char t);
bit LCD_Check_Busy(void);
void LCD_Write_Com(unsigned char com);
void LCD_Write_Data(unsigned char Data);
void LCD_Clear(void);
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s);
void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data);
void LCD_Init(void);

#endif