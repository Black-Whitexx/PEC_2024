//
// Created by BXW on 2024/7/29.
//

#ifndef PEC_2024_MYCAN_H
#define PEC_2024_MYCAN_H
#include "stm32g474xx.h"
#include "fdcan.h"
void InitCan();
void CanSendData(const uint8_t id,uint8_t *data);
#endif //PEC_2024_MYCAN_H
