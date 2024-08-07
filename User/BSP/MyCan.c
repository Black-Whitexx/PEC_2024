//
// Created by BXW on 2024/7/29.
//

#include "MyCan.h"
void InitCan(){

    //CAN1的过滤器配置
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

    //CAN2的过滤器配置
    FDCAN_FilterTypeDef sFilterConfig2;
    sFilterConfig2.IdType = FDCAN_STANDARD_ID;
    sFilterConfig2.FilterIndex = 1;
    sFilterConfig2.FilterType = FDCAN_FILTER_RANGE;
    sFilterConfig2.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig2.FilterID1 = 0X0000;
    sFilterConfig2.FilterID2 = 0X07FF;
    if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig2) != HAL_OK)
    {
        Error_Handler();
    }
    // 整个外设的开启要放在中断使能前面
    HAL_FDCAN_Start(&hfdcan2);
    /* Activate Rx FIFO 0 new message notification on both FDCAN instances */
    if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_FDCAN_ConfigGlobalFilter(&hfdcan2, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE);
}

void CanSendData(const uint8_t id,uint8_t *data){

}

/**
  * @brief  使用fdcan发送四个rm电机的电流(堵塞式)
  * @param  id_range 发送id 前四个电机为200，后四个为1FF
  * @param  currentx 该组中第x个电机的电流
  */
void Set_current(FDCAN_HandleTypeDef *_hfdcan, int16_t id_range, int16_t current1, int16_t current2, int16_t current3, int16_t current4)
{
    FDCAN_TxHeaderTypeDef  TxHeader;
    uint8_t TxData[8];

    TxHeader.Identifier=id_range;                       //32位ID
    TxHeader.IdType=FDCAN_STANDARD_ID;                  //标准ID
    TxHeader.TxFrameType=FDCAN_DATA_FRAME;              //数据帧
    TxHeader.DataLength= FDCAN_DLC_BYTES_8;             //数据长度8字节
    TxHeader.ErrorStateIndicator=FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch=FDCAN_BRS_OFF;               //关闭速率切换
    TxHeader.FDFormat=FDCAN_CLASSIC_CAN;                //传统的CAN模式
    TxHeader.TxEventFifoControl=FDCAN_NO_TX_EVENTS;     //无发送事件
    TxHeader.MessageMarker=0x00;                        //不管捏

    /*数据赋值*/
    TxData[0] = (current1 >> 8) & 0xff;
    TxData[1] = (current1) & 0xff;
    TxData[2] = (current2 >> 8) & 0xff;
    TxData[3] = (current2) & 0xff;
    TxData[4] = (current3 >> 8) & 0xff;
    TxData[5] = (current3) & 0xff;
    TxData[6] = (current4 >> 8) & 0xff;
    TxData[7] = (current4) & 0xff;

    // 等待FDcan的空邮箱 配合下面的判断发送，如果只有下面的判断由于程序运行比数据发送快会产生邮箱拥堵，导致一次进入if判断就出不来了。
    while(HAL_FDCAN_GetTxFifoFreeLevel(_hfdcan) == 0);
    //发送成功了吗？失败就卡住了捏
    if ( HAL_FDCAN_AddMessageToTxFifoQ(_hfdcan, &TxHeader, TxData)!= HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{

    HAL_StatusTypeDef HAL_RetVal;
    FDCAN_RxHeaderTypeDef RxHeader;
    uint8_t rxdata[8];
    uint8_t data;
    /*电机号记录*/
    static uint8_t index;
    if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
    {
        if(hfdcan->Instance == FDCAN2)
        {
            HAL_RetVal = HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, rxdata);      //从CAN1接收数据，通过过滤器后放入FIFO0,存入RxMessage数据帧
            if(HAL_RetVal == HAL_OK)
            {
                if(RxHeader.Identifier >= 0x201 && RxHeader.Identifier <= 0x204)
                {
                    index = RxHeader.Identifier - 0x201;   //结构体数组0-7对应电机ID1-8
                    Motor_info_record(&motor_info[index], rxdata);   //解包
                }
                else if(RxHeader.Identifier >= 0x205 && RxHeader.Identifier <= 0x208)
                {
                    index = RxHeader.Identifier - 0x201;   //结构体数组0-7对应电机ID1-8
                    Motor_info_record(&motor_info[index], rxdata);   //解包

                }
            }
            __HAL_FDCAN_ENABLE_IT(&hfdcan2,FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
        }
    }
}

/**
 * @brief 使用can1向下层板子发送电梯机构上升指令
 * @param speed
 */
void Send_cmd(uint32_t cmd)
{
    FDCAN_TxHeaderTypeDef TxHeader;    //发送报文结构体定义
    union32 TxData;

    TxHeader.Identifier=cmd;                            //32位ID
    TxHeader.IdType=FDCAN_STANDARD_ID;                  //标准ID
    TxHeader.TxFrameType=FDCAN_DATA_FRAME;              //数据帧
    TxHeader.DataLength= FDCAN_DLC_BYTES_0;             //数据长度8字节
    TxHeader.ErrorStateIndicator=FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch=FDCAN_BRS_OFF;               //关闭速率切换
    TxHeader.FDFormat=FDCAN_CLASSIC_CAN;                //传统的CAN模式
    TxHeader.TxEventFifoControl=FDCAN_NO_TX_EVENTS;     //无发送事件
    TxHeader.MessageMarker=0x00;                        //不管捏

    //等一个空の邮箱呢
    while(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) == 0);

    //发送成功了吗？失败就卡住了捏
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData.data_u8) != HAL_OK)
    {
        Error_Handler();
    }
}

moto_info_t motor_info[8];

/**
 * @brief 对大疆电机的反馈报文进行解包
 * @param ptr 目标电机
 * @param data 8字节can报文
 */
void Motor_info_record(moto_info_t *ptr, uint8_t *data)
{
    /*首次上电时获取电机转子的初始位置*/
    if(ptr->msg_cnt == 0)
    {
        ptr->offset_angle = ptr->angle = ((data[0] << 8) | data[1]);    //转子位置
        ptr->msg_cnt++;     //记录初始位置标志位
    }
    else
    {
        /*转子信息获取*/
        ptr->last_angle = ptr->angle;
        ptr->angle = ((data[0] << 8) | data[1]);    //转子位置
        ptr->speed = ((data[2] << 8) | data[3]);    //转子速度
        ptr->torque_current = ((data[4] << 8) | data[5]);    //转子扭矩
        ptr->temperate = data[6];   //电机温度

        /*判断圈数是否加减*/
        if(ptr->angle - ptr->last_angle > 4095)
            ptr->total_cnt--;
        else if(ptr->angle - ptr->last_angle < -4095)
            ptr->total_cnt++;

        /*计算转过的总角度*/
        ptr->total_angle = ptr->total_cnt * 8192 + ptr->angle - ptr->offset_angle;

        ptr->true_angle=(float)ptr->total_angle*360.0/8192;
    }
}