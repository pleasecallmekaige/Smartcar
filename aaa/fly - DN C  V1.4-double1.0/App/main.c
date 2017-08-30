/*!
*     COPYRIGHT NOTICE
*     Copyright (c) 2013,山外科技
*     All rights reserved.
*     技术讨论：山外论坛 http://www.vcan123.com
*
*     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
*     修改内容时必须保留山外科技的版权声明。
*
* @file       main.c
* @brief      山外K60 平台主程序
* @author     山外科技
* @version    v5.3
* @date       2015-04-07
*/
#include "include.h"
#include "define.h"
//FIRSTCAR
//extern uint8 s;

uint32 Time=0;
//void  main(void) 
//{
//  Servo_Motor_init();
//  OLED_init(); 
//  ftm_pwm_duty(Servo_ftm,Servo_CH,200);
//  while(1)
//  {
////        DMA_Speed_get();
////        LED_P6x8Str(0,3,"speed_L     =");
////        LED_PrintValueI5(80,3,(int)motor.speed_L); 
////        LED_P6x8Str(0,4,"speed_R     =");
////        LED_PrintValueI5(80,4,(int)motor.speed_R);
//  }
//}//
void  main(void)
{
  System_init();
  DELAY_MS(1000);
  if(BM8==0)
  {
    //qipao_chaoce();
  }
  start_car();
  
  while(1)
  {
    //获取图像
    camera_get_img();
//    pit_time_start  (PIT0); 
    if(img_switch_flag != 0)
    {
      img_extract(img_handle, img_buffer,CAMERA_SIZE);            
    }
    else
    {
      img_extract(img_handle, img_buffer2,CAMERA_SIZE);                     
    }
//    LED_P6x8Str(0,5,"Distance=");
//    LED_PrintsignValueI4(80,5,(int)Distance); 
    
    dis();
    overtake();
    Last_Prospect_See=Prospect_See;
    Get_Edge();

    direction_control();
    if(PODAO_STATE==PODAO_YES)
    {
          Prospect_See=60;
    }
    speed_control();
//    Time=pit_time_get_us(PIT0);
//    LED_PrintValueI5(80,1,(int)Time); 
    while(ov7725_eagle_img_flag != IMG_FINISH)           //等待图像采集完毕
    {
      if(ov7725_eagle_img_flag == IMG_FAIL)            //假如图像采集错误，则重新开始采集
      {
        ov7725_eagle_img_flag = IMG_START;           //开始采集图像
        PORTA_ISFR = ~0;                             //写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
        enable_irq(PORTA_IRQn);                      //允许PTA的中断
      }
    }
    
    while(BM1==0)
    {
      enable_irq(PORTA_IRQn);
        ftm_pwm_duty(L_BACK_FTM,L_BACK_CH,0);
        ftm_pwm_duty(L_GO_FTM,L_GO_CH,0);
        ftm_pwm_duty(R_GO_FTM,R_GO_CH,0);
        ftm_pwm_duty(R_BACK_FTM,R_BACK_CH,0);
    }
  }
}
//电机测试
//    ftm_pwm_duty(L_BACK_FTM,L_BACK_CH,0);
//    ftm_pwm_duty(L_GO_FTM,L_GO_CH,400);
//    ftm_pwm_duty(R_GO_FTM,R_GO_CH,400);
//    ftm_pwm_duty(R_BACK_FTM,R_BACK_CH,0);
//        LED_P6x8Str(0,5,"speed_L     =");
//        LED_PrintsignValueI4(80,5,(int)motor.speed_L);                   
//        LED_P6x8Str(0,6,"speed_R     =");
//        LED_PrintsignValueI4(80,6,(int)motor.speed_R);
//测距测试
//    DEBUG_PRINTF("distance :%d\n",Distance);
//     LED_PrintsignValueI4(80,6,(int)Distance);
//超车标志位显示
//    LED_PrintValueI4(0,0,(int)Front_flag); 
//    LED_PrintValueI4(30,0,(int)Normal_Front_flag); 
//    LED_PrintValueI4(60,0,(int)start_up); 
//    LED_PrintValueI4(90,0,(int)to_turn);    



