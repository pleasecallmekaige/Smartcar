/********************************S-D舵机驱动**********************************
*
*作者   ： 沙艺已觉
*文件名 ： Motor.c
*描述   ： 舵机驱动函数
*时间   ： 2015/11/2
*
****************************************************************************/

#include  "Motor.h"
//uint8 s=0;
motor_status motor;
static uint8 star=0;
float Leftchasu(int16 num);
float Rightchasu(int16 num);
void myMotor_Setspeed(int16 PWM_L,int16 PWM_R);
/*************************************************************************
沙艺已觉
函数名称：motor_init()
入口参数：无
出口参数：无
修改时间：2015/11/2
功能说明：电机驱动初始化
其他说明：参考let_it_go源代码
************************************************************************/



void speed_control(void)
{
//      gearshift_integral((motor.speed_current_error_L + motor.speed_current_error_R)/2,(motor.speed_current_error_L-motor.speed_last_error_L)/2 + (motor.speed_current_error_R-motor.speed_last_error_R)/2
//                       ,HighSpeed_KP,Speed_KP,HighSpeed_KI,Speed_KI);//变速积分
  //读取编码器脉冲数
    filter_Prospect_See();
    motor.avg_speed= (motor.speed_L+ motor.speed_R)/2; //平均速度
    if(Car_Stop_flag==0)star=0;
    if(Car_Stop_flag==1)//正常速度控制
    {
      motor.speed_set =0;//PID停车
      if(motor.avg_speed<50 && star==0)
      {
        star=1;
        motor.speed_duty_output_L=-200;
        motor.speed_duty_output_R=-200;
      }
//      motor.speed_p=9.3;
//      motor.speed_i=0.62;
    }
    else if(0)
    {
      motor.speed_set =290;
    }
    else 
    {
      if( Front_flag==255)//前车环形换慢速表
      {
        if(HX_STATE == HX_ONON || HX_STATE == HX_OUT )
         UFF=UFF_HX;
        else
          UFF=UFF_BEFORE;
        if(Current_Startline_state == QP_YES && start_line_count == 1)
        {
          UFF=UFF_WAIT;
        }
        if(SZ_Caoceflag == 1)
         UFF=UFF_SLOW;
      }
      else
      {
        if((HX_STATE == HX_BEFORE || HX_STATE == HX_ON) || (255==HX_text && HX_STATE == HX_NORMAL))
          UFF=UFF_HX;
        else
         UFF=UFF_BEHAND;
        if(SZ_Caoceflag == 1 && (SZ_STATE==SZ_BEFORE || SZ_STATE==SZ_ON || SZ_STATE==SZ_BEHAND ))
        {
          UFF=UFF_SZH;
        }
      }
      if(Front_flag==0)
      {
//        if(gpio_get (PTE10))
        motor.speed_set = FuzzySet_Speed(Track_complexity, Prospect_See,MAX_DISTANCE-Distance);//MAX_DISTANCE-Distance
      }
      else
      {
        motor.speed_set = FuzzySet_Speed(Track_complexity, Prospect_See,0);//MAX_DISTANCE-Distance
      }
    }
    motor.speed_filter_error[2] = motor.speed_filter_error[1];
    motor.speed_filter_error[1] = motor.speed_filter_error[0];
    motor.speed_filter_error[0] = motor.speed_set;
    motor.speed_set = (int16)(0.7*motor.speed_filter_error[0]+0.2*motor.speed_filter_error[1]+0.1*motor.speed_filter_error[2]);
//速度滤波左 获得速度中值
    motor.speed_set_L=motor.speed_set;
//速度滤波右 获得速度中值
    motor.speed_set_R=motor.speed_set;
    
//    if(camer.error<0)
//    {
//      motor.speed_set_R=(int16)(motor.speed_set_R*(1-0.0*(float)FuzzySet_Casu((int16)camer.error, Prospect_See)/1000.0));
//      motor.speed_set_L=(int16)(motor.speed_set_L*(1+1.0*(float)FuzzySet_Casu((int16)camer.error, Prospect_See)/1000.0));
//    }
//    else
//    {
//      motor.speed_set_R=(int16)(motor.speed_set_R*(1-1.0*(float)FuzzySet_Casu((int16)camer.error, Prospect_See)/1000.0));
//      motor.speed_set_L=(int16)(motor.speed_set_L*(1+0.0*(float)FuzzySet_Casu((int16)camer.error, Prospect_See)/1000.0));
//    }
    if(camer.error<0)
    {
      motor.speed_set_R=(int16)(motor.speed_set_R*(1-0.0*FuzzySet_Casu((int16)ABS(camer.error), Prospect_See)/1000.0));
      motor.speed_set_L=(int16)(motor.speed_set_L*(1-1.0*FuzzySet_Casu((int16)ABS(camer.error), Prospect_See)/1000.0));
    }
    else
    {
      motor.speed_set_R=(int16)(motor.speed_set_R*(1-1.0*FuzzySet_Casu((int16)ABS(camer.error), Prospect_See)/1000.0));
      motor.speed_set_L=(int16)(motor.speed_set_L*(1-0.0*FuzzySet_Casu((int16)ABS(camer.error), Prospect_See)/1000.0));
    }
//左
        motor.speed_last_error_L = motor.speed_current_error_L;        //更新每次的差值
        motor.speed_current_error_L = motor.speed_set_L-motor.speed_L;   //速度当前差值
        motor.speed_duty_output_L = motor.speed_duty_output_L+(int16)(motor.speed_p*(motor.speed_current_error_L-motor.speed_last_error_L)+motor.speed_i*motor.speed_current_error_L);
//右       
        motor.speed_last_error_R = motor.speed_current_error_R;        //更新每次的差值
        motor.speed_current_error_R = motor.speed_set_R-motor.speed_R;   //速度当前差值
        motor.speed_duty_output_R = motor.speed_duty_output_R+(int16)(motor.speed_p*(motor.speed_current_error_R-motor.speed_last_error_R)+motor.speed_i*motor.speed_current_error_R);

 //限幅  
    if(motor.speed_duty_output_L>L_Speed_MAX) motor.speed_duty_output_L=L_Speed_MAX;
    if(motor.speed_duty_output_L<L_Speed_MIN) motor.speed_duty_output_L=L_Speed_MIN;
    if(motor.speed_duty_output_R>R_Speed_MAX) motor.speed_duty_output_R=R_Speed_MAX;
    if(motor.speed_duty_output_R<R_Speed_MIN) motor.speed_duty_output_R=R_Speed_MIN;
    
    
//    if(BM8==0)
//    {
//        ftm_pwm_duty(L_BACK_FTM,L_BACK_CH,0);
//        ftm_pwm_duty(L_GO_FTM,L_GO_CH,500);
//        ftm_pwm_duty(R_GO_FTM,R_GO_CH,500);
//        ftm_pwm_duty(R_BACK_FTM,R_BACK_CH,0);
//      if(motor.speed_L<20 || motor.speed_R<20)
//      {
//          ftm_pwm_duty(L_BACK_FTM,L_BACK_CH,0);
//          ftm_pwm_duty(L_GO_FTM,L_GO_CH,0);
//          ftm_pwm_duty(R_BACK_FTM,R_BACK_CH,0);
//          ftm_pwm_duty(R_GO_FTM,R_GO_CH,0);
//      }
//    }
//    else
//    {
       myMotor_Setspeed(motor.speed_duty_output_R,motor.speed_duty_output_L);
//    }
    
}//速度控制结束
/*************************************************************************
*  函数名称：Rightchasu
*  功能说明：查表法差速
*  参数说明：uint8 load_type  道路类型
*  函数返回：无
*  修改时间：2016-4-21    
*  备    注：以内轮为目标，右内轮，向右拐弯
*************************************************************************/
#define CASU  0.00250// 舵机100HZ :0.00224//差速系数0.000970~0.001940
float Rightchasu(int16 num)  //num<0 ,从-0-- -220
{
    float K_right;
    float p1,p2;
    int16 i;
    i=num;
       
#ifdef FIRSTCAR  
//    if(i>-100 && i<=-40 )
//    {
//      p1 =    CASU*10/6   ;
//      p2 =    1+CASU*400/6  ;
//    }
//    if(i<=-100)
//    {
//      p1 =    CASU   ;//0.001860;//0.002119  ;
//      p2 =   1.0000  ;
//    }
    if(i>-50 && i<=-20 )
    {
      p1 =    CASU*5/3   ;
      p2 =    1+CASU*100/3  ;
    }
    if(i<=-50)
    {
      p1 =    CASU   ;//0.001860;//0.002119  ;
      p2 =   1.0000  ;
    }
#else
        p1 =    0.001860  ;
        p2 =       1.015  ;
#endif
    K_right = p1*i + p2;
    return (K_right);
}

/*************************************************************************
*  函数名称：Leftchasu
*  功能说明：查表法差速
*  参数说明：uint8 load_type  道路类型
*  函数返回：无
*  修改时间：2016-4-21    
*  备    注：以内轮为目标，左内轮，向左拐弯
*************************************************************************/
float Leftchasu(int16 num)   //num>0 ,从-0--  -220
{
    float K_left;
    float p1,p2;
    int16 i;
    i=num; 
#ifdef FIRSTCAR  
//    if(i>=40 && i<100)
//    {
//      p1 =   -CASU*10/6    ;
//    } p2 =   1+CASU*400/6 ;
//    if(i>=100)
//    {
//      p1 =   -CASU   ;//-0.001860;//-0.002077  ;
//      p2 =   1.000   ;
//    }
    if(i>=20 && i<50)
    {
      p1 =   -CASU*5/3    ;
    } p2 =   1+CASU*100/3 ;
    if(i>=50)
    {
      p1 =   -CASU   ;//-0.001860;//-0.002077  ;
      p2 =   1.000   ;
    }
#else
           p1 =   -0.001860  ;
           p2 =       1.012  ;
#endif
    K_left = p1*i + p2;
    return (K_left);
}


/*************************************************************************
*  函数名称：myMotor_Setspeed
*  功能说明：电机输出、带限幅
*  参数说明：
*  函数返回：无
*  修改时间：2016-4-21    
*  备    注：后轮左右轮的距离为13.7cm，前后轮的距离为19.8cm
*************************************************************************/
void myMotor_Setspeed(int16 PWM_R,int16 PWM_L)//给速度
{
  //左
  if(PWM_L>=0)
    {
      ftm_pwm_duty(L_BACK_FTM,L_BACK_CH,0);
      ftm_pwm_duty(L_GO_FTM,L_GO_CH,(uint32)(PWM_L));
    }
  else
    {
      ftm_pwm_duty(L_GO_FTM,L_GO_CH,0);
      ftm_pwm_duty(L_BACK_FTM,L_BACK_CH,(uint32)(-PWM_L));
    } 
  //右
  if(PWM_R>=0)
    {
      ftm_pwm_duty(R_BACK_FTM,R_BACK_CH,0);
      ftm_pwm_duty(R_GO_FTM,R_GO_CH,(uint32)(PWM_R));
    }
  else
    {
      ftm_pwm_duty(R_GO_FTM,R_GO_CH,0);
      ftm_pwm_duty(R_BACK_FTM,R_BACK_CH,(uint32)(-PWM_R));
    }
  
  
//  if(motor.speed_L<50)
//  {
//      ftm_pwm_duty(L_BACK_FTM,L_BACK_CH,0);
//      ftm_pwm_duty(L_GO_FTM,L_GO_CH,0);
//  }
//  if(motor.speed_R<50)
//  {    
//      ftm_pwm_duty(R_BACK_FTM,R_BACK_CH,0);
//      ftm_pwm_duty(R_GO_FTM,R_GO_CH,0);
//  }
}

/*************************************************************************
*  函数名称：Motor_PID_control
*  功能说明：电机PID算法控制
*  参数说明：uint8 load_type  道路类型
*  函数返回：无
*  修改时间：2016-4-21    
*  备    注：后轮左右轮的距离为13.7cm，前后轮的距离为19.8cm
*************************************************************************/
void Motor_init(void)
{
    ftm_pwm_init(L_GO_FTM,L_GO_CH,MOTOR_FRE,INIT_DUTY);
    ftm_pwm_init(L_BACK_FTM,L_BACK_CH,MOTOR_FRE,INIT_DUTY);
    ftm_pwm_init(R_GO_FTM,R_GO_CH,MOTOR_FRE,INIT_DUTY);
    ftm_pwm_init(R_BACK_FTM,R_BACK_CH,MOTOR_FRE,INIT_DUTY);
    
//    ftm_quad_init(FTM1);//正交解码      
//    ftm_quad_init(FTM2); 
    DMA_count_Init(LEFT_ENCODER_DMA_CH,LEFT_ENCODER_COUNT, 0x7FFF,DMA_rising);//左编码器
    DMA_count_Init(RIGHT_ENCODER_DMA_CH,RIGHT_ENCODER_COUNT, 0x7FFF,DMA_rising);//右编码器
    gpio_init(LEFT_ENCODER_DIRECTION,GPI,1);
    gpio_init(RIGHT_ENCODER_DIRECTION,GPI,1);
}

void DMA_Speed_get()
{
    uint8 L,R;
    L=gpio_get(LEFT_ENCODER_DIRECTION);
    R=gpio_get(RIGHT_ENCODER_DIRECTION);
    if(L==0)
    motor.speed_L= DMA_count_get(LEFT_ENCODER_DMA_CH);
    else
    motor.speed_L=-DMA_count_get(LEFT_ENCODER_DMA_CH); 
    if(R==0)
    motor.speed_R=-DMA_count_get(RIGHT_ENCODER_DMA_CH);
    else                                                                                                                                                                                   
    motor.speed_R= DMA_count_get(RIGHT_ENCODER_DMA_CH); 
    
    DMA_count_reset(LEFT_ENCODER_DMA_CH);
    DMA_count_reset(LEFT_ENCODER_DMA_CH);
    DMA_count_reset(RIGHT_ENCODER_DMA_CH);
    DMA_count_reset(RIGHT_ENCODER_DMA_CH);
    filter_speed_L();
    filter_speed_R();
}

//获取左右电机速度，滤波
#define LAG 40.0 //滞后系数 越小越滞后
void filter_speed_L()
{
    static float a = 0;
    float speed_error ;
 //   motor.speed_L=FTM2_Speed_get();
    speed_error = motor.speed_L - motor.speed_L_save[0];
    a = speed_error/LAG ;                //a是系数
    if(a < 0)
      a = -a ;
    if(a > 0.85)
      a = 0.85 ;
  //速度滤波，一阶滞后滤波算法
  motor.speed_L = (int16)(motor.speed_L * (1-a) + (motor.speed_L_save[0]+motor.speed_L_save[1]+motor.speed_L_save[2])*a/3);//(motor.speed_L * (1-a) + (motor.speed_L_save[0]+motor.speed_L_save[1]+motor.speed_L_save[2]+motor.speed_L_save[3]+motor.speed_L_save[4]+motor.speed_L_save[5]+motor.speed_L_save[6]+motor.speed_L_save[7]+motor.speed_L_save[8]+motor.speed_L_save[9])*a/10);// car.speed_new * 0.1 +  
  
  motor.speed_L_save[2] = motor.speed_L_save[1] ;
  motor.speed_L_save[1] = motor.speed_L_save[0] ;
  motor.speed_L_save[0] = motor.speed_L ;
//  car.speed_error = car.speed_new - car.speed_old;
//  car.speed_old = car.speed_new ; 
}

void filter_speed_R()
{
    static float a = 0;
    float speed_error ;
 //   motor.speed_L=FTM2_Speed_get();
    speed_error = motor.speed_R - motor.speed_R_save[0];
    a = speed_error/LAG ;                //a是系数
    if(a < 0)
      a = -a ;
    if(a > 0.85)
      a = 0.85 ;
  //速度滤波，一阶滞后滤波算法
  motor.speed_R = (int16)(motor.speed_R * (1-a) + (motor.speed_R_save[0]+motor.speed_R_save[1]+motor.speed_R_save[2])*a/3);//(motor.speed_R * (1-a) + (motor.speed_R_save[0]+motor.speed_R_save[1]+motor.speed_R_save[2]+motor.speed_R_save[3]+motor.speed_R_save[4]+motor.speed_R_save[5]+motor.speed_R_save[6]+motor.speed_R_save[7]+motor.speed_R_save[8]+motor.speed_R_save[9])*a/10);// car.speed_new * 0.1 +  
  
  motor.speed_R_save[2] = motor.speed_R_save[1] ;
  motor.speed_R_save[1] = motor.speed_R_save[0] ;
  motor.speed_R_save[0] = motor.speed_R ;
//  car.speed_error = car.speed_new - car.speed_old;
//  car.speed_old = car.speed_new ; 
}
void filter_Prospect_See()
{
    static float a = 0;
    float Prospect_error;
    Prospect_error = Prospect_See - Last_Prospect_See;
    a = Prospect_error/20 ;                //分母越小越滞后
    if(a < 0)
      a = -a;
    if(a > 0.85)
      a = 0.85 ;
    
    if(a>0.25)
    Prospect_See = (int16)((1-a)*Prospect_See+a*Last_Prospect_See);
}
void filter_Distance()
{
    static float a = 0;
    float Distance_error = 0;
    Distance_error=ABS(Distance-Last_Distance);
    a = Distance_error/600 ;                //分母越小越滞后
    if(a > 0.85)
    a = 0.85 ;
    Distance = (int16)((1-a)*Distance+a*Last_Distance);
}