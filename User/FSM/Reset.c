//
// Created by BXW on 2024/7/29.
//

#include "Reset.h"
void FSMReset(void){
    M2006_SetPosition(1,ResetPos1);
    M2006_SetPosition(2,ResetPos2);
    M2006_SetPosition(3,ResetPos3);
}