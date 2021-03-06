#define MatlabDebug 0
#define RTS 0

#define DISPLAY_MLINE 1
#define Fill_Line 1

#if MatlabDebug

#include "mex.h"
#include "math.h"
#define ProgramCar  0

#else 


#define ProgramCar  1

#endif

#if ProgramCar

#include "include.h"
#include "VCAN_camera.h"
#include "extern.h"
#include "start_chaoce.h"

extern camer_status camer;
//volatile HX_STATUS_e    HX_STATE = HX_NORMAL;   //环形状态
//volatile QP_STATUS_e    QP_STSTE = QP_NO;
//volatile PODAO_STATUS_e PODAO_STATE=PODAO_NO;
//volatile ZA_STATUS_e    ZA_STATE = ZA_NO;
#endif

/***********************************************************/
#if MatlabDebug

#define uint8 unsigned char
#define int8 signed char
#define uint16 unsigned short int
#define int16 signed short int 
#define int32 int
#define uint32 unsigned int

#define RANGE(x,max,min)        ((uint8)((x)<(min) ? (min) : ( (x)>(max) ? (max):(x) )))
//定义环形通过状态
typedef enum
{
    HX_OUT=1,             //环形外
    HX_BEFORE=2,              //入环形前
    HX_ON=3,             //入环形中
    HX_ONON=4,
    HX_NORMAL=5,
} HX_STATUS_e;
HX_STATUS_e    HX_STATE = HX_NORMAL;   //环形状态
//定义起跑线状态
typedef enum
{
    QP_YES=1,             
    QP_NO=2,
} QP_STATUS_e;
QP_STATUS_e    QP_STSTE = QP_YES;
//定义坡道状态
typedef enum
{
    PODAO_YES=1,             
    PODAO_NO=2,
} PODAO_STATUS_e;
PODAO_STATUS_e PODAO_STATE=PODAO_NO;
typedef enum
{
    ZA_NO=1,
    ZA_YES=2,
} ZA_STATUS_e;
ZA_STATUS_e    ZA_STATE = ZA_NO;



typedef struct camera_status
{
     float error;     //偏差
     float last_error;//上次偏差
     float curvature; //曲率
     float speed_control_error;   //速度控制偏差    
}camer_status;
camer_status camer;
uint8 Car_Stop_flag=0;
uint8 Car_start_line = 0;
float err;
int16 Track_complexity ;                //赛道复杂程度
int16 Prospect_See ;                    //前瞻 

uint8 Current_Turn_state=1;
uint8 Front_flag = 0;
uint8 current_ring_state = 0;
uint8 last_ring_state = 0;
uint8 start_up = 0;
uint8 HX_text=255;
uint8 Judgment_startline_flag=1;
uint8 Judgment_obstacle_flag=1;

#define ABS(x) (((x) > 0) ? (x) : (-(x)))
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )
#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )


#endif
/**************************通用头文件区***************************/
#include <string.h>
extern float Correction_array[120];/* = { 4.45, 4.3, 4.3, 4.03, 3.79, 3.79, 3.58, 3.58, 3.39, 3.39, 3.31, 3.22, 3.15, 3.07, 
3.0, 2.87, 2.74, 2.74, 2.63, 2.63, 2.63, 2.53, 2.53, 2.48, 2.39, 2.35, 2.3, 2.26, 2.22, 2.22, 2.15, 2.15, 2.08, 
2.08, 2.05, 2.02, 1.98, 1.95, 1.93, 1.93, 1.87, 1.84, 1.82, 1.79, 1.77, 1.77, 1.72, 1.72, 1.7, 1.68, 1.65, 1.65, 
1.63, 1.63, 1.61, 1.59, 1.57, 1.55, 1.54, 1.54, 1.52, 1.5, 1.5, 1.48, 1.45, 1.45, 1.42, 1.42, 1.39, 1.39, 1.37, 
1.36, 1.36, 1.33, 1.33, 1.32, 1.3, 1.3, 1.28, 1.26, 1.26, 1.25, 1.24, 1.23, 1.23, 1.22, 1.21, 1.19, 1.18, 1.18, 
1.17, 1.17, 1.16, 1.15, 1.14, 1.14, 1.13, 1.12, 1.12, 1.11, 1.1, 1.1, 1.09, 1.08, 1.08, 1.07, 1.07, 1.06, 1.05, 
1.04, 1.04, 1.03, 1.03, 1.02, 1.02, 1.02, 1.01, 1.0, 1.0, 1.0
};*/
extern int16 rempcheck_array[120];/*={   24,  25,  26,  27,  28,  29,  30,  32,  32,  34,  35,  36,  37,  38, 39, 40, 
 41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54, 55, 56,
 57,  58,  59,  59,  60,  61,  62,  63,  64,  65,  66,  66,  67,  68, 69, 70,
 70,  71,  72,  73,  74,  75,  76,  76,  77,  78,  78,  80,  80,  81, 82, 82,
 83,  84,  84,  85,  85,  86,  87,  88,  88,  90,  90,  91,  91,  92, 93, 93,
 94,  95,  96,  97,  98,  98,  98, 
100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 111, 111, 111,
111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111,
111, 122, 122, 122, 122
 };*/



#define jump1_lx  l_First_derivative[l.jump_point[0]]
#define jump1_lx1 l_First_derivative[l.jump_point[0]+1]
#define jump1_lx2 l_First_derivative[l.jump_point[0]+2]

#define jump2_lx  l_First_derivative[l.jump_point[1]]
#define jump2_lx1 l_First_derivative[l.jump_point[1]+1]
#define jump2_lx2 l_First_derivative[l.jump_point[1]+2]

#define jump3_lx  l_First_derivative[l.jump_point[2]]
#define jump3_lx1 l_First_derivative[l.jump_point[2]+1]
#define jump3_lx2 l_First_derivative[l.jump_point[2]+2]

#define jump1_rx  r_First_derivative[r.jump_point[0]]
#define jump1_rx1 r_First_derivative[r.jump_point[0]+1]
#define jump1_rx2 r_First_derivative[r.jump_point[0]+2]

#define jump2_rx  r_First_derivative[r.jump_point[1]]
#define jump2_rx1 r_First_derivative[r.jump_point[1]+1]
#define jump2_rx2 r_First_derivative[r.jump_point[1]+2]

#define jump3_rx  r_First_derivative[r.jump_point[2]]
#define jump3_rx1 r_First_derivative[r.jump_point[2]+1]
#define jump3_rx2 r_First_derivative[r.jump_point[2]+2]

#define jump1_r_color_white ((img_handle[r.jump_point[0]][Rline[r.jump_point[0]+1]]==255) ||(img_handle[r.jump_point[0]][Rline[r.jump_point[0]+1]+1]==255))
#define jump1_r_color_black ((img_handle[r.jump_point[0]][Rline[r.jump_point[0]+1]]==0) || (img_handle[r.jump_point[0]][Rline[r.jump_point[0]+1]+1]==0))

#define jump2_r_color_white ((img_handle[r.jump_point[1]][Rline[r.jump_point[1]+1]]==255) ||(img_handle[r.jump_point[1]][Rline[r.jump_point[1]+1]+1]==255))
#define jump2_r_color_black ((img_handle[r.jump_point[1]][Rline[r.jump_point[1]+1]]==0) || (img_handle[r.jump_point[1]][Rline[r.jump_point[1]+1]+1]==0))

#define jump3_r_color_white ((img_handle[r.jump_point[2]][Rline[r.jump_point[2]+1]]==255) ||(img_handle[r.jump_point[2]][Rline[r.jump_point[2]+1]+1]==255))
#define jump3_r_color_black ((img_handle[r.jump_point[2]][Rline[r.jump_point[2]+1]]==0) || (img_handle[r.jump_point[2]][Rline[r.jump_point[2]+1]+1]==0))

#define jump1_l_color_white ((img_handle[l.jump_point[0]][Lline[l.jump_point[0]+1]]==255) || (img_handle[l.jump_point[0]][Lline[l.jump_point[0]+1]-1]==255))
#define jump1_l_color_black ((img_handle[l.jump_point[0]][Lline[l.jump_point[0]+1]]==0) || (img_handle[l.jump_point[0]][Lline[l.jump_point[0]+1]-1]==0))

#define jump2_l_color_white ((img_handle[l.jump_point[1]][Lline[l.jump_point[1]+1]]==255) || (img_handle[l.jump_point[1]][Lline[l.jump_point[1]+1]-1]==255))
#define jump2_l_color_black ((img_handle[l.jump_point[1]][Lline[l.jump_point[1]+1]]==0) || (img_handle[l.jump_point[1]][Lline[l.jump_point[1]+1]-1]==0))

#define jump3_l_color_white ((img_handle[l.jump_point[2]][Lline[l.jump_point[2]+1]]==255) || (img_handle[l.jump_point[2]][Lline[l.jump_point[2]+1]-1]==255))
#define jump3_l_color_black ((img_handle[l.jump_point[2]][Lline[l.jump_point[2]+1]]==0) || (img_handle[l.jump_point[2]][Lline[l.jump_point[2]+1]-1]==0))



#define VARIANCE_THRESHOLD 8   //跳变点方差阈值
#define HUANXING_THRESHOLD 1.5   //环形线性度阈值

//#define LINE_STARTING_POINT  59   //寻线行起始点
#define L_LINE_INITIAL_VALUE  0  //左线初始值
#define R_LINE_INITIAL_VALUE  159  //右线初始值
#define M_LINE_INITIAL_VALUE  80 //中线初始值
#define EMPTY  160 
// #define L_LINE_START  140      //左边线寻线起点
// #define R_LINE_START  20      //右边线寻线起点


typedef struct JUMP_POINT //跳变点结构体
{
        int8 bend_jump_point ;
        int8 jump_point[3] ;
        int8 jump_count ;
        int8 s_jump_point;
        int8 x_jump_point;
}JUMP_POINT;
/************************************************数学工具函数**********************************/
// int16 FuzzySet_Speed(int16 P, int16 D);
//static void Give_Prospect_See();
static void ImgFilter(int16 * line);
static void Imgfilter_Mline(int16 * line);
static float calculation_average(float a,float b, float c);
static float calculation_variance(float a,float b, float c,float avearge);
//static void matchline(int16 start,int16 end,int16 * line);//两点连线
//static float Slope_Range(int16 D,float * line);
static int16 MMN(int16 *line,uint8 s,uint8 e,uint8 choose);
/***********************************************************************************************/
/*********************************************算法函数声明***************************************/
static void find_jump_point();//找跳变点
//static int8 AcquireInflecPoint(int end, int16* line, char flag);//找拐点
static void camer_error_calculation();
static void find_yuanhuan_l();
static void find_yuanhuan_r();
static void avoid_obstacle();//
static void ramp_check(int8 H,int8 L,float ramp_V);//坡道检测
static void fill_bend();
static void fill_r_line(int a);//补右线
static void fill_l_line(int a);
/************************************************************************************************/

/**************************定义变量****************************/
static JUMP_POINT l={0,{0},0};
static JUMP_POINT r={0,{0},0};
static uint8 nomal_track_flag = 1; //正常赛道
//static float Left_slope_range,Right_slope_range;
//static float range_derivative;
//uint8 l_complete_lost_flag = 0 ;  //左线完全丢线标志
//uint8 r_complete_lost_flag = 0;   //右线完全丢失标志      
static int16  l_cross_turn_point = 0;
static int16  r_cross_turn_point = 0;
static float l_First_derivative[118] = {0};   //左线一阶导
static float r_First_derivative[118] = {0};   //右线一阶导
//float k_First_derivative[118] = {0};
static float l_average[116] = {0};            //左线平均值
static float r_average[116] = {0};            //右线平均值
//float k_average[116] = {0};          
//float m_average[116] = {0};
static float l_variance[116] = {0};           //左线方差
static float r_variance[116] = {0};           //右线方差
//float k_variance[116] = {0};          
//float m_variance[116] = {0};
//int16 saoxian[120];
static int16 line_start = 80 ; //寻线起点 
static int16 next_line_start=80;
//int16 fill_line_width =72 ;
static int16 i,j;    //循环变量   
static int8 row ;   //寻线行变量
extern int16 Lline1[120];
extern int16 Rline1[120];
extern int16 Lline2[120];
extern int16 Rline2[120];
static int16 l_head_point;
static int16 r_head_point;
static int8  fork_point  = 0;  
static int8  fork_long  = 0;  
static int16 mid_l_x;
static int16 mid_l_s;
static int16 mid_r_x;
static int16 mid_r_s;

#if MatlabDebug
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 colour[2] = {255, 0}; //0 和 1 分别对应的颜色
  uint8 * mdst = dst;
  uint8 * msrc = src;
  //注：山外的摄像头 0 表示 白色，1表示 黑色
  uint8 tmpsrc;
  while(srclen --)
  {
    tmpsrc = *msrc++;
    *mdst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
    *mdst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
    *mdst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
    *mdst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
    *mdst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
    *mdst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
    *mdst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
    *mdst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
  }
}
    	uint8 img_handle[120][160];
		int16 Rline[120];
		int16 Lline[120];
		int16 Mline[120];
 #endif       

/******************************************************/
#if ProgramCar
void Start_Edge()
{
/*****************************************************************************************************开始处理***************************************************************************************************************/     
#endif

#if MatlabDebug
#define OUTNUM 12
double *inData;
double *outdata0;
double *outdata1;
double *outdata2;
double *outdata3;
double *outdata4;
double *outdata5;
double *outdata6;
double *outdata7;
double *outdata8;
double *outdata9;
double *outdata10;
double *outdata11;
#if RTS
double *outdata_img;
int nlhs=OUTNUM+1;
#else 
int nlhs=OUTNUM;
#endif
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
/*****************************************************************************************************开始处理***************************************************************************************************************/
    //nlhs代表的是输出参数的个数
    //plhs是一个指针数组，里面的指针指向mxArray类型，每一个指针指向一个输出
    //nrhs代表的是输入参数的个数
    //prhs是一个指针数组，里面的指针指向mxArray类型，每一个指针指向一个输入
    uint16 CAMERA_H  ;  //行  高
    uint16 CAMERA_W  ;  //列  宽

	uint16 W  ;        //列  宽
    uint16 H  ;        //行  高
    
    
    inData = mxGetPr(prhs[0]);
    CAMERA_H = mxGetM(prhs[0]);
	CAMERA_W = mxGetN(prhs[0]);
    
    plhs[0] = mxCreateDoubleMatrix(120,1 , mxREAL);
    outdata0 = mxGetPr(plhs[0]);
    plhs[1] = mxCreateDoubleMatrix(120,1 , mxREAL);
    outdata1 = mxGetPr(plhs[1]);
    plhs[2] = mxCreateDoubleMatrix(120,1 , mxREAL);
    outdata2 = mxGetPr(plhs[2]);
    plhs[3] = mxCreateDoubleMatrix(120,1 , mxREAL);
    outdata3 = mxGetPr(plhs[3]);
    plhs[4] = mxCreateDoubleMatrix(120,1 , mxREAL);
    outdata4 = mxGetPr(plhs[4]);
    plhs[5] = mxCreateDoubleMatrix(120,1 , mxREAL);
    outdata5 = mxGetPr(plhs[5]);
    plhs[6] = mxCreateDoubleMatrix(120,1 , mxREAL);
    outdata6 = mxGetPr(plhs[6]);
    plhs[7] = mxCreateDoubleMatrix(240,1 , mxREAL);
    outdata7 = mxGetPr(plhs[7]);
    plhs[8] = mxCreateDoubleMatrix(120,1 , mxREAL);
    outdata8 = mxGetPr(plhs[8]);
    plhs[9] = mxCreateDoubleMatrix(120,1 , mxREAL);
    outdata9 = mxGetPr(plhs[9]);
    plhs[10] = mxCreateDoubleMatrix(120,1 , mxREAL);
    outdata10 = mxGetPr(plhs[10]);
    plhs[11] = mxCreateDoubleMatrix(120,1 , mxREAL);
    outdata11 = mxGetPr(plhs[11]);
#if RTS   
    plhs[12] = mxCreateDoubleMatrix(120,160, mxREAL);
    outdata_img = mxGetPr(plhs[12]);
#endif
#endif    

        
#if MatlabDebug  
#if RTS
uint8 img_buff[120*20]={0};        
        for(H = 0; H < 2400; H++)
		{
				img_buff[H] = inData[H];				        
		}
      img_extract(*img_handle,img_buff,2400);
#else        
    		for(H = 0; H < CAMERA_H; H++)
		{
			for(W = 0; W < CAMERA_W ; W++)
			{
				img_handle[H][W] = (inData[W*CAMERA_H+H]);	 
			}	        
		}    
		
		for(H = 0 ; H < CAMERA_H ; H++)
		{
			for(W = 0 ; W < CAMERA_W ; W++)
			{
				if(img_handle[H][W]==1)
                {
					img_handle[H][W] = 255 ;           
                }
				else
                {
					img_handle[H][W] = 0 ;
                }
			}
		}
#endif  
#endif           
/************************************变量初始化***********************************************/
        nomal_track_flag = 1; //正常赛道
//        fill_line_width = 72;
//        l_complete_lost_flag = 0;  //左线完全丢线标志
//        r_complete_lost_flag = 0;  //右线完全丢失标志
        fork_point  = 0; 
        l_cross_turn_point = 0;
        r_cross_turn_point = 0;
        //将用到的数组全部清零   11us
        memset(l_First_derivative,0,sizeof(l_First_derivative));
        memset(r_First_derivative,0,sizeof(r_First_derivative));
// //         memset(k_First_derivative,0,sizeof(k_First_derivative));
        memset(l_average,0,sizeof(l_average));
        memset(r_average,0,sizeof(r_average));
//        memset(k_average,0,sizeof(k_average));
//        memset(m_average,0,sizeof(m_average));
        memset(l_variance,0,sizeof(l_variance));
        memset(r_variance,0,sizeof(r_variance));
// //         memset(k_variance,0,sizeof(k_variance));
// //         memset(m_variance,0,sizeof(m_variance));
        
#if MatlabDebug     
        line_start = 80 ; //寻线起点 
#endif
  /******************************************/
        l.bend_jump_point = 0;
        r.bend_jump_point = 0;
        
        l.jump_point[0] = 0;
        l.jump_point[1] = 0;
        l.jump_point[2] = 0;
        
        r.jump_point[0] = 0;
        r.jump_point[1] = 0;
        r.jump_point[2] = 0;

        l.jump_count = 0;
        r.jump_count = 0;
        
        l.s_jump_point =0;
        l.x_jump_point =0;
        r.s_jump_point =0;
        r.x_jump_point =0;
        
        mid_l_x = 0;
        mid_l_s = 0;
        mid_r_x = 0;
        mid_r_s = 0;

        Prospect_See=0;

  /*******************************************/
/*****************************************************************************开始扫线***********************************************************************/ 
        for(row = 0 ; row <120 ; row ++)//54us
        {
            Lline[row] = L_LINE_INITIAL_VALUE ; 
            Rline[row] = R_LINE_INITIAL_VALUE ;
            Mline[row] = M_LINE_INITIAL_VALUE ; 
            Lline1[row]= EMPTY;
            Rline1[row]= EMPTY;
            Lline2[row]= EMPTY;
            Rline2[row]= EMPTY;
        }
        for(row = 119 ; row >=0 ;row --)//230us-1200us
        {//寻线大循环开始
            if(row==119)
            {//处理第119行
#if MatlabDebug        
                line_start = 80; 
#endif      
#if ProgramCar              
                line_start=next_line_start;//第一行起始扫线点等于上一场图像的第二行起始扫线点
#endif
                for( i = line_start ; i>0 ; i--)
                {
                    if((img_handle[row][i+1]) == 255 && (img_handle[row][i]) == 0)
                    {
                          Lline1[row] = i ;break;
                    }
                }
                if(i==0)Lline1[row]=0;
                
                for( j = line_start;j<159;j++)
                {
                   if((img_handle[row][j-1]) == 255 && (img_handle[row][j]) == 0)
                   {
                      Rline1[row] = j;break;
                   }
                }
                if(j==159)Rline1[row]=159;
            }//处理第119行结束
            if(row == 118)
            {//处理第118行
                line_start = (Lline1[row+1]+Rline1[row+1])/2;
#if ProgramCar
                next_line_start=line_start;//将第二行起始扫线点记下准备赋给下一场
#endif
                for( i = line_start ; i>0 ; i--)
                {
                    if((img_handle[row][i+1]) == 255 && (img_handle[row][i]) == 0)
                    {
                          Lline1[row] = i ;break;
                    }
                }
                if(i==0)Lline1[row]=0;
                for( j = line_start;j<159;j++)
                {
                   if((img_handle[row][j-1]) == 255 && (img_handle[row][j]) == 0)
                   {
                      Rline1[row] = j;break;
                   }
                }
                if(j==159)Rline1[row]=159;
            }//处理第118行结束 
            if(row < 118)
            {//处理剩余117行
                //提左边线
                l_head_point=Lline1[row+1];
                for(j = l_head_point;j>=0;j--)
                {
                    if(img_handle[row][j]==0)
                    {
                        l_head_point=j;break;
                    }
                }
                if(j==-1&& img_handle[row][0]==255)//j=0;
                {
                    l_head_point=-1;
                }//找头点
                if(j==0 && img_handle[row][0]==0)//j=0;
                {
                    l_head_point=0;
                }//找头点        
                for( j = l_head_point;j<=159;j++)//找左边线
                {
                    if(j==-1){ Lline1[row] = 0;break;}
                    else if(j==0)
                    {
                        if((img_handle[row][j]) == 0 && (img_handle[row][j+1]) == 255)
                        {
                            Lline1[row] = j;break;
                        }
                    }
                    else if(j>=158)
                    {
                        if((img_handle[row][j-1]) == 0 && (img_handle[row][j]) == 255)
                        {
                          Lline1[row] = j-1;break;  
                        }
                    }
                    else if((img_handle[row][j]) == 0 && (img_handle[row][j+1]) == 255)
                    {
                        Lline1[row] = j;break;
                    }
                }
                //提左线结束
                //提右边线
                r_head_point=Rline1[row+1];
                for(j = r_head_point;j<=159;j++)
                {
                    if(img_handle[row][j]==0)
                    {
                        r_head_point=j;break;
                    }
                }

                if(j==160&& img_handle[row][159]==255)
                {
                    r_head_point=160;
                }//找头点
                if(j==159 && img_handle[row][159]==0)
                {
                    r_head_point=159;
                }//找头点        
                for( j = r_head_point;j>=0;j--)//找右边线
                {
                    if(j==160){Rline1[row] = 159;break;}
                    else if(j==159)
                    {
                        if((img_handle[row][j]) == 0 && (img_handle[row][j-1]) == 255)
                        {
                            Rline1[row] = j;break;
                        }
                    }
                    else if(j<=1)
                    {
                        if((img_handle[row][j]) == 255 && (img_handle[row][j+1]) == 0)
                        {
                            Rline1[row] = j+1;break;
                        }
                    }
                    else if((img_handle[row][j]) == 0 && (img_handle[row][j-1]) == 255)
                        {
                          Rline1[row] = j;break;  
                        }
                }
                //提右边线结束
//                 if(HX_STATE==HX_BEFORE)
//                 {//环形中间扫线
//                     //找中间左边线
//                     for( j = Lline1[row];j<Rline1[row];j++)
//                     {
//                         if(j<=1)
//                         {
//                             if((img_handle[row][j]) == 255 && (img_handle[row][j+1]) ==0)
//                             {
//                                 Lline2[row] = j+1;break;
//                             }
//                         }
//                         else if(j>=158)
//                         {
//                             if((img_handle[row][j]) == 0 && (img_handle[row][j-1]) ==255)
//                             {
//                                 Lline2[row] = j;break;
//                             }
//                         }
//                         else if((img_handle[row][j-2] + img_handle[row][j-1]) == 510 && (img_handle[row][j] + img_handle[row][j+1]) == 0)
//                         {
//                             Lline2[row] = j;break;
//                         }
//                     }
//                     //找中间右边线
//                     for( j = Rline1[row];j>Lline1[row];j--)
//                     {
//                         if(j>=158)
//                         {
//                             if((img_handle[row][j]) == 255 && (img_handle[row][j-1]) ==0)
//                             {
//                                 Rline2[row] = j-1;break;
//                             }
//                         }
//                         else if(j<=1)
//                         {
//                             if((img_handle[row][j]) == 0 && (img_handle[row][j+1]) ==255)
//                             {
//                                 Rline2[row] = j;break;
//                             }
//                         }
//                         else if((img_handle[row][j+2] + img_handle[row][j+1]) == 510 && (img_handle[row][j] + img_handle[row][j-1]) == 0)
//                         {
//                             Rline2[row] = j;break;
//                         }
//                     }
//                 }//环形中间扫线
                if(Lline1[row]>=Rline1[row+1] || Rline1[row]<=Lline1[row+1]){Lline1[row]=EMPTY;Rline1[row]=EMPTY;break;}//扫到头跳出循环
                //if(Lline2[row+1]!=EMPTY && Rline2[row+1]!=EMPTY && (Lline1[row]>=Lline2[row+1] || Rline1[row]<=Rline2[row+1])){Lline1[row]=EMPTY;Rline1[row]=EMPTY;break;}
            }//处理剩余117行结束
        }//寻线大循环结束
       for(row=0;row<=119;row++)//确定前瞻
       {
           if(Lline1[row]!=EMPTY || Rline1[row]!=EMPTY)
           {
               Prospect_See=row;
               break;
           }
       }
        for(row = Prospect_See ; row <120 ; row ++)//给左右线 33us
        {
            Lline[row] = Lline1[row] ; 
            Rline[row] = Rline1[row] ;
        }
//         if(MMN(Rline,115,119,1)-MMN(Lline,115,119,0)<40 && MMN(Lline,115,119,0)!=0 && MMN(Rline,115,119,1)!=159)
//         {
//             QP_STSTE = QP_YES;
//         }
//         else QP_STSTE = QP_NO;
/******************************************************************************************大循环结束************************************************************************************************/           
        ImgFilter(Lline);//
        ImgFilter(Rline);//480us-560us
         //跳变点判断
        find_jump_point();//2700us-3000us
        if(l.jump_point[0])
        {
            if(jump1_lx>0)l.x_jump_point=l.jump_point[0];
            if(jump1_lx<0 && Lline[l.jump_point[0]+2]==0 && Lline[l.jump_point[0]+3]==0)l.s_jump_point=l.jump_point[0];
        }
        if(l.jump_point[1])
        {
            if(jump2_lx>0)l.x_jump_point=l.jump_point[1];
            if(jump2_lx<0 && Lline[l.jump_point[1]+2]==0 && Lline[l.jump_point[1]+3]==0)l.s_jump_point=l.jump_point[1];
        }
//         if(l.jump_point[2])
//         {
//             if(jump3_lx>0)l.x_jump_point=l.jump_point[2];
//             if(jump3_lx<0 && Lline[l.jump_point[2]+2]==0 && Lline[l.jump_point[2]+3]==0)l.s_jump_point=l.jump_point[2];
//         }
        
        if(r.jump_point[0])
        {
            if(jump1_rx<0)r.x_jump_point=r.jump_point[0];
            if(jump1_rx>0 && Rline[r.jump_point[0]+2]==159 && Rline[r.jump_point[0]+3]==159)r.s_jump_point=r.jump_point[0];
        }
        if(r.jump_point[1])
        {
            if(jump2_rx<0)r.x_jump_point=r.jump_point[1];
            if(jump2_rx>0 && Rline[r.jump_point[1]+2]==159 && Rline[r.jump_point[1]+3]==159)r.s_jump_point=r.jump_point[1];
        }
//         if(r.jump_point[2])
//         {
//             if(jump3_rx<0)r.x_jump_point=r.jump_point[2];
//             if(jump3_rx>0 && Rline[r.jump_point[2]+2]==159 && Rline[r.jump_point[2]+3]==159)r.s_jump_point=r.jump_point[2];
//         }
// /************************************************提取赛道参数完毕**********************************************************/
// /*************************************************开始判断处理十字********************************************************/
#if MatlabDebug    
         mexPrintf("l.jump_count=%d\n",l.jump_count);
         mexPrintf("r.jump_count=%d\n",r.jump_count);
         
         mexPrintf("l.jump_point[0]=%d\n",l.jump_point[0]);
         mexPrintf("l.jump_point[1]=%d\n",l.jump_point[1]);
         mexPrintf("l.jump_point[2]=%d\n",l.jump_point[2]);
         
         mexPrintf("r.jump_point[0]=%d\n",r.jump_point[0]);
         mexPrintf("r.jump_point[1]=%d\n",r.jump_point[1]);
         mexPrintf("r.jump_point[2]=%d\n",r.jump_point[2]);
         
         mexPrintf("l.s_jump_point=%d\n",l.s_jump_point);
         mexPrintf("l.x_jump_point=%d\n",l.x_jump_point);
         mexPrintf("r.s_jump_point=%d\n",r.s_jump_point);
         mexPrintf("r.x_jump_point=%d\n",r.x_jump_point);
 #endif
/******************找拐点*****************///58us
             for(row = 115;row >15 ; row --)//远处的拐点无意义 故只找到15行
             {
                 if(((Lline[row]-Lline[row-4])*(Lline[row+4]-Lline[row]))<0 || ((Lline[row+4]-Lline[row])==0 && (Lline[row]-Lline[row-4])>2))//找左拐点的方法
                 {
                     if(Lline[row]==0)continue;
                     if(l_cross_turn_point == 0)
                     {
                         l_cross_turn_point = row-3;
                         break;      //找到拐点跳出循环
                     }
                 }
             }
             for(row = 115;row > 15 ; row --)//远处的拐点无意义 故只找到15行
             {
                 if(((Rline[row]-Rline[row-4])*(Rline[row+4]-Rline[row]))<0 || ((Rline[row+4]-Rline[row])==0 && (Rline[row]-Rline[row-4])<-2))//找右拐点的方法
                 {
                     if(Rline[row]==159)continue;
                     if(r_cross_turn_point == 0)
                     {
                          r_cross_turn_point = row-3;
                         break;      //找到拐点跳出循环
                     }
                 }
             }
//         l_cross_turn_point=AcquireInflecPoint(Prospect_See, Lline, 'L');
//         r_cross_turn_point=AcquireInflecPoint(Prospect_See, Rline, 'R');
#if MatlabDebug
            mexPrintf("l_cross_turn_point=%d\n",l_cross_turn_point);
            mexPrintf("r_cross_turn_point=%d\n",r_cross_turn_point);
#endif 


/************************************************************开始判断处理弯道*************************************************************/
        if(
               nomal_track_flag == 1 
           )//弯道
        {
          fill_bend();//1us-20us
        }
/*********************************8***************弯道处理完毕********************************************************/        

#if MatlabDebug
        mexPrintf("l.bend_jump_point=%d\n",l.bend_jump_point);
        mexPrintf("r.bend_jump_point=%d\n",r.bend_jump_point);
        mexPrintf("Prospect_See=%d\n",Prospect_See);
#endif
      for(row = 119 ; row >= 0; row --)//50us-92us
      {
        if(Lline[row] !=0 && Rline[row] != 159)
        {
          Mline[row] = (Lline[row] + Rline[row])/2 ;
        } 
        if(Lline[row] != 0 && Rline[row] == 159)
        {
          Mline[row] = (Lline[row] + 72);
        }
        if(Lline[row] ==0 && Rline[row] != 159)
        {
          Mline[row] = (Rline[row] - 72);
        }
      }
      //Imgfilter_Mline(Mline);//900us-1500us时间太长去掉
#if MatlabDebug       
      for(i = 119; i >= 35; i --)
      {
            err += (Mline[i] - 79)*Correction_array[i];
      }
      err = err/85.0 ;
      mexPrintf("err=%f\n",err);
#endif
#if ProgramCar
      camer_error_calculation();//178us-195us
      Track_complexity = ABS((int16)camer.error);
#endif
      ramp_check(60,40,0.8);//坡道检测
      avoid_obstacle();
// /*******************************特殊赛道处理*****************************************/
//   if(nomal_track_flag == 1 || ZA_STATE == ZA_YES || QP_STSTE == QP_YES)  //正常赛道前瞻和复杂程度赋值//80us
//   {
//        if(Prospect_See <15 || ZA_STATE == ZA_YES || QP_STSTE == QP_YES)// && l.jump_point[0] == 0 && r.jump_point[0] == 0 && MMN(Lline,0,50,0)!=0 && MMN(Rline,0,50,1)!=159)
//        {
//             Left_slope_range = Slope_Range(30,l_First_derivative); //左线斜率范围；
//             Right_slope_range = Slope_Range(30,r_First_derivative);//右线斜率范围；
//             range_derivative =  ABS(Left_slope_range - Right_slope_range);
//             Track_complexity =   (int16)(range_derivative*range_derivative *10);
//             if( (Track_complexity < 10 && (Left_slope_range<2 || Right_slope_range<2)) || ZA_STATE == ZA_YES  || QP_STSTE == QP_YES)//直道
//             {
// #if ProgramCar
//               if( Front_flag==255 || (Front_flag==0 && (Judgment_obstacle_flag >=1 || Judgment_startline_flag>=1) ))
// #endif  
//                  avoid_obstacle();
//             }
// /**********************************小s***********************************/
//             if(0 == Prospect_See && l.jump_point[0] == 0 && r.jump_point[0] == 0 && MMN(Lline,0,50,0)!=0 && MMN(Rline,0,50,1)!=159)
//             {
//             if(Track_complexity < 25 && (Left_slope_range>2 || Right_slope_range>2) )  //复杂程度滤S弯 和 直道；左右边线都会抖动但差值很小
//             {
//               Track_complexity = 0;
// #if ProgramCar
//               servos.direction_p = XS_Servo_KP;  //小s的参数
//               servos.direction_d = XS_Servo_KD;
// #endif
//             }
//             else 
//             {
// #if ProgramCar
//                Track_complexity = ABS((int16)camer.error);
//                servos.direction_p = Servo_KP;
//                servos.direction_d = Servo_KD;
// #endif
//             }
//             }
//              else
//             {
// #if ProgramCar
//              Track_complexity = ABS((int16)camer.error);
//              servos.direction_p = Servo_KP;
//              servos.direction_d = Servo_KD;
// #endif
//             }
// /************************************小s*********************************************/            
//        }
//   }


    
 
/************************************************************************/
/******************************************结束扫线**********************************************/
#if MatlabDebug        
        for(row = 119 ; row >=0 ; row--)
        {
            outdata0[row] = Lline[row]+1;
            outdata1[row] = Rline[row]+1;
#if DISPLAY_MLINE          
            outdata2[row] = Mline[row]+1;
#else            
//            outdata2[row] = saoxian[row]+1;

#endif
            outdata8[row] = Lline1[row]+1;
            outdata9[row] = Rline1[row]+1;
            outdata10[row] =Lline2[row]+1;
            outdata11[row] =Rline2[row]+1;
        }
#if RTS
int16   column;    
        for(row = 0 ; row <=119 ; row++)
        {
            for(column = 159 ; column >=0 ; column--)
           {
                if(img_handle[row][column]==255)
                   outdata_img[column*120+row] = 1;
                else
                   outdata_img[column*120+row] = 0; 
            }
        }
#endif
#endif
/*****************************************************************************************************处理完毕***************************************************************************************************************/        
}
#if ProgramCar
/***********************计算偏差**********/
void camer_error_calculation()
{
  int8 i,mid;
  float err = 0;
  camer.last_error = camer.error;
  if(Front_flag==255)
      mid=62;//62前右//97前左
  else
      mid=102;//102后左//57后右
  for(i = 119; i > (119-hang); i --)
  {
    err += (Mline[i] - mid)*Correction_array[i];
  }
  camer.error = err /hang;
  if(camer.error-camer.last_error>30)//限幅滤波
  camer.error=camer.last_error+30;
  if(camer.error-camer.last_error<-30)//限幅滤波
  camer.error=camer.last_error-30;
  if(camer.error>150) camer.error=150;
  if(camer.error<-150) camer.error=-150;
}
#endif
/*************用到的数学工具函数***************/
void ImgFilter(int16 * line)
{
    int8 i,j,k;
    int8 m,n;
    int16 num[5],t;
    for(i=117;i>1;i--)
    {
        k = i+2;
        for(j=0;j<5;j++,k--)
            num[j] = line[k];
        for(m=0;m<5;m++)
        {
            for(n=0;n<4-m;n++)
            {
                if(num[n]>num[n+1])
                {
                    t=num[n];
                    num[n]=num[n+1];
                    num[n+1]=t;
                }
            }
        }
        line[i] = num[2];
    }
}
float calculation_average(float a,float b, float c)
{
    float average_value;
    average_value = (a + b + c)/3;
    return average_value;
}
float calculation_variance(float a,float b, float c,float average)//方差
{
    float variance;
    variance = ((a - average)*(a - average)+(b - average)*(b - average)+(c - average)*(c - average))/3;
    return variance;
}
//float Slope_Range(int16 D,float * line)
//{
//  float MAX,MIN;
//  int16 i;
//  MAX = line[117];
//  MIN = line[117];
//  for(i = 116; i>= D;i--)
//  {
//    if(MAX < line[i])
//      MAX = line[i];
//    else if(MIN > line[i])
//      MIN = line[i];
//    
//  }
//  return (MAX - MIN);
//}
/****寻出一个数组中的最大最小点120*160****/
int16 MMN(int16 *line,uint8 s,uint8 e,uint8 choose)
{
    int16 temp;
    if(choose==1)//最大值
    {
        temp=0;
        for(;s<e+1;s++)
        {
            if(temp<line[s])
                temp = line[s];
        }
    }
    else
    {
        temp = 159;
        for(;s<e+1;s++)
        {
            if(temp>line[s])
                temp = line[s];
        }
    }
    return temp;
}
/**********************************补线函数*************************************/
//void matchline(int16 start,int16 end,int16 * line)//两点连线
//{
//    float a0,a1;
//    int16 i,divide;
//    divide = line[start]-line[end];
//     if(divide != 0)
//     {
//        a1 = (start-end)*1.0/divide;
//        a0 = (start-a1*line[start])*1.0;
//         if(a1 != 0)
//         {
//            for(i = start-1;i > end;i --)
//                line[i] = (int16)((i-a0)/a1);
//         }
//     }
//}
/****************************算法函数*******找跳变点**************************************/
void find_jump_point()//找跳变点
{
        for(row = 117;row>=0;row--)
        {
            l_First_derivative[row] = (Lline[row+2] - Lline[row])/2.0;
            r_First_derivative[row] = (Rline[row+2] - Rline[row])/2.0;
//             k_First_derivative[row] = (Kline[row+2] - Kline[row])/2.0;
#if MatlabDebug     
            outdata3[row]=l_First_derivative[row];
            outdata4[row]=r_First_derivative[row];
#endif
        }
/************************************************/
        for(row = 117; row>=101;row--)
        {     
            if(Lline[row]==0 && Lline[row+1]>=1 && l.jump_count < 3)
            {
                    l.jump_point[l.jump_count] = row ;
                    //mexPrintf("l_jump_point[%d]=%d\n",l_jump_count,l_jump_point[l_jump_count]);
                    if(l.jump_count>0 && l.jump_point[l.jump_count-1]-l.jump_point[l.jump_count]<15)
                    {
                        l.jump_point[l.jump_count] = 0;
                        l.jump_count --;
                    }
#if MatlabDebug
//                    mexPrintf("l.jump_point[%d]=%d\n",l.jump_count,l.jump_point[l.jump_count]);
#endif
                    l.jump_count ++ ;
            }
            if(Rline[row]==159 && Rline[row+1]<=158 && r.jump_count < 3)
            {
                    r.jump_point[r.jump_count] = row ;
                    if(r.jump_count>0 && r.jump_point[r.jump_count-1]-r.jump_point[r.jump_count]<15)
                    {
                        r.jump_point[r.jump_count] = 0;
                        r.jump_count --;
                    }
#if MatlabDebug
//                    mexPrintf("r.jump_point[%d]=%d\n",r.jump_count,r.jump_point[r.jump_count]);
#endif
                    r.jump_count ++ ;
            }
        }
                
/**************************************************/        
        //跳变点判断
        for(row = 114; row>=5;row--)//图像远处的跳变点毫无意义 故只找到第5行
        {
            l_average[row] = calculation_average(l_First_derivative[row+2],l_First_derivative[row+1],l_First_derivative[row]);   //计算平均值
            
            l_variance[row] = calculation_variance(l_First_derivative[row+2],l_First_derivative[row+1],l_First_derivative[row],l_average[row]);    //计算方差
#if MatlabDebug
            outdata5[row]=l_variance[row];
#endif    
            
            if(l_variance[row] >= VARIANCE_THRESHOLD)
            {
//                 if(l.bend_jump_point == 0 )
//                 {
//                     l.bend_jump_point = row;
//                 }
                if(l_variance[row+1] < VARIANCE_THRESHOLD && l.jump_count < 3)
                {
                    l.jump_point[l.jump_count] = row ;
                    //mexPrintf("l_jump_point[%d]=%d\n",l_jump_count,l_jump_point[l_jump_count]);
                    if(l.jump_count>0 && l.jump_point[l.jump_count-1]-l.jump_point[l.jump_count]<15)
                    {
                        l.jump_point[l.jump_count] = 0;
                        l.jump_count --;
                    }
#if MatlabDebug
//                    mexPrintf("l.jump_point[%d]=%d\n",l.jump_count,l.jump_point[l.jump_count]);
#endif
                    l.jump_count ++ ;
                }
            }
            r_average[row] = calculation_average(r_First_derivative[row+2],r_First_derivative[row+1],r_First_derivative[row]);   //计算平均值
            r_variance[row] = calculation_variance(r_First_derivative[row+2],r_First_derivative[row+1],r_First_derivative[row],r_average[row]);   //计算方差
#if MatlabDebug
            outdata6[row]=r_variance[row];
#endif           
            if(r_variance[row] >= VARIANCE_THRESHOLD)
            {
//                 if(r.bend_jump_point ==0)
//                 {
//                     r.bend_jump_point = row; 
//                 }
                if(r_variance[row+1] < VARIANCE_THRESHOLD && r.jump_count < 3)
                {
                    r.jump_point[r.jump_count] = row ;
                    if(r.jump_count>0 && r.jump_point[r.jump_count-1]-r.jump_point[r.jump_count]<15)
                    {
                        r.jump_point[r.jump_count] = 0;
                        r.jump_count --;
                    }
#if MatlabDebug
//                    mexPrintf("r.jump_point[%d]=%d\n",r.jump_count,r.jump_point[r.jump_count]);
#endif
                    r.jump_count ++ ;
                }
            }
        }
}

/**********************************************补线********************************************************/
void fill_r_line(int a)
{
    int8 r_jump_derivative;

//       for(row = l.bend_jump_point; row >= 0 ;row--)
//       {
//         Lline[row] = 0;
//       }
      r_jump_derivative = Rline[a+5] - Rline[a+9];
      if(a >= 110)a=110;
      for(row = a+2;row>=0;row--)
      {
        Rline[row] = Rline[row+1] + r_jump_derivative;
      }
}
void fill_l_line(int a)
{
    int8 l_jump_derivative;

//       for(row = r.bend_jump_point; row >= 0 ;row--)
//       {
//         Rline[row] = 159;
//       }
        if(a >= 110)a=110;
      l_jump_derivative = Lline[a+5] - Lline[a+9];
      for(row = a+2;row>=0;row--)
      {
        Lline[row] = Lline[row+1] + l_jump_derivative;
      }
}
//void Give_Prospect_See()
//{
////给前瞻               
//               if(l.bend_jump_point && r.bend_jump_point && l_First_derivative[l.bend_jump_point+2]<0 && r_First_derivative[r.bend_jump_point+2]<0)
//               {
//                   Prospect_See= l.bend_jump_point;                     
//               }
//               else if(l.bend_jump_point && r.bend_jump_point && l_First_derivative[l.bend_jump_point+2]>0 && r_First_derivative[r.bend_jump_point+2]>0)
//               {
//                   Prospect_See= r.bend_jump_point;                     
//               }
//               else if(l.bend_jump_point && r.bend_jump_point==0 && l_First_derivative[l.bend_jump_point+2]<0 )
//               {
//                   Prospect_See= l.bend_jump_point;                     
//               }
//               else if(l.bend_jump_point && r.bend_jump_point==0 && l_First_derivative[l.bend_jump_point+2]>0 )
//               {
//                   Prospect_See= r.bend_jump_point;                     
//               }
//               else if(l.bend_jump_point==0 && r.bend_jump_point && r_First_derivative[r.bend_jump_point+2]<0 )
//               {
//                   Prospect_See= l.bend_jump_point;                     
//               }
//               else if(l.bend_jump_point==0 && r.bend_jump_point && r_First_derivative[r.bend_jump_point+2]>0 )
//               {
//                   Prospect_See= r.bend_jump_point;                     
//               }
//               else if(l.bend_jump_point == r.bend_jump_point)
//               {
//                   Prospect_See= l.bend_jump_point;   
//               }
//               else Prospect_See= 0;
//}
/*****************************找拐点***************************************/
//int8 AcquireInflecPoint(int end, int16* line, char flag)//拐点
//{
//    int8 row = 119,i,count1=0,count2=0,inf_point=0;
//    i = MAX(15,end);
//    if(flag == 'L')
//    {
//      while(row > i)
//      {
//        if(*(line+row-1)<*(line+row+count2) && count1>5)
//        {
//          count2 ++;
//          if(count2 > 5) {inf_point = row+5;break;}
//        }
//        else if(*(line+row-1)>=*(line+row))     
//        {
//          count1 ++;
//          if(count2)    {count1=0;count2=0;}
//        }
//        row --;
//      }
//    }
//    else
//    {
//       while(row > i)
//      {
//        if(*(line+row-1)>*(line+row+count2) && count1>5)
//        {
//          count2 ++;
//          if(count2 > 5) {inf_point = row+5;break;}
//        }
//        else if(*(line+row-1)<=*(line+row))    
//        {
//          count1 ++;
//          if(count2)    {count1=0;count2=0;}
//        }
//        row --;
//      }
//    }
//    return inf_point;
//}
/******************************************************************************************************/
void find_yuanhuan_l()
{
                for(row=119;row>=Prospect_See;row--)
                {//环形中间扫线
                    //找中间左边线
                    for( j = Lline1[row];j<Rline1[row];j++)
                    {
                        if( ((img_handle[row][j] + img_handle[row][j+1]) == 0  &&  (img_handle[row][j-2] + img_handle[row][j-1]) == 510) || ((img_handle[row][j-2] + img_handle[row][j-1]) == 0 && (img_handle[row][j] + img_handle[row][j+1]) == 0) )
                        {
                            Lline2[row] = j;break;
                        }
                    }
                }//环形中间扫线
               for(row=119;row>=Prospect_See;row--)//左岔路
               {
                   if(Lline2[row]!=EMPTY)
                   {
                       mid_l_x=row;
                       break;
                   }
               }
               for(row=mid_l_x;row>=Prospect_See;row--)//左岔路
               {
                   if(Lline2[row]==EMPTY)
                   {
                       mid_l_s=row+1;
                       break;
                   }
               }
             if(mid_l_x>=(Prospect_See+15))
             {
             fork_long=mid_l_x-mid_l_s;
             fork_point=mid_l_x;
             }
}
void find_yuanhuan_r()
{
                for(row=119;row>=Prospect_See;row--)
                {//环形中间扫线
                    for( j = Rline1[row];j>Lline1[row];j--)
                    {
                        if( ((img_handle[row][j] + img_handle[row][j-1]) == 0  &&  (img_handle[row][j+2] + img_handle[row][j+1]) == 510) ||((img_handle[row][j+2] + img_handle[row][j+1]) == 0 && (img_handle[row][j] + img_handle[row][j-1]) == 0) )
                        {
                            Rline2[row] = j;break;
                        }
                    }
                }//环形中间扫线
               for(row=119;row>=Prospect_See;row--)//右岔路
               {
                   if(Rline2[row]!=EMPTY)
                   {
                       mid_r_x=row;
                       break;
                   }
               }
               for(row=mid_r_x;row>=Prospect_See;row--)//右岔路
               {
                   if(Rline2[row]==EMPTY)
                   {
                       mid_r_s=row+1;
                       break;
                   }
               }
             if(mid_r_x>=(Prospect_See+15))
             {
             fork_long=mid_r_x-mid_r_s;
             fork_point=mid_r_x;
             }
}
void fill_bend()
{
#if MatlabDebug
               mexPrintf("弯道补线\n");
#endif
               if(l.s_jump_point==0 && r.s_jump_point==0 && ( (l.jump_point[0]&&l.jump_point[0]<101) || (r.jump_point[0]&& r.jump_point[0]<101) )  )
               {
#if MatlabDebug
               mexPrintf("弯道补线成功\n");
#endif
                   if(l.jump_point[0] && l.jump_point[0]<101)
                   {
                   l.bend_jump_point=l.jump_point[0];
                   }
                   if(r.jump_point[0] && r.jump_point[0]<101)
                   {
                   r.bend_jump_point=r.jump_point[0];
                   }
#if Fill_Line
               if(r.bend_jump_point && Rline[r.bend_jump_point+2]<70)
               fill_r_line(r.bend_jump_point);//补右线
               if(l.bend_jump_point && Lline[l.bend_jump_point+2]>90)
               fill_l_line(l.bend_jump_point);
#endif
               }
               else if(l.s_jump_point==0 && r.s_jump_point==0 && 
                       (   (l.jump_point[0]&&l.jump_point[0]>=101 && MMN(Rline,l.jump_point[0],119,0)==159 && Lline[l.jump_point[0]+1]>140) 
                       ||(r.jump_point[0] && r.jump_point[0]>=101 && MMN(Lline,r.jump_point[0],119,1)==0   && Rline[r.jump_point[0]+1]<20)  )
                       )//近处补线
               {
#if MatlabDebug
               mexPrintf("弯道近处补线成功\n");
#endif
                   if(l.jump_point[0] )
                   {
                   l.bend_jump_point=l.jump_point[0];
                   }
                   if(r.jump_point[0] )
                   {
                   r.bend_jump_point=r.jump_point[0];
                   }
#if Fill_Line
               if(r.bend_jump_point)
               fill_r_line(r.bend_jump_point);//补右线
               if(l.bend_jump_point)
               fill_l_line(l.bend_jump_point);
#endif   
               }
               else
               {
#if MatlabDebug
               mexPrintf("弯道补线失败\n");
#endif
               }
}

void ramp_check(int8 H,int8 L,float ramp_V)
{
 #if MatlabDebug
             mexPrintf("坡道判断\n");
 #endif 
    int16 i,j;
    int16 ramp_W=0,normal_W=0;
    float value=0;
#if MatlabDebug       
    if(ABS(err)<20 && nomal_track_flag == 1)
#endif
#if ProgramCar
    if( ABS(camer.error)<30.0 && nomal_track_flag==1)
#endif        
    {
        for(i=H;i>=L;i--)
        {
            j=Rline[i]-Lline[i];
            ramp_W+=j;
            normal_W+=rempcheck_array[i];
             
        }
            value=(float)ramp_W/(float)normal_W;
 #if MatlabDebug
             mexPrintf("坡道变窄比例=%f\n",value);
 #endif
            if(value<ramp_V)
            {
            PODAO_STATE=PODAO_YES;  
 #if MatlabDebug
             mexPrintf("坡道判断成功\n");
 #endif
            }
            else PODAO_STATE=PODAO_NO;
    }
    else PODAO_STATE=PODAO_NO;
}
#define SUMMM 20
void avoid_obstacle()
{
    static int8 start=0,end=0;
    static int16 current_kongxi=0,last_kongxi=0;
    static int16 cow=0,Black_count=0;
    float bili=0;
    last_kongxi=current_kongxi;
    current_kongxi=Rline[119]-Lline[119];
    find_yuanhuan_l();
    find_yuanhuan_r();
 #if MatlabDebug
    mexPrintf("避障\n");  
    mexPrintf("岔路fork_point=%d\n",fork_point);
    mexPrintf("岔路长度fork_long=%d\n",fork_long);
    mexPrintf("Lline2[%d]=%d\n",fork_point,Lline2[fork_point]);
    mexPrintf("Rline2[%d]=%d\n",fork_point,Rline2[fork_point]);
    mexPrintf("Mline[%d]=%d\n",fork_point,Mline[fork_point]);
    mexPrintf("障碍宽度=%d\n",Rline2[fork_point-fork_long/2]-Lline2[fork_point-fork_long/2]);
    mexPrintf("空隙宽度=%d\n",current_kongxi);
#endif 
    if(fork_point>25 && fork_long>2)
    {
        row=fork_point-fork_long/2;
        for(cow=Lline2[fork_point-fork_long/2];cow<=Rline2[fork_point-fork_long/2];cow++)
        {
            Black_count+=img_handle[row][cow];
        }
        Black_count/=255;
    }
    if( 1 )
    {
        if(fork_point>25 && fork_long>2 && (Rline2[fork_point-fork_long/2]-Lline2[fork_point-fork_long/2])>=rempcheck_array[fork_point-fork_long/2]/2)//障碍大于赛道宽度一半则不为障碍
        { 
                        bili=(float)Black_count/(float)(rempcheck_array[fork_point-fork_long/2]);
                        if(bili>0.4)
                        {
                            QP_STSTE = QP_YES;
                        }
          }
        if(MMN(Rline,115,119,1)-MMN(Lline,115,119,0)<40 && MMN(Lline,115,119,0)!=0 && MMN(Rline,115,119,1)!=159)
        {
                QP_STSTE = QP_NO;
        }
    }
    if(  Front_flag==255 || (Front_flag==0 && Judgment_obstacle_flag>=1) )
    {
        bili=(float)Black_count/(float)(Rline2[fork_point-fork_long/2]-Lline2[fork_point-fork_long/2]);
        if(bili<0.1 && fork_point>25 && fork_long>4 && Rline2[fork_point-fork_long/2]-Lline2[fork_point-fork_long/2]>4 && (Lline2[fork_point]+Rline2[fork_point])/2<Mline[fork_point] && (Rline2[fork_point-fork_long/2]-Lline2[fork_point-fork_long/2])<rempcheck_array[fork_point-fork_long/2]/2)
        {
 #if MatlabDebug 
          mexPrintf("左障碍\n");
 #endif  
          ZA_STATE = ZA_YES;
          start = MIN(fork_point+SUMMM,119);
          end = MAX(fork_point-fork_long-SUMMM,0);
          for(row=start;row>=end;row--)
          {
              Lline[row]=Rline2[fork_point-fork_long/2]+SUMMM;
          }
        }
        else if(bili<0.1 && fork_point>25 && fork_long>4 && Rline2[fork_point-fork_long/2]-Lline2[fork_point-fork_long/2]>4 &&(Lline2[fork_point]+Rline2[fork_point])/2>Mline[fork_point] && (Rline2[fork_point-fork_long/2]-Lline2[fork_point-fork_long/2])<rempcheck_array[fork_point-fork_long/2]/2)
        {
#if MatlabDebug 
          mexPrintf("右障碍\n"); 
#endif  
          ZA_STATE = ZA_YES;    
          start = MIN(fork_point+SUMMM,119);
          end = MAX(fork_point-fork_long-SUMMM,0);
          for(row=start;row>=end;row--)
          {
              Rline[row]=Lline2[fork_point-fork_long/2]-SUMMM;
          }
        }
        else
        {
 #if MatlabDebug
          mexPrintf("无障碍\n");
 #endif
        }
        if(current_kongxi>108 && last_kongxi<=108)
          ZA_STATE = ZA_NO;
    /************重新算偏差******************/
        if(ZA_STATE == ZA_YES)
        {
          for(row = 119 ; row >= 0; row --)//50us-92us
          {
            if(Lline[row] !=0 && Rline[row] != 159)
            {
              Mline[row] = (Lline[row] + Rline[row])/2;
            }
            if(Lline[row] != 0 && Rline[row] == 159)
            {
              Mline[row] = (Lline[row] + 72);
            }
            if(Lline[row] ==0 && Rline[row] != 159)
            {
              Mline[row] = (Rline[row] - 72);
            }
          }
 #if ProgramCar
         camer_error_calculation();//178us-195us
 #endif 
        }
    }
}