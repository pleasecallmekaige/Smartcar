/********************************系统初始化**********************************
*
*作者   ： 沙艺已觉
*文件名 ： System.c
*描述   ： 系统初始化
*时间   ： 2015/11/9
说明    ：使用山外V5.3库
*
****************************************************************************/
#include "System.h"
#include "include.h"

void System_init()
{  
        Tft_init();//显示屏初始化
        LCD_Clear(BLUE);//清屏
        OLED_init();
        //初始化摄像头
        camera_init(img_buffer);                                  //这里设定  imgbuff 为采集缓冲区！！！！！！

        NVIC_SetPriorityGrouping(4);//配置中断服务函数      //设置优先级分组,4bit 抢占优先级,没有亚优先级
        NVIC_SetPriority(PORTB_IRQn,1);         //配置优先级
        NVIC_SetPriority(PORTA_IRQn,0);         //配置优先级
        NVIC_SetPriority(DMA0_IRQn,0);           //配置优先级
        NVIC_SetPriority(PORTE_IRQn,1);         //配置优先级
        NVIC_SetPriority(UART1_RX_TX_IRQn,1);         //配置优先级
        NVIC_SetPriority(PIT0_IRQn,1);          //配置优先级
        NVIC_SetPriority(PIT2_IRQn,1);          //配置优先级
        NVIC_SetPriority(PIT3_IRQn,1);          //配置优先级
         
        set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置PORTA的中断服务函数为 PORTA_IRQHandler 摄像头场中断 不需要使能
        set_vector_handler(DMA8_VECTORn ,DMA8_IRQHandler);      //设置DMA0的中断服务函数为 DMA0_IRQHandler  用于摄像头
        
        uart_init(UART1,115200);                             //ZigBee通信串口初始化        
        set_vector_handler(UART1_RX_TX_VECTORn,uart1_handler); //ZigBee通信串口中断
        uart_rx_irq_en (UART1);                                //使能ZigBee串口中断
 
        Key_init();                                            //按键初始化 
        
//        nrf_init();
//        set_vector_handler(PORTB_VECTORn ,PORTB_IRQHandler);    //NRF中断
//        enable_irq(PORTB_IRQn); //使能NRF中断
//        
//        
        Caosheng_init();
        Buzzer_init();           //蜂鸣器初始化     
        Servo_Motor_init();       //舵机初始化
        Motor_init();            //电机初始化	       
        
        ADC_init();
        BM_init();               //拨码开关初始化
        
        pit_init_ms(PIT0,13);     //初始化PIT0,定时时间为： 13ms
        set_vector_handler(PIT0_VECTORn,PIT0_IRQHandler);
        enable_irq (PIT0_IRQn);    //允许PIT0定时器中断
        
        pit_init_ms(PIT2,1); 
        set_vector_handler(PIT2_VECTORn,PIT2_IRQHandler);
        
        pit_init_ms(PIT3,1); 
        set_vector_handler(PIT3_VECTORn,PIT3_IRQHandler);

        beep();
        
        flash_Winit();
        
        if(0==BM2)//顺时针
        {
          Turn_state=Clockwise;//00000110
        }
        else//逆时针
        {
          Turn_state=Anti_clockwise;//00000100
        }
        if(Front_flag == 255)
        SZ_STATE = SZ_NONE;//为了避免开始图像全为白的那副图
        else
        SZ_STATE = SZ_NORMAL;  
        Current_Turn_state = Turn_state & Texting_state;//判定Current_Turn_state是否为0来左右转；0左1右 
        Texting_state = Texting_state << 1;

//各项参数初始化        
  servos.direction_p = Servo_KP;
  servos.direction_d = Servo_KD;
  motor.speed_p =Speed_KP;
  motor.speed_i =Speed_KI;
  
  motor.speed_filter_error[0] = 0;
  motor.speed_filter_error[1] = 0;
  motor.speed_filter_error[2] = 0;
//  if(0==BM3)
//  {
//    MAX_DISTANCE=1400;
//    hang       = 85.0;
//    UFF_BEFORE = UFF1_BEFORE ;
//    UFF_BEHAND = UFF1_BEHAND ;
//    UFF_HX     = UFF1_HX     ;
//    UFF_SZH    = UFF1_SZH    ;
//    UFF_SLOW   = UFF1_SLOW   ; 
//    UFF_WAIT   = UFF1_WAIT   ;
//  }   
//  if(0==BM4)
//  {
//    MAX_DISTANCE=1400;
//    hang       = 85.0;
//    UFF_BEFORE = UFF2_BEFORE ;
//    UFF_BEHAND = UFF2_BEHAND ;
//    UFF_HX     = UFF2_HX     ;
//    UFF_SZH    = UFF2_SZH    ;
//    UFF_SLOW   = UFF2_SLOW   ; 
//    UFF_WAIT   = UFF2_WAIT   ;
//  }  
  if(0==BM5)
  {
    MAX_DISTANCE=1400;
    hang       = 85.0;
    UFF_BEFORE = UFF3_BEFORE ;
    UFF_BEHAND = UFF3_BEHAND ;
    UFF_HX     = UFF3_HX     ;
    UFF_SZH    = UFF3_SZH    ;
    UFF_SLOW   = UFF3_SLOW   ; 
    UFF_WAIT   = UFF3_WAIT   ;
  }
  if(0==BM6)
  {
    MAX_DISTANCE=1400;
    hang       = 85.0;
    UFF_BEFORE = UFF4_BEFORE ;
    UFF_BEHAND = UFF4_BEHAND ;
    UFF_HX     = UFF4_HX     ;
    UFF_SZH    = UFF4_SZH    ;
    UFF_SLOW   = UFF4_SLOW   ;
    UFF_WAIT   = UFF4_WAIT   ;
  }
  if(0==BM7)
  {
    MAX_DISTANCE=1400;
    hang       = 82.0;
    UFF_BEFORE = UFF0_BEFORE ;
    UFF_BEHAND = UFF0_BEHAND ;
    UFF_HX     = UFF0_HX     ;
    UFF_SZH    = UFF0_SZH    ;
    UFF_SLOW   = UFF0_SLOW   ; 
    UFF_WAIT   = UFF0_WAIT   ;
  }
}

void Caosheng_init()
{
  gpio_init (CSGET, GPI,0);
  gpio_init (PTE12, GPI,0);
  port_init (PTE12, IRQ_EITHER | PF | ALT1 );
  gpio_init (CSOUT, GPO,0);
  gpio_init (PTC15, GPO,0);
  set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);    //超声波中断
  enable_irq(PORTE_IRQn); //使能超声波中断
  if(Front_flag == 255)
  {
    gpio_set (CSOUT,1);
  }
  else
  {
    gpio_set (CSOUT,0);
  }
}

void ADC_init()
{
  adc_init(ADC1_SE6a);       //power_adc   
}

void Buzzer_init()
{
  gpio_init(PTE5,GPO,0);
  port_init(PTE5, ALT1 |PULLUP);
}

void beep()
{
  gpio_set (PTE5, 1);
  DELAY_MS(20);
  gpio_set (PTE5, 0);
}

void didi()
{
  beep();
  DELAY_MS(100);
  beep();
}

void Key_init()
{
  gpio_init (PTA10, GPI,0);
  gpio_init (PTA9, GPI,0);
  gpio_init (PTA17, GPI,0);
  gpio_init (PTA16, GPI,0);
  
  port_init(PTA10, IRQ_FALLING | PF | ALT1 |PULLUP);
  port_init(PTA9, ALT1 |PULLUP);
  port_init(PTA17, ALT1 |PULLUP);
  port_init(PTA16, ALT1 |PULLUP);
  
//  set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);    //按键中断
//  enable_irq(PORTE_IRQn);
}


void BM_init()
{
  gpio_ddr (PTC11, GPI); 
  gpio_ddr (PTC10, GPI);
  gpio_ddr (PTC9, GPI);
  gpio_ddr (PTC8, GPI);
  gpio_ddr (PTC7, GPI);
  gpio_ddr (PTC6, GPI);
  gpio_ddr (PTC5, GPI);
  gpio_ddr (PTC4, GPI);
  
  
  port_init(PTC11,ALT1 |PULLUP);
  port_init(PTC10,ALT1 |PULLUP);
  port_init(PTC9,ALT1 |PULLUP);
  port_init(PTC8,ALT1 |PULLUP);
  port_init(PTC7,ALT1 |PULLUP);
  port_init(PTC6,ALT1 |PULLUP);
  port_init(PTC5,ALT1 |PULLUP);
  port_init(PTC4,ALT1 |PULLUP);
}

float Battery_voltage()
{
        #ifdef FIRSTCAR //后
        #define Vola 0.00219946
        #else
        #define Vola 0.00225931
        #endif
        uint16 ad_value;    
        uint16 value[15];
        float  voltage;
        value[0] = adc_once( ADC1_SE6a,ADC_12bit);
        value[1] = adc_once( ADC1_SE6a,ADC_12bit);
        value[2] = adc_once( ADC1_SE6a,ADC_12bit);
        value[3] = adc_once( ADC1_SE6a,ADC_12bit);
        value[4] = adc_once( ADC1_SE6a,ADC_12bit);
        value[5] = adc_once( ADC1_SE6a,ADC_12bit);
        value[6] = adc_once( ADC1_SE6a,ADC_12bit);
        value[7] = adc_once( ADC1_SE6a,ADC_12bit);
        value[8] = adc_once( ADC1_SE6a,ADC_12bit);
        value[9] = adc_once( ADC1_SE6a,ADC_12bit);
        value[10] = adc_once(ADC1_SE6a,ADC_12bit);
        value[11] = adc_once(ADC1_SE6a,ADC_12bit);
        value[12] = adc_once(ADC1_SE6a,ADC_12bit);
        value[13] = adc_once(ADC1_SE6a,ADC_12bit);
        value[14] = adc_once(ADC1_SE6a,ADC_12bit);
        ad_value=(value[0]+value[1]+value[2]+value[3]+value[4]+value[5]+value[6]+value[7]+value[8]+value[9]+value[10]+value[11]+value[12]+value[13]+value[14])/15;
//        LED_PrintValueI4(0,7,ad_value);
        voltage = (float)(ad_value)*0.00219946; // 0.002360 = (3.3/2^12)*(300/100) 
        adc_stop(ADC1);
        return(voltage);              
}
//void beepms(uint8 ms)//使用中断
//{
//    if(beep_flag==1)
//    { 
//        BEEP = 1;
//      //  stop++;
//        beep_flag=0;   
//        pit_init_ms(PIT3,150);
//        enable_irq (PIT3_IRQn);
//    }
//}
void RED_init()
{
  gpio_init(PTD2,GPI,0); //起跑红外
  gpio_init(PTD3,GPI,0);
  gpio_init(PTD0,GPI,0);
  gpio_init(PTD11,GPI,0);
  gpio_init(PTD12,GPI,0);
  gpio_init(PTD13,GPI,0);
}
void flash_Winit()
{
        
        flash_init();                                       //初始化flash
        DELAY_MS(100);
        SZ_COUNT1=    flash_read(SECTOR_NUM, 0*4, int32);
        SZ_COUNT2=    flash_read(SECTOR_NUM, 1*4, int32);
        SZ_COUNT3=    flash_read(SECTOR_NUM, 2*4, int32);
        SZ_COUNT4=    flash_read(SECTOR_NUM, 3*4, int32);
        SZ_QLR[SZ_COUNT1]=    flash_read(SECTOR_NUM, 4*4, int32);
        SZ_QLR[SZ_COUNT2]=    flash_read(SECTOR_NUM, 5*4, int32);
        SZ_QLR[SZ_COUNT3]=    flash_read(SECTOR_NUM, 6*4, int32);
        SZ_QLR[SZ_COUNT4]=    flash_read(SECTOR_NUM, 7*4, int32);
        SZ_QLR1= SZ_QLR[SZ_COUNT1];
        SZ_QLR2= SZ_QLR[SZ_COUNT2];
        SZ_QLR3= SZ_QLR[SZ_COUNT3];
        SZ_QLR4= SZ_QLR[SZ_COUNT4];
        

        if(0==BM1)//
        {

           LED_CLS();
           LED_P6x8Str(8,0,"SZ_COUNT1=");
           LED_PrintsignValueI4(72,0,SZ_COUNT1);
           LED_P6x8Str(8,1,"SZ_COUNT2=");
           LED_PrintsignValueI4(72,1,SZ_COUNT2);
           LED_P6x8Str(8,2,"SZ_COUNT3=");
           LED_PrintsignValueI4(72,2,SZ_COUNT3);
           LED_P6x8Str(8,3,"SZ_COUNT4=");
           LED_PrintsignValueI4(72,3,SZ_COUNT4);
           LED_P6x8Str(8,4,"SZ_QLR1=");
           LED_PrintsignValueI4(72,4,SZ_QLR1);
           LED_P6x8Str(8,5,"SZ_QLR2=");
           LED_PrintsignValueI4(72,5,SZ_QLR2);
           LED_P6x8Str(8,6,"SZ_QLR3=");
           LED_PrintsignValueI4(72,6,SZ_QLR3);
           LED_P6x8Str(8,7,"SZ_QLR4=");
           LED_PrintsignValueI4(72,7,SZ_QLR4);
           while(out2)
           {
             LED_P6x8Str(0,b,">");
             switch(Key_Test())
             {
                case 1:
                LED_P6x8Str(0,b," ");
                b++;
                if(b==8)b=0;
                break; 
                
                case 2:
                if(b==0)  
                {
                  SZ_COUNT1++;
                  LED_PrintsignValueI4(72,b,SZ_COUNT1);
                }
                if(b==1)
                {
                  SZ_COUNT2++;
                  LED_PrintsignValueI4(72,b,SZ_COUNT2);
                }
                if(b==2)
                {
                  SZ_COUNT3++;
                  LED_PrintsignValueI4(72,b,SZ_COUNT3);
                }
                if(b==3)
                {
                  SZ_COUNT4++;
                  LED_PrintsignValueI4(72,b,SZ_COUNT4);
                }
                if(b==4)  
                {
                  if(SZ_QLR1==1)
                    SZ_QLR1=0;
                  else
                    SZ_QLR1=1;
                  LED_PrintsignValueI4(72,b,SZ_QLR1);
                }
                if(b==5)
                {
                  if(SZ_QLR2==1)
                    SZ_QLR2=0;
                  else
                    SZ_QLR2=1;
                  LED_PrintsignValueI4(72,b,SZ_QLR2);
                }
                if(b==6)
                {
                  if(SZ_QLR3==1)
                    SZ_QLR3=0;
                  else
                    SZ_QLR3=1;
                  LED_PrintsignValueI4(72,b,SZ_QLR3);
                }
                if(b==7)
                {
                  if(SZ_QLR4==1)
                    SZ_QLR4=0;
                  else
                    SZ_QLR4=1;
                  LED_PrintsignValueI4(72,b,SZ_QLR4);
                }
                break;
                
                case 3:
                if(b==0)  
                {
                  SZ_COUNT1--;
                  LED_PrintsignValueI4(72,b,SZ_COUNT1);
                }
                if(b==1)
                {
                  SZ_COUNT2--;
                  LED_PrintsignValueI4(72,b,SZ_COUNT2);
                }
                if(b==2)
                {
                  SZ_COUNT3--;
                  LED_PrintsignValueI4(72,b,SZ_COUNT3);
                }
                if(b==3)
                {
                  SZ_COUNT4--;
                  LED_PrintsignValueI4(72,b,SZ_COUNT4);
                }
                break;
                case 4:
                flash_erase_sector(SECTOR_NUM);                     //擦除扇区
                                                                    //写入flash数据前，需要先擦除对应的扇区(不然数据会乱)
                flash_write(SECTOR_NUM, 0*4, SZ_COUNT1);
                flash_write(SECTOR_NUM, 1*4, SZ_COUNT2);
                flash_write(SECTOR_NUM, 2*4, SZ_COUNT3);
                flash_write(SECTOR_NUM, 3*4, SZ_COUNT4);
                flash_write(SECTOR_NUM, 4*4, SZ_QLR1)  ;
                flash_write(SECTOR_NUM, 5*4, SZ_QLR2)  ;
                flash_write(SECTOR_NUM, 6*4, SZ_QLR3)  ;
                flash_write(SECTOR_NUM, 7*4, SZ_QLR4)  ;
                out2=0;LED_CLS();break;
                default:break;
             }
           }
          
        }
        LED_P6x8Str(0,1,"SZ_COUNT1 =");
        LED_PrintsignValueI4(66,1,(int)SZ_COUNT1);                   
        LED_P6x8Str(0,3,"SZ_COUNT2 =");
        LED_PrintsignValueI4(66,3,(int)SZ_COUNT2);
        LED_P6x8Str(0,5,"SZ_COUNT3 =");
        LED_PrintsignValueI4(66,5,(int)SZ_COUNT3);                   
        LED_P6x8Str(0,7,"SZ_COUNT4 =");
        LED_PrintsignValueI4(66,7,(int)SZ_COUNT4);
        if(SZ_QLR[SZ_COUNT1]==0)
          LED_P6x8Str(102,1,"L");
        else
          LED_P6x8Str(102,1,"R");
        if(SZ_QLR[SZ_COUNT2]==0)
          LED_P6x8Str(102,3,"L");
        else
          LED_P6x8Str(102,3,"R");
        if(SZ_QLR[SZ_COUNT3]==0)
          LED_P6x8Str(102,5,"L");
        else
          LED_P6x8Str(102,5,"R");
        if(SZ_QLR[SZ_COUNT4]==0)
          LED_P6x8Str(102,7,"L");
        else
          LED_P6x8Str(102,7,"R");
}
/******************************************************
* @author : WWW
* @function name : OLED_Show_Road
* @ data : 2016/1/28
* @function description : oled显示赛道
******************************************************/
void OLED_Show_Road()
{
    uint8 i,j;
    uint8 left,right;
    uint8 mid;
    uint8 TLline[CAMERA_H];
    uint8 TRline[CAMERA_H];
    uint8 TMline[CAMERA_H]; 
      
    for(i=0;i<60;i++)
    {
        TLline[i]=(uint8)(Lline[i*2]/2);
        TRline[i]=(uint8)(Rline[i*2]/2);
        if( Mline[i*2] < 0)
         TMline[i] = 0 ;
        else if( Mline[i*2] > 159)
          TMline[i] = 79;
        else if( Mline[i*2] > 0 && Mline[i*2] <159 )
          TMline[i] = (uint8)(Mline[i*2]/2);
    }     
    //画出两条边界
   for(i=0;i<8;i++)
   {
     LED_Set_Pos(4,i);	     
     LED_WrDat(0xff);	
   }
   for(i=0;i<8;i++)
   {
     LED_Set_Pos(92,i);	     
     LED_WrDat(0xff);	
   }
   //清空显示区
   for(i=0;i<8;i++)
   {
     for(j=0;j<80;j++)
     {
       LCDRAM[i][j]=0;
     }
   }
 
   for(i=0;i<60;i++)
   {
     left=TLline[i];
     LCDRAM[i/8][left]=LCDRAM[i/8][left]|(0x01<<(i%8));
   }

   for(i=0;i<60;i++)
   {
     right=TRline[i];
     LCDRAM[i/8][right]=LCDRAM[i/8][right]|(0x01<<(i%8));
   }
   //存中线
     for(i=0;i<60;i++)
     {
      mid=TMline[i];
       LCDRAM[i/8][mid]=LCDRAM[i/8][mid]|(0x01<<(i%8));
     }   
   for(i=8;i>0;i--)
   {
     LED_Set_Pos(9,i-1);				
     for(j=0;j<80;j++)
     {      
         LED_WrDat(LCDRAM[i-1][j]);	    	
     }
   }
   LED_PrintValueF(96,0,camer.speed_control_error, 0);
}

void show_img()//120*160
{
    uint8 i,j;
     //清空显示区
   for(i=0;i<8;i++)
   {
     for(j=0;j<80;j++)
     {
       LCDRAM[i][j]=0;
     }
   }
  for(j=0;j<80;j++)
  {
    for(i=0;i<60;i++)
    {
        if(img_handle[i*2][j*2]==255)
           LCDRAM[i/8][j]=LCDRAM[i/8][j]|(0x01<<(i%8));
    }  
  }
     //显示
   for(i=8;i>0;i--)
   {
     LED_Set_Pos(9,i-1);				
     for(j=0;j<80;j++)
     {      
         LED_WrDat(LCDRAM[i-1][j]);	    	
     }
   }

}

