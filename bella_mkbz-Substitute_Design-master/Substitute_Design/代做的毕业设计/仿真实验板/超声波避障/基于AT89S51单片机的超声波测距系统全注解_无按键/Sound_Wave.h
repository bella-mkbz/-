#ifndef __SOUND_WAVE_H_
#define __SOUND_WAVE_H_

#include <REGX51.H>				//ͷ�ļ�
#include <intrins.h>      //ͷ�ļ�
#include "lcd.h"

#define csbout_1 P2_7 			    //������ 1 ����
#define csbint_1 P3_5 			    //������ 1 ����

#define csbout_2 P1_3 			    //������ 2 ����
#define csbint_2 P1_4 			    //������ 2 ����

#define csbout_3 P1_5 			    //������ 3 ����
#define csbint_3 P1_6 			    //������ 3 ����

#define csbout_4 P3_6 			    //������ 3 ����
#define csbint_4 P3_7 			    //������ 3 ����

unsigned int csbcj_1();
unsigned int csbcj_2();
unsigned int csbcj_3();
unsigned int csbcj_4();
void Delay12us();

#endif