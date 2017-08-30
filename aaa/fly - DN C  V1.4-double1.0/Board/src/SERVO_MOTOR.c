/********************************S-D舵机驱动**********************************
* @author   ：沙艺已觉
* @file ： Servo_Motor.c
* @description   ： 舵机驱动函数
* @date  ： 2015/10/30
* @Instructions ： 
              黑色        GND
              红色        VCC
              白色        PWM
驱动频率：   100     在100~130HZ   限制在200HZ 
*   
***************************************************************************/
#include "Servo_Motor.h"


servos_status servos;
/*************************************************************************
*作者： 沙艺已觉
*函数名称：Servo_Motor_init
*函数功能：舵机初始化
*入口参数：无
*出口参数：无
*外部调用：是
*修改时间：2015/10/30
************************************************************************/

void Servo_Motor_init(void)
{
        ftm_pwm_init(Servo_ftm,Servo_CH,Servo_freq,Servo_mid);    //初始化舵机       
}

/*********************************************************** 
*作者： 沙艺已觉
*函数名称：Servos_setAngle
*函数功能：舵机打角
*入口参数：u16 duty 占空比
*出口参数：无 
*外部调用：否
*修改时间：2015/10/30
***********************************************************/
void direction_control(void)
{
       
        servos.direction_last_error = servos.diretion_current_error;
        servos.diretion_current_error = camer.error; 
  
        servos.PID = (int16)(servos.direction_p*servos.diretion_current_error+servos.direction_d*(servos.diretion_current_error-servos.direction_last_error));
        servos.direction_duty_output = Servo_mid - servos.PID ;
        if(servos.direction_duty_output>Servo_rmax)   //舵机打角限幅
          servos.direction_duty_output = Servo_rmax;
        else  if(servos.direction_duty_output<Servo_lmax)
          servos.direction_duty_output = Servo_lmax;
        
        servo_stop_duty = servos.direction_duty_output;       //停车舵机停止打角

//          ftm_pwm_duty(Servo_ftm, Servo_CH, Servo_mid);
//        if(Car_Stop_flag==1)
//        {
//         ftm_pwm_duty(Servo_ftm,Servo_CH,Servo_mid);  
//        }
//        else
//        {
         ftm_pwm_duty(Servo_ftm,Servo_CH,servos.direction_duty_output); 
//        }
}








