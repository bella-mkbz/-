#ifndef __LCD_H_
#define __LCD_H_

/**********************************
����ͷ�ļ�
**********************************/
#include <reg51.h>

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

/**********************************
PIN�ڶ���
**********************************/
#define LCD1602_DATAPINS P1
sbit LCD1602_E  = P3^5;
sbit LCD1602_RW = P3^6;
sbit LCD1602_RS = P3^7;

/**********************************
��������
**********************************/
/*��51��Ƭ��12MHZʱ���µ���ʱ����*/
void Lcd1602_Delay1ms(uint c);   //��� 0us
/*LCD1602д��8λ�����Ӻ���*/
void LcdWriteCom(uchar com);
/*LCD1602д��8λ�����Ӻ���*/	
void LcdWriteData(uchar dat);
/*LCD1602��ʼ���ӳ���*/		
void LcdInit();	
void Show_String(uchar addr,uchar *str);
void Show_Dec_Number(uchar addr,uint temp);

#endif
