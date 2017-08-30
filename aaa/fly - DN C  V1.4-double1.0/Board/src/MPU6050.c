#include "include.h"


//加速度变量
int16 ACCEL_XOUT_DATA = 0;
int16 ACCEL_YOUT_DATA = 0;
int16 ACCEL_ZOUT_DATA = 0;
//陀螺仪变量
int16 GYRO_XOUT_DATA = 0;
int16 GYRO_YOUT_DATA = 0;
int16 GYRO_ZOUT_DATA = 0;
/*************
6050加速计零偏
************/
#define   ACCEL_XOUT_DATA0     250    //
#define   ACCEL_YOUT_DATA0     250    //
#define   ACCEL_ZOUT_DATA0     430    //
/*************
6050陀螺仪零偏
************/
int16 GYRO_XOUT_DATA0 = 0 ;
int16 GYRO_YOUT_DATA0 = 0 ;
int16 GYRO_ZOUT_DATA0 = 0 ;

float angle=0,omega=0;
//float th_acc=0,w_gyro=0;
#define   Kalman_time   0.005 //定义采样时间5毫秒
//状态变换阵
float A1_1 = 1;
float A1_2 = -1*Kalman_time;
float A2_1 = 0;
float A2_2 = 1;
float B1_1 = Kalman_time;
float B2_1 = 0;
float H1_1 = 1;
float H1_2 = 0;
//系数阵  
float Pest1_1;          //先验估计协方差阵 
float Pest1_2;
float Pest2_1;
float Pest2_2;    
float Psta1_1 = 1;      //后验估计协方差阵
float Psta1_2 = 1;  
float Psta2_1 = 1;  
float Psta2_2 = 1;   
float Q1_1 = 0.00001;   //过程激励噪声协方差，0.00001即可
float Q1_2 = 0;  
float Q2_1 = 0;  
float Q2_2 = 0.00001;   //0.00001即可
float K1_1;             //卡尔曼增益
float K2_1;    
float R = 0.005;          //观测噪声协方差，主要调此参数
float I1_1 = 1;         //单位阵
float I1_2 = 0;  
float I2_1 = 0;  
float I2_2 = 1;   
//状态阵
float Xest1_1;          //先验状态估计
float Xest2_1;          
float Xsta1_1 = 0;      //后验状态估计
float Xsta2_1 = 0;
void Delay5us()
{
//   DELAY_US(5);
  systick_delay_ns(200);
}

//**************************************
//I2C起始信号
//**************************************
void I2C_Start()
{
    Delay5us();
    MPU6050_SDA_OUT = 1;                    //拉高数据线
    Delay5us();
    MPU6050_SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    MPU6050_SDA_OUT = 0;                    //产生下降沿
    Delay5us();                 //延时
    MPU6050_SCL = 0;                    //拉低时钟线
    Delay5us();
}
//**************************************
//I2C停止信号
//**************************************
void I2C_Stop()
{
    MPU6050_SDA_OUT = 0;                    //拉低数据线
    Delay5us();
    MPU6050_SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    MPU6050_SDA_OUT = 1;                    //产生上升沿
    Delay5us();                 //延时
}
//**************************************
//I2C发送应答信号
//入口参数:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(uint8 ack)
{
    MPU6050_SDA_OUT = ack;                  //写应答信号
    Delay5us();
    MPU6050_SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    MPU6050_SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
}
//**************************************
//I2C接收应答信号
//**************************************
uint8 I2C_RecvACK()
{
    uint8 tem;
    MPU6050_SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    MPU6050_SDA_DDR0;
    tem = MPU6050_SDA_IN;                   //读应答信号
//    Delay5us(1000);
    MPU6050_SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
    return tem;
}
//**************************************
//向I2C总线发送一个字节数据
//**************************************
void I2C_SendByte(uint8 dat)
{
    int8 i;
    uint8 tem;
    for (i=7; i>=0; i--)         //8位计数器
    {
      tem=0x01 & (dat >> i) ;              //移出数据的最高位
        MPU6050_SDA_OUT = tem;               //送数据口
        Delay5us();
        MPU6050_SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        MPU6050_SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    I2C_RecvACK();
}
//**************************************
//从I2C总线接收一个字节数据
//**************************************
uint8 I2C_RecvByte()
{
    uint8 i;
    uint8 dat = 0;
    MPU6050_SDA_OUT = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        MPU6050_SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        MPU6050_SDA_DDR0;
        dat |= MPU6050_SDA_IN;             //读数据               
        MPU6050_SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    return dat;
}
//**************************************
//向I2C设备写入一个字节数据
//**************************************
void Single_WriteI2C(uint8 REG_Address,uint8 REG_data)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号    
    I2C_SendByte(REG_Address);    //内部寄存器地址，
    I2C_SendByte(REG_data);       //内部寄存器数据，
    I2C_Stop();                   //发送停止信号
}
//**************************************
//从I2C设备读取一个字节数据
//**************************************
uint8 Single_ReadI2C(uint8 REG_Address)
{
	uint8 REG_data;
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始	
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
	REG_data=I2C_RecvByte();       //读出寄存器数据
	I2C_SendACK(1);                //接收应答信号
	I2C_Stop();                    //停止信号
	return REG_data;
}
//**************************************
//初始化MPU6050
//**************************************
void InitMPU6050()
{
        gpio_init (M6050_SCL, GPO,1);
        gpio_init (M6050_SDA, GPO,1);
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
}
//void InitMPU6050()
//{
//        gpio_init (M6050_SCL, GPO,1);
//        gpio_init (M6050_SDA, GPO,1);
//	Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
//	Single_WriteI2C(SMPLRT_DIV, 0x07);
//	Single_WriteI2C(CONFIG, 0x06);
//	Single_WriteI2C(GYRO_CONFIG, 0x18);//不自检   ±500 °/s，
//	Single_WriteI2C(ACCEL_CONFIG, 0x01);
//}
/**************************************
合成数据
GetData(ACCEL_XOUT_H)	//显示X轴加速度
GetData(ACCEL_YOUT_H)	//显示Y轴加速度
GetData(ACCEL_ZOUT_H)	//显示Z轴加速度
GetData(GYRO_XOUT_H)	//显示X轴角速度
GetData(GYRO_YOUT_H)	//显示Y轴角速度
GetData(GYRO_ZOUT_H)	//显示Z轴角速度
**************************************/
int16 GetData(uint8 REG_Address)
{
	char H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return (H<<8)+L;   //合成数据
}

/***************************************************************************************/ 
//陀螺仪上电自检
void IIC_Gyroscope_self_inspection()
{
    int i;
    long int x=0,y=0,z=0;
    for(i=0;i<50;i++)
    {
      GYRO_XOUT_DATA = GetData(GYRO_XOUT_H);
      GYRO_YOUT_DATA = GetData(GYRO_YOUT_H);
      GYRO_ZOUT_DATA = GetData(GYRO_ZOUT_H);
      x = x + GYRO_XOUT_DATA;
      y = y + GYRO_YOUT_DATA;
      z = z + GYRO_ZOUT_DATA;
   }
   GYRO_XOUT_DATA0 = (int16)(1.0*x/50);
   GYRO_YOUT_DATA0 = (int16)(1.0*y/50);
   GYRO_ZOUT_DATA0 = (int16)(1.0*z/50);
}
/**MPU6050加速计值读取**/
void IIC_Accelerometer_Get()
{
    ACCEL_XOUT_DATA = GetData(ACCEL_XOUT_H);
    ACCEL_YOUT_DATA = GetData(ACCEL_YOUT_H);
    ACCEL_ZOUT_DATA = GetData(ACCEL_ZOUT_H);
}


/**MPU6050陀螺仪值读取**/
void IIC_Gyroscope_Get()
{
    GYRO_XOUT_DATA = GetData(GYRO_XOUT_H);
    GYRO_YOUT_DATA = GetData(GYRO_YOUT_H);
    GYRO_ZOUT_DATA = GetData(GYRO_ZOUT_H);
}
float angle_get(void)
{
    //data[0] = ad_sum(ADC0, AD12,  ADC_16bit, ADCOUT);//ccd1.CURRENT_ERROR
    //data[1] = ad_sum(ADC1, AD14,  ADC_16bit, ADCOUT);//doubleerror
    float z_data = ACCEL_ZOUT_DATA - ACCEL_ZOUT_DATA0;
    float x_data = ACCEL_XOUT_DATA - ACCEL_XOUT_DATA0;
    return(57.296*atan2(x_data,z_data));
}

// 6050X轴角加速度获取
int xangular_get(void)
{
    //int16 gyro_ad;
    int angular;
    //data[0] = ad_sum(ADC0, AD13,  ADC_16bit, ADCOUT);
    //gyro_ad =  GYRO_YOUT_DATA ;
   
    angular = (int)((GYRO_YOUT_DATA - GYRO_YOUT_DATA0 ) / 66);        
   
    return  (angular);
}
///*********************************************************** 
//函数名称：Kalman_filter()
//函数功能：卡尔曼滤波
//***********************************************************/
void Kalman_filter()
{
    
    float th_acc,w_gyro;
    th_acc = angle_get();		//加速度计测量的角度
    w_gyro = xangular_get();		//陀螺仪测量的角速度   


    /***********************卡尔曼滤波***********************/
    Xest1_1 = Xsta1_1 + A1_2*Xsta2_1 + B1_1*w_gyro;
    Xest2_1 = Xsta2_1;
    
    Pest1_1 = (Psta1_1 + A1_2*Psta2_1) + (Psta1_2 + A1_2*Psta2_2)*A1_2 + Q1_1;
    Pest1_2 =  Psta1_2 + A1_2*Psta2_2;
    Pest2_1 = Psta2_1 + Psta2_2*A1_2;
    Pest2_2 = Psta2_2 + Q2_2;

    K1_1 = Pest1_1/(Pest1_1 + R);
    K2_1 = Pest2_1/(Pest1_1 + R);

    Xsta1_1 = Xest1_1+K1_1*(th_acc - Xest1_1);        
    Xsta2_1 = Xest2_1+K2_1*(th_acc - Xest1_1);        

    Psta1_1 = (I1_1-K1_1) * Pest1_1;   
    Psta1_2 = (I1_1-K1_1) * Pest1_2;
    Psta2_1 =  -K2_1*Pest1_1 + Pest2_1; 
    Psta2_2 =  -K2_1*Pest1_2+  Pest2_2; 
    
    
    angle = Xsta1_1;		//滤波后的角度值
    omega = w_gyro - Xsta2_1;	//滤波后的角速度值
}

//void get_angle()
//{
//      th_acc = (float)GetData(ACCEL_XOUT_H);
//      w_gyro = (float)GetData(GYRO_XOUT_H);
//      Kalman_filter();
//      printf("angle=%d\n",(int16)angle);
//}

void check_Podao()
{
   ACCEL_ZOUT_DATA = GetData(ACCEL_ZOUT_H);
   ACCEL_XOUT_DATA = GetData(ACCEL_XOUT_H);
   GYRO_YOUT_DATA  = GetData(GYRO_YOUT_H);
   Kalman_filter();
//    LED_PrintsignValueI4(80,2,(int)angle); 
//   if(angle>11)
//     PODAO_STATE=PODAO_UP;
//   else if(angle<-11)
//     PODAO_STATE=PODAO_DOWN;
//   else PODAO_STATE=PODAO_NORMAL;
//    Current_Podao_state=PODAO_STATE;
//    if(Current_Podao_state==PODAO_NORMAL && Last_Podao_state==PODAO_UP)
//      Current_Podao_state=PODAO_ON;
//    else if(Current_Podao_state==PODAO_NORMAL && Last_Podao_state==PODAO_ON)
//      Current_Podao_state=PODAO_ON;
}
