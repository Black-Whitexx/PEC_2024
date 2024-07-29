//
// Created by BXW on 2024/7/29.
//

#ifndef PEC_2024_MYCAN_H
#define PEC_2024_MYCAN_H
#include "stm32g474xx.h"
#include "fdcan.h"
void InitCan();
void CanSendData(const uint8_t id,uint8_t *data);

typedef struct
{
    int16_t set_voltage;          //电压
    uint16_t angle;          //转子角度 abs angle_now range:[0,8191]
    int16_t speed;          //转子速度
    int16_t torque_current;       //扭矩（以电流值为单位）
    uint8_t temperate;                 //电机温度
    int32_t total_angle;          //转子转过的总角度
    int16_t total_cnt;            //转子转过的总圈数
    uint16_t offset_angle;        //上电时的转子位置（初始位置）
    uint16_t last_angle;          //abs angle_now range:[0,8191]
    uint32_t msg_cnt;              //消息计数值，收到一次就+1
    float true_angle;
}moto_info_t;

typedef union
{
    uint8_t data_u8[4];
    int8_t data_i8[4];
    int32_t data_i32;
    float data_f;
}union32;

extern moto_info_t motor_info[8];
extern void Motor_info_record(moto_info_t *ptr, uint8_t *data);
extern void Set_current(FDCAN_HandleTypeDef *_hcan, int16_t id_range, int16_t current1, int16_t current2, int16_t current3, int16_t current4);
void Send_cmd(uint32_t cmd);

#endif //PEC_2024_MYCAN_H
