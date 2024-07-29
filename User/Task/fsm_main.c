//
// Created by BXW on 2024/7/29.
//

#include "fsm_main.h"
uint8_t fsm_status = 0;
uint8_t vision_data[10];
uint8_t aim_position = 5;
void fsm_main(void){
    switch(fsm_status){
        /* Reset */
        case 0:
            FSMReset();
            get_vision_cmd(vision_data);
            if(vision_data[2] == 0x22){
                fsm_status = 1;
            }
            break;
        case 1:
            place_chess(aim_position);
            if(vision_data[2] = 0x23){
                fsm_status = 2;
            }
        case 2:
            put_down_chess();
            HAL_Delay(1000);
            get_up_chess();
            HAL_Delay(1000);
            fsm_status = 0;
    }
}