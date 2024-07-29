//
// Created by BXW on 2024/7/29.
//

#include "MyCan.h"
void InitCan(){
    FDCAN_FilterTypeDef  CAN_filter1;
    CAN_filter1.IdType = FDCAN_STANDARD_ID; // 标准ID
    CAN_filter1.FilterIndex = 0;
    CAN_filter1.FilterType = FDCAN_FILTER_MASK; // 掩码模式
    CAN_filter1.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    CAN_filter1.FilterID1 = 0x7FFE; // 高位ID
    CAN_filter1.FilterID2 = 0x0000; // 掩码

    if (HAL_FDCAN_ConfigFilter(&hfdcan1, &CAN_filter1) != HAL_OK)
    {
        Error_Handler();
    }

    // 整个外设的开启要放在中断使能前面
    HAL_FDCAN_Start(&hfdcan1);

    /* Activate Rx FIFO 0 new message notification on both FDCAN instances */
    if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE);
}

void CanSendData(const uint8_t id,uint8_t *data){

}
