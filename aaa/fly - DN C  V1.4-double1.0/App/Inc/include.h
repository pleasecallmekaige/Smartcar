#ifndef __INCLUDE_H__
#define __INCLUDE_H__
#define FIRSTCAR//��
#define   Clockwise  0x07//00000111 ˳ʱ��10
#define   Anti_clockwise  0x07//00000111 ��1a
#include  "common.h"

/*
 * Include �û��Զ����ͷ�ļ�
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"      //IO�ڲ���
#include  "MK60_uart.h"      //����
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"     //�͹��Ķ�ʱ��(��ʱ)
#include  "MK60_pit.h"       //PIT
#include  "MK60_FLASH.h"     //FLASH
#include  "MK60_FTM.h"       //FTM
#include  "MK60_sdhc.h"      //SDHC
#include  "MK60_spi.h"       //SPI
#include  "MK60_adc.h"       //ADC
#include  "MK60_dma.h"       //DMA
#include  "MK60_i2c.h"       //I2C
#include  "MK60_it.h"        //isr


#include  "VCAN_LED.H"          //LED
#include  "VCAN_KEY.H"          //KEY
#include  "VCAN_SCCB.h"         //OV����ͷ��������SCCB������
#include  "VCAN_camera.h"       
#include  "VCAN_OV7725_REG.h"   
#include  "VCAN_NRF24L0.h"           
#include  "ff.h"                //FatFs
#include  "io.h"



#include  "FuzzySet_Speed.h"  //ģ���㷨�趨�ٶ�
#include  "FuzzySet_Casu.h" 
#include  "OLED.h"           //OLED0.96��LED
#include  "Servo_Motor.h"    //���ͷ�ļ�
#include  "Motor.h"          //�������ͷ�ļ�
#include  "System.h"         //ϵͳ��ʼ��
#include  "lcd.h"
#include  "Key.h"
//#include  "Dis.h"
#include "Screen.h"
#include "overtake.h"
//#include "MPU6050.h"
#include "start_chaoce.h"

#include  "extern.h"         //ȫ�ֱ�������            
#include  "VCAN_computer.h"     //�๦�ܵ�������


#define  RED1 PTD2_IN
#define  RED2 PTD3_IN
#define  RED3 PTD0_IN
#define  RED6 PTD11_IN
#define  RED7 PTD12_IN
#define  RED8 PTD13_IN

//���� ���뿪�� ����
#define BM1 gpio_get(PTC11)
#define BM2 gpio_get(PTC10)
#define BM3 gpio_get(PTC9)
#define BM4 gpio_get(PTC8)
#define BM5 gpio_get(PTC7)
#define BM6 gpio_get(PTC6)
#define BM7 gpio_get(PTC5)
#define BM8 gpio_get(PTC4)

//�������ӿ�
#define CSGET PTE10
#define CSOUT PTC14

/*
�ٶ�P 2����10
�ٶ�I 0.15����0.5
*/
#ifdef FIRSTCAR    //��
#define Servo_KP        0.95//0.68
#define Servo_KD        0.78//0.84
#define XS_Servo_KP     0.85//0.35
#define XS_Servo_KD     0.92//0.8
#define Speed_KP        9.3
#define Speed_KI        0.62
#define HighSpeed_KP    15.3
#define HighSpeed_KI    0.85//
#else//ǰ
#define Servo_KP        0.95//0.95//0.95
#define Servo_KD        0.78//0.78//0.78
#define XS_Servo_KP     0.85//0.85//0.85
#define XS_Servo_KD     0.92//0.92//0.92
#define Speed_KP        9.3//8.5 //9.3
#define Speed_KI        0.62//0.62//0.62
#define HighSpeed_KP    15.3//11.5//15.3
#define HighSpeed_KI    0.85//0.72//0.85
#endif




#endif  //__INCLUDE_H__
//����