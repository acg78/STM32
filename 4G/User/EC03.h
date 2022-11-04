#ifndef __EC03_H
#define __EC03_H

#include "stm32f10x.h"                  // Device header
#include <string.h>

//宏定义接受标志
#define Rec_ok   1
#define Rec_wait 0

void EC4Ginit(void);
void ONENET_MQTT_MQTTPUBLISH(void);
void ONENET_MQTT_CONNECT_4G(void);
_Bool EC_SendCmd(char *cmd, char *res, u16 time);
void EC_clear(void);
_Bool EC_WaitRecive(void);
#endif
