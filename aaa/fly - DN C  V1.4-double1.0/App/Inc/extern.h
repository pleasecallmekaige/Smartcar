/******************************************************************
*
*作者   ： 沙艺已觉
*文件名 ： extern.h
*描述   ： 全局变量头文件
*时间   ： 2015/11/2
*
*****************************************************************************/
#ifndef  _EXTERN_H_
#define  _EXTERN_H_

#include "include.h"

extern uint8 host_flag ;                       //前后车标志 0表示前车; 1表示后车
extern int8 CarGo ;                          //档位
extern uint8 beep_flag ;
extern uint8 img_switch_flag;                      //图像地址切换标志 
extern uint8 img_buffer[CAMERA_SIZE];              //图像缓存地址一
extern uint8 img_buffer2[CAMERA_SIZE];             //图像缓存地址二
extern uint8 img_handle[CAMERA_H][CAMERA_W];       //用于图像处理
extern int16 Lline[CAMERA_H];   //左右线数组
extern int16 Rline[CAMERA_H];
extern int16 Mline[CAMERA_H];

extern int16  Track_complexity ;                //赛道复杂程度
extern int16 Prospect_See ;                    //前瞻
extern int16 Last_Prospect_See ;
extern volatile int16 Distance;
extern int16 Last_Distance;
extern float hang;


extern uint8 PID_Control_flag ;         //速度控制PID控制开启标志
extern uint8 Robust_Control_count;      //鲁棒控制计数
extern uint8 Robust_Control_flag;       //鲁棒控制标志
extern uint8 Robust_Control_Sub_flag ;  
extern volatile uint8 Car_Stop_flag;             //停车标志
extern volatile uint8 Car_Start_flag;            //前车超车起步标志
extern volatile uint8 Car_Start_flag2;
extern int16 cartime;
extern uint8 start_line_count;

extern uint8 LCDRAM[8][80];     //OLED显示
extern float var[8];

extern uint8 stop ;                     //蓝牙控制停车标志
extern uint8 oled_show_road_flag  ;      //oled是否开启标志
extern uint8 img_send_flag ;            //图像是否发送到上位机
extern uint8 img_save_flag ;            //图像是否用sd卡保存标志

extern uint8 send_osc_flag ;             //是否发送到虚拟示波器标志
extern uint8 img_send_flag ;            //图像是否发送到上位机
extern uint8 img_sendmatlab_flag;
extern uint8 TFTShow_img_flag;
extern uint8 OLED_dis_flag;

//extern float voltage ;

extern uint16 servo_stop_duty ;              //停车舵机停止打角  

extern uint32 Time;


extern uint8 Current_Startline_state  ;
extern uint8 Last_Startline_state ;
/************nrf*************/
extern uint8 nrf_rxbuff[DATA_PACKET];
extern uint8 nrf_txbuff[DATA_PACKET];
extern int8 nrf_rxbuff1[DATA_PACKET];
extern uint8 nrf_txbuff1[DATA_PACKET];

extern volatile uint8 Ultrasonic_flag;
extern volatile uint8 Judgment_obstacle_flag;
extern volatile uint8 Judgment_startline_flag;//开启起跑识别标志位
extern volatile uint8 Return_normalflag ;  //起步前车回归正常标志
extern volatile uint8 Front_flag ;//前后车标志位0后1前
extern volatile uint8 SZ_Caoceflag ;  //十字超车标志
extern uint8 Turn_state ;//转向16进制数0左1右
extern uint8 Current_Turn_state;
extern uint8 Texting_state;//判定转向16进制数
extern uint8 start_up;
extern int8 current_ring_state;
extern int8 last_ring_state;
extern uint8 HX_text;
extern int8 current_ZA_state;
extern int8 last_ZA_state;
extern uint8 Current_SZ_state;
extern uint8 Last_SZ_state;
//extern int8 Front_flag ;
//extern int8 Normal_Front_flag;
//extern int8 current_ring_state;
//extern int8 last_ring_state;
//extern int8 start_up;
//extern int8 to_turn;//0左转；1右转


extern int16 MAX_DISTANCE;
extern int32   SZ_COUNT1;
extern int32   SZ_COUNT2;
extern int32   SZ_COUNT3;
extern int32   SZ_COUNT4;

extern int32   SZ_QLR[20];
extern int32   SZ_QLR1;
extern int32   SZ_QLR2;
extern int32   SZ_QLR3;
extern int32   SZ_QLR4;
#endif