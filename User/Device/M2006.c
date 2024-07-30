//
// Created by BXW on 2024/7/29.
//

#include "M2006.h"
extern moto_info_t motor_info[8];

void M2006_SetPosition(const uint8_t number, int32_t position)
{
    PID_realise(&M2006Test_Position_PID,pid_val.angle,motor_info[0].true_angle,4000);
    PID_realise(&M2006Test_Speed_PID,M2006Test_Position_PID.total_out,motor_info[0].speed,M2006_CURRENT_MAX);
}

