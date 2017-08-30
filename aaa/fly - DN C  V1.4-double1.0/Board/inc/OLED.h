/*****************************************
文 件 名：LED.h
创 建 者：信意电子科技
创建日期：2012/11/08
描    述：LED128x64显示屏底层驱动
------------------------------------------
调用头文件：


 void LED_Init(void);
 void LED_CLS(void); 
 void LED_Set_Pos(byte x, byte y);//设置坐标函数
 void LED_WrDat(uchar data);   //写数据函数
 void LED_Fill(byte dat);
 void LED_P6x8Char(byte x,byte y,byte ch);
 void LED_P6x8Str(byte x,byte y,byte ch[]);
 void LED_P8x16Str(byte x,byte y,byte ch[]);
 void LED_P14x16Str(byte x,byte y,byte ch[]);
 void LED_PXx16MixStr(byte x, byte y, byte ch[]);
 
void LED_PrintBMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]); 
void LED_PrintValueC(uchar x, uchar y,char data);
void LED_PrintValueI(uchar x, uchar y, int data);
void LED_PrintValueF(uchar x, uchar y, float data, uchar num);
void LED_PrintPIC(uchar table[ROW][COLUMN]);
void LED_PrintPIC2(uchar table[ROW][COLUMN]);
void LED_PrintEdge(void);
void LED_Cursor(uchar cursor_column, uchar cursor_row);

*****************************************/

#ifndef __OLED_H__
#define __OLED_H__

#include "include.h"
#include "common.h"
#include "MK60_gpio.h"

#define byte uint8
#define word uint16
#define GPIO_PIN_MASK      0x1Fu    //0x1f=31,限制位数为0--31有效
#define GPIO_PIN(x)        (((1)<<(x & GPIO_PIN_MASK)))  //把当前位置1





/*****************************************
【功 能 】:初始化引脚定义

【说 明 】:直接在这里改IO就可以了
*******************************************/
#define SCL_PIN         PTC16  //D0 (CLK时钟)
#define SDA_PIN         PTC17   //D1（MOSI数据）
#define RST_PIN         PTC18   //RST（复位）
#define DC_PIN          PTC19  //DC（数据/命令）

#define INIT 0
#define LED_SCL_Init  gpio_init(SCL_PIN,GPO,INIT) // 时钟初始化定义
#define LED_SDA_Init  gpio_init(SDA_PIN,GPO,INIT)//数据口D1
#define LED_RST_Init  gpio_init(RST_PIN,GPO,INIT)//复位低能电平
#define LED_DC_Init   gpio_init(DC_PIN,GPO,INIT)//偏置常低
/******************************  ****************

【功 能 】:IO 定义 OUTPUT

【说 明 】:直接在这里改IO就可以了
*******************************************/
#define LED_SCLH  gpio_set(SCL_PIN,HIGH)// 时钟定义 
#define LED_SCLL  gpio_set(SCL_PIN,LOW)

#define LED_SDAH  gpio_set(SDA_PIN,HIGH)//数据口D1
#define LED_SDAL  gpio_set(SDA_PIN,LOW)

#define LED_RSTH  gpio_set(RST_PIN,HIGH)//复位低能电平
#define LED_RSTL  gpio_set(RST_PIN,LOW)

#define LED_DCH   gpio_set(DC_PIN,HIGH)
#define LED_DCL   gpio_set(DC_PIN,LOW)//偏置常低

/************************************************/

 void LEDPIN_Init(void);   //LED控制引脚初始化
 void OLED_init(void);
 void LED_CLS(void);
 void LED_Set_Pos(byte x, byte y);//设置坐标函数
 void LED_WrDat(uint8 data);   //写数据函数
 void LED_P6x8Char(byte x,byte y,byte ch);
 void LED_P6x8int(byte x,byte y,int a);//显示三位整数
 void LED_P6x8Str(byte x,byte y,byte ch[]);//写入一组6x8标准ASCII字符串
 void LED_P8x16Str(byte x,byte y,byte ch[]);
 void LED_P14x16Str(byte x,byte y,byte ch[]);
 void LED_PXx16MixStr(byte x, byte y, byte ch[]);
 void LED_PrintBMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]); 
 void LED_Fill(byte dat);
 void LED_PrintValueC(uint8 x, uint8 y,char data);//将一个char型数转换成3位数进行显示
 void LED_PrintValueI(uint8 x, uint8 y, int data);//将一个int型数转换成5位数进行显示
 void LED_PrintValueF(uint8 x, uint8 y, float data, uint8 num);//将一个float型数转换成整数部分5位带小数和符号的数据并进行显示
 void LED_PrintEdge(void);
 void LED_Cursor(uint8 cursor_column, uint8 cursor_row);
 void LED_PrintLine(void);

 
void LED_PrintValueI2(uint8 x, uint8 y, int data);//  将一个int型数转换成2位数进行显示 
void LED_PrintValueI3(uint8 x, uint8 y, int data);//  将一个int型数转换成3位数进行显示
void LED_PrintValueI4(uint8 x, uint8 y, int data);//  将一个int型数转换成4位数进行显示
void LED_PrintValueI5(uint8 x, uint8 y, int data);//  将一个int型数转换成5位数进行显示
void LED_PrintsignValueF4(uint8 x, uint8 y, float data);//  将一个float型数转换成4位数带符号进行显示
void LED_PrintsignValueI4(uint8 x, uint8 y, int data);//  将一个int型数转换成4位有符号数进行显示
#endif