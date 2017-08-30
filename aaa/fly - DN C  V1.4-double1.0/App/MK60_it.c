/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_it.c
 * @brief      ɽ��K60 ƽ̨�жϷ�����
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */

#include    "MK60_it.h"
#include    "include.h"
//#include    "define.h"
#include    "Extern.h"

extern reg_s ov7725_eagle_reg[];
extern  volatile QP_STATUS_e    QP_STSTE;
uint16 Wait_Time=0;
extern int speed;
extern int8 SZ_count;
int8 sz_speedcount=0;
uint8 relen;
/***************************************************
* @author :����ҿ���
* @function name : PIT0_IRQHandler(void)
* @ data : 2017/4/25
�������ɼ��ٶȶ�ʱ���ж�
****************************************************/
void  PIT0_IRQHandler(void)  
{
    PIT_Flag_Clear(PIT0); 
    DMA_Speed_get();
}
/***************************************************
* @author :����ҿ���
* @function name : PIT2_IRQHandler(void)
* @ data : 2017/4/25
������ʶ��ʱ���ж�
****************************************************/
void PIT2_IRQHandler(void)
{
  static int16 PIT2_count=0;
  PIT2_count++;
  if( Front_flag == 255  &&  1000 == PIT2_count )
  {
    Car_Stop_flag=1;
    PIT2_count=0;
    disable_irq(PIT2_IRQn);
  }
  if( Front_flag == 255  &&  20 == PIT2_count && SZ_Caoceflag==1)
  {
    PIT2_count=0;
    sz_speedcount++;
    if(sz_speedcount==45)
    {
      disable_irq(PIT2_IRQn);
    }
  }
  else if( Front_flag == 0  &&  200 == PIT2_count )
  {
    Car_Stop_flag=1;
    PIT2_count=0;
    disable_irq(PIT2_IRQn);
  }

  PIT_Flag_Clear(PIT2); 
}
/***************************************************
* @author :����ҿ���
* @function name : PIT3_IRQHandler(void)
* @ data : 2017/4/25
�������ɼ��ٶȶ�ʱ���ж�
****************************************************/
void  PIT3_IRQHandler(void)  
{
  static int16 PIT3_count=0;
  PIT3_count++;
  if( Front_flag == 255  &&  400 == PIT3_count && SZ_Caoceflag==1)//
  {
    Car_Stop_flag=1;
    PIT3_count=0;
    disable_irq(PIT3_IRQn);
  }
    PIT_Flag_Clear(PIT3); 
}
/***************************************************
* @author :����ҿ���
* @function name : uart1_handler(void)
* @ data : 2017/4/25
ZigBee����ͨ���ж�
****************************************************/
void uart1_handler(void)
{
    uint8 temp;
    UARTn_e uratn = UART1;
    while (!(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)); 
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //�������ݼĴ�����
    {
      

     //�û���Ҫ�����������
//      if(nrf_rxbuff1[0]==46 && nrf_rxbuff1[1]==64)//˫��ͨ��У��
//       {
          temp = UART_D_REG(UARTN[uratn]);
//          LED_PrintsignValueI4(80,1,(int)temp); 
          if(temp==1)//ת����־
          {
             HX_text = 255;
          }
          else if(temp==2)
          {
            Judgment_obstacle_flag ++;
          }
          else if(temp==3)
          {
            Car_Start_flag = 1;
            DELAY_MS(10);
          }
          else if(temp==33)
          {
            Car_Start_flag2 = 1;
            DELAY_MS(10);
          }
          else if(temp==4)
          {
            Judgment_startline_flag = 1;
          }
          else if(temp==5)
          {
            SZ_count++;
            if(SZ_count == SZ_COUNT1 || SZ_count == SZ_COUNT2 || SZ_count == SZ_COUNT3 || SZ_count == SZ_COUNT4)
            {
              SZ_Caoceflag = 1;
            }
          }
          else if(temp==10)//���ܳ�����
          {
            Return_normalflag = 1;
          } 
//          nrf_rxbuff1[0]=0;
//          nrf_rxbuff1[1]=0;
//          nrf_rxbuff1[2]=0;
//       }
 
    }

//    if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK )  //�������ݼĴ�����
//    {
//        //�û���Ҫ����������
//    }
}
void PORTC_IRQHandler(void)
{ 
    uint8  n = 0;    //���ź�
    uint32 flag = PORTC_ISFR;
    PORTC_ISFR  = ~0;                                   //���жϱ�־λ
    n = 0;
    if(flag & (1 << n))                                 //PTE0��NRF�������ж�
    {
    }

}
/******************************************************
* @author :����ҿ���
* @function name : PORTA_Handler()
* @ data : 2017/4/25
* @function description : ���жϷ�����
******************************************************/
void PORTA_IRQHandler(void)
{    
    uint8  n = 0;    //���ź�
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ
    
    n = 29;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
      camera_vsync();
    }
    n = 10;
    if(flag & (1 << n))                                 //PTE4�����ж�
    {
      int8 out=1,out1=1,out2=1;
      int8 a=0;//'>'
      int8 b=1;
      float voltage;
      int16 c=Servo_mid;
      beep();
      DELAY_MS(1);//����
      if(gpio_get(PTA10)==0)
      {
      while(!gpio_get(PTA10));//��ֹ�ж��˳���
      LED_CLS();
      while(out==1)
      {
        out1=1;
      voltage=Battery_voltage();
      LED_P6x8Str(6,0,"Debug");
      LED_P6x8Str(6,1,"1:senddata");
      LED_P6x8Str(6,2,"2:adjust_Servo");
      LED_P6x8Str(6,3,"3:OV7725_CNST");
      LED_P6x8Str(6,4,"Powervoltage:");
      LED_PrintValueF(84,4,voltage,3);
      LED_P6x8Str(122,4,"V");
        switch(Key_Test())
        {
        case 1:
                LED_CLS();
                LED_P6x8Str(8,0,"Matlabimg    off");
                LED_P6x8Str(8,1,"oscilloscope off");
                LED_P6x8Str(8,2,"TFT_img      off");
                LED_P6x8Str(8,3,"OLED_dispiay off");
                while(out1==1)
                {
                    LED_P6x8Str(0,a,">");
                    switch(Key_Test())
                    {    
                      case 1:
                        if(a==0 && img_sendmatlab_flag==0)
                        {
                          img_sendmatlab_flag=1;
                          LED_P6x8Str(86,0,"on ");
                        }
                        else if(a==0 && img_sendmatlab_flag==1)
                        {
                          img_sendmatlab_flag=0;
                          LED_P6x8Str(86,0,"off");
                        }
                        ///////
                        if(a==1 && send_osc_flag==0)
                        {
                          send_osc_flag=1;
                          LED_P6x8Str(86,1,"on ");
                        }
                        else if(a==1 && send_osc_flag==1)
                        {
                          send_osc_flag=0;
                          LED_P6x8Str(86,1,"off ");
                        }
                        /////////
                        if(a==2 && TFTShow_img_flag==0)
                        {
                          TFTShow_img_flag=1;
                          LED_P6x8Str(86,2,"on ");
                        }
                        else if(a==2 && TFTShow_img_flag==1)
                        {
                          TFTShow_img_flag=0;
                          LED_P6x8Str(86,2,"off");
                        }
                        //////////
                        if(a==3 && OLED_dis_flag==0)
                        {
                          OLED_dis_flag=1;
                          LED_P6x8Str(86,3,"on ");
                        }
                        else if(a==3 && OLED_dis_flag==1)
                        {
                          OLED_dis_flag=0;
                          LED_P6x8Str(86,3,"off");
                        }                         
                        break;
                      case 2:
                        LED_P6x8Str(0,a," ");
                        a--;
                        if(a==-1)a=3;
                        break;
                      case 3:
                        LED_P6x8Str(0,a," ");
                        a++;
                        if(a==4)a=0;
                        break;
                      case 4:out1=0;LED_CLS();break;
                      default:break;
                    }
                }
          break;
/**********************************************************************************/
         case 2:
                        LED_CLS();
                        LED_P6x8Str(8,0,"Servo_mid");
                        LED_PrintValueI4(80,0,Servo_mid);
                        while(out1==1)
                        {
                           switch(Key_Test())
                           { 
                             case 1:
                             out2=1;
                             LED_CLS();
                             LED_P6x8Str(8,0,"b=");
                             LED_PrintValueI4(16,0,b);
                               while(out2==1)
                               {
                                 switch(Key_Test())
                                 {
                                    case 2:
                                    b++;
                                    LED_P6x8Str(8,0,"b===");
                                    LED_PrintValueI4(16,0,b);
                                    break;
                                    
                                    case 3:
                                    b--;
                                    LED_P6x8Str(8,0,"b=");
                                    LED_PrintValueI4(16,0,b);
                                    break;
                                    case 4:
                                    LED_P6x8Str(8,0,"Servo_mid");
                                    LED_PrintValueI4(80,0,c);
                                    out2=0;break;
                                    default:break;
                                 }
                               }
                             break;
                             case 2:
                               c += b;
                               LED_P6x8Str(8,0,"Servo_mid");
                               LED_PrintValueI4(80,0,c);
                               break;
                             
                             case 3:
                               c -= b;
                               LED_P6x8Str(8,0,"Servo_mid");
                               LED_PrintValueI4(80,0,c);
                               break;
                             
                             case 4:out1=0;LED_CLS();break;
                             default:break;
                           }
                           ftm_pwm_duty(Servo_ftm,Servo_CH,c);
                         }  
                         break;

/**********************************************************************************/
        case 3:
                        LED_CLS();
                        LED_P6x8Str(8,0,"OV7725_CNST:");
                        LED_PrintValueI4(80,0,(int)(ov7725_eagle_reg[45].val));
                        while(out1==1)
                        {
                           switch(Key_Test())
                           { 
                             case 1:
                             out2=1;
                             LED_CLS();
                             LED_P6x8Str(8,0,"b=");
                             LED_PrintValueI4(16,0,b);
                               while(out2==1)
                               {
                                 switch(Key_Test())
                                 {
                                    case 2:
                                    b++;
                                    LED_P6x8Str(8,0,"b===");
                                    LED_PrintValueI4(16,0,b);
                                    break;
                                    
                                    case 3:
                                    b--;
                                    LED_P6x8Str(8,0,"b=");
                                    LED_PrintValueI4(16,0,b);
                                    break;
                                    case 4:
                                    LED_P6x8Str(8,0,"OV7725_CNST:");
                                    LED_PrintValueI4(80,0,(int)(ov7725_eagle_reg[45].val));
                                    out2=0;break;
                                    default:break;
                                 }
                               }
                             break;
                             case 2:
                               (ov7725_eagle_reg[45].val) += b;
                               LED_P6x8Str(8,0,"OV7725_CNST:");
                               LED_PrintValueI4(80,0,(int)(ov7725_eagle_reg[45].val));
                               break;
                             
                             case 3:
                               (ov7725_eagle_reg[45].val) -= b;
                               LED_P6x8Str(8,0,"OV7725_CNST:");
                               LED_PrintValueI4(80,0,(int)(ov7725_eagle_reg[45].val));
                               break;
                             
                             case 4:out1=0;LED_CLS();break;
                             default:break;
                           } 
                         } 
                         SCCB_WriteByte(ov7725_eagle_reg[45].addr, ov7725_eagle_reg[45].val);
//               LED_CLS();
//               LED_P6x8Str(6,0,"Servo_KP:");
//               LED_P6x8Str(6,1,"Servo_KD:");
//               LED_P6x8Str(6,2,"Speed_KP:");
//               LED_P6x8Str(6,3,"Speed_KI:");
//               a=0;
//               while(out1==1)
//               {
//                 LED_P6x8Str(0,a,">");
//                 LED_PrintValueF(60,0,servos.direction_p,1);
//                 LED_PrintValueF(60,1,servos.direction_d,1);
//                 LED_PrintValueF(60,2,motor.speed_p,1);
//                 LED_PrintValueF(60,3,motor.speed_i,1);
//                 switch(Key_Test())
//                 { 
//                   case 1:
//                     LED_P6x8Str(0,a," ");
//                     a++;
//                     if(a==4)a=0;
//                     break;
//                   case 2: 
//                     switch(a)
//                     {
//                     case 0:servos.direction_p=servos.direction_p+0.1;break;
//                     case 1:servos.direction_d=servos.direction_d+0.1;break;
//                     case 2:motor.speed_p=motor.speed_p+0.1;break;  
//                     case 3:motor.speed_i=motor.speed_i+0.1;break;
//                     default:break;
//                     }
//                     break;
//                   case 3: 
//                     switch(a)
//                     {
//                     case 0:servos.direction_p=servos.direction_p-0.1;break;
//                     case 1:servos.direction_d=servos.direction_d-0.1;break;
//                     case 2:motor.speed_p=motor.speed_p-0.1;break;  
//                     case 3:motor.speed_i=motor.speed_i-0.1;break;
//                     default:break;
//                     }
//                     break;
//                   case 4:out1=0;LED_CLS();break;  
//                   default:break;
//                 }
//               }
               
          break;
/**********************************************************************************/
        case 4:out=0;LED_CLS();break;//�����ж�
        default: break;
        }
      }
    }
    }
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;
}
/******************************************************
* DMA��������ж�
******************************************************/
void DMA8_IRQHandler()
{
    camera_dma();
}
/******************************************************
* @author :����ҿ���
* @function name : PORTB_Handler()
* @ data : 2017/3/17
* @function description : NRF�жϷ�����
******************************************************/
void PORTB_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    flag = PORTB_ISFR;
    PORTB_ISFR  = ~0;                                   //���жϱ�־λ
    n = 0;
    if(flag & (1 << n))                                 //PTE0��NRF�������ж�
    {
    }

}

void PORTD_IRQHandler()
{
     uint8  n;    //���ź�
    uint32 flag;

    flag = PORTD_ISFR;
    PORTD_ISFR  = ~0;                                   //���жϱ�־λ
    n = 0;
    if(flag & (1 << n))                                 //PTE0��NRF�������ж�
    {
    }
}
/******************************************************
* @author :����ҿ���
* @function name : PORTE_Handler()
* @ data : 2017/3/17
* @function description : �������жϷ�����
******************************************************/
void PORTE_IRQHandler()
{
  
    uint8  n;    //���ź�
    uint32 flag;

    flag = PORTE_ISFR;
    PORTE_ISFR  = ~0;                                   //���жϱ�־λ

    n = 12;//�������ж�
    int32 chaoshengboTime;
    if(flag & (1 << n))                                 //PTE12���������������ж�
    {
      if(gpio_get(PTE12)==1)
      {
          PIT_TCTRL1  &= ~PIT_TCTRL_TEN_MASK;//ֹͣ��ʱ��
          PIT_LDVAL1  = 0xFFFFFFFF;//32λ��ʱ����װ���ֵ����
          PIT_TCTRL1  |= PIT_TCTRL_TEN_MASK;//������ʱ��
      }
      else
      {
          if(gpio_get (CSGET))
          {
            Last_Distance=Distance;
            chaoshengboTime = (0xFFFFFFFF - PIT_CVAL1)/MAX_BUS_CLK;//90M����ʱ�ӣ�����õ�ʱ�䣬��λ��΢��
            Distance = chaoshengboTime * 340/1000;//һ���ӵ������ٶȼ���Ϊ340�ף�����chaoshengboTime��λ��΢�룬/1000��õ���λ��mm
            PIT_TCTRL1  &= ~PIT_TCTRL_TEN_MASK;   //ֹͣ��ʱ��
            if(Distance>MAX_DISTANCE)Distance=MAX_DISTANCE;
            if(Distance<0)Distance=0;
            if(Distance<1000)Ultrasonic_flag = 1;//�������źű�־λ��1 //Ҫ�ȳ����ĳ��߹�50CM�������
            else Ultrasonic_flag = 0;
          }
          
      }
      
    }
}