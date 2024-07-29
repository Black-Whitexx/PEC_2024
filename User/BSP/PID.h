/**
  ******************************************************************************
  * @file           : PID.h
  * @author         : 86153
  * @brief          : None
  * @attention      : 都用的位置式PID
  * @date           : 2023/10/31
  ******************************************************************************
  */

#ifndef PID_H
#define PID_H

#include "main.h"

#define M3508_CURRENT_MAX   16384
#define M6020_CURRENT_MAX   30000
#define M2006_CURRENT_MAX   10000

/**
  * @brief  PID参数结构体定义
  */
typedef struct{

    float kp;
    float ki;
    float kd;

    float target;
    float actual;

    float err;
    float last_err;

    float pout;
    float iout;
    float dout;
    float total_out;

    float integral;
    float differentiation;

    float integral_limit;
    int differentiation_limit;

}PIDStruct;


typedef union
{
    uint8_t data_u8[8];
    float   data_f[2];
    int data_i32[2];
    uint32_t data_u32[2];
}union_64;



//外部可调用声明
extern PIDStruct M2006_MID_Position,M2006_Head_Position;
extern PIDStruct M2006_Mid_Speed,M2006_Head_Speed;

//以下为函数声明
void Pid_init(PIDStruct *pid, float kp, float ki, float kd,
              float integral_limit, float differentiation_limit);
void Pid_set(PIDStruct *pid, float kp, float ki, float kd,float integral_limit, float differentiation_limit);
float PID_realise(PIDStruct *PID, float target, float current, float max_output);


/*-------------------------------------PID赋值与计算------------------------------------------------------*/
extern PIDStruct M2006Test_Position_PID,M2006Test_Speed_PID,M6020Test_Position_PID,M6020Test_Speed_PID;


#define HEADER      0XC5
#define TAIL        0X5C

#define H_SKP       0x31
#define H_SKI       0x32
#define H_SKD       0x33
#define H_DKP       0x34
#define H_DKI       0x35
#define H_DKD       0x36
#define H_IL        0X37
#define H_IS        0x38
#define H_SP        0X23
#define H_PS        0X24

#define PID_TEST_SPEED     0XAA
#define PID_TEST_POSITION  0XAB

typedef struct{
    float S_KP;
    float S_KI;
    float S_KD;
    float D_KP;
    float D_KI;
    float D_KD;
    float speed;
    float angle;
} PID_ValStruct;

typedef union{
    uint8_t data_u8[32];
    uint16_t data_u16[16];
    int16_t data_16[16];
    int32_t data_32[8];
    uint32_t data_u32[8];
    float data_f[8];
} union_32;

extern union_32 union_pid_val;
extern PID_ValStruct pid_val;

void vofa(uint8_t *data,PID_ValStruct *p_data);
void pid_test_motor(uint8_t Mode,int Motor_Type,uint8_t index);
float pid_test(PIDStruct *pid,float actual,float target);


#endif
