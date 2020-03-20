#ifndef __TEMP_H_
#define __TEMP_H_

#include  "STC12C5Axx.H"
#include  <intrins.h>
sbit DSPORT = P3^7;

void Delay642us();
void Delay500us();
void Delay68us();
void Delay6us();
void Delay48us();
void Delay1ms();
unsigned char Ds18b20Init();
void Ds18b20WriteByte(unsigned char com);
unsigned char Ds18b20ReadByte();
void  Ds18b20ChangTemp();
void  Ds18b20ReadTempCom();
int Ds18b20ReadTemp();
/**********************************************************************
 * 调用Temp_Result函数即可，Buff里面
 * 存放的数据格式如下:
 * Buff[0] Buff[1]  Buff[2] Buff[3]    Buff[4]          Buff[5] 
 * 符号位  温度百位  温度十位 温度个位 温度小数点后一位  温度小数点后两位
 *********************************************************************/
void Temp_Result(unsigned char* Buff);

#endif
