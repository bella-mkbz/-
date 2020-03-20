#ifndef __LCD_H_
#define __LCD_H_

/**********************************
����ͷ�ļ�
**********************************/
#include <REGX51.H>

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
#define LCD1602_DATAPINS P0
#define LCD1602_E  P1_2
#define LCD1602_RW P1_1
#define LCD1602_RS P1_0

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
