#ifndef	__STC_EEPROM_H
#define	__STC_EEPROM_H
/****************************************************************
STC125C5A60S2共有两个扇区共1K字节, 每个扇区分配512个字节

字节分配地址   SECTOR1    SECTOR2
             0000~01FF  0200~03FF

在写入数据之前必须要擦出扇区, 不能进行字节擦除	
****************************************************************/

#include "STC12C5Axx.H"
#include "intrins.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define IAP_ADDRESS 0x0000        //Start address for STC11/10xx EEPROM

void IapIdle();
BYTE IapReadByte(WORD addr);
void IapProgramByte(WORD addr, BYTE dat);
void IapEraseSector(WORD addr);


#endif