#ifndef __PID_H
#define __PID_H
#include "system.h"
typedef struct
{
    float Kp; // 比例系数
    float Ki; // 积分系数
    float Kd; // 微分系数

    float setpoint; // 目标值
    float actual;   // 实际值
    float output; // 输出值

    float integral; // 积分值
    float now_error; // 当前误差
    float last_error; // 上一次误差
    float before_last_error; // 上上次误差

    float integral_limit; // 积分限幅最大值（绝对值）
    // 在PID结构体中添加输出限幅参数
    float output_max; // 输出最大值
    float output_min; // 输出最小值

} PID_CNTROLLER;

float pid_compute(PID_CNTROLLER *pid);
float pid_compute_incremental(PID_CNTROLLER *pid);
#endif // !_PID_H