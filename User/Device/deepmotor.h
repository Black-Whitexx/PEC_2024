//
// Created by BXW on 2024/7/29.
//

#ifndef PEC_2024_DEEPMOTOR_H
#define PEC_2024_DEEPMOTOR_H
#include "stm32g4xx.h"
#include "fdcan.h"
void deepmotor_setposition(const float _position);
void deepmotor_setspeed(const float _speed);
void deepmotor_stop(void);
void deepmotor_control(float angle, float speed, float kp,
                       float kd, float torque);
void deepmotor_enable(void);
#endif //PEC_2024_DEEPMOTOR_H
