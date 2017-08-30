/********************************电机驱动**********************************
*
*作者   ： 沙艺已觉
*文件名 ： System.h
*描述   ： 系统初始化头文件
*时间   ： 2015/11/9
说明    ： 使用山外V5.3库
*
****************************************************************************/
#ifndef   _SYSTEM_H_
#define   _SYSTEM_H_

#include  "common.h"
static int8 b;
static int8 c;
static uint8 out2=1;
#define SECTOR_NUM  (FLASH_SECTOR_NUM-1)         //尽量用最后面的扇区，确保安全
//#define key1   PTB3_IN
//#define key2   PTB9_IN
//#define key3   PTB10_IN
//#define key4   PTB11_IN


void show_img(void);
void OLED_Show_Road(void);
void System_init(void);
void Buzzer_init(void); 
//void beepms(uint8 ms);//使用中断
void Caosheng_init();
void Key_init(void);  
void BM_init(void);
void RED_init(void);
//void Boot_reset(void);
void ADC_init(void);
//void beep(uint8 ms);
float Battery_voltage(void);
void beep();
void didi();
void flash_Winit();
extern __RAMFUNC uint8 flash_cmd();

























#endif
