#ifndef __SOUND_WAVE_H_
#define __SOUND_WAVE_H_

#include <REGX51.H>				//头文件
#include <intrins.h>      //头文件
#include "lcd.h"

#define csbout_1 P2_7 			    //超声波 1 发送
#define csbint_1 P3_5 			    //超声波 1 接收

#define csbout_2 P1_3 			    //超声波 2 发送
#define csbint_2 P1_4 			    //超声波 2 接收

#define csbout_3 P1_5 			    //超声波 3 发送
#define csbint_3 P1_6 			    //超声波 3 接收

#define csbout_4 P3_6 			    //超声波 3 发送
#define csbint_4 P3_7 			    //超声波 3 接收

unsigned int csbcj_1();
unsigned int csbcj_2();
unsigned int csbcj_3();
unsigned int csbcj_4();
void Delay12us();

#endif