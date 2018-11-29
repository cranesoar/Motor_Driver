#include "mpu6050.h"

struct _sensor sensor;


//MPU6050初始化，传入参数：采样率，低通滤波频率
void Mpu6050_Init(uint16_t sample_rate, uint16_t lpf)
{
	uint8_t default_filter;
	
	switch (lpf) {
	case 5:
			default_filter = MPU6050_LPF_5HZ;
			break;
	case 10:
			default_filter = MPU6050_LPF_10HZ;
			break;
	case 20:
			default_filter = MPU6050_LPF_20HZ;
			break;
	case 42:
			default_filter = MPU6050_LPF_42HZ;
			break;
	case 98:
			default_filter = MPU6050_LPF_98HZ;
			break;
	case 188:
			default_filter = MPU6050_LPF_188HZ;
			break;
	case 256:
			default_filter = MPU6050_LPF_256HZ;
			break;
	default:
			default_filter = MPU6050_LPF_98HZ;
			break;
	}	
	
	//设备复位
	I2C_Soft_Single_Write(MPU6050_ADDRESS,MPU_RA_PWR_MGMT_1, 0x80);	
	
	DelayMs(5);
	
	//陀螺仪采样率，0x00(1000Hz)   采样率 = 陀螺仪的输出率 / (1 + SMPLRT_DIV)
	I2C_Soft_Single_Write(MPU6050_ADDRESS,MPU_RA_SMPLRT_DIV, (1000/sample_rate - 1));	
	//设置设备时钟源，陀螺仪Z轴
	I2C_Soft_Single_Write(MPU6050_ADDRESS, MPU_RA_PWR_MGMT_1, 0x03);	
	//i2c旁路模式
	I2C_Soft_Single_Write(MPU6050_ADDRESS, MPU_RA_INT_PIN_CFG, 0 << 7 | 0 << 6 | 0 << 5 | 0 << 4 | 0 << 3 | 0 << 2 | 1 << 1 | 0 << 0); 
	//INT_PIN_CFG   -- INT_LEVEL_HIGH, INT_OPEN_DIS, LATCH_INT_DIS, INT_RD_CLEAR_DIS, FSYNC_INT_LEVEL_HIGH, FSYNC_INT_DIS, I2C_BYPASS_EN, CLOCK_DIS
	//低通滤波频率，0x03(42Hz)
	I2C_Soft_Single_Write(MPU6050_ADDRESS,MPU_RA_CONFIG, default_filter);	
	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	I2C_Soft_Single_Write(MPU6050_ADDRESS, MPU_RA_GYRO_CONFIG, 0x18); 
	//加速计自检、测量范围(不自检，+-8G)			
	I2C_Soft_Single_Write(MPU6050_ADDRESS,MPU_RA_ACCEL_CONFIG, 2 << 3);	
	
}

//读取加速度
void Mpu6050_Read_Acc_Data(void)
{
	u8 dataBuffer[2];
	dataBuffer[0] = I2C_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_XOUT_L); 
	dataBuffer[1] = I2C_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_XOUT_H);
	sensor.acc.origin.x = ((((int16_t)dataBuffer[1]) << 8) | dataBuffer[0]) ;  //加速度X轴

	dataBuffer[0] = I2C_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_YOUT_L);
	dataBuffer[1] = I2C_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_YOUT_H);
	sensor.acc.origin.y = ((((int16_t)dataBuffer[1]) << 8) | dataBuffer[0]) ;  //加速度Y轴

	dataBuffer[0] = I2C_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_ZOUT_L);
	dataBuffer[1] = I2C_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_ZOUT_H);
	sensor.acc.origin.z = ((((int16_t)dataBuffer[1]) << 8) | dataBuffer[0]) ;  //加速度Z轴
}

//读取角速度
void Mpu6050_Read_Gyro_Data(void)
{
	u8 dataBuffer[2];
	dataBuffer[0] = I2C_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_XOUT_L); 
	dataBuffer[1] = I2C_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_XOUT_H);
	sensor.gyro.origin.x = ((((int16_t)dataBuffer[1]) << 8) | dataBuffer[0]) ;	 //陀螺仪X轴

	dataBuffer[0] = I2C_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_YOUT_L);
	dataBuffer[1] = I2C_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_YOUT_H);
	sensor.gyro.origin.y = ((((int16_t)dataBuffer[1]) << 8) | dataBuffer[0]) ;	 //陀螺仪Y轴

	dataBuffer[0] = I2C_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_ZOUT_L);
	dataBuffer[1] = I2C_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_ZOUT_H);
	sensor.gyro.origin.z = ((((int16_t)dataBuffer[1]) << 8) | dataBuffer[0]) ; //陀螺仪Z轴		
}

void Mpu6050_Read(void)
{
	I2C_FastMode = 1;
	Mpu6050_Read_Acc_Data();
	Mpu6050_Read_Gyro_Data();
}

//加速度零偏矫正
void Mpu6050_CalOffset_Acc(void)
{
	sensor.acc.offset.x =100;//前-后+
	sensor.acc.offset.y =-32;//左-右+
	sensor.acc.offset.z =0;
}

//陀螺仪零偏矫正
void Mpu6050_CalOffset_Gyro(void)
{
	uint16_t cnt_g =1000;
	int32_t tempgx =0, tempgy =0, tempgz =0;

	sensor.gyro.offset_max.x = -32768;
	sensor.gyro.offset_max.y = -32768;
	sensor.gyro.offset_max.z = -32768;
	sensor.gyro.offset_min.x = 32767;
	sensor.gyro.offset_min.x = 32767;
	sensor.gyro.offset_min.x = 32767;

	while(cnt_g--)	 
	{
		DelayMs(2);			
		Mpu6050_Read();
			if(sensor.gyro.origin.x>sensor.gyro.offset_max.x) 
				sensor.gyro.offset_max.x = sensor.gyro.origin.x;
			if(sensor.gyro.origin.y>sensor.gyro.offset_max.y)  			
				sensor.gyro.offset_max.y = sensor.gyro.origin.y;
 			if(sensor.gyro.origin.z>sensor.gyro.offset_max.z) 
				sensor.gyro.offset_max.z = sensor.gyro.origin.z;
 			
 			if(sensor.gyro.origin.x<sensor.gyro.offset_min.x) 
 				sensor.gyro.offset_min.x = sensor.gyro.origin.x;
 			if(sensor.gyro.origin.y<sensor.gyro.offset_min.y) 
 				sensor.gyro.offset_min.y = sensor.gyro.origin.y;
 			if(sensor.gyro.origin.z<sensor.gyro.offset_min.z) 
 				sensor.gyro.offset_min.z = sensor.gyro.origin.z;
		
			tempgx += sensor.gyro.origin.x;
			tempgy += sensor.gyro.origin.x;
			tempgz += sensor.gyro.origin.x;
	}
	 
	 //1000次数据有一个异常,重新校准
 		 if(sensor.gyro.offset_max.x-sensor.gyro.offset_min.x>20||
 			  sensor.gyro.offset_max.y-sensor.gyro.offset_min.y>20||
 		    sensor.gyro.offset_max.z-sensor.gyro.offset_min.z>20)
 				Mpu6050_CalOffset_Gyro();	 	 
	 else
	 {
			sensor.gyro.offset.x = (float)(tempgx)/1000;
			sensor.gyro.offset.x = (float)(tempgy)/1000;
			sensor.gyro.offset.x = (float)(tempgz)/1000;
	 }
}

//mpu6050数据准备
void Mpu6050_Data_Prepare(void)
{	
	u8 i;
	int32_t FILT_TMP[ITEMS] = {0};
  static int16_t FILT_BUF[ITEMS][MPU6050_FILTER_NUM] = {0};

	/* 得出校准后的数据 */
	sensor.acc.calibration.x = sensor.acc.origin.x  - sensor.acc.offset.x ;
	sensor.acc.calibration.y = sensor.acc.origin.y  - sensor.acc.offset.y ;
	sensor.acc.calibration.z = sensor.acc.origin.z  - sensor.acc.offset.z ;
	sensor.gyro.calibration.x = sensor.gyro.origin.x - sensor.gyro.offset.x ;
	sensor.gyro.calibration.y = sensor.gyro.origin.y - sensor.gyro.offset.y ;
	sensor.gyro.calibration.z = sensor.gyro.origin.z - sensor.gyro.offset.z ;
	
  for(i=MPU6050_FILTER_NUM-1;i>=1;i--)
  {
    FILT_BUF[A_X][i] = FILT_BUF[A_X][i-1];
		FILT_BUF[A_Y][i] = FILT_BUF[A_Y][i-1];
		FILT_BUF[A_Z][i] = FILT_BUF[A_Z][i-1];
		FILT_BUF[G_X][i] = FILT_BUF[G_X][i-1];
		FILT_BUF[G_Y][i] = FILT_BUF[G_Y][i-1];
		FILT_BUF[G_Z][i] = FILT_BUF[G_Z][i-1];
  }

	  FILT_BUF[A_X][0] = sensor.acc.calibration.x;
		FILT_BUF[A_Y][0] = sensor.acc.calibration.y;
		FILT_BUF[A_Z][0] = sensor.acc.calibration.z;
		FILT_BUF[G_X][0] = sensor.gyro.calibration.x;
		FILT_BUF[G_Y][0] = sensor.gyro.calibration.y;
		FILT_BUF[G_Z][0] = sensor.gyro.calibration.z;

	for(i=0;i<MPU6050_FILTER_NUM;i++)
	{
		FILT_TMP[A_X] += FILT_BUF[A_X][i];
		FILT_TMP[A_Y] += FILT_BUF[A_Y][i];
		FILT_TMP[A_Z] += FILT_BUF[A_Z][i];
		FILT_TMP[G_X] += FILT_BUF[G_X][i];
		FILT_TMP[G_Y] += FILT_BUF[G_Y][i];
		FILT_TMP[G_Z] += FILT_BUF[G_Z][i];
	}


	sensor.acc.filter.x = (float)( FILT_TMP[A_X] )/(float)MPU6050_FILTER_NUM;
	sensor.acc.filter.y = (float)( FILT_TMP[A_Y] )/(float)MPU6050_FILTER_NUM;
	sensor.acc.filter.z = (float)( FILT_TMP[A_Z] )/(float)MPU6050_FILTER_NUM;

	sensor.gyro.filter.x = (float)( FILT_TMP[G_X] )/(float)MPU6050_FILTER_NUM;
	sensor.gyro.filter.y = (float)( FILT_TMP[G_Y] )/(float)MPU6050_FILTER_NUM;
	sensor.gyro.filter.z = (float)( FILT_TMP[G_Z] )/(float)MPU6050_FILTER_NUM;
	
	sensor.gyro.dps.x = sensor.gyro.filter.x * TO_ANGLE;
	sensor.gyro.dps.y = sensor.gyro.filter.y * TO_ANGLE;
	sensor.gyro.dps.z = sensor.gyro.filter.z * TO_ANGLE;
	
	sensor.gyro.radps.x = sensor.gyro.dps.x * ANGLE_TO_RAD;
	sensor.gyro.radps.y = sensor.gyro.dps.y * ANGLE_TO_RAD;
	sensor.gyro.radps.z = sensor.gyro.dps.z * ANGLE_TO_RAD;
	
}
