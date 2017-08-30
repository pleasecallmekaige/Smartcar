#include "include.h"


//���ٶȱ���
int16 ACCEL_XOUT_DATA = 0;
int16 ACCEL_YOUT_DATA = 0;
int16 ACCEL_ZOUT_DATA = 0;
//�����Ǳ���
int16 GYRO_XOUT_DATA = 0;
int16 GYRO_YOUT_DATA = 0;
int16 GYRO_ZOUT_DATA = 0;
/*************
6050���ټ���ƫ
************/
#define   ACCEL_XOUT_DATA0     250    //
#define   ACCEL_YOUT_DATA0     250    //
#define   ACCEL_ZOUT_DATA0     430    //
/*************
6050��������ƫ
************/
int16 GYRO_XOUT_DATA0 = 0 ;
int16 GYRO_YOUT_DATA0 = 0 ;
int16 GYRO_ZOUT_DATA0 = 0 ;

float angle=0,omega=0;
//float th_acc=0,w_gyro=0;
#define   Kalman_time   0.005 //�������ʱ��5����
//״̬�任��
float A1_1 = 1;
float A1_2 = -1*Kalman_time;
float A2_1 = 0;
float A2_2 = 1;
float B1_1 = Kalman_time;
float B2_1 = 0;
float H1_1 = 1;
float H1_2 = 0;
//ϵ����  
float Pest1_1;          //�������Э������ 
float Pest1_2;
float Pest2_1;
float Pest2_2;    
float Psta1_1 = 1;      //�������Э������
float Psta1_2 = 1;  
float Psta2_1 = 1;  
float Psta2_2 = 1;   
float Q1_1 = 0.00001;   //���̼�������Э���0.00001����
float Q1_2 = 0;  
float Q2_1 = 0;  
float Q2_2 = 0.00001;   //0.00001����
float K1_1;             //����������
float K2_1;    
float R = 0.005;          //�۲�����Э�����Ҫ���˲���
float I1_1 = 1;         //��λ��
float I1_2 = 0;  
float I2_1 = 0;  
float I2_2 = 1;   
//״̬��
float Xest1_1;          //����״̬����
float Xest2_1;          
float Xsta1_1 = 0;      //����״̬����
float Xsta2_1 = 0;
void Delay5us()
{
//   DELAY_US(5);
  systick_delay_ns(200);
}

//**************************************
//I2C��ʼ�ź�
//**************************************
void I2C_Start()
{
    Delay5us();
    MPU6050_SDA_OUT = 1;                    //����������
    Delay5us();
    MPU6050_SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    MPU6050_SDA_OUT = 0;                    //�����½���
    Delay5us();                 //��ʱ
    MPU6050_SCL = 0;                    //����ʱ����
    Delay5us();
}
//**************************************
//I2Cֹͣ�ź�
//**************************************
void I2C_Stop()
{
    MPU6050_SDA_OUT = 0;                    //����������
    Delay5us();
    MPU6050_SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    MPU6050_SDA_OUT = 1;                    //����������
    Delay5us();                 //��ʱ
}
//**************************************
//I2C����Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(uint8 ack)
{
    MPU6050_SDA_OUT = ack;                  //дӦ���ź�
    Delay5us();
    MPU6050_SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    MPU6050_SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
}
//**************************************
//I2C����Ӧ���ź�
//**************************************
uint8 I2C_RecvACK()
{
    uint8 tem;
    MPU6050_SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    MPU6050_SDA_DDR0;
    tem = MPU6050_SDA_IN;                   //��Ӧ���ź�
//    Delay5us(1000);
    MPU6050_SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
    return tem;
}
//**************************************
//��I2C���߷���һ���ֽ�����
//**************************************
void I2C_SendByte(uint8 dat)
{
    int8 i;
    uint8 tem;
    for (i=7; i>=0; i--)         //8λ������
    {
      tem=0x01 & (dat >> i) ;              //�Ƴ����ݵ����λ
        MPU6050_SDA_OUT = tem;               //�����ݿ�
        Delay5us();
        MPU6050_SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        MPU6050_SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    I2C_RecvACK();
}
//**************************************
//��I2C���߽���һ���ֽ�����
//**************************************
uint8 I2C_RecvByte()
{
    uint8 i;
    uint8 dat = 0;
    MPU6050_SDA_OUT = 1;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        MPU6050_SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        MPU6050_SDA_DDR0;
        dat |= MPU6050_SDA_IN;             //������               
        MPU6050_SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    return dat;
}
//**************************************
//��I2C�豸д��һ���ֽ�����
//**************************************
void Single_WriteI2C(uint8 REG_Address,uint8 REG_data)
{
    I2C_Start();                  //��ʼ�ź�
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�    
    I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
    I2C_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
    I2C_Stop();                   //����ֹͣ�ź�
}
//**************************************
//��I2C�豸��ȡһ���ֽ�����
//**************************************
uint8 Single_ReadI2C(uint8 REG_Address)
{
	uint8 REG_data;
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ	
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress+1);  //�����豸��ַ+���ź�
	REG_data=I2C_RecvByte();       //�����Ĵ�������
	I2C_SendACK(1);                //����Ӧ���ź�
	I2C_Stop();                    //ֹͣ�ź�
	return REG_data;
}
//**************************************
//��ʼ��MPU6050
//**************************************
void InitMPU6050()
{
        gpio_init (M6050_SCL, GPO,1);
        gpio_init (M6050_SDA, GPO,1);
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//�������״̬
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
}
//void InitMPU6050()
//{
//        gpio_init (M6050_SCL, GPO,1);
//        gpio_init (M6050_SDA, GPO,1);
//	Single_WriteI2C(PWR_MGMT_1, 0x00);	//�������״̬
//	Single_WriteI2C(SMPLRT_DIV, 0x07);
//	Single_WriteI2C(CONFIG, 0x06);
//	Single_WriteI2C(GYRO_CONFIG, 0x18);//���Լ�   ��500 ��/s��
//	Single_WriteI2C(ACCEL_CONFIG, 0x01);
//}
/**************************************
�ϳ�����
GetData(ACCEL_XOUT_H)	//��ʾX����ٶ�
GetData(ACCEL_YOUT_H)	//��ʾY����ٶ�
GetData(ACCEL_ZOUT_H)	//��ʾZ����ٶ�
GetData(GYRO_XOUT_H)	//��ʾX����ٶ�
GetData(GYRO_YOUT_H)	//��ʾY����ٶ�
GetData(GYRO_ZOUT_H)	//��ʾZ����ٶ�
**************************************/
int16 GetData(uint8 REG_Address)
{
	char H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return (H<<8)+L;   //�ϳ�����
}

/***************************************************************************************/ 
//�������ϵ��Լ�
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
/**MPU6050���ټ�ֵ��ȡ**/
void IIC_Accelerometer_Get()
{
    ACCEL_XOUT_DATA = GetData(ACCEL_XOUT_H);
    ACCEL_YOUT_DATA = GetData(ACCEL_YOUT_H);
    ACCEL_ZOUT_DATA = GetData(ACCEL_ZOUT_H);
}


/**MPU6050������ֵ��ȡ**/
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

// 6050X��Ǽ��ٶȻ�ȡ
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
//�������ƣ�Kalman_filter()
//�������ܣ��������˲�
//***********************************************************/
void Kalman_filter()
{
    
    float th_acc,w_gyro;
    th_acc = angle_get();		//���ٶȼƲ����ĽǶ�
    w_gyro = xangular_get();		//�����ǲ����Ľ��ٶ�   


    /***********************�������˲�***********************/
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
    
    
    angle = Xsta1_1;		//�˲���ĽǶ�ֵ
    omega = w_gyro - Xsta2_1;	//�˲���Ľ��ٶ�ֵ
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
