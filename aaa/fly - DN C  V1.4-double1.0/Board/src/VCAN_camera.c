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

camer_status camer;
volatile HX_STATUS_e    HX_STATE = HX_NORMAL;   //环形状态
volatile QP_STATUS_e    QP_STSTE = QP_NO;
volatile PODAO_STATUS_e PODAO_STATE=PODAO_NO;
volatile ZA_STATUS_e    ZA_STATE = ZA_NO;
volatile SZ_STATUS_e  SZ_STATE = SZ_NONE;
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
QP_STATUS_e    QP_STSTE = QP_NO;
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
//定义十字超车状态
typedef enum
{
    SZ_BEFORE=2,
    SZ_ON=3,
    SZ_BEHAND=4,
    SZ_NORMAL=1,
    SZ_NONE=0,
} SZ_STATUS_e;
SZ_STATUS_e  SZ_STATE = SZ_NORMAL;

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
uint8 Front_flag = 255;
uint8 current_ring_state = 0;
uint8 last_ring_state = 0;
uint8 start_up = 0;
uint8 HX_text=255;
uint8 Judgment_startline_flag=1;
uint8 Judgment_obstacle_flag=1;
uint8 SZ_Caoceflag=1;
uint8 Return_normalflag =1;
uint8 sz_speedcount=0;

#define ABS(x) (((x) > 0) ? (x) : (-(x)))
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )
#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )


#endif
/**************************通用头文件区***************************/
#include <string.h>
float Correction_array[120] = { 4.45, 4.3, 4.3, 4.03, 3.79, 3.79, 3.58, 3.58, 3.39, 3.39, 3.31, 3.22, 3.15, 3.07, 
3.0, 2.87, 2.74, 2.74, 2.63, 2.63, 2.63, 2.53, 2.53, 2.48, 2.39, 2.35, 2.3, 2.26, 2.22, 2.22, 2.15, 2.15, 2.08, 
2.08, 2.05, 2.02, 1.98, 1.95, 1.93, 1.93, 1.87, 1.84, 1.82, 1.79, 1.77, 1.77, 1.72, 1.72, 1.7, 1.68, 1.65, 1.65, 
1.63, 1.63, 1.61, 1.59, 1.57, 1.55, 1.54, 1.54, 1.52, 1.5, 1.5, 1.48, 1.45, 1.45, 1.42, 1.42, 1.39, 1.39, 1.37, 
1.36, 1.36, 1.33, 1.33, 1.32, 1.3, 1.3, 1.28, 1.26, 1.26, 1.25, 1.24, 1.23, 1.23, 1.22, 1.21, 1.19, 1.18, 1.18, 
1.17, 1.17, 1.16, 1.15, 1.14, 1.14, 1.13, 1.12, 1.12, 1.11, 1.1, 1.1, 1.09, 1.08, 1.08, 1.07, 1.07, 1.06, 1.05, 
1.04, 1.04, 1.03, 1.03, 1.02, 1.02, 1.02, 1.01, 1.0, 1.0, 1.0
};
int16 rempcheck_array[120]={   24,  25,  26,  27,  28,  29,  30,  32,  32,  34,  35,  36,  37,  38, 39, 40, 
 41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54, 55, 56,
 57,  58,  59,  59,  60,  61,  62,  63,  64,  65,  66,  66,  67,  68, 69, 70,
 70,  71,  72,  73,  74,  75,  76,  76,  77,  78,  78,  80,  80,  81, 82, 82,
 83,  84,  84,  85,  85,  86,  87,  88,  88,  90,  90,  91,  91,  92, 93, 93,
 94,  95,  96,  97,  98,  98,  98, 
100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 111, 111, 111,
111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111,
111, 122, 122, 122, 122
 };



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
void Give_Prospect_See();
void ImgFilter(int16 * line);
void Imgfilter_Mline(int16 * line);
float calculation_average(float a,float b, float c);
float calculation_variance(float a,float b, float c,float avearge);
void matchline(int16 start,int16 end,int16 * line);//两点连线
float Slope_Range(int16 D,float * line);
int16 MMN(int16 *line,int16 s,int16 e,uint8 choose);
/***********************************************************************************************/
/*********************************************算法函数声明***************************************/
void find_jump_point();//找跳变点
int8 AcquireInflecPoint(int end, int16* line, char flag);//找拐点
void camer_error_calculation();
void find_yuanhuan_l();
void find_yuanhuan_r();
void process_hx_l();
void qianceL_process_cross();
void qianceR_process_cross();
void houceR_process_cross();
void houceL_process_cross();
void process_hx_r();
void process_xcross();
void avoid_obstacle();//
void ramp_check(int8 H,int8 L,float ramp_V);//坡道检测
void fill_bend();
void fill_r_line(int a);//补右线
void fill_l_line(int a);
/************************************************************************************************/

/**************************定义变量****************************/
JUMP_POINT l={0,{0},0};
JUMP_POINT r={0,{0},0};
static uint8 nomal_track_flag = 1; //正常赛道
static float Left_slope_range,Right_slope_range;
static float range_derivative;
//uint8 l_complete_lost_flag = 0 ;  //左线完全丢线标志
//uint8 r_complete_lost_flag = 0;   //右线完全丢失标志      
int16  l_cross_turn_point = 0;
int16  r_cross_turn_point = 0;
float l_First_derivative[118] = {0};   //左线一阶导
float r_First_derivative[118] = {0};   //右线一阶导
//float k_First_derivative[118] = {0};
float l_average[116] = {0};            //左线平均值
float r_average[116] = {0};            //右线平均值
//float k_average[116] = {0};          
//float m_average[116] = {0};
float l_variance[116] = {0};           //左线方差
float r_variance[116] = {0};           //右线方差
//float k_variance[116] = {0};          
//float m_variance[116] = {0};
//int16 saoxian[120];
static int16 line_start = 80 ; //寻线起点 
static int16 next_line_start=80;
//int16 fill_line_width =72 ;
static int16 i,j;    //循环变量   
static int8 row ;   //寻线行变量
int16 Lline1[120];
int16 Rline1[120];
int16 Lline2[120];
int16 Rline2[120];
static int16 l_head_point;
static int16 r_head_point;
static int8  fork_point  = 0;  
static int8  fork_long  = 0;  
static int16 mid_l_x;
static int16 mid_l_s;
static int16 mid_r_x;
static int16 mid_r_s;
static int16 mid_l_x1;
static int16 mid_l_s1;
static int16 mid_r_x1;
static int16 mid_r_s1;
int16 white_point=0;
int16 shizhi_row=0;

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
void img_extract(void *dst, void *src, uint32_t srclen)
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
void Get_Edge()
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
            if(jump1_lx>0 || jump1_lx1>0)l.x_jump_point=l.jump_point[0];
            if((jump1_lx<0 || jump1_lx1<0) && MMN(Lline,l.jump_point[0],l.jump_point[0]+15,0)==0)l.s_jump_point=l.jump_point[0];
        }
        if(l.jump_point[1])
        {
            if(jump2_lx>0 || jump2_lx1>0)l.x_jump_point=l.jump_point[1];
            if((jump2_lx<0 || jump2_lx1<0) && MMN(Lline,l.jump_point[1],l.jump_point[1]+15,0)==0)l.s_jump_point=l.jump_point[1];
        }
//         if(l.jump_point[2])
//         {
//             if(jump3_lx>0)l.x_jump_point=l.jump_point[2];
//             if(jump3_lx<0 && Lline[l.jump_point[2]+2]==0 && Lline[l.jump_point[2]+3]==0)l.s_jump_point=l.jump_point[2];
//         }
        
        if(r.jump_point[0])
        {
            if(jump1_rx<0 || jump1_rx1<0)r.x_jump_point=r.jump_point[0];
            if((jump1_rx>0 || jump1_rx1>0) && MMN(Rline,r.jump_point[0],r.jump_point[0]+15,1)==159)r.s_jump_point=r.jump_point[0];
        }
        if(r.jump_point[1])
        {
            if(jump2_rx<0 || jump2_rx1<0)r.x_jump_point=r.jump_point[1];
            if((jump2_rx>0 || jump2_rx1>0) && MMN(Rline,r.jump_point[1],r.jump_point[1]+15,1)==159)r.s_jump_point=r.jump_point[1];
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
//             for(row = 115;row >15 ; row --)//远处的拐点无意义 故只找到15行
//             {
//                 if(((Lline[row]-Lline[row-4])*(Lline[row+4]-Lline[row]))<0 || ((Lline[row+4]-Lline[row])==0 && (Lline[row]-Lline[row-4])>2))//找左拐点的方法
//                 {
//                     if(Lline[row]==0)continue;
//                     if(l_cross_turn_point == 0)
//                     {
//                         l_cross_turn_point = row-3;
//                         break;      //找到拐点跳出循环
//                     }
//                 }
//             }
//             for(row = 115;row > 15 ; row --)//远处的拐点无意义 故只找到15行
//             {
//                 if(((Rline[row]-Rline[row-4])*(Rline[row+4]-Rline[row]))<0 || ((Rline[row+4]-Rline[row])==0 && (Rline[row]-Rline[row-4])<-2))//找右拐点的方法
//                 {
//                     if(Rline[row]==159)continue;
//                     if(r_cross_turn_point == 0)
//                     {
//                          r_cross_turn_point = row-3;
//                         break;      //找到拐点跳出循环
//                     }
//                 }
//             }
             for(row = 115;row >15 ; row --)//远处的拐点无意义 故只找到15行
             {
                 if( ( (Lline[row+4]-Lline[row])<0 && (Lline[row]-Lline[row-4])>0 ) || ((Lline[row+4]-Lline[row])==0 && (Lline[row]-Lline[row-4])>3))//找左拐点的方法
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
                 if( ( (Rline[row+4]-Rline[row])>0 && (Rline[row]-Rline[row-4])<0 ) || ((Rline[row+4]-Rline[row])==0 && (Rline[row]-Rline[row-4])<-3))//找右拐点的方法
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
//    LED_P6x8Str(0,5,"Turn_state=");
//    LED_PrintsignValueI4(80,5,(int)Current_Turn_state); 
           if(Front_flag==255)
           {
               if(Current_Turn_state)
               process_hx_r();
               else
               process_hx_l(); 
           }
           if(Front_flag==0 && HX_text==255)
           {
               if(Current_Turn_state)
               process_hx_l();
               else
               process_hx_r(); 
           }
// /************************************************************************************/
             if(1==BM3)
  {
            if(Front_flag==255 && HX_STATE == HX_NORMAL && nomal_track_flag == 1)
            {
              if(SZ_QLR[SZ_count]==0)
                qianceL_process_cross();//处理正十字
              else
                qianceR_process_cross();
            }
            else if(Front_flag==0 && SZ_Caoceflag == 1 && HX_STATE == HX_NORMAL && nomal_track_flag == 1)
            {
              if(SZ_QLR[SZ_count]==0)
                houceR_process_cross();
              else
                houceL_process_cross();
            }
  }
/*************************************************************十字处理完毕****************************************************************/
/**********************************************************开始判断处理斜十字***************************************************************/
           process_xcross();//100us-200us
/**********************************************************斜十字处理完毕*******************************************************************/
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
/*******************************特殊赛道处理*****************************************/
  if(nomal_track_flag == 1 || ZA_STATE == ZA_YES || QP_STSTE == QP_YES)  //正常赛道前瞻和复杂程度赋值//80us
  {
       if(Prospect_See <15 || ZA_STATE == ZA_YES || QP_STSTE == QP_YES)// && l.jump_point[0] == 0 && r.jump_point[0] == 0 && MMN(Lline,0,50,0)!=0 && MMN(Rline,0,50,1)!=159)
       {
            Left_slope_range = Slope_Range(30,l_First_derivative); //左线斜率范围；
            Right_slope_range = Slope_Range(30,r_First_derivative);//右线斜率范围；
            range_derivative =  ABS(Left_slope_range - Right_slope_range);
            Track_complexity =   (int16)(range_derivative*range_derivative *10);
#if MatlabDebug
        mexPrintf(" Left_slope_range=%f\n", Left_slope_range);
        mexPrintf("Right_slope_range=%f\n",Right_slope_range);
        mexPrintf("Track_complexity=%d\n",Track_complexity);
#endif
            if( (Track_complexity < 10 && (Left_slope_range<2 || Right_slope_range<2)) || ZA_STATE == ZA_YES  || QP_STSTE == QP_YES)//直道
            {
#if ProgramCar
              if( Front_flag==255 || (Front_flag==0 && (Judgment_obstacle_flag >=1 || Judgment_startline_flag>=1) ))
#endif  
                 avoid_obstacle();
            }
/**********************************小s***********************************/
            if(0 == Prospect_See && l.jump_point[0] == 0 && r.jump_point[0] == 0 && MMN(Lline,0,50,0)!=0 && MMN(Rline,0,50,1)!=159)
            {
            if(Track_complexity < 25 && (Left_slope_range>2 || Right_slope_range>2) )  //复杂程度滤S弯 和 直道；左右边线都会抖动但差值很小
            {
              Track_complexity = 0;
#if ProgramCar
              servos.direction_p = XS_Servo_KP;  //小s的参数
              servos.direction_d = XS_Servo_KD;
#endif
            }
            else 
            {
#if ProgramCar
               Track_complexity = ABS((int16)camer.error);
               servos.direction_p = Servo_KP;
               servos.direction_d = Servo_KD;
#endif
            }
            }
             else
            {
#if ProgramCar
             Track_complexity = ABS((int16)camer.error);
             servos.direction_p = Servo_KP;
             servos.direction_d = Servo_KD;
#endif
            }
/************************************小s*********************************************/            
       }
  }
//  if(l_cross_turn_point && r_cross_turn_point && MIN(l_cross_turn_point,r_cross_turn_point)<30)
//    Prospect_See=MIN(l_cross_turn_point,r_cross_turn_point);

    
 
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
  int8 i,mid = 79;
  float err = 0;
//  static int8 flag=0;
  camer.last_error = camer.error;
//   if( ( SZ_Caoceflag == 1 && ABS(camer.last_error)<30 ) || (flag == 1 && SZ_Caoceflag == 1 ) )
//   {
//     flag = 1;
//     if(Front_flag==255)
//       mid = 100;
//     else
//       mid = 62;
//   }
//   else
//   {
//     flag = 0;
//     mid = 79;
//   }
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
  if(Front_flag==0 && SZ_Caoceflag == 1 && (SZ_STATE==SZ_BEFORE || SZ_STATE==SZ_ON || SZ_STATE==SZ_BEHAND))
  {
      if(camer.error-camer.last_error>9)//限幅滤波
      camer.error=camer.last_error+9;
      if(camer.error-camer.last_error<-9)//限幅滤波
      camer.error=camer.last_error-9;
  }
  if(Front_flag==255 && SZ_Caoceflag == 1 && (SZ_STATE==SZ_BEFORE || SZ_STATE==SZ_ON || SZ_STATE==SZ_BEHAND))
  {
      if(camer.error-camer.last_error>15)//限幅滤波
      camer.error=camer.last_error+15;
      if(camer.error-camer.last_error<-15)//限幅滤波
      camer.error=camer.last_error-15;
  }
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
void Imgfilter_Mline(int16 * line)
{
    int8 i,j,k;
    int8 m,n;
    int16 num[11],t;
    for(i=114;i>4;i--)
    {
        k = i+5;
        for(j=0;j<11;j++,k--)
            num[j] = line[k];
        for(m=0;m<11;m++)
        {
            for(n=0;n<10-m;n++)
            {
                if(num[n]>num[n+1])
                {
                    t=num[n];
                    num[n]=num[n+1];
                    num[n+1]=t;
                }
            }
        }
        line[i] = (int16)(num[5]);
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
float Slope_Range(int16 D,float * line)
{
  float MAX,MIN;
  int16 i;
  MAX = line[117];
  MIN = line[117];
  for(i = 116; i>= D;i--)
  {
    if(MAX < line[i])
      MAX = line[i];
    else if(MIN > line[i])
      MIN = line[i];
    
  }
  return (MAX - MIN);
}
/****寻出一个数组中的最大最小点120*160****/
int16 MMN(int16 *line,int16 s,int16 e,uint8 choose)
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
void matchline(int16 start,int16 end,int16 * line)//两点连线
{
    float a0,a1;
    int16 i,divide;
    if(line[start]-line[end]==0)
    line[start]=line[start]+1;
    divide = line[start]-line[end];
     if(divide != 0)
     {
        a1 = (start-end)*1.0/divide;
        a0 = (start-a1*line[start])*1.0;
         if(a1 != 0)
         {
            for(i = start-1;i > end;i --)
                line[i] = (int16)((i-a0)/a1);
         }
     }
}
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
/*********************************************************************前车处理十字左***************************************************************************/
void qianceL_process_cross()
{
    float value=0;
    int16 SZ_shavelr;
#if MatlabDebug
            mexPrintf("判断正入十字\n"); 
            mexPrintf("white_point=%d\n",white_point); 
            mexPrintf("shizhi_row=%d\n",shizhi_row);
#endif
    if(white_point>0 && shizhi_row>0)
    value=(float)white_point/(float)rempcheck_array[shizhi_row];
#if MatlabDebug
    mexPrintf("value=%f\n",value);
#endif
    if(nomal_track_flag == 1 && SZ_STATE==SZ_NONE)
    {
        nomal_track_flag = 0;
        SZ_STATE=SZ_NORMAL;
    }
    if(nomal_track_flag == 1 && SZ_STATE==SZ_BEHAND)
    {
        nomal_track_flag = 0;
        if(SZ_Caoceflag == 0)
        SZ_STATE=SZ_NORMAL;
    }
    if(nomal_track_flag == 1 && SZ_STATE==SZ_ON)
    {
        nomal_track_flag = 0;
        if(MMN(Lline,80,119,0)!=0)
            SZ_STATE=SZ_BEHAND;
    }
    if(nomal_track_flag == 1 && MMN(Lline,80,119,1)==0 && MMN(Rline,80,119,0)==159 && SZ_STATE==SZ_NORMAL && Car_Stop_flag==0)
    {
        nomal_track_flag = 0;
        SZ_STATE=SZ_ON;
    }
    if(nomal_track_flag == 1 && value>0.9 && value <1.1 && SZ_STATE==SZ_NORMAL && MMN(Lline,shizhi_row,l_cross_turn_point,0)==0 && MMN(Rline,shizhi_row,r_cross_turn_point,1)==159 && MAX(l_cross_turn_point,r_cross_turn_point)-shizhi_row>15)
    {
#if MatlabDebug
            mexPrintf("正入十字\n"); 
#endif       
        SZ_STATE=SZ_BEFORE;
    }
    if(nomal_track_flag == 1 && SZ_STATE==SZ_BEFORE)
    {
        nomal_track_flag = 0;
        if(MMN(Lline,105,119,0)==0 && MMN(Rline,105,119,1)==159)
            SZ_STATE=SZ_ON;
    }
    if(SZ_Caoceflag == 1 )
    {
        if(SZ_STATE==SZ_ON)
        {
            row=119;
            while(Lline[row]!=0 && row>0)   row--;
            while(Lline[row]==0 && row>0)  row --;
            SZ_shavelr=row;
            if(SZ_shavelr-3>0)SZ_shavelr=SZ_shavelr-3;
            else SZ_shavelr=0;
            if(SZ_shavelr>7 && Lline[SZ_shavelr]>30)
            {
                Lline[119]=Lline[SZ_shavelr];
                matchline(119,SZ_shavelr,Lline);
                for(row=119;row>0;row--)
                {
                    Rline[row]=Lline[119]+35;
                }
            }
            else if(Lline[SZ_shavelr]<10)//十字前有特殊情况
            {
              find_yuanhuan_r();
              if(fork_point>10 && fork_long>10)
              {
                  for(row=fork_point;row>=fork_point-fork_long;row--)
                  {
                      Lline[row]=Rline2[row];
                  }
                  Lline[119]=Lline[fork_point-3];
                  matchline(119,fork_point-3,Lline);
                  for(row=119;row>0;row--)
                  {
                      Rline[row]=Lline[119]+35;
                  }
              }
            }
            Prospect_See=sz_speedcount;
        }//十字中补线
        if(SZ_STATE==SZ_BEFORE)
        {
            row=119;
            while(Lline[row]!=0 && row>0)   row--;
            while(Lline[row]==0 && row>0)  row --;
            SZ_shavelr=row;
            if(SZ_shavelr-8>0)SZ_shavelr=SZ_shavelr-8;
            else SZ_shavelr=0;
            if(SZ_shavelr>2 && SZ_shavelr<l_cross_turn_point && Lline[SZ_shavelr]>Lline[l_cross_turn_point])
            {    
                matchline(119,SZ_shavelr,Lline);
                for(row=SZ_shavelr;row>0;row--)
                {
                    Rline[row]=Lline[row];
                }
                Rline[119]=Lline[119]+45;
                matchline(119,SZ_shavelr,Rline);
            }
            else if(SZ_shavelr<l_cross_turn_point && Lline[SZ_shavelr]<Lline[l_cross_turn_point])//十字前有特殊情况
            {
              find_yuanhuan_r();
              if(fork_point>10 && fork_long>10 && Rline2[fork_point-8]<110)
              {
                  for(row=fork_point;row>=fork_point-fork_long;row--)
                  {
                      Lline[row]=Rline2[row];
                  }
                  matchline(119,fork_point-8,Lline);
                  for(row=fork_point;row>0;row--)
                  {
                      Rline[row]=Lline[row];
                  }
                  Rline[119]=Lline[119]+45;
                  matchline(119,fork_point,Rline);
              }
            }
             Prospect_See=sz_speedcount;
        }//十字前补线
        if(SZ_STATE==SZ_BEHAND)
        {
            for(row=119;row>0;row--)
            {
                Rline[row]=Lline[119]+48;
            }
            for(row=119;row>0;row--)
            {
                if(Lline[row]==0)
                {
                    Lline[row]=Lline[119];
                }
            }
            Prospect_See=sz_speedcount;
        }//十字后补线
    }//超车补线
}                     
/*********************************************************************前车处理十字右***************************************************************************/
void qianceR_process_cross()
{
    float value=0;
    int16 SZ_shavelr;
#if MatlabDebug
            mexPrintf("判断正入十字\n"); 
            mexPrintf("white_point=%d\n",white_point); 
            mexPrintf("shizhi_row=%d\n",shizhi_row);
#endif
    if(white_point>0 && shizhi_row>0)
    value=(float)white_point/(float)rempcheck_array[shizhi_row];
#if MatlabDebug
    mexPrintf("value=%f\n",value);
#endif
    if(nomal_track_flag == 1 && SZ_STATE==SZ_NONE)
    {
        nomal_track_flag = 0;
        SZ_STATE=SZ_NORMAL;
    }
    if(nomal_track_flag == 1 && SZ_STATE==SZ_BEHAND)
    {
        nomal_track_flag = 0;
        if(SZ_Caoceflag == 0)
        SZ_STATE=SZ_NORMAL;
    }
    if(nomal_track_flag == 1 && SZ_STATE==SZ_ON)
    {
        nomal_track_flag = 0;
        if(MMN(Rline,80,119,1)!=159)
            SZ_STATE=SZ_BEHAND;
    }
    if(nomal_track_flag == 1 && MMN(Lline,80,119,1)==0 && MMN(Rline,80,119,0)==159 && SZ_STATE==SZ_NORMAL && Car_Stop_flag==0)
    {
        nomal_track_flag = 0;
        SZ_STATE=SZ_ON;
    }
    if(nomal_track_flag == 1 && value>0.9 && value <1.1 && SZ_STATE==SZ_NORMAL && MMN(Lline,shizhi_row,l_cross_turn_point,0)==0 && MMN(Rline,shizhi_row,r_cross_turn_point,1)==159 && MAX(l_cross_turn_point,r_cross_turn_point)-shizhi_row>15)
    {
#if MatlabDebug
            mexPrintf("正入十字\n"); 
#endif       
        SZ_STATE=SZ_BEFORE;
    }
    if(nomal_track_flag == 1 && SZ_STATE==SZ_BEFORE)
    {
        nomal_track_flag = 0;
        if(MMN(Lline,105,119,0)==0 && MMN(Rline,105,119,1)==159)
            SZ_STATE=SZ_ON;
    }
    if(SZ_Caoceflag == 1 )
    {
        if(SZ_STATE==SZ_ON)
        {
            row=119;
            while(Rline[row]!=159 && row>0)   row--;
            while(Rline[row]==159 && row>0)  row --;
            SZ_shavelr=row;
            if(SZ_shavelr-3>0)SZ_shavelr=SZ_shavelr-3;
            else SZ_shavelr=0;
            if(SZ_shavelr>7 && Rline[SZ_shavelr]<130)
            {
                Rline[119]=Rline[SZ_shavelr];
                matchline(119,SZ_shavelr,Rline);
                for(row=119;row>0;row--)
                {
                    Lline[row]=Rline[119]-35;
                }
            }
            else if(Rline[SZ_shavelr]>149)//十字前有特殊情况
            {
              find_yuanhuan_l();
              if(fork_point>10 && fork_long>10)
              {
                  for(row=fork_point;row>=fork_point-fork_long;row--)
                  {
                      Rline[row]=Lline2[row];
                  }
                  Rline[119]=Rline[fork_point-3];
                  matchline(119,fork_point-3,Rline);
                  for(row=119;row>0;row--)
                  {
                      Lline[row]=Rline[119]-35;
                  }
              }
            }
            Prospect_See=sz_speedcount;
        }//十字中补线
        if(SZ_STATE==SZ_BEFORE)
        {
            row=119;
            while(Rline[row]!=159 && row>0)   row--;
            while(Rline[row]==159 && row>0)  row --;
            SZ_shavelr=row;
            if(SZ_shavelr-8>0)SZ_shavelr=SZ_shavelr-8;
            else SZ_shavelr=0;
            if(SZ_shavelr>2 && SZ_shavelr<r_cross_turn_point && Rline[SZ_shavelr]<Rline[r_cross_turn_point])
            {    
                matchline(119,SZ_shavelr,Rline);
                for(row=SZ_shavelr;row>0;row--)
                {
                    Lline[row]=Rline[row];
                }
                Lline[119]=Rline[119]-45;
                matchline(119,SZ_shavelr,Lline);
            }
            else if(SZ_shavelr<r_cross_turn_point && Rline[SZ_shavelr]>Rline[r_cross_turn_point])//十字前有特殊情况
            {
              find_yuanhuan_l();
              if(fork_point>10 && fork_long>10 && Lline2[fork_point-8]>50)
              {
                  for(row=fork_point;row>=fork_point-fork_long;row--)
                  {
                      Rline[row]=Lline2[row];
                  }
                  matchline(119,fork_point-8,Rline);
                  for(row=fork_point;row>0;row--)
                  {
                      Lline[row]=Rline[row];
                  }
                  Lline[119]=Rline[119]-45;
                  matchline(119,fork_point,Lline);
              }
            }
            Prospect_See=sz_speedcount;
        }//十字前补线
        if(SZ_STATE==SZ_BEHAND)
        {
            for(row=119;row>0;row--)
            {
                Lline[row]=Rline[119]-48;
            }
            for(row=119;row>0;row--)
            {
                if(Rline[row]==159)
                {
                    Rline[row]=Rline[119];
                }
            }
            Prospect_See=sz_speedcount;
        }//十字后补线
    }//超车补线
}                     
/*************************后车十字超车处理右*******************************/
void houceR_process_cross()
{
    int16 SZ_shavelr;
//    float k=0;
//    int16 x=0,y=0;
    static int16 current_konghang=120,last_konghang=120;
    last_konghang=current_konghang;
    current_konghang=Rline1[119]-Lline1[119];
    
    if(nomal_track_flag == 1 && SZ_STATE==SZ_NORMAL && r_cross_turn_point>25 && Rline[r_cross_turn_point]<139 && r_cross_turn_point>Prospect_See+12  && gpio_get (CSGET)==1 && Ultrasonic_flag == 1)//(nomal_track_flag == 1 && SZ_STATE==SZ_NORMAL && r.s_jump_point>15 && r_cross_turn_point>30 && MMN(Rline,r.s_jump_point,r_cross_turn_point,1)==159)
    {
        nomal_track_flag = 0;
        SZ_STATE=SZ_BEFORE;
    }
    
    if(nomal_track_flag == 1 && SZ_STATE==SZ_BEFORE)
    {//十字前补线
        nomal_track_flag = 0;
        if(MMN(Rline,105,119,1)==159)
            SZ_STATE=SZ_ON;
        row=119;
        while(Rline[row]!=159 && row>0)   row--;
        while(Rline[row]==159 && row>0)  row --;
        SZ_shavelr=row;
        if(SZ_shavelr-6>0)SZ_shavelr=SZ_shavelr-6;
        else SZ_shavelr=0;
        if(SZ_shavelr>14 && r_cross_turn_point>25 && Rline[SZ_shavelr]<Rline[r_cross_turn_point])
        {
            matchline(119,SZ_shavelr,Rline);

            for(row=r_cross_turn_point;row>=0;row--)
            {
                Lline[row]=Rline[r_cross_turn_point+1]-rempcheck_array[r_cross_turn_point+1]/2;
            }
            matchline(119,r_cross_turn_point,Lline);
        }
        else if(Rline[SZ_shavelr]>Rline[r_cross_turn_point])//十字前有特殊情况
        {
              find_yuanhuan_l();
              if(fork_point>10 && fork_long<10 && Lline2[fork_point-6]>50)
              {
                    for(row=mid_l_s-1;row>=Prospect_See;row--)//左岔路
                   {
                       if(Lline2[row]!=EMPTY)
                       {
                           mid_l_x1=row;
                           break;
                       }
                   }
                   for(row=mid_l_x1;row>=Prospect_See;row--)//左岔路
                   {
                       if(Lline2[row]==EMPTY)
                       {
                           mid_l_s1=row+1;
                           break;
                       }
                   }
                  if(mid_l_x>=(Prospect_See+6))
                  {
                     fork_point=mid_l_x1;
                     fork_long=mid_l_x1-mid_l_s1;
                  }
              }
              if(fork_point>10 && fork_long>=10 && Lline2[fork_point-6]>50)
              {
                  for(row=fork_point;row>=fork_point-fork_long;row--)
                  {
                      Rline[row]=Lline2[row];
                  }
                  matchline(119,fork_point-6,Rline);

                  for(row=r_cross_turn_point;row>=0;row--)
                  {
                      Lline[row]=Rline[r_cross_turn_point+1]-rempcheck_array[r_cross_turn_point+1]/2;
                  }
                  matchline(119,r_cross_turn_point,Lline);
              }
        }
    }//十字前补线结束
    if(nomal_track_flag == 1 && MMN(Lline,80,119,1)==0 && MMN(Rline,80,119,0)==159 && SZ_STATE==SZ_NORMAL && Car_Stop_flag==0)
    {
        nomal_track_flag = 0;
        SZ_STATE=SZ_ON; 
    }
    if(nomal_track_flag == 1 && SZ_STATE==SZ_ON)
    {//十字中补线
        nomal_track_flag = 0;
        if(MMN(Rline,80,119,1)!=159)
        {
            SZ_STATE=SZ_BEHAND;
        }
        row=119;
        while(Rline[row]!=159 && row>0)   row--;
        while(Rline[row]==159 && row>0)  row --;
        SZ_shavelr=row;
        if(SZ_shavelr-7>0)SZ_shavelr=SZ_shavelr-7;
        else SZ_shavelr=0;
        if(SZ_shavelr>7 && Rline[SZ_shavelr]<=139)
        {
            Rline[119]=Rline[SZ_shavelr];
            matchline(119,SZ_shavelr,Rline);
            for(row=119;row>0;row--)
            {
                Lline[row]=Rline[119]-40;
            }
        }
        else if(Rline[SZ_shavelr]>139)//十字前有特殊情况
        {
          find_yuanhuan_l();
          if(fork_point>10 && fork_long<10)
          {
                for(row=mid_l_s-1;row>=Prospect_See;row--)//左岔路
               {
                   if(Lline2[row]!=EMPTY)
                   {
                       mid_l_x1=row;
                       break;
                   }
               }
               for(row=mid_l_x1;row>=Prospect_See;row--)//左岔路
               {
                   if(Lline2[row]==EMPTY)
                   {
                       mid_l_s1=row+1;
                       break;
                   }
               }
              if(mid_l_x>=(Prospect_See+6))
              {
                 fork_point=mid_l_x1;
                 fork_long=mid_l_x1-mid_l_s1;
              }
          }
          if(fork_point>10 && fork_long>=10 );//&& Lline[mid_l_s+1]>5)
          {
            for(row=fork_point;row>=fork_point-fork_long;row--)
            {
                Rline[row]=Lline2[row];
            }
            Rline[119]=Rline[fork_point-3];
            matchline(119,fork_point-3,Rline);
            for(row=119;row>0;row--)
            {
                Lline[row]=Rline[119]-40;
            }
          }
//          else
//          {
//            Rline[119]=Rline[SZ_shavelr];
//            matchline(119,SZ_shavelr,Rline);
//            for(row=119;row>0;row--)
//            {
//                Lline[row]=Rline[119]-40;
//            }
//          }
        }
    }//十字中补线
    if(nomal_track_flag == 1 && SZ_STATE==SZ_BEHAND)
    {
        nomal_track_flag = 0;
        for(row=119;row>0;row--)
        {
            Lline[row]=Rline[119]-56;
        }
        for(row=119;row>0;row--)
        {
            if(Rline[row]==159)
            {
                Rline[row]=158;
            }
        }
        if(current_konghang>95 && last_konghang<=95)
        {
          SZ_STATE = SZ_NORMAL;
          Return_normalflag =1;
        }
    }
}
/*************************后车十字超车处理左*******************************/
void houceL_process_cross()
{
    int16 SZ_shavelr;
//    float k=0;
//    int16 x=0,y=0;
    static int16 current_konghang=120,last_konghang=120;
    last_konghang=current_konghang;
    current_konghang=Rline1[119]-Lline1[119];
    
    if(nomal_track_flag == 1 && SZ_STATE==SZ_NORMAL && l_cross_turn_point>25 && Lline[l_cross_turn_point]>20 && l_cross_turn_point>Prospect_See+12 && gpio_get (CSGET)==1 && Ultrasonic_flag == 1)//(nomal_track_flag == 1 && SZ_STATE==SZ_NORMAL && r.s_jump_point>15 && r_cross_turn_point>30 && MMN(Rline,r.s_jump_point,r_cross_turn_point,1)==159)
    {
        nomal_track_flag = 0;
        SZ_STATE=SZ_BEFORE;
    }
    
    if(nomal_track_flag == 1 && SZ_STATE==SZ_BEFORE)
    {//十字前补线
        nomal_track_flag = 0;
        if(MMN(Lline,105,119,0)==0)
            SZ_STATE=SZ_ON;
        row=119;
        while(Lline[row]!=0 && row>0)   row--;
        while(Lline[row]==0 && row>0)  row --;
        SZ_shavelr=row;
        if(SZ_shavelr-6>0)SZ_shavelr=SZ_shavelr-6;
        else SZ_shavelr=0;
        if(SZ_shavelr>14 && l_cross_turn_point>25 && Lline[SZ_shavelr]>Lline[l_cross_turn_point])
        {
            matchline(119,SZ_shavelr,Lline);

            for(row=l_cross_turn_point;row>=0;row--)
            {
                Rline[row]=Lline[l_cross_turn_point+1]+rempcheck_array[l_cross_turn_point+1]/2;
            }
            matchline(119,l_cross_turn_point,Rline);
        }
        else if(Lline[SZ_shavelr]<Lline[l_cross_turn_point])//十字前有特殊情况
        {
              find_yuanhuan_r();
              if(fork_point>10 && fork_long<10 && Rline2[fork_point-6]<110)
              {
                    for(row=mid_r_s-1;row>=Prospect_See;row--)//左岔路
                   {
                       if(Rline2[row]!=EMPTY)
                       {
                           mid_r_x1=row;
                           break;
                       }
                   }
                   for(row=mid_r_x1;row>=Prospect_See;row--)//左岔路
                   {
                       if(Rline2[row]==EMPTY)
                       {
                           mid_r_s1=row+1;
                           break;
                       }
                   }
                  if(mid_r_x>=(Prospect_See+6))
                  {
                     fork_point=mid_r_x1;
                     fork_long=mid_r_x1-mid_r_s1;
                  }
              }
              if(fork_point>10 && fork_long>=10 && Rline2[fork_point-6]<110)
              {
                  for(row=fork_point;row>=fork_point-fork_long;row--)
                  {
                      Lline[row]=Rline2[row];
                  }
                  matchline(119,fork_point-6,Lline);
                  for(row=l_cross_turn_point;row>=0;row--)
                  {
                      Rline[row]=Lline[l_cross_turn_point+1]+rempcheck_array[l_cross_turn_point+1]/2;
                  }
                  matchline(119,l_cross_turn_point,Rline);
              }
        }
    }//十字前补线结束
    if(nomal_track_flag == 1 && MMN(Lline,80,119,1)==0 && MMN(Rline,80,119,0)==159 && SZ_STATE==SZ_NORMAL && Car_Stop_flag==0)
    {
        nomal_track_flag = 0;
        SZ_STATE=SZ_ON; 
    }
    if(nomal_track_flag == 1 && SZ_STATE==SZ_ON)
    {//十字中补线
        nomal_track_flag = 0;
        if(MMN(Lline,80,119,0)!=0)
        {
            SZ_STATE=SZ_BEHAND;
        }
        row=119;
        while(Lline[row]!=0 && row>0)   row--;
        while(Lline[row]==0 && row>0)  row --;
        SZ_shavelr=row;
        if(SZ_shavelr-7>0)SZ_shavelr=SZ_shavelr-7;
        else SZ_shavelr=0;
        if(SZ_shavelr>7 && Lline[SZ_shavelr]>=20)
        {
            Lline[119]=Lline[SZ_shavelr];
            matchline(119,SZ_shavelr,Lline);
            for(row=119;row>0;row--)
            {
                Rline[row]=Lline[119]+40;
            }
        }
        else if(Lline[SZ_shavelr]<20)//十字前有特殊情况
        {
          find_yuanhuan_r();
          if(fork_point>10 && fork_long<10)
          {
                for(row=mid_r_s-1;row>=Prospect_See;row--)//左岔路
               {
                   if(Rline2[row]!=EMPTY)
                   {
                       mid_r_x1=row;
                       break;
                   }
               }
               for(row=mid_r_x1;row>=Prospect_See;row--)//左岔路
               {
                   if(Rline2[row]==EMPTY)
                   {
                       mid_r_s1=row+1;
                       break;
                   }
               }
              if(mid_r_x>=(Prospect_See+6))
              {
                 fork_point=mid_r_x1;
                 fork_long=mid_r_x1-mid_r_s1;
              }
          }
          if(fork_point>10 && fork_long>=10 )//&& Rline[mid_r_s+1]<155)
          {
            for(row=fork_point;row>=fork_point-fork_long;row--)
            {
                Lline[row]=Rline2[row];
            }
            Lline[119]=Lline[fork_point-3];
            matchline(119,fork_point-3,Lline);
            for(row=119;row>0;row--)
            {
                Rline[row]=Lline[119]+40;
            }
          }
//          else
//          {
//            Lline[119]=Lline[SZ_shavelr];
//            matchline(119,SZ_shavelr,Lline);
//            for(row=119;row>0;row--)
//            {
//                Rline[row]=Lline[119]+40;
//            }
//          }
        }
    }//十字中补线
    if(nomal_track_flag == 1 && SZ_STATE==SZ_BEHAND)
    {
        nomal_track_flag = 0;
        for(row=119;row>0;row--)
        {
            Rline[row]=Lline[119]+56;
        }
        for(row=119;row>0;row--)
        {
            if(Lline[row]==0)
            {
                Lline[row]=1;
            }
        }
        if(current_konghang>95 && last_konghang<=95)
        {
          SZ_STATE = SZ_NORMAL;
          Return_normalflag =1;
        }
    }
}
void process_xcross()
{
#if MatlabDebug
            mexPrintf("判断斜十字\n");
#endif
        if( nomal_track_flag == 1 && l_cross_turn_point &&  (r_cross_turn_point<(l_cross_turn_point-20) || r_cross_turn_point>(l_cross_turn_point+20)) && Rline[l_cross_turn_point]==159 && Rline[l_cross_turn_point+2]==159 && Rline[l_cross_turn_point-2]==159 //&&
//              (      (l.jump_point[0] && l.jump_point[0]<l_cross_turn_point && Lline[l.jump_point[0]-1]>50 && jump1_lx<0 &&  r.jump_point[0]==0 )   ||  
//                     ( r.jump_point[0] && r.jump_point[0]<l_cross_turn_point && Rline[r.jump_point[0]]>40 && jump1_rx>0 )      )
           )//左斜十字
        {
            find_yuanhuan_l();
            find_yuanhuan_r();
#if MatlabDebug
            mexPrintf("岔路fork_point=%d\n",fork_point);
            mexPrintf("岔路长度fork_long=%d\n",fork_long);
            mexPrintf("Lline2[%d]=%d\n",fork_point,Lline2[fork_point]);
            mexPrintf("Rline2[%d]=%d\n",fork_point,Rline2[fork_point]);
            mexPrintf("Lline2[%d]=%d\n",fork_point-fork_long,Lline2[fork_point-fork_long]);
            mexPrintf("Rline2[%d]=%d\n",fork_point-fork_long,Rline2[fork_point-fork_long]);
#endif
            if(fork_point && fork_point<l_cross_turn_point && fork_long>=3 )//&& Rline2[fork_point]-Lline2[fork_point]<11 && Rline2[fork_point-fork_long]-Lline2[fork_point-fork_long]>fork_long)
            {
                nomal_track_flag = 0;
#if MatlabDebug
            mexPrintf("判断为左斜十字\n");
#endif
                for(row=fork_point;row>=fork_point-fork_long;row--)
                {
                    Lline[row]=Rline2[row];
                }
                matchline(l_cross_turn_point,fork_point-2,Lline);
            }
        }
        else if(nomal_track_flag == 1 && r_cross_turn_point && (l_cross_turn_point<(r_cross_turn_point-20) || l_cross_turn_point>(r_cross_turn_point+20)) && Lline[r_cross_turn_point]==0 && Lline[r_cross_turn_point+2]==0 && Lline[r_cross_turn_point-2]==0 //&&
//                (    (r.jump_point[0] && r.jump_point[0]<r_cross_turn_point && Rline[r.jump_point[0]-1]<110 && jump1_rx>0 && l.jump_point[0]==0 )   ||  
//                     ( l.jump_point[0] && l.jump_point[0]<r_cross_turn_point  && Lline[l.jump_point[0]]<120 && jump1_lx<0 )    )
                )//右斜十字
        {
            find_yuanhuan_l();
            find_yuanhuan_r();
#if MatlabDebug
            mexPrintf("岔路fork_point=%d\n",fork_point);
            mexPrintf("岔路长度fork_long=%d\n",fork_long);
            mexPrintf("Lline2[%d]=%d\n",fork_point,Lline2[fork_point]);
            mexPrintf("Rline2[%d]=%d\n",fork_point,Rline2[fork_point]);
            mexPrintf("Lline2[%d]=%d\n",fork_point-fork_long,Lline2[fork_point-fork_long]);
            mexPrintf("Rline2[%d]=%d\n",fork_point-fork_long,Rline2[fork_point-fork_long]);
#endif
            if(fork_point && fork_point<r_cross_turn_point && fork_long>=3 )//&& Rline2[fork_point]-Lline2[fork_point]<11 && Rline2[fork_point-fork_long]-Lline2[fork_point-fork_long]>fork_long )
            {
            nomal_track_flag = 0;
#if MatlabDebug
            mexPrintf("判断为右斜十字\n");
#endif
            for(row=fork_point;row>=fork_point-fork_long;row--)
            {
                Rline[row]=Lline2[row];
            }
            matchline(r_cross_turn_point,fork_point-2,Rline);
            }
        }
#if MatlabDebug
        else mexPrintf("没有判断为斜十字\n");
#endif
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
void Give_Prospect_See()
{
//给前瞻               
               if(l.bend_jump_point && r.bend_jump_point && l_First_derivative[l.bend_jump_point+2]<0 && r_First_derivative[r.bend_jump_point+2]<0)
               {
                   Prospect_See= l.bend_jump_point;                     
               }
               else if(l.bend_jump_point && r.bend_jump_point && l_First_derivative[l.bend_jump_point+2]>0 && r_First_derivative[r.bend_jump_point+2]>0)
               {
                   Prospect_See= r.bend_jump_point;                     
               }
               else if(l.bend_jump_point && r.bend_jump_point==0 && l_First_derivative[l.bend_jump_point+2]<0 )
               {
                   Prospect_See= l.bend_jump_point;                     
               }
               else if(l.bend_jump_point && r.bend_jump_point==0 && l_First_derivative[l.bend_jump_point+2]>0 )
               {
                   Prospect_See= r.bend_jump_point;                     
               }
               else if(l.bend_jump_point==0 && r.bend_jump_point && r_First_derivative[r.bend_jump_point+2]<0 )
               {
                   Prospect_See= l.bend_jump_point;                     
               }
               else if(l.bend_jump_point==0 && r.bend_jump_point && r_First_derivative[r.bend_jump_point+2]>0 )
               {
                   Prospect_See= r.bend_jump_point;                     
               }
               else if(l.bend_jump_point == r.bend_jump_point)
               {
                   Prospect_See= l.bend_jump_point;   
               }
               else Prospect_See= 0;
}
/*****************************找拐点***************************************/
int8 AcquireInflecPoint(int end, int16* line, char flag)//拐点
{
    int8 row = 119,i,count1=0,count2=0,inf_point=0;
    i = MAX(15,end);
    if(flag == 'L')
    {
      while(row > i)
      {
        if(*(line+row-1)<*(line+row+count2) && count1>5)
        {
          count2 ++;
          if(count2 > 5) {inf_point = row+5;break;}
        }
        else if(*(line+row-1)>=*(line+row))     
        {
          count1 ++;
          if(count2)    {count1=0;count2=0;}
        }
        row --;
      }
    }
    else
    {
       while(row > i)
      {
        if(*(line+row-1)>*(line+row+count2) && count1>5)
        {
          count2 ++;
          if(count2 > 5) {inf_point = row+5;break;}
        }
        else if(*(line+row-1)<=*(line+row))    
        {
          count1 ++;
          if(count2)    {count1=0;count2=0;}
        }
        row --;
      }
    }
    return inf_point;
}
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
             if(mid_l_x>=(Prospect_See+6))
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
             if(mid_r_x>=(Prospect_See+6))
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
/********************************环形左转****************************************************************************************************************************************************************************************/
void process_hx_l()
{
 #if MatlabDebug
             mexPrintf("判断环形\n");
 #endif       
  /**********************************出环形*****************************************/
             if( HX_STATE == HX_OUT)//出环形
             { 
 #if MatlabDebug
             mexPrintf("出环形\n");
 #endif 
               nomal_track_flag = 0;
 #if ProgramCar
                 if(start_up == 0 && Front_flag==255 && l_cross_turn_point>25 && MMN(Lline,l_cross_turn_point-5,l_cross_turn_point,0)!=0  && (MMN(Rline,l_cross_turn_point-10,l_cross_turn_point+10,1)==159 || fork_point>15) )
                 {
                           Car_Stop_flag=1;
                           start_up = ~start_up;  
                 }
 #endif
               if(MMN(Lline,95,115,0)!= 0 && MMN(Rline,95,115,1)!= 159 && Car_Stop_flag == 0)//&& l_cross_turn_point>55)
               {
                 HX_STATE = HX_NORMAL;
               }
               find_yuanhuan_l();
 #if Fill_Line
              if(fork_point>40 && fork_long>2)
               {
                  for(row=fork_point;row>=fork_point-fork_long;row--)
                  {
                      Rline[row]=Lline2[row];
                  }
 //                  Lline[119]=Lline[fork_point]-1;
                  matchline(119,fork_point,Rline);
//                   if(r.x_jump_point<fork_point)
//                      fill_r_line(r.x_jump_point);
//                  if(mid_l_s>0)
//                  fill_r_line(mid_l_s);
//                  if(fork_point)
//                      Prospect_See = fork_point;
               }
//	       else nomal_track_flag = 1;
#endif 
             }
 /**********************************环形中*****************************************/
             if( HX_STATE == HX_ONON)//环形中
             {     
 #if MatlabDebug
             mexPrintf("环形中中\n");
 #endif
                 nomal_track_flag = 0;
                 find_yuanhuan_l();
 #if ProgramCar
                 if(start_up == 0 && Front_flag==255 && l_cross_turn_point>25  && MMN(Lline,l_cross_turn_point-5,l_cross_turn_point,0)!=0 && (MMN(Rline,l_cross_turn_point-10,l_cross_turn_point+10,1)==159 || fork_point>15) )
                 {
                           Car_Stop_flag=1;
                           start_up = ~start_up; 
                 }
 #endif
                 if(MMN(Rline,117,119,0)== 159 && fork_point>40 && fork_point<110)//( (fork_point>50 && fork_long>2) ||( r_cross_turn_point>35 && MMN(Lline,r_cross_turn_point,r_cross_turn_point+3,1)==0 && MMN(Rline,r_cross_turn_point-5,r_cross_turn_point,1)!=159))
                 {
                       HX_STATE = HX_OUT;
                 }
                 else fill_bend();
                 if(fork_point>25 && fork_long>2)
                  {
                     for(row=fork_point;row>=fork_point-fork_long;row--)
                     {
                         Rline[row]=Lline2[row];
                     }
    //                     Lline[119]=Lline[fork_point]-1;
                  if(Lline2[fork_point]<140)    //岔路到中间一点再补
                     matchline(119,fork_point,Rline);
                  if(mid_l_s>0)
                  fill_r_line(mid_l_s);
                     if(fork_point)
                         Prospect_See = fork_point;
                 }
             }//环形中中结束
             if( HX_STATE == HX_ON)//环形中
             {
                 if(MMN(Rline,98,119,1)!= 159)//( (fork_point>50 && fork_long>2) ||( r_cross_turn_point>35 && MMN(Lline,r_cross_turn_point,r_cross_turn_point+3,1)==0 && MMN(Rline,r_cross_turn_point-5,r_cross_turn_point,1)!=159))
                 {
                       HX_STATE = HX_ONON;
                 }
//                  else
//                  fill_bend();
#if MatlabDebug
             mexPrintf("环形中\n");
 #endif                  
                 nomal_track_flag = 0;
                 find_yuanhuan_l();
                 if(fork_point && fork_long>6)
                  {
                     for(row=mid_l_x;row>=mid_l_s;row--)
                     {
                         Rline[row]=Lline2[row];
                     }
#if MatlabDebug
             mexPrintf("环形补线\n");
#endif 
#if Fill_Line
                     matchline(119,(int16)(fork_point)-6,Rline);
#endif    
                         Prospect_See = (int16)(fork_point);
                  }
                 else if(HX_STATE == HX_ON && (fork_point<10 ||(fork_point>10 && fork_long<=6)))
                 {
                     row=119;
                     while(Rline[row]!=159 && row>0)   row--;
                     while(Rline[row]==159 && row>0)  row --;
                     for(;row>0;row--)
                     {
                         if((Rline[row]-Rline[119])/(row-119)*1.0>r_First_derivative[row])
                             break;
                     }
                     matchline(119,row,Rline);
                     Prospect_See=row+10; 
                 }
             }//环形中结束括号  
 /*************************************************************环形前****************************************************************/
          if(HX_STATE == HX_BEFORE)
          {//环形前
 #if MatlabDebug
             mexPrintf("环形前\n");
 #endif 
             nomal_track_flag=0;
             find_yuanhuan_l();
             if(MMN(Rline,100,119,1)== 159  && fork_long<10 && (l_cross_turn_point<fork_point || l_cross_turn_point>105) )//
             {
               HX_STATE = HX_ON;
             }
 #if MatlabDebug
             mexPrintf("岔路fork_point=%d\n",fork_point);
             mexPrintf("岔路长度fork_long=%d\n",fork_long);
 #endif    
             if(fork_point && fork_long>6)//环形前有岔路 
             {
               for(row=mid_l_x;row>=mid_l_s;row--)//右岔路
               {
                   Rline[row] = Lline2[row];
               }
  #if MatlabDebug
              mexPrintf("环形补线\n");
  #endif         
  #if Fill_Line
              if(r_cross_turn_point)
              {
                r_cross_turn_point=r_cross_turn_point+30;
                if(r_cross_turn_point>119)r_cross_turn_point=119;
                matchline(r_cross_turn_point,(int16)(fork_point)-6,Rline);
              }
              else
              matchline(119,(int16)(fork_point)-6,Rline);    
 #endif  
              Prospect_See=(int16)(fork_point); 
              }
             else if(fork_point<10 ||(fork_point>10 && fork_long<=6))//环形前无岔路针对大圆环
             {
  #if MatlabDebug
              mexPrintf("R150环形补线\n");
  #endif       
                 if(r_cross_turn_point)
                 {
                     row=r_cross_turn_point;
                     while(Rline[row]!=159 && row>0)   row--;
                     while(Rline[row]==159 && row>0)  row --;
                     for(;row>0;row--)
                     {
                         if((Rline[row]-Rline[r_cross_turn_point])/(row-r_cross_turn_point)*1.0>r_First_derivative[row])
                             break;
                     }
                     r_cross_turn_point=r_cross_turn_point+30;
                     if(l_cross_turn_point>119)r_cross_turn_point=119;
                     if(Rline[row]<Rline[r_cross_turn_point])
                     {
                       matchline(r_cross_turn_point,row,Rline);
                       Prospect_See=row+15; 
                     }
                     else if(Rline[row]>=Rline[r_cross_turn_point])//防护措施
                     {
                      row=r_cross_turn_point;
                      while(Lline[row]!=0 && row>0)   row--;
                      while(Lline[row]==0 && row>0)  row --;
                      Rline[row]=Lline[row+1];
                      matchline(r_cross_turn_point,row,Rline);
                      Prospect_See=row+15; 
                     }
                 }
                 else
                 {
                     row=119;
                     while(Rline[row]!=159 && row>0)   row--;
                     while(Rline[row]==159 && row>0)  row --;
                     for(;row>0;row--)
                     {
                         if((Rline[row]-Rline[119])/(row-119)*1.0>r_First_derivative[row])
                             break;
                     }
                     matchline(119,row,Rline);
                     Prospect_See=row+15; 
                 }
             }
          }//环形前结束括号
/*******************************************判断环形**************************************************/
   if(nomal_track_flag == 1 && HX_STATE == HX_NORMAL && ABS(l_cross_turn_point-r_cross_turn_point)<20 && l_cross_turn_point>20 && r_cross_turn_point>20 && !(Lline[119]==0 && Rline[119]==159) )
    {
 #if MatlabDebug
             mexPrintf("进入判断环形\n");
 #endif
    float k1=0,k2=0,k3=0;
    float b1=0,b2=0;
    float l_line[120]={0},r_line[120]={161};
    int8 low_Lpoint=0,low_Rpoint=0;
    int16 hx_Lpoint=0,hx_Rpoint=0,cow=0,Black_count=0;
    low_Lpoint=l_cross_turn_point+18;
    low_Rpoint=r_cross_turn_point+18;
    if(low_Lpoint>=119)low_Lpoint=119;
    if(low_Rpoint>=119)low_Rpoint=119;
    k1=(float)(Lline[l_cross_turn_point+3]-Lline[low_Lpoint])/(float)(-15);//(l_cross_turn_point+3-low_Lpoint);
    k2=(float)(Rline[low_Rpoint]-Rline[r_cross_turn_point+3])/(float)(15);//(low_Rpoint-r_cross_turn_point+3);    
    b1=Lline[l_cross_turn_point+3]-k1*(l_cross_turn_point+3);
    b2=Rline[r_cross_turn_point+3]-k2*(r_cross_turn_point+3);
    for(row=119;row>=0;row--)
    {
        l_line[row]=k1*row+b1;
    }
    for(row=119;row>=0;row--)
    {
        r_line[row]=k2*row+b2;
    }
    for(row=l_cross_turn_point-3;row>=0;row--)
    {
        if(255==img_handle[row+1][(int16)(l_line[row+1])] && 0==img_handle[row][(int16)(l_line[row])])
        {
         hx_Lpoint=row;   
         break;
        }
    }
    for(row=r_cross_turn_point-3;row>=0;row--)
    {
        if(255==img_handle[row+1][(int16)(r_line[row+1])] && 0==img_handle[row][(int16)(r_line[row])])
        {
         hx_Rpoint=row; 
         break;
        }
    }
    shizhi_row=MAX(hx_Rpoint,hx_Lpoint);
//    if(hx_Lpoint && hx_Rpoint)
//    {
//      hx_Lpoint=hx_Lpoint-1;
//      hx_Rpoint=hx_Rpoint-1;
//    }
    if(hx_Lpoint && hx_Rpoint && (r_line[row])-l_line[row]>10)
    {
        if(hx_Rpoint==hx_Lpoint)
        {
            for(cow=(int16)(l_line[hx_Lpoint]);cow<=(int16)(r_line[hx_Rpoint]);cow++)
            {
                Black_count+=img_handle[row][cow];
            }
            Black_count/=255;
        }
        else
        {
            k3=(r_line[hx_Rpoint]-l_line[hx_Lpoint])/(hx_Rpoint-hx_Lpoint);
            for(cow=(int16)(l_line[hx_Lpoint]);cow<=(int16)(r_line[hx_Rpoint]);cow++)
            {
                row=(int8)(hx_Lpoint+(cow-l_line[hx_Lpoint])/k3);
                Black_count+=img_handle[row][cow];
            }
            Black_count/=255;
        }
    }
    else Black_count=100;//右线必须大于左线
    white_point=Black_count;
#if MatlabDebug   
    for(row = 119 ; row >=0 ; row--)
    {
        outdata7[row] = l_line[row]+1;
    }
    for(row = 119 ; row >=0 ; row--)
    {
        outdata7[row+120] = r_line[row]+1;
    }
#endif
//        find_yuanhuan_l();
//        find_yuanhuan_r();
       
 #if MatlabDebug
//              mexPrintf("岔路fork_point=%d\n",fork_point);
//              mexPrintf("岔路长度fork_long=%d\n",fork_long);
//              mexPrintf("Lline2[fork_point]=%d\n",Lline2[fork_point]);
//              mexPrintf("Rline2[fork_point]=%d\n",Rline2[fork_point]);
//              mexPrintf("Mid[fork_point]=%d\n",(Lline2[fork_point]+Rline2[fork_point])/2);
//              mexPrintf("l_line[fork_point]+4=%f\n",l_line[fork_point]+4);
//              mexPrintf("r_line[fork_point]-4=%f\n",r_line[fork_point]-4);
             mexPrintf("k1=%f\n",k1);
             mexPrintf("k2=%f\n",k2);
             mexPrintf("k3=%f\n",k3);
             mexPrintf("hx_Lpoint=%d\n",hx_Lpoint);   
             mexPrintf("hx_Rpoint=%d\n",hx_Rpoint);
             mexPrintf("Black_count=%d\n",Black_count);
             mexPrintf("hx_Lpoint=%d hx_Rpoint=%d\n",hx_Lpoint,hx_Rpoint);
             mexPrintf("ABS(hx_Lpoint-hx_Rpoint)=%d\n",ABS(hx_Lpoint-hx_Rpoint));
 #endif 
      if(hx_Lpoint && hx_Rpoint && MAX(hx_Lpoint,hx_Rpoint)>(Prospect_See+10) && ABS(hx_Lpoint-hx_Rpoint)<12 && MAX(l_cross_turn_point,r_cross_turn_point)-MIN(hx_Lpoint,hx_Rpoint)>15 && 0==Black_count&& (k3>2.0 || k3<-2.0) && k1<=0 && k1>=-1.0 && k2>=0 && k2<=1.0 )//(fork_point && fork_point<MIN(l_cross_turn_point,r_cross_turn_point) && (Lline2[fork_point]+Rline2[fork_point])/2>l_line[fork_point]+4 && (Lline2[fork_point]+Rline2[fork_point])/2<r_line[fork_point]-4 && fork_long>3)
      {
          nomal_track_flag=0;
          HX_STATE = HX_BEFORE;
 #if MatlabDebug
             mexPrintf("判断环形成功\n");
 #endif   
//              if(fork_point && fork_long>3)//环形前有岔路 
//              {
//                for(row=mid_r_x;row>=mid_r_s;row--)//右岔路
//                {
//                    Lline[row] = Rline2[row];
//                }
 #if MatlabDebug
             mexPrintf("环形补线\n");
 #endif         
 #if Fill_Line
             Rline[hx_Lpoint]=(int16)(l_line[hx_Lpoint]);
             if(r_cross_turn_point)
             {
                matchline(r_cross_turn_point,hx_Lpoint,Rline);
             }
             else
             matchline(119,hx_Lpoint,Rline);    
#endif  
             Prospect_See=MAX(hx_Lpoint,hx_Rpoint); 
//              }
      }
    }//判断环形结束
}//全部环形结束括号
/********************************环形右转****************************************************************************************************************************************************************************************/
void process_hx_r()
{
 #if MatlabDebug
             mexPrintf("判断环形\n");
 #endif  
 /**********************************出环形*****************************************/
             if( HX_STATE == HX_OUT)//出环形
             {
 #if MatlabDebug
             mexPrintf("出环形\n");
 #endif 
               nomal_track_flag = 0;
 #if ProgramCar
                 if(start_up == 0 && Front_flag==255 && r_cross_turn_point>25  && MMN(Rline,r_cross_turn_point-5,r_cross_turn_point,1)!=159 && (MMN(Lline,r_cross_turn_point-10,r_cross_turn_point+10,0)==0  || fork_point>15) )
                 {
                           Car_Stop_flag=1;
                           start_up = ~start_up;  
                 }
 #endif
               if(MMN(Lline,95,115,0)!= 0 && MMN(Rline,95,115,1)!= 159 && Car_Stop_flag == 0)//&& l_cross_turn_point>55)
               {
                 HX_STATE = HX_NORMAL;
               }
               find_yuanhuan_r(); 
 #if Fill_Line
              if(fork_point>40 && fork_long>2)
               {
                  for(row=fork_point;row>=fork_point-fork_long;row--)
                  {
                      Lline[row]=Rline2[row];
                  }
 //                  Lline[119]=Lline[fork_point]-1;
                  matchline(119,fork_point,Lline);
//                   if(l.x_jump_point<fork_point)
//                      fill_l_line(l.x_jump_point);
//                  if(mid_r_s>0)
//                  fill_l_line(mid_r_s);
//                  if(fork_point)
//                      Prospect_See = fork_point;
              }
//	       else nomal_track_flag = 1;
#endif 
             }
 /**********************************环形中*****************************************/
             if( HX_STATE == HX_ONON)//环形中
             {             
 #if MatlabDebug
             mexPrintf("环形中中\n");
 #endif
                 nomal_track_flag = 0;
                 find_yuanhuan_r();
 #if ProgramCar
                 if(start_up == 0 && Front_flag==255 && r_cross_turn_point>25 && MMN(Rline,r_cross_turn_point-5,r_cross_turn_point,1)!=159 && (MMN(Lline,r_cross_turn_point-10,r_cross_turn_point+10,0)==0 || fork_point>15) )
                 {
                           Car_Stop_flag=1;
                           start_up = ~start_up;  
                 }
 #endif
                 if(MMN(Lline,117,119,1)== 0 && fork_point>40 && fork_point<110)//( (fork_point>50 && fork_long>2) ||( r_cross_turn_point>35 && MMN(Lline,r_cross_turn_point,r_cross_turn_point+3,1)==0 && MMN(Rline,r_cross_turn_point-5,r_cross_turn_point,1)!=159))
                 {
                       HX_STATE = HX_OUT;
                 }
                 else fill_bend();
                 if(fork_point>25 && fork_long>2)
                  {
                     for(row=fork_point;row>=fork_point-fork_long;row--)
                     {
                         Lline[row]=Rline2[row];
                     }
    //                     Lline[119]=Lline[fork_point]-1;
                  if(Rline2[fork_point]>20) //岔路到中间一点再补
                     matchline(119,fork_point,Lline);
                  if(mid_r_s>0)
                  fill_l_line(mid_r_s);
                     if(fork_point)
                         Prospect_See = fork_point;
                 }
             }//环形中中结束
             if( HX_STATE == HX_ON)//环形中
             {
                 if(MMN(Lline,98,119,0)!= 0)//( (fork_point>50 && fork_long>2) ||( r_cross_turn_point>35 && MMN(Lline,r_cross_turn_point,r_cross_turn_point+3,1)==0 && MMN(Rline,r_cross_turn_point-5,r_cross_turn_point,1)!=159))
                 {
                       HX_STATE = HX_ONON;
                 }
//                  else
//                  fill_bend();
 #if MatlabDebug
             mexPrintf("环形中\n");
 #endif 
                 nomal_track_flag = 0;
                 find_yuanhuan_r();
                 if(fork_point && fork_long>6)//环形前有岔路 
                 {
                     for(row=mid_r_x;row>=mid_r_s;row--)//右岔路
                     {
                       Lline[row] = Rline2[row];
                     }
#if MatlabDebug
             mexPrintf("环形补线\n");
#endif         
#if Fill_Line
                     matchline(119,(int16)(fork_point)-6,Lline);    
#endif  
                     Prospect_See=(int16)(fork_point); 
                 }
                 else if(HX_STATE == HX_ON && (fork_point<10 ||(fork_point>10 && fork_long<=6)))
                 {
                     row=119;
                     while(Lline[row]!=0 && row>0)   row--;
                     while(Lline[row]==0 && row>0)  row --;
                     for(;row>0;row--)
                     {
                         if((Lline[row]-Lline[119])/(row-119)*1.0<l_First_derivative[row])
                             break;
                     }
                     matchline(119,row,Lline);
                     Prospect_See=row+10; 
                 }
             }//环形中结束括号  
 /*************************************************************环形前****************************************************************/   
          if(HX_STATE == HX_BEFORE)
          {
 #if MatlabDebug
             mexPrintf("环形前\n");
 #endif 
             nomal_track_flag=0;
             find_yuanhuan_r();
             if(MMN(Lline,100,119,0)== 0  && fork_long<10 && (r_cross_turn_point<fork_point || r_cross_turn_point>105))//
             {
               HX_STATE = HX_ON;
             }
 #if MatlabDebug
             mexPrintf("岔路fork_point=%d\n",fork_point);
             mexPrintf("岔路长度fork_long=%d\n",fork_long);
 #endif    
             if(fork_point && fork_long>6)//环形前有岔路 
             {
               for(row=mid_r_x;row>=mid_r_s;row--)//右岔路
               {
                   Lline[row] = Rline2[row];
               }
  #if MatlabDebug
              mexPrintf("环形补线\n");
  #endif         
  #if Fill_Line
              if(l_cross_turn_point)
              {
                l_cross_turn_point=l_cross_turn_point+30;
                if(l_cross_turn_point>119)l_cross_turn_point=119;
                matchline(l_cross_turn_point,(int16)(fork_point)-6,Lline);
              }
              else
              matchline(119,(int16)(fork_point)-6,Lline);    
 #endif  
              Prospect_See=(int16)(fork_point); 
              }
             else if(fork_point<10 ||(fork_point>10 && fork_long<=6))//环形前无岔路针对大圆环
             {
  #if MatlabDebug
              mexPrintf("R150环形补线\n");
  #endif       
                 if(l_cross_turn_point)
                 {
                     row=l_cross_turn_point;
                     while(Lline[row]!=0 && row>0)   row--;
                     while(Lline[row]==0 && row>0)  row --;
                     for(;row>0;row--)
                     {
                         if((Lline[row]-Lline[l_cross_turn_point])/(row-l_cross_turn_point)*1.0<l_First_derivative[row])
                             break;
                     }
                     l_cross_turn_point=l_cross_turn_point+30;
                     if(l_cross_turn_point>119)l_cross_turn_point=119;
                     if(Lline[row]>Lline[l_cross_turn_point])
                     {
                      matchline(l_cross_turn_point,row,Lline);
                      Prospect_See=row+15; 
                     }
                     else if(Lline[row]<=Lline[l_cross_turn_point])//防护措施
                     {
                      row=l_cross_turn_point;
                      while(Rline[row]!=159 && row>0)   row--;
                      while(Rline[row]==159 && row>0)  row --;
                      Lline[row]=Rline[row+1];
                      matchline(l_cross_turn_point,row,Lline);
                      Prospect_See=row+15; 
                     }
                 }
                 else
                 {
                     row=119;
                     while(Lline[row]!=0 && row>0)   row--;
                     while(Lline[row]==0 && row>0)  row --;
                     for(;row>0;row--)
                     {
                         if((Lline[row]-Lline[119])/(row-119)*1.0<l_First_derivative[row])
                             break;
                     }
                     matchline(119,row,Lline);
                     Prospect_See=row+15; 
                 }
             }
          }//环形前结束
/*******************************************判断环形**************************************************/
    if(nomal_track_flag == 1 && HX_STATE == HX_NORMAL && ABS(l_cross_turn_point-r_cross_turn_point)<20 && l_cross_turn_point>20 && r_cross_turn_point>20 && !(Lline[119]==0 && Rline[119]==159) )
    {
 #if MatlabDebug
             mexPrintf("进入判断环形\n");
 #endif
    float k1=0,k2=0,k3=0;
    float b1=0,b2=0;
    float l_line[120]={0},r_line[120]={161};
    int8 low_Lpoint=0,low_Rpoint=0;
    int16 hx_Lpoint=0,hx_Rpoint=0,cow=0,Black_count=0;
    low_Lpoint=l_cross_turn_point+18;
    low_Rpoint=r_cross_turn_point+18;
    if(low_Lpoint>=119)low_Lpoint=119;
    if(low_Rpoint>=119)low_Rpoint=119;
    k1=(float)(Lline[l_cross_turn_point+3]-Lline[low_Lpoint])/(float)(-15);//(l_cross_turn_point+3-low_Lpoint);
    k2=(float)(Rline[low_Rpoint]-Rline[r_cross_turn_point+3])/(float)(15);//(low_Rpoint-r_cross_turn_point+3);    
    b1=Lline[l_cross_turn_point+3]-k1*(l_cross_turn_point+3);
    b2=Rline[r_cross_turn_point+3]-k2*(r_cross_turn_point+3);
    for(row=119;row>=0;row--)
    {
        l_line[row]=k1*row+b1;
    }
    for(row=119;row>=0;row--)
    {
        r_line[row]=k2*row+b2;
    }
    for(row=l_cross_turn_point-3;row>=0;row--)
    {
        if(255==img_handle[row+1][(int16)(l_line[row+1])] && 0==img_handle[row][(int16)(l_line[row])])
        {
         hx_Lpoint=row;   
         break;
        }
    }
    for(row=r_cross_turn_point-3;row>=0;row--)
    {
        if(255==img_handle[row+1][(int16)(r_line[row+1])] && 0==img_handle[row][(int16)(r_line[row])])
        {
         hx_Rpoint=row; 
         break;
        }
    }
    shizhi_row=MAX(hx_Rpoint,hx_Lpoint);
//    if(hx_Lpoint && hx_Rpoint)
//    {
//      hx_Lpoint=hx_Lpoint-1;
//      hx_Rpoint=hx_Rpoint-1;
//    }
    if(hx_Lpoint && hx_Rpoint && (r_line[row])-l_line[row]>10)
    {
        if(hx_Rpoint==hx_Lpoint)
        {
            for(cow=(int16)(l_line[hx_Lpoint]);cow<=(int16)(r_line[hx_Rpoint]);cow++)
            {
                Black_count+=img_handle[row][cow];
            }
            Black_count/=255;
        }
        else
        {
            k3=(r_line[hx_Rpoint]-l_line[hx_Lpoint])/(hx_Rpoint-hx_Lpoint);
            for(cow=(int16)(l_line[hx_Lpoint]);cow<=(int16)(r_line[hx_Rpoint]);cow++)
            {
                row=(int8)(hx_Lpoint+(cow-l_line[hx_Lpoint])/k3);
                Black_count+=img_handle[row][cow];
            }
            Black_count/=255;
        }
    }
    else Black_count=100;//右线必须大于左线
    white_point=Black_count;
    
#if MatlabDebug   
    for(row = 119 ; row >=0 ; row--)
    {
        outdata7[row] = l_line[row]+1;
    }
    for(row = 119 ; row >=0 ; row--)
    {
        outdata7[row+120] = r_line[row]+1;
    }
#endif
//        find_yuanhuan_l();
//        find_yuanhuan_r();
       
 #if MatlabDebug
//              mexPrintf("岔路fork_point=%d\n",fork_point);
//              mexPrintf("岔路长度fork_long=%d\n",fork_long);
//              mexPrintf("Lline2[fork_point]=%d\n",Lline2[fork_point]);
//              mexPrintf("Rline2[fork_point]=%d\n",Rline2[fork_point]);
//              mexPrintf("Mid[fork_point]=%d\n",(Lline2[fork_point]+Rline2[fork_point])/2);
//              mexPrintf("l_line[fork_point]+4=%f\n",l_line[fork_point]+4);
//              mexPrintf("r_line[fork_point]-4=%f\n",r_line[fork_point]-4);
             mexPrintf("k1=%f\n",k1);
             mexPrintf("k2=%f\n",k2);
             mexPrintf("k3=%f\n",k3);
             mexPrintf("hx_Lpoint=%d\n",hx_Lpoint);   
             mexPrintf("hx_Rpoint=%d\n",hx_Rpoint);
             mexPrintf("Black_count=%d\n",Black_count);
             mexPrintf("hx_Lpoint=%d hx_Rpoint=%d\n",hx_Lpoint,hx_Rpoint);
             mexPrintf("ABS(hx_Lpoint-hx_Rpoint)=%d\n",ABS(hx_Lpoint-hx_Rpoint));
 #endif 
      if(hx_Lpoint && hx_Rpoint && MAX(hx_Lpoint,hx_Rpoint)>(Prospect_See+10) && ABS(hx_Lpoint-hx_Rpoint)<12 && MAX(l_cross_turn_point,r_cross_turn_point)-MIN(hx_Lpoint,hx_Rpoint)>15  && 0==Black_count && (k3>2.0 || k3<-2.0) && k1<=0 && k1>=-1.0 && k2>=0 && k2<=1.0 )//(fork_point && fork_point<MIN(l_cross_turn_point,r_cross_turn_point) && (Lline2[fork_point]+Rline2[fork_point])/2>l_line[fork_point]+4 && (Lline2[fork_point]+Rline2[fork_point])/2<r_line[fork_point]-4 && fork_long>3)
      {
          nomal_track_flag=0;
          HX_STATE = HX_BEFORE;
 #if MatlabDebug
             mexPrintf("判断环形成功\n");
 #endif   
//              if(fork_point && fork_long>3)//环形前有岔路 
//              {
//                for(row=mid_r_x;row>=mid_r_s;row--)//右岔路
//                {
//                    Lline[row] = Rline2[row];
//                }
 #if MatlabDebug
             mexPrintf("环形补线\n");
 #endif         
 #if Fill_Line
             Lline[hx_Rpoint]=(int16)(r_line[hx_Rpoint]);
             if(l_cross_turn_point)
             {
                matchline(l_cross_turn_point,hx_Rpoint,Lline);
             }
             else
             matchline(119,hx_Rpoint,Lline);    
#endif  
             Prospect_See=MAX(hx_Lpoint,hx_Rpoint); 
//              }
      }
    }//判断环形结束
}//全部环形结束括号
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
    if( Front_flag==255 || (Front_flag==0 && Judgment_startline_flag>=1)  )
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