/********************************电机驱动**********************************
*
*作者   ： 沙艺已觉
*文件名 ： FuzzySet_Speed.c
*描述   ： 模糊算法设定速度C文件
*时间   ： 2016/4/14
说明    ：参考FreeCars模糊控制例程  使用山外V5.3库
*
****************************************************************************/
#include "FuzzySet_Speed.h"
#include "include.h"


const int16 EMAX = 600;      //    MAX_DISTANCE-EMAX
const int16 EMIN = 0;       //     MAX_DISTANCE
const int16 PMAX = 110 ;     //大于440的值将被当成 440
const int16 PMIN = 15 ;      //小于PMIN的值将被当成      赛道越简单，速度越快，否则越是复杂就越慢
const int16 DMAX = 45;       //这个值表示图像后DMAX行空的多少，空的越多，速度越慢
const int16 DMIN = 3;       //
const int16 SMAX = 100;      //两个参数都统一到0~100的值（归一化）
const int16 SMIN = 0 ;
const int16 FMAX = 100 ;     //语言值的满幅值
              // 0  1   2   3   4   5   6
int16 PFF[7] = { 0, 16, 32, 48, 64, 80, 96 };     //统一到最大值； 最小值；
int16 DFF[7] = { 0, 16, 32, 48, 64, 80, 96 };     //
int16 EFF[7] = { 0, 16, 32, 48, 64, 80, 96 };
/*输出量U语言值特征点    0    1    2    3    4    5    6 */
 //数组里面的数值为速度
//正跑速度表
#ifdef FIRSTCAR //后

/*四档 小弯入环极限*/
int16 UFF4_BEFORE[7] =     {  220, 223, 226, 230, 235, 240, 250 };//280, 285, 290, 305, 320, 340, 360
int16 UFF4_BEHAND[7] =     {  220, 225, 230, 240, 250, 260, 270 };
int16 UFF4_HX[7]     =     {  210, 215, 220, 225, 230, 235, 240 };
int16 UFF4_SZH[7]    =     {  150, 160, 170, 180, 190, 200, 210 };
int16 UFF4_SLOW[7]   =     {  120, 130, 140, 150, 170, 190, 220 };//240, 250, 260, 270, 300, 320, 340

/*三档 弯道慢 */
int16 UFF3_BEFORE[7] =     {  240, 245, 250, 255, 260, 265, 270 };//{  210, 213, 216, 219, 222, 226, 230 };//280, 285, 290, 305, 320, 340, 360
int16 UFF3_BEHAND[7] =     {  240, 250, 260, 270, 275, 280, 300 };//{  210, 213, 216, 220, 230, 240, 250 };
int16 UFF3_HX[7]     =     {  210, 215, 220, 230, 240, 250, 260 };//{  210, 213, 216, 219, 222, 226, 230 };
int16 UFF3_SZH[7]    =     {  210, 215, 220, 230, 240, 250, 260 };//{  150, 160, 170, 180, 190, 200, 210 };
int16 UFF3_SLOW[7]   =     {  120, 130, 140, 150, 160, 190, 220 };//{  120, 130, 140, 150, 170, 190, 220 };//240, 250, 260, 270, 300, 320, 340

/*二档  直道快*/
int16 UFF2_BEFORE[7] =     {  240, 245, 250, 255, 260, 265, 270 };//280, 285, 290, 305, 320, 340, 360
int16 UFF2_BEHAND[7] =     {  250, 255, 260, 270, 275, 280, 300 };
int16 UFF2_HX[7]     =     {  210, 215, 220, 230, 240, 250, 260 };
int16 UFF2_SZH[7]    =     {  210, 215, 220, 230, 240, 250, 260 };
int16 UFF2_SLOW[7]   =     {  170, 185, 200, 215, 230, 245, 260 };//240, 250, 260, 270, 300, 320, 340

/*一档 跟随档*/
int16 UFF1_BEFORE[7] =     {  220, 225, 230, 240, 250, 260, 270 };//280, 285, 290, 305, 320, 340, 360
int16 UFF1_BEHAND[7] =     {  235, 240, 250, 260, 270, 280, 300 };
int16 UFF1_HX[7]     =     {  210, 215, 220, 230, 240, 250, 260 };
int16 UFF1_SZH[7]    =     {  210, 215, 220, 230, 240, 250, 260 };
int16 UFF1_SLOW[7]   =     {  120, 130, 140, 150, 170, 190, 220 };//240, 250, 260, 270, 300, 320, 340

/*最稳档*/
int16 UFF0_BEFORE[7] =     {  220, 225, 230, 240, 250, 260, 270 };
int16 UFF0_BEHAND[7] =     {  220, 230, 240, 250, 260, 270, 290 };
int16 UFF0_HX[7]     =     {  210, 215, 220, 225, 230, 235, 240 };
int16 UFF0_SZH[7]    =     {  150, 160, 170, 180, 190, 200, 210 };
int16 UFF0_SLOW[7]   =     {  120, 130, 140, 150, 160, 190, 220 };
int16 UFF0_CX[7]     =     {  249, 260, 270, 290, 310, 320, 340 };


#else  //前

/*四档*/
int16 UFF4_BEFORE[7] =     {  220, 223, 226, 230, 235, 240, 250 };//290, 295, 300, 310, 320, 350, 410
int16 UFF4_BEHAND[7] =     {  220, 225, 230, 240, 250, 260, 270 };
int16 UFF4_HX[7]     =     {  210, 215, 220, 225, 230, 235, 240 };  
int16 UFF4_SZH[7]    =     {  150, 160, 170, 180, 190, 200, 210 };
int16 UFF4_SLOW[7]   =     {  120, 130, 140, 150, 170, 190, 220 };

/*三档*/
int16 UFF3_BEFORE[7] =     {  240, 245, 250, 255, 260, 265, 270 };//290, 295, 300, 310, 320, 350, 410
int16 UFF3_BEHAND[7] =     {  240, 250, 260, 270, 275, 280, 300 };
int16 UFF3_HX[7]     =     {  210, 215, 220, 230, 240, 250, 260 };
int16 UFF3_SZH[7]    =     {  210, 215, 220, 230, 240, 250, 260 };
int16 UFF3_SLOW[7]   =     {  120, 130, 140, 150, 160, 190, 220 };

/*二档 */
int16 UFF2_BEFORE[7] =     {  240, 245, 250, 255, 260, 265, 270 };//280, 285, 290, 305, 320, 340, 360
int16 UFF2_BEHAND[7] =     {  250, 255, 260, 270, 275, 280, 300 };
int16 UFF2_HX[7]     =     {  210, 215, 220, 230, 240, 250, 260 };
int16 UFF2_SZH[7]    =     {  210, 215, 220, 230, 240, 250, 260 };
int16 UFF2_SLOW[7]   =     {  170, 185, 200, 215, 230, 245, 260 };//240, 250, 260, 270, 300, 320, 340

/*一档 随档*/
int16 UFF1_BEFORE[7] =     {  220, 225, 230, 240, 250, 260, 270 };//280, 285, 290, 305, 320, 340, 360
int16 UFF1_BEHAND[7] =     {  235, 240, 250, 260, 270, 280, 300 };
int16 UFF1_HX[7]     =     {  210, 215, 220, 230, 240, 250, 260 };
int16 UFF1_SZH[7]    =     {  210, 215, 220, 230, 240, 250, 260 };
int16 UFF1_SLOW[7]   =     {  120, 130, 140, 150, 170, 190, 220 };//240, 250, 260, 270, 300, 320, 340

/*最稳档*/
int16 UFF0_BEFORE[7] =     {  220, 225, 230, 240, 250, 260, 270 };//280, 285, 290, 305, 320, 340, 360
int16 UFF0_BEHAND[7] =     {  220, 230, 240, 250, 260, 270, 290 };
int16 UFF0_HX[7]     =     {  210, 215, 220, 225, 230, 235, 240 }; 
int16 UFF0_SZH[7]    =     {  150, 160, 170, 180, 190, 200, 210 };
int16 UFF0_SLOW[7]   =     {  120, 130, 140, 150, 160, 190, 220 };//240, 250, 260, 270, 300, 320, 340
int16 UFF0_CX[7]     =     {  249, 260, 270, 290, 310, 320, 340 };

#endif
int16 UFF0_WAIT[7]   =     {   100, 100, 100, 100, 100, 100, 100 };//240, 250, 260, 270, 300, 320, 340
int16 UFF1_WAIT[7]   =     {   100, 100, 100, 100, 100, 100, 100 };//240, 250, 260, 270, 300, 320, 340
int16 UFF2_WAIT[7]   =     {   100, 100, 100, 100, 100, 100, 100 };
int16 UFF3_WAIT[7]   =     {   100, 100, 100, 100, 100, 100, 100 };
int16 UFF4_WAIT[7]   =     {   100, 100, 100, 100, 100, 100, 100 };

uint16 rule[7][7][7] =
{
  /*Track_complexity */  //  0,  1,  2,  3,  4,  5,  6    Prospect_See
                       {
  /* 0 */                   {  6,  5,  4,  3,  2,  1,  1,} ,
  /* 1 */                   {  5,  5,  4,  3,  2,  1,  1,} ,
  /* 2 */                   {  4,  4,  3,  2,  1,  1,  0,} ,
  /* 3 */                   {  3,  3,  3,  2,  1,  1,  0,} ,
  /* 4 */                   {  2,  2,  2,  1,  1,  0,  0,} ,
  /* 5 */                   {  2,  1,  1,  0,  0,  0,  0,} ,
  /* 6 */                   {  1,  1,  0,  0,  0,  0,  0,}
                       },

                       {
                            {  5,  4,  3,  2,  1,  1,  1,} ,
			    {  4,  4,  3,  2,  1,  1,  0,} ,
			    {  3,  3,  2,  1,  1,  0,  0,} ,
			    {  2,  2,  2,  1,  0,  0,  0,} ,
			    {  1,  1,  1,  0,  0,  0,  0,} ,
			    {  1,  1,  0,  0,  0,  0,  0,} ,
			    {  1,  0,  0,  0,  0,  0,  0,}
                       },
					  {
                            {  4,  3,  2,  1,  1,  1,  0,} ,
			    {  3,  3,  2,  1,  1,  0,  0,} ,
			    {  2,  2,  1,  1,  0,  0,  0,} ,
			    {  1,  1,  1,  0,  0,  0,  0,} ,
			    {  1,  1,  0,  0,  0,  0,  0,} ,
			    {  1,  0,  0,  0,  0,  0,  0,} ,
			    {  0,  0,  0,  0,  0,  0,  0,}
                       },
					  {
                            {  3,  2,  1,  1,  1,  0,  0,} ,
                            {  2,  2,  1,  1,  0,  0,  0,} ,
                            {  1,  1,  1,  0,  0,  0,  0,} ,
                            {  1,  1,  0,  0,  0,  0,  0,} ,
                            {  1,  0,  0,  0,  0,  0,  0,} ,
                            {  0,  0,  0,  0,  0,  0,  0,} ,
                            {  0,  0,  0,  0,  0,  0,  0,}
                       },
					   {
                            {  2,  1,  1,  1,  0,  0,  0,} ,
                            {  1,  1,  1,  0,  0,  0,  0,} ,
                            {  1,  1,  0,  0,  0,  0,  0,} ,
                            {  1,  0,  0,  0,  0,  0,  0,} ,
                            {  0,  0,  0,  0,  0,  0,  0,} ,
                            {  0,  0,  0,  0,  0,  0,  0,} ,
                            {  0,  0,  0,  0,  0,  0,  0,}
                       },
					   {
                            {  1,  1,  1,  0,  0,  0,  0,} ,
                            {  1,  1,  0,  0,  0,  0,  0,} ,
                            {  1,  0,  0,  0,  0,  0,  0,} ,
                            {  0,  0,  0,  0,  0,  0,  0,} ,
                            {  0,  0,  0,  0,  0,  0,  0,} ,
                            {  0,  0,  0,  0,  0,  0,  0,} ,
                            {  0,  0,  0,  0,  0,  0,  0,}
                       },
					   {
                            {  1,  1,  0,  0,  0,  0,  0,} ,
                            {  1,  0,  0,  0,  0,  0,  0,} ,
                            {  0,  0,  0,  0,  0,  0,  0,} ,
                            {  0,  0,  0,  0,  0,  0,  0,} ,
                            {  0,  0,  0,  0,  0,  0,  0,} ,
                            {  0,  0,  0,  0,  0,  0,  0,} ,
                            {  0,  0,  0,  0,  0,  0,  0,}
                       }
};

int16 *UFF = UFF0_BEFORE;  //这样可以选择不用的U语言数组
int16 *UFF_BEFORE =UFF0_BEFORE;
int16 *UFF_BEHAND =UFF0_BEHAND;
int16 *UFF_HX     =UFF0_HX    ;
int16 *UFF_SZH    =UFF0_SZH   ;
int16 *UFF_SLOW   =UFF0_SLOW  ;
int16 *UFF_WAIT   =UFF0_WAIT  ;

/*  P 代表赛道复杂程度
D 表示前瞻
模糊控制算法通过这两个跟定量以及规则表给出当前应该输出的速度
*/
int16 FuzzySet_Speed(int16 P, int16 D,int16 E)          //模糊运算引擎，返回速度值
{
   int16 U;       /*偏差，以及输出值的精确量 */
   uint16 PF[2];
   uint16 DF[2];
   uint16 EF[2];
   uint16 UF[8];  /*偏差，偏差微分以及输出值的隶属度PF[1]是P的隶属度，PF[0]是隶属度的补集 */
   int16 Pn = 0, Dn = 0,En = 0;
   int16 Un[8];
   int32 temp1,temp2;

   if(P < PMIN)
     P = PMIN;
   else
     if(P > PMAX)
       P = PMAX;

   P = (int16)((double)(P - PMIN) / (PMAX - PMIN) * (SMAX - SMIN) + SMIN); //归一化到SMIN ~ SMAX


   if(D < DMIN)
     D = DMIN;
   else
     if( D > DMAX)
       D = DMAX;

   D = (int16)((double)(D - DMIN) / (DMAX - DMIN) * (SMAX - SMIN) + SMIN) ;    //归一化到SMIN ~ SMAX

   if(E < EMIN)
       E = EMIN;
   else
       if(E > EMAX)
         E = EMAX;
    E = (int16)((double)(E - EMIN) / (EMAX - EMIN) * (SMAX - SMIN) + SMIN);

   /*隶属度的确定*/
   /*根据PD的指定语言获得有效的隶属度*/

   if(P > PFF[0] && P < PFF[6])
   {
        if (P <= PFF[1])
        {
          Pn = 1;
          PF[0] = (uint16)(FMAX * ((float)(PFF[1] - P) / (PFF[1] - PFF[0])));
        }
        else if (P <= PFF[2])
        {
          Pn = 2;
          PF[0] = (uint16)(FMAX * ((float)(PFF[2] - P) / (PFF[2] - PFF[1])));
        }
        else if (P <= PFF[3])
        {
          Pn = 3;
          PF[0] = (uint16)(FMAX * ((float)(PFF[3] - P) / (PFF[3] - PFF[2])));
        }
        else if (P <= PFF[4])
        {
          Pn = 4;
          PF[0] = (uint16)(FMAX * ((float)(PFF[4] - P) / (PFF[4] - PFF[3])));
        }
        else if (P <= PFF[5])
        {
          Pn = 5;
          PF[0] = (uint16)(FMAX * ((float)(PFF[5] - P) / (PFF[5] - PFF[4])));
        }
        else if (P <= PFF[6])
        {
          Pn = 6;
          PF[0] = (uint16)(FMAX * ((float)(PFF[6] - P) / (PFF[6] - PFF[5])));
        }
   }
   else if (P <= PFF[0])
   {
        Pn = 1;
        PF[0] = (uint16)(FMAX);
    }
   else if (P >= PFF[6])
   {
        Pn = 6;
        PF[0] = 0;
    }
   PF[1] = (uint16)(FMAX - PF[0]);


     if (D > DFF[0] && D < DFF[6])
      {
        if (D <= DFF[1])
        {
          Dn = 1; DF[0] = (uint16)(FMAX * ((float)(DFF[1] - D) / (DFF[1] - DFF[0])));
        }
        else if (D <= DFF[2])
        {
          Dn = 2;
          DF[0] = (uint16)(FMAX * ((float)(DFF[2] - D) / (DFF[2] - DFF[1])));
        }
        else if (D <= DFF[3])
        {
          Dn = 3;
          DF[0] = (uint16)(FMAX * ((float)(DFF[3] - D) / (DFF[3] - DFF[2])));
        }
        else if (D <= DFF[4])
        {
          Dn = 4;
          DF[0] = (uint16)(FMAX * ((float)(DFF[4] - D) / (DFF[4] - DFF[3])));
        }
        else if (D <= DFF[5])
        {
          Dn = 5;
          DF[0] = (uint16)(FMAX * ((float)(DFF[5] - D) / (DFF[5] - DFF[4])));
        }
        else if (D <= DFF[6])
        {
          Dn = 6;
          DF[0] = (uint16)(FMAX * ((float)(DFF[6] - D) / (DFF[6] - DFF[5])));
        }
      }
      else if (D <= DFF[0])
      {
        Dn = 1;
        DF[0] = (uint16)(FMAX);
      }
      else if (D >= DFF[6])
      {
        Dn = 6;
        DF[0] = 0;
      }
      DF[1] = (uint16)(FMAX - DF[0]);


       if (E > EFF[0] && E < EFF[6])
      {
        if (E <= EFF[1])
        {
          En = 1; EF[0] = (uint16)(FMAX * ((float)(EFF[1] - E) / (EFF[1] - EFF[0])));
        }
        else if (E <= EFF[2])
        {
          En = 2;
          EF[0] = (uint16)(FMAX * ((float)(EFF[2] - E) / (EFF[2] - EFF[1])));
        }
        else if (E <= EFF[3])
        {
          En = 3;
          EF[0] = (uint16)(FMAX * ((float)(EFF[3] - E) / (EFF[3] - EFF[2])));
        }
        else if (E <= EFF[4])
        {
          En = 4;
          EF[0] = (uint16)(FMAX * ((float)(EFF[4] - E) / (EFF[4] - EFF[3])));
        }
        else if (E <= EFF[5])
        {
          En = 5;
          EF[0] = (uint16)(FMAX * ((float)(EFF[5] - E) / (EFF[5] - EFF[4])));
        }
        else if (E <= EFF[6])
        {
          En = 6;
          EF[0] = (uint16)(FMAX * ((float)(EFF[6] - E) / (EFF[6] - EFF[5])));
        }
      }
      else if (E <= EFF[0])
      {
        En = 1;
        EF[0] = (uint16)(FMAX);
      }
      else if (E >= EFF[6])
      {
        En = 6;
        EF[0] = 0;
      }
      EF[1] = (uint16)(FMAX - EF[0]);


       /*使用误差范围优化后的规则表rule[7][7]*/
      /*输出值使用13个隶属函数,中心值由UFF[7]指定*/
      /*一般都是四个规则有效*/
      Un[0] = rule[Pn - 1][ Dn - 1][En -1];
      Un[1] = rule[Pn][ Dn - 1][En -1];
      Un[2] = rule[Pn - 1][Dn][En -1];
      Un[3] = rule[Pn][Dn][En -1];
      Un[4] = rule[Pn - 1][ Dn - 1][En];
      Un[5] = rule[Pn][ Dn - 1][En];
      Un[6] = rule[Pn - 1][Dn][En];
      Un[7] = rule[Pn][Dn][En];

	  if (PF[0] <= DF[0]) //计算R表，隶属函数求交集
	  {
		UF[0] = PF[0];
		if(UF[0] <= EF[0])
			UF[0] = UF[0];
		else
		    UF[0] = EF[0];
	  }
      else
	  {
		   UF[0] = DF[0];
		if(UF[0] <= EF[0])
			UF[0] = UF[0];
		else
			UF[0] = EF[0];
	  }

      if (PF[1] <= DF[0])
	  {
		UF[1] = PF[1];
		if(UF[1] <= EF[0])
			UF[1] = UF[1];
		else
			UF[1] = EF[0];
	  }
      else
	  {
        UF[1] = DF[0];
		if(UF[1] <= EF[0])
			UF[1] = UF[1];
		else
			UF[1] = EF[0];
	  }

      if (PF[0] <= DF[1])
	  {
		UF[2] = PF[0];
		if(UF[2] <= EF[0])
		    UF[2] = UF[2];
		else
			UF[2] = EF[0];
	  }
      else
	  {
		UF[2] = DF[1];
		if(UF[2] <= EF[0])
			UF[2] = UF[2];
		else
			UF[2] = EF[0];
	  }

      if (PF[1] <= DF[1])
	  {
		UF[3] = PF[1];
		if(UF[3] <= EF[0])
		    UF[3] = UF[3];
		else
			UF[3] = EF[0];
	  }
      else
	  {
		UF[3] = DF[1];
		if(UF[3] <= EF[0])
			UF[3] = UF[3];
		else
			UF[3] = EF[0];
	  }

	  if (PF[0] <= DF[0])
	  {
		UF[4] = PF[0];
		if(UF[4] <= EF[1])
			UF[4] = UF[4];
		else
		    UF[4] = EF[1];
	  }
      else
	  {
		UF[4] = DF[0];
		if(UF[4] <= EF[1])
			UF[4] = UF[4];
		else
			UF[4] = EF[1];
	  }

      if (PF[1] <= DF[0])
	  {
		UF[5] = PF[1];
		if(UF[5] <= EF[1])
			UF[5] = UF[5];
		else
			UF[5] = EF[1];
	  }
      else
	  {
        UF[5] = DF[0];
		if(UF[5] <= EF[1])
			UF[5] = UF[5];
		else
			UF[5] = EF[1];
	  }

      if (PF[0] <= DF[1])
	  {
		UF[6] = PF[0];
		if(UF[6] <= EF[1])
		    UF[6] = UF[6];
		else
			UF[6] = EF[1];
	  }
      else
	  {
		UF[6] = DF[1];
		if(UF[6] <= EF[1])
			UF[6] = UF[6];
		else
			UF[6] = EF[1];
	  }

      if (PF[1] <= DF[1])
	  {
		UF[7] = PF[1];
		if(UF[7] <= EF[1])
		    UF[7] = UF[7];
		else
			UF[7] = EF[1];
	  }
      else
	  {
		UF[7] = DF[1];
		if(UF[7] <= EF[1])
			UF[7] = UF[7];
		else
			UF[7] = EF[1];
	  }

	  /*同隶属函数输出语言值求大*/

      if (Un[0] == Un[1])
      {
        if (UF[0] > UF[1])
          UF[1] = 0;
        else
          UF[0] = 0;
      }
      if (Un[0] == Un[2])
      {
        if (UF[0] > UF[2])
          UF[2] = 0;
        else
          UF[0] = 0;
      }
      if (Un[0] == Un[3])
      {
        if (UF[0] > UF[3])
          UF[3] = 0;
        else
          UF[0] = 0;
      }
	  if(Un[0] == Un[4])
	  {
		if (UF[0] > UF[4])
		  UF[4] = 0;
		else
		  UF[0] = 0;
	  }
	  if(Un[0] == Un[5])
	  {
		if (UF[0] > UF[5])
		  UF[5] = 0;
		else
		  UF[0] = 0;
	  }
	  if(Un[0] == Un[6])
	  {
		if (UF[0] > UF[6])
		  UF[6] = 0;
		else
		  UF[0] = 0;
	  }
	  if(Un[0] == Un[7])
	  {
		if (UF[0] > UF[7])
		  UF[7] = 0;
		else
		  UF[0] = 0;
	  }


      if (Un[1] == Un[2])
      {
        if (UF[1] > UF[2])
          UF[2] = 0;
        else
          UF[1] = 0;
      }
      if (Un[1] == Un[3])
      {
        if (UF[1] > UF[3])
          UF[3] = 0;
        else
          UF[1] = 0;
      }
	  if (Un[1] == Un[4])
      {
        if (UF[1] > UF[4])
          UF[4] = 0;
        else
          UF[1] = 0;
      }
	  if (Un[1] == Un[5])
      {
        if (UF[1] > UF[5])
          UF[5] = 0;
        else
          UF[1] = 0;
      }
	  if (Un[1] == Un[6])
      {
        if (UF[1] > UF[6])
          UF[6] = 0;
        else
          UF[1] = 0;
      }
	  if (Un[1] == Un[7])
      {
        if (UF[1] > UF[7])
          UF[7] = 0;
        else
          UF[1] = 0;
      }


      if (Un[2] == Un[3])
      {
        if (UF[2] > UF[3])
          UF[3] = 0;
        else
          UF[2] = 0;
      }
	  if (Un[2] == Un[4])
      {
        if (UF[2] > UF[4])
          UF[4] = 0;
        else
          UF[2] = 0;
      }
	  if (Un[2] == Un[5])
      {
        if (UF[2] > UF[5])
          UF[5] = 0;
        else
          UF[2] = 0;
      }
	  if (Un[2] == Un[6])
      {
        if (UF[2] > UF[6])
          UF[6] = 0;
        else
          UF[2] = 0;
      }
	  if (Un[2] == Un[7])
      {
        if (UF[2] > UF[7])
          UF[7] = 0;
        else
          UF[2] = 0;
      }


	  if(Un[3] == Un[4])
	  {
		if(UF[3] > UF[4])
		   UF[4] = 0;
		else
		   UF[3] = 0;
	  }
	  if(Un[3] == Un[5])
	  {
		if(UF[3] > UF[5])
		   UF[5] = 0;
		else
		   UF[3] = 0;
	  }
	  if(Un[3] == Un[6])
	  {
		if(UF[3] > UF[6])
		   UF[6] = 0;
		else
		   UF[3] = 0;
	  }
	  if(Un[3] == Un[7])
	  {
		if(UF[3] > UF[7])
		   UF[7] = 0;
		else
		   UF[3] = 0;
	  }


	  if(Un[4] == Un[5])
	  {
		if(UF[4] > UF[5])
		   UF[5] = 0;
		else
		   UF[4] = 0;
	  }
	  if(Un[4] == Un[6])
	  {
		if(UF[4] > UF[6])
		   UF[6] = 0;
		else
		   UF[4] = 0;
	  }
	  if(Un[4] == Un[7])
	  {
		if(UF[4] > UF[7])
		   UF[7] = 0;
		else
		   UF[4] = 0;
	  }


	  if(Un[5] == Un[6])
	  {
		if(UF[5] > UF[6])
		   UF[6] = 0;
		else
		   UF[5] = 0;
	  }
	  if(Un[5] == Un[7])
	  {
		if(UF[5] > UF[7])
		   UF[7] = 0;
		else
		   UF[5] = 0;
	  }

	  if(Un[6] == Un[7])
	  {
		if(UF[6] > UF[7])
		   UF[7] = 0;
		else
		   UF[6] = 0;
	  }

      /*重心法(按隶属度加权平均)反模糊*/
      /*Un[]原值为输出隶属函数标号，转换为隶属函数值*/

      Un[0] = UFF[Un[0]];

      Un[1] = UFF[Un[1]];

      Un[2] = UFF[Un[2]];

      Un[3] = UFF[Un[3]];

	  Un[4] = UFF[Un[4]];

	  Un[5] = UFF[Un[5]];

	  Un[6] = UFF[Un[6]];

	  Un[7] = UFF[Un[7]];

      temp1 = UF[0] * Un[0] + UF[1] * Un[1] + UF[2] * Un[2] + UF[3] * Un[3] + UF[4] * Un[4] + UF[5] * Un[5] + UF[6] * Un[6] + UF[7] * Un[7];
      temp2 = UF[0] + UF[1] + UF[2] + UF[3] + UF[4] + UF[5] + UF[6] + UF[7];
      U = (int16)(temp1 / temp2);
      return U;
}
   
 
const int16 SPMAX = 180 ;     //速度偏差最大值
const int16 SPMIN = 0 ;      //速度偏差最小值
const int16 SDMAX = 40;       //速度偏差变化率最大值
const int16 SDMIN = 0;        //速度偏差变化率最小值
const int16 SSMAX = 100;       //两个参数都统一到0~100的值（归一化）
const int16 SSMIN = 0 ;

void gearshift_integral(int16 SP,int16 SD,float PS,float PX,float IS,float IX)//变速积分
{
  if(SP<0)SP=2*SP;
  
  if(ABS(SP)>SPMAX)
    SP=SPMAX;
  else
    SP=ABS(SP);

  SP = (int16)((double)(SP - SPMIN) / (SPMAX - SPMIN) * (SSMAX - SSMIN) + SSMIN);//归一化
  
  if(ABS(SD)>SDMAX)
    SD=SDMAX;
  else
    SD=ABS(SD);

  SD = (int16)((double)(SD - SDMIN) / (SDMAX - SDMIN) * (SSMAX - SSMIN) + SSMIN);//归一化
  
  
  motor.speed_p=(float)SP / 100.0 * (PS - PX) + PX ;
  motor.speed_i=IS - (float)SP / 100.0 * (IS - IX) ;
  
}



