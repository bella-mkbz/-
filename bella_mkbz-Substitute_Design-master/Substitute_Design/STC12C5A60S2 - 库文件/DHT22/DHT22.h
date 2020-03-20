#ifndef __DHT22_H
#define __DHT22_H

#includ  "STC12C5Axx.h"
#include <intrins.h>
#include <string.h>

sbit Sensor_SDA = P2^0;            //DHT22数据接口

//使用的时候只需在其他地方调用Read_SensorData即可，从此函数取到的值需要除以10才为正确数据
unsigned char Read_SensorData(unsigned int *temprature, unsigned int *humidity);

#endif