#include "include.h"
#include "overtake.h"

int16 vag[1];
uint8 send;
extern IMG_STATUS_e  ov7725_eagle_img_flag;   //图像状态 
int8 SZ_count=0;
extern int8 sz_speedcount;
int x=0;
void overtake()
{
  //起跑线处理 
  Last_Startline_state = Current_Startline_state;
  Current_Startline_state = QP_STSTE;
  if(Current_Startline_state==QP_NO && Last_Startline_state==QP_YES)
  {
    start_line_count++;
    if(start_line_count>=2)//第二次过起跑线停车
    {
      enable_irq(PIT2_IRQn);
    }
    if(Front_flag == 0)//后车清除起跑检测标志
      Judgment_startline_flag = 0;
  }
  if(Current_Startline_state==QP_YES && Last_Startline_state==QP_NO)//前车发起跑线标志
  {
    if(Front_flag == 255)
    {
      send = 4;
      UART_D_REG(UARTN[UART1]) = send;
    }
  }
  if( Front_flag == 255 && Car_Stop_flag == 1 && (Car_Start_flag == 1))
  {
    Car_Stop_flag=0;
//    Car_Start_flag = 0;
  }
//  if( Front_flag == 0 && start_line_count == 1 && Distance<=1000 && sstar==0)//终点前后车发信号让等待的前车起步
//  {
//      send = 3;
//      UART_D_REG(UARTN[UART1]) = send;
//      sstar = 1;
//  }
  //障碍处理
  last_ZA_state = current_ZA_state;
  current_ZA_state = ZA_STATE;
  if(current_ZA_state == ZA_YES && last_ZA_state == ZA_NO && Front_flag == 255)//发送识别障碍信号标志位
  {
    send = 2;
    UART_D_REG(UARTN[UART1]) = send;
    //uart_putchar(UART1 ,send); //发送字符串
  }

  if(current_ZA_state == ZA_NO && last_ZA_state == ZA_YES && Front_flag == 0)//清除识别障碍信号标志位
  {
    Judgment_obstacle_flag--;
  }
  
  
  
  //十字处理
  Last_SZ_state = Current_SZ_state;
  Current_SZ_state = SZ_STATE;
  if( (Last_SZ_state == SZ_NORMAL && Current_SZ_state == SZ_BEFORE && Front_flag==255) || (Last_SZ_state == SZ_NORMAL && Current_SZ_state == SZ_ON && Front_flag==255) )
  {
    SZ_count++;
    send = 5;
    UART_D_REG(UARTN[UART1]) = send;
    if(SZ_count == SZ_COUNT1 || SZ_count == SZ_COUNT2 || SZ_count == SZ_COUNT3 || SZ_count == SZ_COUNT4)
    {
      enable_irq(PIT2_IRQn);
      SZ_Caoceflag = 1;
    }
//        LED_PrintsignValueI4(80,3,(int)SZ_count); 
//    if(SZ_count == 2)
//    {
//      if(Front_flag==255)
//      {
//        gpio_set (CSOUT,0);//前车关超声波
//        SZ_Caoceflag = 1;
//        send = 5;
//        UART_D_REG(UARTN[UART1]) = send;
//      }
//      else
//      {
//        gpio_set (CSOUT,1);//后车开超声波
//      }
//    }
  }


  if(SZ_Caoceflag == 1 && Front_flag == 0 && Return_normalflag ==1  && HX_STATE == HX_NORMAL)
  {
    gpio_set (CSOUT,1);//后车开超声波
    SZ_Caoceflag = 0;
    Return_normalflag = 0;
    Front_flag = 255;//后车变前车
    HX_text = 255;
    Distance=0;
    send = 33;
    UART_D_REG(UARTN[UART1]) = send;
  }
  if(SZ_Caoceflag == 1 && Front_flag == 255 && Car_Start_flag2 == 1  && HX_STATE == HX_NORMAL)
  {
    if(SZ_STATE==SZ_BEHAND)//前车十字后停车起步变后车时回归正常
    {
        SZ_STATE=SZ_NORMAL;
    }
      gpio_set (CSOUT,0);//前车关超声波
      sz_speedcount=0;
      Front_flag = 0;//前车变后车
      HX_text = 0;
      SZ_Caoceflag = 0;
      Car_Stop_flag=0;//起步
      Car_Start_flag2 = 0;
      DELAY_MS(300);
  }
  if(Last_SZ_state == SZ_ON && Current_SZ_state == SZ_BEHAND && Front_flag==255 && SZ_Caoceflag == 1)
          enable_irq (PIT3_IRQn); 

  //环形处理
  last_ring_state = current_ring_state;
  current_ring_state = HX_STATE;  //HX_OUT=1     HX_BFFORE=2     HX_ON=3
  if(current_ring_state == HX_BEFORE && last_ring_state == HX_NORMAL)
  {
    start_up = 0;
  }
  if(current_ring_state == HX_BEFORE && last_ring_state == HX_NORMAL && Front_flag == 255)//环形识别
  {
    send = 1;
    UART_D_REG(UARTN[UART1]) = send;
    HX_text = 0;
  }
  if(current_ring_state == HX_NORMAL && last_ring_state == HX_OUT )//超车起步
  {
    Current_Turn_state = Turn_state & Texting_state;//判定Current_Turn_state是否为0来左右转；0左1右 
    Texting_state = Texting_state << 1;
    if(Texting_state == 0x80)//1000 0000
    {
      Texting_state = 0x01;
    } 
    if(Front_flag == 0)
    {
      send = 3;
      UART_D_REG(UARTN[UART1]) = send;
      Distance=0;
      gpio_set (CSOUT,1);//后车变前车开超声波
    }
    else
    {
      gpio_set (CSOUT,0);//前车变后车关超声波
      Car_Start_flag = 0;
    }
    Front_flag = ~Front_flag;
  }
  
//       var[0] = HX_STATE;
//       var[1] = Front_flag;
//       var[2] = HX_text;
//       var[3] = Judgment_obstacle_flag;   
//       var[4] = Judgment_startline_flag;
//       var[5] = Distance;
//       var[6] = Car_Stop_flag;
//       var[7] = Car_Start_flag;
         var[0] = SZ_STATE;
         var[1] = HX_STATE;
         var[2] = Front_flag;
         var[3] = HX_text;
         var[4] = SZ_count; 
         var[5] = (motor.speed_set_R+motor.speed_set_L)/2;
         var[6] = motor.avg_speed;
//         var[6] = Prospect_See;
         var[7] = camer.error;
       vcan_sendware((uint8_t *)var, sizeof(var));
}

void qipao_chaoce()
{
  while(1)
  {
      if(Front_flag==255)//后车停下后跳出超车循环等待前车超过
      {
        while(Car_Start_flag == 0);
        if(Return_normalflag == 1)
        {
         Return_normalflag = 0 ;
         Car_Start_flag = 0 ;
         break;
        }
      }
      camera_get_img();
      if(img_switch_flag != 0)
      {
        img_extract(img_handle, img_buffer,CAMERA_SIZE);            
      }
      else
      {
        img_extract(img_handle, img_buffer2,CAMERA_SIZE);                     
      }
      
      Last_Startline_state = Current_Startline_state;
      Current_Startline_state = QP_STSTE;
      if(Current_Startline_state==QP_NO && Last_Startline_state==QP_YES)
      {
        start_line_count++;
        if(Front_flag==0 && start_line_count>=1)//后车第一次过起跑线停车
        {
          enable_irq(PIT2_IRQn);
        }
      }
      
      if(Front_flag==0 && motor.avg_speed<10 && Car_Stop_flag==1)//后车停下后跳出超车循环等待前车超过
      {
          Car_Stop_flag = 0;
          ftm_pwm_duty(L_BACK_FTM,L_BACK_CH,0);
          ftm_pwm_duty(L_GO_FTM,L_GO_CH,0);
          ftm_pwm_duty(R_GO_FTM,R_GO_CH,0);
          ftm_pwm_duty(R_BACK_FTM,R_BACK_CH,0);
          send = 3;
          UART_D_REG(UARTN[UART1]) = send;
          break;
      }
      dis();
      Last_Prospect_See=Prospect_See;
      Start_Edge();
      direction_control();
      speed_control();
      while(ov7725_eagle_img_flag != IMG_FINISH)           //等待图像采集完毕
      {
        if(ov7725_eagle_img_flag == IMG_FAIL)            //假如图像采集错误，则重新开始采集
        {
          ov7725_eagle_img_flag = IMG_START;           //开始采集图像
          PORTA_ISFR = ~0;                             //写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
          enable_irq(PORTA_IRQn);                      //允许PTA的中断
        }
      }
  }
}

void start_car()
{
  gpio_init (PTE26, GPO,1);
  gpio_init (PTE25, GPO,1);
  while(Front_flag == 0 && Distance<300)//起跑后车等待
  {
    if(gpio_get (CSGET)==1 && BM8==0 && send != 10)
    {
      send = 10;
      UART_D_REG(UARTN[UART1]) = 10;
    }
  }
//  if(Front_flag == 0 && BM8==0)
//  {
//    send = 10;
//    UART_D_REG(UARTN[UART1]) = 10;
//  }
}


void dis()
{
/****************************环形警示******************************/
//     if((current_ring_state == HX_NORMAL && last_ring_state == HX_OUT)||(current_ring_state == HX_BEFORE && last_ring_state == HX_NORMAL))
//      beep();
        if(HX_STATE == HX_BEFORE || HX_STATE == HX_ON || HX_STATE == HX_ONON || HX_STATE == HX_OUT || (SZ_STATE==SZ_BEFORE ||  SZ_STATE==SZ_ON))//(SZ_STATE==SZ_BEFORE ||  SZ_STATE==SZ_ON)///*|| ZA_STATE == ZA_YES || QP_STSTE == QP_YES || PODAO_STATE==PODAO_YES*/)
          gpio_set (PTE5, 1); 
        else
          gpio_set (PTE5, 0); 
    /*上位机接收*/
    if(1 == send_osc_flag)
    {
        LED_P6x8Str(0,1,"Front_flag=");
        LED_PrintsignValueI4(80,1,(int)Front_flag); 
        LED_P6x8Str(0,2,"HX_text=");
        LED_PrintsignValueI4(80,2,(int)HX_text); 
        LED_P6x8Str(0,3,"Judgment=");
        LED_PrintsignValueI4(80,3,(int)Judgment_obstacle_flag); 
    }
    if(1 == img_send_flag)
    {
        if(img_switch_flag != 0)
           vcan_sendimg(img_buffer, CAMERA_SIZE);                   //发送到上位机
        else
           vcan_sendimg(img_buffer2, CAMERA_SIZE);                  //发送到上位机
    }
    /*Matlab接收图像*/
    if(1 == img_sendmatlab_flag)
    {
        if(img_switch_flag != 0)
        vcan_sendmatlabimg(img_buffer,CAMERA_SIZE);
        else
        vcan_sendmatlabimg(img_buffer2,CAMERA_SIZE);
    }
    if(0)//Matlab 示波器发送控制
    {
      vag[0] = motor.avg_speed;
      vcan_sendmatlabware((uint8_t *)vag, sizeof(vag));
    }
    /*示波器*/
    if(0)
    {
       var[0] = camer.error;
//       var[5] = motor.speed_duty_output_R;
       var[1] = (motor.speed_duty_output_L+motor.speed_duty_output_R)/2;//Prospect_See;
       
       var[2] = motor.speed_set;
       var[3] = motor.avg_speed;
       var[4] = Time;
//       var[4] = (motor.speed_current_error_L + motor.speed_current_error_R)/2;
//       var[5] = (motor.speed_current_error_L-motor.speed_last_error_L)/2 + (motor.speed_current_error_R-motor.speed_last_error_R)/2;
       
//       var[2] = motor.speed_L;
//       var[4] = motor.speed_R;
//       var[3] = motor.speed_set_L;
//       var[5] = motor.speed_set_R;
       
       vcan_sendware((uint8_t *)var, sizeof(var));
    }
    if(1 == TFTShow_img_flag)
    {
      ScreenShow();
    }
    if(1 == OLED_dis_flag /*== OLED_dis_flag*/)
    {
      if(HX_STATE == HX_BEFORE)
      {
        LED_P6x8Str(0,0,"camer.error =");
        LED_PrintsignValueF4(80,0,camer.error); 
        LED_P6x8Str(0,1,"Prospect_See=");
        LED_PrintValueI4(80,1,(int)Prospect_See);
        LED_P6x8Str(0,2,"speed_set   =");
        LED_PrintsignValueI4(80,2,(int)motor.speed_set);  
        LED_P6x8Str(0,3,"l_turn_point=");
        LED_PrintsignValueI4(80,3,(int)l_cross_turn_point); 
        LED_P6x8Str(0,4,"r_turn_point=");
        LED_PrintsignValueI4(80,4,(int)r_cross_turn_point);
        LED_P6x8Str(0,5,"speed_L     =");
        LED_PrintsignValueI4(80,5,(int)motor.speed_L); 
        LED_P6x8Str(0,6,"speed_R     =");
        LED_PrintsignValueI4(80,6,(int)motor.speed_R);
        LED_P6x8Str(0,7,"HX_BEFORE");
//        LED_P6x8Str(0,7,"servo       =");
//        LED_PrintsignValueI4(80,7,(int)(servos.direction_duty_output-Servo_mid));
      }
      if(HX_STATE == HX_ON )
      {
        LED_P6x8Str(0,0,"camer.error =");
        LED_PrintsignValueF4(80,0,camer.error); 
        LED_P6x8Str(0,1,"Prospect_See=");
        LED_PrintValueI4(80,1,(int)Prospect_See);
        LED_P6x8Str(0,2,"speed_set   =");
        LED_PrintsignValueI4(80,2,(int)motor.speed_set);  
        LED_P6x8Str(0,3,"l_turn_point=");
        LED_PrintsignValueI4(80,3,(int)l_cross_turn_point); 
        LED_P6x8Str(0,4,"r_turn_point=");
        LED_PrintsignValueI4(80,4,(int)r_cross_turn_point);
        LED_P6x8Str(0,5,"speed_set_L =");
        LED_PrintsignValueI4(80,5,(int)motor.speed_set_L); 
        LED_P6x8Str(0,6,"speed_set_R =");
        LED_PrintsignValueI4(80,6,(int)motor.speed_set_R);
        LED_P6x8Str(0,7,"HX_ON    ");
//        LED_P6x8Str(0,7,"servo       =");
//        LED_PrintsignValueI4(80,7,(int)(servos.direction_duty_output-Servo_mid));
      }
      if(HX_STATE == HX_ONON)
      {  
        LED_P6x8Str(0,3,"l_turn_point=");
        LED_PrintsignValueI4(80,3,(int)l_cross_turn_point); 
        LED_P6x8Str(0,4,"r_turn_point=");
        LED_PrintsignValueI4(80,4,(int)r_cross_turn_point);
        LED_P6x8Str(0,7,"HX_ONON   ");
      }
      if(HX_STATE == HX_OUT)
      {
        LED_P6x8Str(0,0,"camer.error =");
        LED_PrintsignValueF4(80,0,camer.error); 
        LED_P6x8Str(0,1,"Prospect_See=");
        LED_PrintValueI4(80,1,(int)Prospect_See);
        LED_P6x8Str(0,2,"speed_set   =");
        LED_PrintsignValueI4(80,2,(int)motor.speed_set);  
//        LED_P6x8Str(0,3,"speedL_R:");
//        LED_PrintsignValueI4(54,3,(int)motor.speed_L);
//        LED_PrintsignValueI4(90,3,(int)motor.speed_R);
        LED_P6x8Str(0,3,"l_turn_point=");
        LED_PrintsignValueI4(80,3,(int)l_cross_turn_point); 
        LED_P6x8Str(0,4,"r_turn_point=");
        LED_PrintsignValueI4(80,4,(int)r_cross_turn_point);
//        LED_P6x8Str(0,4,"speed_set_L =");
//        LED_PrintsignValueI4(80,4,(int)motor.speed_set_L); 
        LED_P6x8Str(0,5,"speed_set_R =");
        LED_PrintsignValueI4(80,5,(int)motor.speed_set_R);
        LED_P6x8Str(0,6,"SERVE_OUT:");
        LED_PrintsignValueI4(80,6,(int)(servos.direction_duty_output-Servo_mid));
        LED_P6x8Str(0,7,"HX_OUT   ");
      }
      if(HX_STATE == HX_NORMAL)
      {
        LED_P6x8Str(0,0,"camer.error =");
        LED_PrintsignValueF4(80,0,camer.error); 
        LED_P6x8Str(0,1,"Prospect_See=");
        LED_PrintValueI4(80,1,(int)Prospect_See);
        LED_P6x8Str(0,2,"speed_set   =");
        LED_PrintsignValueI4(80,2,(int)motor.speed_set);  
//        LED_P6x8Str(0,3,"speedL_R:");
//        LED_PrintsignValueI4(54,3,(int)motor.speed_L);
//        LED_PrintsignValueI4(90,3,(int)motor.speed_R);
        LED_P6x8Str(0,3,"l_turn_point=");
        LED_PrintsignValueI4(80,3,(int)l_cross_turn_point); 
        LED_P6x8Str(0,4,"r_turn_point=");
        LED_PrintsignValueI4(80,4,(int)r_cross_turn_point);
//        LED_P6x8Str(0,4,"speed_set =");
//        LED_PrintsignValueI4(54,4,(int)motor.speed_set_L);
//        LED_PrintsignValueI4(90,4,(int)motor.speed_set_R);
        LED_P6x8Str(0,5,"TIME     :");
        LED_PrintValueI5(80,5,(int)Time);
        LED_P6x8Str(0,6,"SERVE_OUT:");
        LED_PrintsignValueI4(80,6,(int)(servos.direction_duty_output-Servo_mid));
        LED_P6x8Str(0,7,"HX_NORMAL");
      }
    }
        if(Car_Stop_flag == 1)
        {
          gpio_init (PTC0,GPO, 0);
          gpio_set (PTC0, 0);
        }
        else
        {
          gpio_init (PTC0,GPO, 0);
          gpio_set (PTC0, 1);
        }
    
        if(Front_flag == 255)
        {
          PTE26_OUT=0;
          PTD15_OUT=1;
        }
        else
        {
          PTE26_OUT=1;
          PTD15_OUT=0;
        }
}