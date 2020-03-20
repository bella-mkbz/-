#ifndef __MQTT_H
#define __MQTT_H

#include "uart.h"
#include "delay.h"
#include <stdio.h>

void Connect_MQTT(void);
void Subscribe_Topic(u8 *Topic, u16 Topic_Len);
void Publish_Msg(u8 *Topic, u16 Topic_Len, u8 *Msg, u16 Msg_Len);
void Send_Heart(void);

#endif