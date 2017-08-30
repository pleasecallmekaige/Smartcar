#ifndef __MPU6050_H__
#define __MPU6050_H__

#define    M6050_SCL        PTD8
#define    M6050_SDA        PTD9
#define    MPU6050_SCL        PTD8_OUT			//IIC时钟引脚定义
#define    MPU6050_SDA_OUT    PTD9_DDR=1;PTD9_OUT                      //IIC数据引脚定义
#define    MPU6050_SDA_DDR0   PTD9_DDR=0
#define    MPU6050_SDA_IN     PTD9_IN

//****************************************
// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据，+1为读取


//MPU6050操作函数
void  InitMPU6050();													//初始化MPU6050
void  Delay5us();
void  I2C_Start();
void  I2C_Stop();
void  I2C_SendACK(uint8 ack);
uint8   I2C_RecvACK();
void  I2C_SendByte(uint8 dat);
uint8 I2C_RecvByte();
void  I2C_ReadPage();
void  I2C_WritePage();
void  display_ACCEL_x();
void  display_ACCEL_y();
void  display_ACCEL_z();
uint8 Single_ReadI2C(uint8 REG_Address);						//读取I2C数据
void  Single_WriteI2C(uint8 REG_Address,uint8 REG_data);	//向I2C写入数据
int16 GetData(uint8 REG_Address);
//void get_angle();//测量角度
void check_Podao();
void IIC_Gyroscope_self_inspection();
void IIC_Accelerometer_Get();
void IIC_Gyroscope_Get();

#endif //__VCAN_MPU6050_H__