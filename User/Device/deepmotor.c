//
// Created by BXW on 2024/7/29.
//

#include "deepmotor.h"

void deepmotor_disable(void)
{
    FDCAN_TxHeaderTypeDef  TxHeader;
    uint32_t can_id;
    uint8_t TxData[8];

    TxHeader.Identifier= 1 + 0x020;                       //32位ID
    TxHeader.IdType=FDCAN_STANDARD_ID;                  //标准ID
    TxHeader.TxFrameType=FDCAN_DATA_FRAME;              //数据帧
    TxHeader.DataLength= FDCAN_DLC_BYTES_0;             //数据长度8字节
    TxHeader.ErrorStateIndicator=FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch=FDCAN_BRS_OFF;               //关闭速率切换
    TxHeader.FDFormat=FDCAN_CLASSIC_CAN;                //传统的CAN模式
    TxHeader.TxEventFifoControl=FDCAN_NO_TX_EVENTS;     //无发送事件
    TxHeader.MessageMarker=0x00;                        //不管捏

    // 等待FDcan的空邮箱 配合下面的判断发送，如果只有下面的判断由于程序运行比数据发送快会产生邮箱拥堵，导致一次进入if判断就出不来了。
    while(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) == 0);
    //发送成功了吗？失败就卡住了捏
    if ( HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData)!= HAL_OK)
    {
        Error_Handler();
    }
}

void deepmotor_enable(void)
{
    FDCAN_TxHeaderTypeDef  TxHeader;
    uint32_t can_id;
    uint8_t TxData[8];

    TxHeader.Identifier= 1 + 0x040;                       //32位ID
    TxHeader.IdType=FDCAN_STANDARD_ID;                  //标准ID
    TxHeader.TxFrameType=FDCAN_DATA_FRAME;              //数据帧
    TxHeader.DataLength= FDCAN_DLC_BYTES_0;             //数据长度8字节
    TxHeader.ErrorStateIndicator=FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch=FDCAN_BRS_OFF;               //关闭速率切换
    TxHeader.FDFormat=FDCAN_CLASSIC_CAN;                //传统的CAN模式
    TxHeader.TxEventFifoControl=FDCAN_NO_TX_EVENTS;     //无发送事件
    TxHeader.MessageMarker=0x00;                        //不管捏

    // 等待FDcan的空邮箱 配合下面的判断发送，如果只有下面的判断由于程序运行比数据发送快会产生邮箱拥堵，导致一次进入if判断就出不来了。
    while(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) == 0);
    //发送成功了吗？失败就卡住了捏
    if ( HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData)!= HAL_OK)
    {
        Error_Handler();
    }
}

void deepmotor_control(float angle, float speed, float kp,
                       float kd, float torque)
{
    angle = 65535 * angle / 80.0f + 65535.0f / 2;
    speed = 16383 * speed / 80.0f + 16383.0f / 2;
    kd = kd * 5.0f;
    torque = 65535 * torque / 80.0f + 65535.0f / 2;

    FDCAN_TxHeaderTypeDef  TxHeader;
    uint8_t TxData[8];

    TxHeader.Identifier= 1 + 0x080;                       //32位ID
    TxHeader.IdType=FDCAN_STANDARD_ID;                  //标准ID
    TxHeader.TxFrameType=FDCAN_DATA_FRAME;              //数据帧
    TxHeader.DataLength= FDCAN_DLC_BYTES_8;             //数据长度8字节
    TxHeader.ErrorStateIndicator=FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch=FDCAN_BRS_OFF;               //关闭速率切换
    TxHeader.FDFormat=FDCAN_CLASSIC_CAN;                //传统的CAN模式
    TxHeader.TxEventFifoControl=FDCAN_NO_TX_EVENTS;     //无发送事件
    TxHeader.MessageMarker=0x00;                        //不管捏

    TxData[0] = (int )angle;
    TxData[1] = (int )angle >> 8;

    TxData[2] = (int )speed;
    TxData[3] = (((int )speed >> 8 ) & 0x3F) | ((int )kp << 6);

    TxData[4] = (int )kp >> 2;
    TxData[5] = (int )kd;

    TxData[6] = (int )torque;
    TxData[7] = (int )torque >> 8;


    // 等待FDcan的空邮箱 配合下面的判断发送，如果只有下面的判断由于程序运行比数据发送快会产生邮箱拥堵，导致一次进入if判断就出不来了。
    while(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) == 0);
    //发送成功了吗？失败就卡住了捏
    if ( HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData)!= HAL_OK)
    {
        Error_Handler();
    }

}

void deepmotor_setposition(const float _position)
{
    const int32_t v_des = 0;
    const int32_t t_ff = 0;
    const int32_t kp = 50;
    const int32_t kd = 5;
    deepmotor_control(_position,v_des,kp,kd,t_ff);
}

void deepmotor_setspeed(const float _speed)
{
    const float p_des = 0;
    const float t_ff = 0;
    const float kp = 0;
    const float kd = 5;
    deepmotor_control(p_des,_speed,kp,kd,t_ff);
}