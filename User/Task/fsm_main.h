//
// Created by BXW on 2024/7/29.
//

#ifndef PEC_2024_FSM_MAIN_H
#define PEC_2024_FSM_MAIN_H
#include "stm32g4xx.h"
#include "Reset.h"
#include "Place.h"
#include "Vision.h"
#include "magnet.h"
#include "deepmotor.h"
#include "MyCan.h"
#include "usart.h"
void fsm_main(void);
void debug(void);
void init_fsm(void);
#endif //PEC_2024_FSM_MAIN_H
