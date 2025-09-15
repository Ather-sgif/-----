#include "PID.h"


//位置式PID计算函数
float pid_compute(PID_CNTROLLER *pid)
{
    pid->last_error = pid->now_error;
    // 计算当前误差
    pid->now_error = pid->setpoint - pid->actual;

   // 积分计算
    if(pid->Ki != 0)
    {
         pid->integral += pid->now_error;
         if (pid->integral > pid->integral_limit) // 积分限幅
         {
             pid->integral = pid->integral_limit;
         }
         else if (pid->integral < -pid->integral_limit)
         {
             pid->integral = -pid->integral_limit;
         }
         
    }
    else
    {
        pid->integral = 0.0f; // 如果Ki为0，积分值清零
    }

    // 微分计算
    float derivative = pid->now_error - pid->last_error;

    // PID 输出计算
    pid->output = (pid->Kp * pid->now_error) + (pid->Ki * pid->integral) + (pid->Kd * derivative);
    
    // 位置式输出限幅（计算output后）
    if (pid->output > pid->output_max)
        pid->output = pid->output_max;
    else if (pid->output < pid->output_min)
        pid->output = pid->output_min;

    return pid->output; // 
}
//增量式PID计算函数
float pid_compute_incremental(PID_CNTROLLER *pid)
{
    pid->before_last_error = pid->last_error;
    // 更新上一次误差
    pid->last_error = pid->now_error;
    // 计算当前误差
    pid->now_error = pid->setpoint - pid->actual;

    // PID 输出计算
    pid->output += (pid->Kp * (pid->now_error - pid->last_error)) 
                + (pid->Ki * pid->now_error) 
                + pid->Kd*(pid->now_error-2*pid->last_error+pid->before_last_error);
    // 增量式输出限幅（计算output后）
if (pid->output > pid->output_max)
    pid->output = pid->output_max;
else if (pid->output < pid->output_min)
    pid->output = pid->output_min;
    return pid->output; 
}