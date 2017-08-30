/********************************S-D舵机驱动**********************************
*作者   ： 沙艺已觉
*文件名 ： Servo_Motor.c
*描述   ： 舵机驱动头文件
*时间   ： 2015/10/30
*使用说明： 
              黑色        GND
              红色        VCC
              白色        PWM
驱动频率：   300HZ    
*   
***************************************************************************/
#ifndef  _SERVO_MOTOR_H_
#define  _SERVO_MOTOR_H_

#include "include.h"



typedef struct servos_status
{
    float direction_p;                         //方向控制P参数
    float direction_d;                         //方向控制D参数
    int16  PID ;
    float direction_last_error;               //方向上次偏差
    float diretion_current_error;             //方向当前偏差
    int16 direction_duty_output;               //方向控制输出占空比
    
}servos_status;

extern servos_status servos;


//以下参数为FTM_PRECISON 10000u    ]即占空比精度为10000时数据
#define   Servo_freq    50        //c车舵机驱动频率为100HZ b：300
#define   Servo_ftm     FTM2      //FTM模块
#define   Servo_CH      FTM_CH1       //通道 
#ifdef FIRSTCAR
#define   Servo_mid     791// 784
#define   Servo_lmax    709// 700 //702
#define   Servo_rmax    873// 867 //870
#else  //前车
#define   Servo_mid     824//816//827
#define   Servo_lmax    742//734//745 82
#define   Servo_rmax    906//898//909 82
//#define   Servo_mid     791// 784
//#define   Servo_lmax    709// 700 //702
//#define   Servo_rmax    873// 867 //870
#endif

/*
CI车模
驱动频率为100HZ
#define   Servo_mid     1290     // 大左 小右
#define   Servo_lmax    1120     //
#define   Servo_rmax    1460     //
CI车模
驱动频率为150HZ
#define   Servo_mid     1930     // 大左 小右
#define   Servo_lmax    1640     //
#define   Servo_rmax    2225     //
C车模
驱动频率为100HZ
#define   Servo_mid     1644//1480 大左 小右
#define   Servo_lmax    1470//1336      //最小到 325 355   320
#define   Servo_rmax    1818//1630    //最大能到 555 585   555
C车模
驱动频率为50HZ
#define   Servo_mid     822
#define   Servo_lmax    735
#define   Servo_rmax    906
*/
void Servo_Motor_init(void);
void direction_control();


#endif  