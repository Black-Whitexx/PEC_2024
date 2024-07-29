//
// Created by BXW on 2024/7/29.
//

#include "Vision.h"

uint8_t vision_buffer[10];
void get_vision_cmd(uint8_t *pdata){
    memcpy(pdata,vision_buffer,10);
}