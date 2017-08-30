#ifndef __LCD_H
#define __LCD_H

////画笔颜色
//#define WHITE         	 0xFFFF
//#define BLACK         	 0x0000	  
//#define BLUE         	 0x001F  
//#define BRED             0XF81F
//#define GRED 		 0XFFE0
//#define GBLUE		 0X07FF
//#define RED           	 0xF800
//#define MAGENTA       	 0xF81F
//#define GREEN         	 0x07E0
//#define CYAN          	 0x7FFF
//#define YELLOW        	 0xFFE0
//#define BROWN 			 0XBC40 //棕色
//#define BRRED 			 0XFC07 //棕红色
//#define GRAY  			 0X8430 //灰色
////GUI颜色
//
//#define DARKBLUE      	 0X01CF	//深蓝色
//#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
//#define GRAYBLUE       	 0X5458 //灰蓝色
////以上三色为PANEL的颜色 
//
//#define LIGHTGREEN     	 0X841F //浅绿色
//#define LGRAY 		 0XC618 //浅灰色(PANNEL),窗体背景色
//
//#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#define WHITE		0xFFFF
#define BLACK		0x0000
#define RED		0x001F
#define BLUE2		0x051F
#define BLUE		0xF800
#define Magenta		0xF81F
#define GREEN		0x07E0
#define YELLOW		0x7FFF
#define CYAN		0xFFE0

#define CS       PTE7_OUT     //片选	
#define LCD_DC   PTE4_OUT	  //数据/命令切换
#define SDA      PTE3_OUT	  //数据
#define SCL      PTD15_OUT	  //时钟
#define RES      PTE6_OUT	  //复位  

#define LCD_W  160
#define LCD_H  128

extern  uint16 BACK_COLOR, POINT_COLOR;   //背景色，画笔色

void LCD_Write_Data(uint8 dat);
void LCD_Write_Data2(uint16 dat16);
void LCD_WriteCommand(uint8 c);
void Tft_init();
void TFT_Reset(void);
void LCD_Clear(uint16 Color);
void Address_set(uint16 x1,uint16 y1,uint16 x2,uint16 y2);
void LCD_DrawPoint(uint16 x,uint16 y);//画点
void LCD_Fill(uint16 xsta,uint16 ysta,uint16 xend,uint16 yend,uint16 color);//在指定区域内填充指定颜色
void LCD_DrawPoint_big(uint16 x,uint16 y,uint16 color);//画一个大点
void LCD_DrawLine(uint16 x1, uint16 y1, uint16 x2, uint16 y2);//画线//x1,y1:起点坐标//x2,y2:终点坐标 
void LCD_DrawRectangle(uint16 x1, uint16 y1, uint16 x2, uint16 y2);//画矩形
void Draw_Circle(uint16 x0,uint16 y0,uint8 r);//在指定位置画一个指定大小的圆//(x,y):中心点//r    :半径
void LCD_ShowChar(uint16 x,uint16 y,uint8 num,uint8 mode);//在指定位置显示一个字符
uint32 mypow(uint8 m,uint8 n);//m^n函数
void LCD_ShowNum(uint16 x,uint16 y,int32 num,uint8 len);//显示2个数字
void LCD_Show2Num(uint16 x,uint16 y,uint16 num,uint8 len);//显示2个数字
void LCD_ShowString(uint16 x,uint16 y,uint8 *p);//显示字符串
void LCD_img(void);
void All_LCD_img(void);
#endif
