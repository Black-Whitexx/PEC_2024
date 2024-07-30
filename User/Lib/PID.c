/**
  ******************************************************************************
  * @file           : PID.c
  * @author         : 86153
  * @brief          : None
  * @attention      : 都用的位置式PID
  * @date           : 2023/10/31
  ******************************************************************************
  */
#include "PID.h"
#include "MyCan.h"
#include "stdio.h"

/** 定义PID结构体 **/

PIDStruct M2006_MID_Position,M2006_Head_Position;
PIDStruct M2006_Mid_Speed,M2006_Head_Speed;

/*-------------------------------------PID赋值与计算------------------------------------------------------*/

void Pid_set(PIDStruct *pid, float kp, float ki, float kd,
             float integral_limit, float differentiation_limit)
{
    if(pid->kp!=kp || pid->ki!=ki || pid->kd!=kd)
    {
        pid->kp = kp;
        pid->ki = ki;
        pid->kd = kd;

        pid->integral_limit = integral_limit;
        pid->differentiation_limit = differentiation_limit;

        pid->target = 0;
        pid->actual = 0;
        pid->err = 0;
        pid->last_err = 0;
        pid->integral = 0;
        pid->differentiation = 0;
        pid->pout = 0;
        pid->iout = 0;
        pid->dout = 0;
        pid->total_out = 0;
    }
}

void Pid_init(PIDStruct *pid, float kp, float ki, float kd,
              float integral_limit, float differentiation_limit)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->integral_limit = integral_limit;
    pid->differentiation_limit = differentiation_limit;

    pid->target = 0;
    pid->actual = 0;
    pid->err = 0;
    pid->last_err = 0;
    pid->integral = 0;
    pid->differentiation = 0;
    pid->pout = 0;
    pid->iout = 0;
    pid->dout = 0;
    pid->total_out = 0;
}

float PID_realise(PIDStruct *PID, float target, float current, float max_output)
{
    PID->target = target;
    PID->actual = current;
    PID->err = PID->target - PID->actual;



    PID->integral +=PID->err;
    if(PID->integral_limit != 0)     //如果开启了积分限幅
    {
        if(PID->integral < -PID->integral_limit)
            PID->integral = -PID->integral_limit;
        if(PID->integral > PID->integral_limit)
            PID->integral = PID->integral_limit;
    }


    PID->differentiation = PID->err-PID->last_err;

    if(PID->differentiation_limit != 0)  //开启微分限幅
    {
        if(PID->differentiation > PID->differentiation_limit)
            PID->differentiation = PID->differentiation_limit;
        else if(PID->differentiation < -PID->differentiation_limit)
            PID->differentiation = -PID->differentiation_limit;
    }

    PID->pout = PID->kp * PID->err;
    PID->iout = PID->ki * PID->integral;
    PID->dout = PID->kd * PID->differentiation;

    PID->total_out =(int32_t)( PID->pout + PID->iout + PID->dout);

    PID->last_err = PID->err;

    if(PID->total_out > max_output)		//输出限幅 正向
        PID->total_out = max_output;

    else if(PID->total_out < -max_output)	//输出限幅 负向
        PID->total_out = -max_output;

    return PID->total_out;
}

/*-------------------------------------PID赋值与计算------------------------------------------------------*/
PIDStruct M2006Test_Position_PID,M2006Test_Speed_PID,M6020Test_Position_PID,M6020Test_Speed_PID;


union_32 union_pid_val;
PID_ValStruct pid_val={0};
void vofa(uint8_t *data,PID_ValStruct *p_data)
{
    if((data[0]==HEADER)&&data[6]==TAIL)
    {
//        LED_Flashing;
        switch (data[1]) {
            case H_SKP:
                union_pid_val.data_u8[0]=data[2];
                union_pid_val.data_u8[1]=data[3];
                union_pid_val.data_u8[2]=data[4];
                union_pid_val.data_u8[3]=data[5];
                p_data->S_KP = union_pid_val.data_f[0];
                break;

            case H_SKI:
                union_pid_val.data_u8[4]=data[2];
                union_pid_val.data_u8[5]=data[3];
                union_pid_val.data_u8[6]=data[4];
                union_pid_val.data_u8[7]=data[5];
                p_data->S_KI = union_pid_val.data_f[1];
                break;

            case H_SKD:
                union_pid_val.data_u8[8]=data[2];
                union_pid_val.data_u8[9]=data[3];
                union_pid_val.data_u8[10]=data[4];
                union_pid_val.data_u8[11]=data[5];
                p_data->S_KD = union_pid_val.data_f[2];
                break;

            case H_DKP:
                union_pid_val.data_u8[12]=data[2];
                union_pid_val.data_u8[13]=data[3];
                union_pid_val.data_u8[14]=data[4];
                union_pid_val.data_u8[15]=data[5];
                p_data->D_KP = union_pid_val.data_f[3];
                break;

            case H_DKI:
                union_pid_val.data_u8[16]=data[2];
                union_pid_val.data_u8[17]=data[3];
                union_pid_val.data_u8[18]=data[4];
                union_pid_val.data_u8[19]=data[5];
                p_data->D_KI = union_pid_val.data_f[4];
                break;

            case H_DKD:
                union_pid_val.data_u8[20]=data[2];
                union_pid_val.data_u8[21]=data[3];
                union_pid_val.data_u8[22]=data[4];
                union_pid_val.data_u8[23]=data[5];
                p_data->D_KD = union_pid_val.data_f[5];
                break;

            case H_SP:
                union_pid_val.data_u8[24]=data[2];
                union_pid_val.data_u8[25]=data[3];
                union_pid_val.data_u8[26]=data[4];
                union_pid_val.data_u8[27]=data[5];
                p_data->speed = union_pid_val.data_f[6];
                break;

            case H_PS:
                union_pid_val.data_u8[28]=data[2];
                union_pid_val.data_u8[29]=data[3];
                union_pid_val.data_u8[30]=data[4];
                union_pid_val.data_u8[31]=data[5];
                p_data->angle = union_pid_val.data_f[7];
                break;

            default:
                printf("pid Set Value\r\n");
                break;
        }
    }
}

float pid_test(PIDStruct *pid,float actual,float target)
{
    Pid_set(pid, pid_val.S_KP, pid_val.S_KI,pid_val.S_KD, 0, 0);

    PID_realise(pid,target,actual,50000);

    printf("%f,%d,%d,%.2f,%.2f,%.2f\r\n",pid->total_out,actual,target,pid->kp,pid->ki,pid->kd);

    return pid->total_out;
}

void pid_test_motor(uint8_t Mode,int motor_info_Type,uint8_t index)
{
    static int M3508F2_current;
    index=index-1;
    switch (motor_info_Type)
    {
        case M2006_CURRENT_MAX:

            Pid_set(&M2006Test_Speed_PID, pid_val.S_KP, pid_val.S_KI,pid_val.S_KD, 0, 0);
            Pid_set(&M2006Test_Position_PID, pid_val.D_KP,pid_val.D_KI,pid_val.D_KD,0,0);

            if (Mode == PID_TEST_SPEED)
            {
                PID_realise(&M2006Test_Speed_PID,pid_val.speed,motor_info[index].speed,M2006_CURRENT_MAX);
            }
            else if (Mode == PID_TEST_POSITION)
            {
                PID_realise(&M2006Test_Position_PID,pid_val.angle,motor_info[index].true_angle,4000);
                PID_realise(&M2006Test_Speed_PID,M2006Test_Position_PID.total_out,motor_info[index].speed,M2006_CURRENT_MAX);
            }

            Set_current(&hfdcan2,0X1ff,0,0,(int16_t )M2006Test_Speed_PID.total_out,0);
            printf("%d,%f,%f,%f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%ld\r\n",motor_info[index].speed,M2006Test_Speed_PID.target,motor_info[index].true_angle,M2006Test_Position_PID.target,M2006Test_Speed_PID.kp,M2006Test_Speed_PID.ki,M2006Test_Speed_PID.kd,M2006Test_Position_PID.kp,M2006Test_Position_PID.ki,M2006Test_Position_PID.kd,(int32_t )M2006Test_Speed_PID.total_out);
            break;

        default:
            printf("error\r\n");
            break;
    }

}




