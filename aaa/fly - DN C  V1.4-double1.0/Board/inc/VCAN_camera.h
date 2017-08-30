/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_camera.h
 * @brief      ����ͷ�����ӿ��ض���
 * @author     ɽ��Ƽ�
 * @version    v5.2.1
 * @date       2015-04-01
 */


#ifndef _VCAN_CAMERA_H_
#define _VCAN_CAMERA_H_


#define CAMERA_OV7725_EAGLE         2       //ɽ��ӥ��
#define CAMERA_OV7725_WOLF          3       //ɽ������


#define USE_CAMERA      CAMERA_OV7725_EAGLE   //ѡ��ʹ�õ� ����ͷ

typedef struct
{
    uint8 addr;                  /*�Ĵ�����ַ*/
    uint8 val;                   /*�Ĵ���ֵ*/
} reg_s;

//����ͼ��ɼ�״̬
typedef enum
{
    IMG_NOTINIT = 0,
    IMG_FINISH,             //ͼ��ɼ����
    IMG_FAIL,               //ͼ��ɼ�ʧ��(�ɼ���������)
    IMG_GATHER,             //ͼ��ɼ���
    IMG_START,              //��ʼ�ɼ�ͼ��
    IMG_STOP,               //��ֹͼ��ɼ�
} IMG_STATUS_e;

//���廷��ͨ��״̬
typedef enum
{
    HX_OUT=5,             //������
    HX_BEFORE=2,              //�뻷��ǰ
    HX_ON=3,             //�뻷����
    HX_ONON=4,
    HX_NORMAL=1,
} HX_STATUS_e;

//����������״̬
typedef enum
{
    QP_YES=1,             
    QP_NO=2,
} QP_STATUS_e;

//�����µ�״̬
typedef enum
{
    PODAO_YES=1,             
    PODAO_NO=2,
} PODAO_STATUS_e;
//�����ϰ�״̬
typedef enum
{
    ZA_NO=1,
    ZA_YES=2,
} ZA_STATUS_e;
//����ʮ�ֳ���״̬
typedef enum
{
    SZ_BEFORE=2,
    SZ_ON=3,
    SZ_BEHAND=4,
    SZ_NORMAL=1,
    SZ_NONE=0,
} SZ_STATUS_e;

typedef struct camera_status
{
     float error;     //ƫ��
     float last_error;//�ϴ�ƫ��
     float curvature; //����
     float speed_control_error;   //�ٶȿ���ƫ��    
}camer_status;

extern camer_status camer;
extern volatile HX_STATUS_e    HX_STATE;
extern volatile QP_STATUS_e    QP_STSTE;
extern volatile PODAO_STATUS_e PODAO_STATE;
extern volatile ZA_STATUS_e    ZA_STATE;
extern volatile SZ_STATUS_e  SZ_STATE;
extern int16 l_cross_turn_point;
extern int16 r_cross_turn_point;

extern int8 sz_speedcount;
extern int8 SZ_count;
//#define VARIANCE_THRESHOLD 4    //����㷽����ֵ
//#define LINE_STARTING_POINT  59   //Ѱ������ʼ��
//#define L_LINE_INITIAL_VALUE  0  //���߳�ʼֵ
//#define R_LINE_INITIAL_VALUE  159  //���߳�ʼֵ
//#define M_LINE_INITIAL_VALUE  80 //���߳�ʼֵ
//#define L_LINE_START  100      //�����Ѱ�����
//#define R_LINE_START  60     //�ұ���Ѱ�����
#define Data_Line 120
#define Data_Count 160


#include  "VCAN_SCCB.h"
#include  "VCAN_OV7725_Eagle.h"

//void speed_control_error();
//void fill_line(float *line,int x1,int x2,float y1,float y2);
float calculation_abs(float x);
//float max_error_point(float x, float y,float z,float average_value);
float calculation_average(float a,float b, float c);
float calculation_variance(float a,float b, float c,float avearge);
//void error_requirements_calculation();
//void camer_error_calculation();
//void least_square_method(int8 *line,int8 raw_start,int8 raw_end);
//void servo_err_get();
void ImgFilter(int16 *line);
void Imgfilter_Mline(int16 *line);
void ImgFilter_range(int16 start,int16 end,int16 *line);
//void matchline(int16 start,int16 end,int16* line);//��������
//float ComSlope_Range(int16 start,int16 end,float * line);
//void crossfit(int16* line,int8 point,uint8 d);//һ�����
//float Slope_Range(int16 D,float * line);
//int16 sum(int16 s,int16 e,int16*line);
//int16 MMN(int16 *line,uint8 s,uint8 e,uint8 choose);
//uint8 Lost_Line_Statistics(int16 D,int16 * line, int16 P); //0Ϊ���� 1Ϊ����   ����ֵ0Ϊδ����  1Ϊ����


extern void img_extract(void *dst, void *src, uint32_t srclen);
//extern void img_sd_init(void);
//extern void img_sd_save(uint8 * imgadrr);
extern void Get_Edge();


 //pit_time_start  (PIT2); 
// Time=pit_time_get(PIT2)/MAX_BUS_CLK;


#endif


