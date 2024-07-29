//
// Created by BXW on 2024/7/29.
//

#ifndef PEC_2024_PLACE_H
#define PEC_2024_PLACE_H
#include "stm32g4xx.h"
#include "M2006.h"

typedef struct{
    uint32_t x_axis;
    uint32_t y_axis;
}chess_position;

void place_chess(const uint8_t position);

#endif //PEC_2024_PLACE_H
