/********************************�������**********************************
*
*����   �� ɳ���Ѿ�
*�ļ��� �� FuzzySet_Speed.h
*����   �� ģ���㷨�趨�ٶ�ͷ�ļ�
*ʱ��   �� 2016/4/14
˵��    ���ο�FreeCarsģ����������  ʹ��ɽ��V5.3��
*
****************************************************************************/
#ifndef _FUZZYSET_SPEED_H_
#define _FUZZYSET_SPEED_H_

#include "include.h" 

int16 FuzzySet_Speed(int16 P, int16 D,int16 E);  /*ģ����������*/

void gearshift_integral(int16 SP,int16 SD,float PS,float PX,float IS,float IX);

extern int16 *UFF ;
extern int16 *UFF_BEFORE ;
extern int16 *UFF_BEHAND ;
extern int16 *UFF_HX     ;
extern int16 *UFF_SZH    ;
extern int16 *UFF_SLOW   ;
extern int16 *UFF_WAIT   ;

extern int16 UFF0_BEFORE[7] ;
extern int16 UFF0_BEHAND[7] ;
extern int16 UFF0_HX[7]     ;
extern int16 UFF0_SZH[7]    ;
extern int16 UFF0_SLOW[7]   ;
extern int16 UFF0_CX[7]   ;

extern int16 UFF4_BEFORE[7] ;
extern int16 UFF4_BEHAND[7] ;
extern int16 UFF4_HX[7]     ;
extern int16 UFF4_SZH[7]    ;
extern int16 UFF4_SLOW[7]   ;

extern int16 UFF3_BEFORE[7] ;
extern int16 UFF3_BEHAND[7] ;
extern int16 UFF3_HX[7]     ;
extern int16 UFF3_SZH[7]    ;
extern int16 UFF3_SLOW[7]   ;

extern int16 UFF2_BEFORE[7] ;
extern int16 UFF2_BEHAND[7] ;
extern int16 UFF2_HX[7]     ;
extern int16 UFF2_SZH[7]    ;
extern int16 UFF2_SLOW[7]   ;

extern int16 UFF1_BEFORE[7] ;
extern int16 UFF1_BEHAND[7] ;
extern int16 UFF1_HX[7]     ;
extern int16 UFF1_SZH[7]    ;
extern int16 UFF1_SLOW[7]   ;

extern int16 UFF0_WAIT[7]  ;
extern int16 UFF1_WAIT[7]  ;
extern int16 UFF2_WAIT[7]  ;
extern int16 UFF3_WAIT[7]  ;
extern int16 UFF4_WAIT[7]  ;



#endif