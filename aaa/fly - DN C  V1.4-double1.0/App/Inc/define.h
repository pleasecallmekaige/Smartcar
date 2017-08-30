/**********************************ȫ�ֱ�������****************************
*�ļ�����  define.h
*˵����   ȫ�ֱ�������
*ʱ�䣺    2015/11/21
**************************************************************************/


#ifndef _DEFINE_H_
#define _DEFINE_H_

#include "include.h"


uint8 beep_flag = 1;
uint8 a=0;

int8 CarGo = 0 ;                           //��λ
uint8 img_switch_flag = 0;                  //ͼ���ַ�л���־                        
uint8 img_buffer[CAMERA_SIZE];              //����洢����ͼ�������
uint8 img_buffer2[CAMERA_SIZE];             //����洢����ͼ�����һ����    ��ƹ���㷨ʵ�ֽ���洢��
uint8 img_handle[Data_Line][Data_Count];    //����洢��ѹ��Ҫ�����ͼ�������
int16 Lline[CAMERA_H];   //����������
int16 Rline[CAMERA_H];
int16 Mline[CAMERA_H];

int16 Track_complexity ;                //�������ӳ̶�
int16 Prospect_See ;                    //ǰհ 
int16 Last_Prospect_See ;               //��һ����ǰհ
volatile int16 Distance=0 ;                   //����
int16 Last_Distance=0 ;                  //��һ������
float hang=85;


volatile uint8 Car_Stop_flag = 0;                //ͣ����־
volatile uint8 Car_Start_flag = 0;
volatile uint8 Car_Start_flag2 = 0;
uint8 start_line_count=0;
int16 cartime=0;
uint8 Robust_Control_flag=0;            //³�����Ʊ�־                
uint8 Robust_Control_count=0;

uint16 servo_stop_duty ;              //ͣ�����ֹͣ���
uint8 LCDRAM[8][80];

uint8 stop = 0;                       //����һ��ͣ����־
uint8 oled_show_road_flag = 0 ;       //oled�Ƿ�����־
uint8 img_send_flag = 0 ;             //ͼ���Ƿ��͵���λ��
uint8 img_sendmatlab_flag = 0;
uint8 img_save_flag = 0 ;             //ͼ���Ƿ���sd�������־
uint8 send_osc_flag = 0 ;             //�Ƿ��͵�����ʾ������־
uint8 TFTShow_img_flag = 0 ;
uint8 OLED_dis_flag = 0 ;

float var[8];
//float voltage = 7.2;                          

extern IMG_STATUS_e  ov7725_eagle_img_flag;   //ͼ��״̬ 

uint8 nrf_rxbuff[DATA_PACKET];
uint8 nrf_txbuff[DATA_PACKET];
int8 nrf_rxbuff1[DATA_PACKET];
uint8 nrf_txbuff1[DATA_PACKET];


volatile uint8 Ultrasonic_flag = 0;//�������źű�־λ��
volatile uint8 Judgment_obstacle_flag = 0;//�����ϰ�ʶ���־λ
volatile uint8 Judgment_startline_flag = 0;//��������ʶ���־λ
volatile uint8 Return_normalflag = 0;  //��ǰ���ع�������־
volatile uint8 SZ_Caoceflag = 0;  //ʮ�ֳ�����־
int8 current_ZA_state = 0;//�ϰ�״̬��¼
int8 last_ZA_state = 0;
uint8 Current_Startline_state = 0 ;//������״̬��¼
uint8 Last_Startline_state = 0 ;
uint8 Current_SZ_state = 0;
uint8 Last_SZ_state = 0 ;

#ifdef FIRSTCAR//��
volatile uint8 Front_flag = 0;
uint8 Turn_state = 0;//10101010
uint8 Current_Turn_state = 0x00;
uint8 Texting_state = 0x01;
uint8 start_up = 0;
int8 current_ring_state = 0;
int8 last_ring_state = 0;
uint8 HX_text=0;

//int8 Front_flag = 0;
//int8 Normal_Front_flag = 0;
//int8 current_ring_state = 0;
//int8 last_ring_state = 0;
//int8 start_up = 0;
//int8 to_turn = 1;//0��ת��1��ת
  
#else  //ǰ
volatile uint8 Front_flag = 255;
uint8 Turn_state = 0;//01010101
uint8 Current_Turn_state = 0x00;
uint8 Texting_state = 0x01;
uint8 start_up = 0;
int8 current_ring_state = 0;
int8 last_ring_state = 0;
uint8 HX_text=255;
// int8 Front_flag = 1;
// int8 Normal_Front_flag = 1;
// int8 current_ring_state = 0;
// int8 last_ring_state = 0;
// int8 start_up = 0;
// int8 to_turn = 0;//0��ת��1��ת
 
#endif

int16 MAX_DISTANCE=1200; //��Զ�������
int32   SZ_COUNT1=1;
int32   SZ_COUNT2=2;
int32   SZ_COUNT3=3;
int32   SZ_COUNT4=4;
                  //0,  1, 2, 3, 4, 5, 6, 7, 8, 9,
int32   SZ_QLR[20]={0 , 1, 1, 0, 0, 0, 0, 0, 0, 0,
                    0 , 0, 0, 0, 0, 0, 0, 0, 0, 0};
                  //10,11,12,13,14,15,16,17,18,19,
int32   SZ_QLR1;
int32   SZ_QLR2;
int32   SZ_QLR3;
int32   SZ_QLR4;

#endif