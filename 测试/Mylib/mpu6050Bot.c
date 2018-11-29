#include "mpu6050.h"

mpu mpu6050Bot;
int16_t gyroz_tx,accz_tx;
//MPU6050初始化，传入参数：采样率，低通滤波频率
void Mpu6050Bot_Init(uint16_t sample_rate, uint16_t lpf)
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
	I2C2_Soft_Single_Write(MPU6050_ADDRESS,MPU_RA_PWR_MGMT_1, 0x80);	
	
	DelayMs(5);
	
	//陀螺仪采样率，0x00(1000Hz)   采样率 = 陀螺仪的输出率 / (1 + SMPLRT_DIV)
	I2C2_Soft_Single_Write(MPU6050_ADDRESS,MPU_RA_SMPLRT_DIV, (1000/sample_rate - 1));	
	//设置设备时钟源，陀螺仪Z轴
	I2C2_Soft_Single_Write(MPU6050_ADDRESS, MPU_RA_PWR_MGMT_1, 0x03);	
	//i2c旁路模式
	I2C2_Soft_Single_Write(MPU6050_ADDRESS, MPU_RA_INT_PIN_CFG, 0 << 7 | 0 << 6 | 0 << 5 | 0 << 4 | 0 << 3 | 0 << 2 | 1 << 1 | 0 << 0); 
	//INT_PIN_CFG   -- INT_LEVEL_HIGH, INT_OPEN_DIS, LATCH_INT_DIS, INT_RD_CLEAR_DIS, FSYNC_INT_LEVEL_HIGH, FSYNC_INT_DIS, I2C_BYPASS_EN, CLOCK_DIS
	//低通滤波频率，0x03(42Hz)
	I2C2_Soft_Single_Write(MPU6050_ADDRESS,MPU_RA_CONFIG, default_filter);	
	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	I2C2_Soft_Single_Write(MPU6050_ADDRESS, MPU_RA_GYRO_CONFIG, 0x18); 
	//加速计自检、测量范围(不自检，+-8G)			
	I2C2_Soft_Single_Write(MPU6050_ADDRESS,MPU_RA_ACCEL_CONFIG, 2 << 3);		
}

//读取加速度
void Mpu6050Bot_Read_Acc_Data(void)
{
	u8 dataBuffer[2];
	
	dataBuffer[0] = I2C2_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_XOUT_L); 
	dataBuffer[1] = I2C2_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_XOUT_H);
	mpu6050Bot.acc.origin.data[x] = ((((int16_t)dataBuffer[1]) << 8) | dataBuffer[0]) ;  //加速度X轴

	dataBuffer[0] = I2C2_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_YOUT_L);
	dataBuffer[1] = I2C2_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_YOUT_H);
	mpu6050Bot.acc.origin.data[y] = ((((int16_t)dataBuffer[1]) << 8) | dataBuffer[0]) ;  //加速度Y轴

	dataBuffer[0] = I2C2_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_ZOUT_L);
	dataBuffer[1] = I2C2_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_ZOUT_H);
	mpu6050Bot.acc.origin.data[z] = ((((int16_t)dataBuffer[1]) << 8) | dataBuffer[0]) ;  //加速度Z轴
}

//读取角速度
void Mpu6050Bot_Read_Gyro_Data(void)
{
	u8 dataBuffer[2];
	
	dataBuffer[0] = I2C2_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_XOUT_L); 
	dataBuffer[1] = I2C2_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_XOUT_H);
	mpu6050Bot.gyro.origin.data[x] = ((((int16_t)dataBuffer[1]) << 8) | dataBuffer[0]) ;	 //陀螺仪X轴

	dataBuffer[0] = I2C2_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_YOUT_L);
	dataBuffer[1] = I2C2_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_YOUT_H);
	mpu6050Bot.gyro.origin.data[y] = ((((int16_t)dataBuffer[1]) << 8) | dataBuffer[0]) ;	 //陀螺仪Y轴

	dataBuffer[0] = I2C2_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_ZOUT_L);
	dataBuffer[1] = I2C2_Soft_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_ZOUT_H);
	mpu6050Bot.gyro.origin.data[z] = ((((int16_t)dataBuffer[1]) << 8) | dataBuffer[0]) ;  //陀螺仪Z轴		
}

//mpu6050读取数据
void Mpu6050Bot_Read(void)
{
	I2C2_FastMode = 1;
	Mpu6050Bot_Read_Acc_Data();
	Mpu6050Bot_Read_Gyro_Data();
}

//加速度零偏矫正
void Mpu6050Bot_CalOffset_Acc(void)
{
	mpu6050Bot.acc.offset.data[x] =256;
	mpu6050Bot.acc.offset.data[y] =20;
	mpu6050Bot.acc.offset.data[z] =3720;
}

//陀螺仪零偏矫正
void Mpu6050Bot_CalOffset_Gyro(void)
{
	uint16_t cnt_g =1000;
	int32_t tempgx =0, tempgy =0, tempgz =0;

//	mpu6050Bot.gyro.offset_max.data[x] = -32768;
//	mpu6050Bot.gyro.offset_max.data[y] = -32768;
//	mpu6050Bot.gyro.offset_max.data[z] = -32768;
//	mpu6050Bot.gyro.offset_min.data[x] = 32767;
//	mpu6050Bot.gyro.offset_min.data[y] = 32767;
//	mpu6050Bot.gyro.offset_min.data[z] = 32767;

//	while(cnt_g--)	 
//	{
//		DelayMs(2);			
//		Mpu6050Bot_Read();
//		if(mpu6050Bot.gyro.origin.data[x]>mpu6050Bot.gyro.offset_max.data[x]) 
//			mpu6050Bot.gyro.offset_max.data[x] = mpu6050Bot.gyro.origin.data[x];
//		if(mpu6050Bot.gyro.origin.data[y]>mpu6050Bot.gyro.offset_max.data[y]) 
//			mpu6050Bot.gyro.offset_max.data[y] = mpu6050Bot.gyro.origin.data[y];
//		if(mpu6050Bot.gyro.origin.data[z]>mpu6050Bot.gyro.offset_max.data[z]) 
//			mpu6050Bot.gyro.offset_max.data[z] = mpu6050Bot.gyro.origin.data[z];
//		
//		if(mpu6050Bot.gyro.origin.data[x]<mpu6050Bot.gyro.offset_min.data[x]) 
//			mpu6050Bot.gyro.offset_min.data[x] = mpu6050Bot.gyro.origin.data[x];
//		if(mpu6050Bot.gyro.origin.data[y]<mpu6050Bot.gyro.offset_min.data[y]) 
//			mpu6050Bot.gyro.offset_min.data[y] = mpu6050Bot.gyro.origin.data[y];
//		if(mpu6050Bot.gyro.origin.data[z]<mpu6050Bot.gyro.offset_min.data[z]) 
//			mpu6050Bot.gyro.offset_min.data[z] = mpu6050Bot.gyro.origin.data[z];
//		
//			tempgx += mpu6050Bot.gyro.origin.data[x];
//			tempgy += mpu6050Bot.gyro.origin.data[y];
//			tempgz += mpu6050Bot.gyro.origin.data[z];
//	}
//	 
//	 //1000次数据有一个异常,重新校准
//	 if(mpu6050Bot.gyro.offset_max.data[x]-mpu6050Bot.gyro.offset_min.data[x]>20||
//			mpu6050Bot.gyro.offset_max.data[y]-mpu6050Bot.gyro.offset_min.data[y]>20||
//			mpu6050Bot.gyro.offset_max.data[z]-mpu6050Bot.gyro.offset_min.data[z]>20)
//			Mpu6050Bot_CalOffset_Gyro();	 
//	 else
//	 {
//			mpu6050Bot.gyro.offset.data[x] = (float)(tempgx)/1000;//-87.10
//			mpu6050Bot.gyro.offset.data[y] = (float)(tempgy)/1000;//39.60
//			mpu6050Bot.gyro.offset.data[z] = (float)(tempgz)/1000;//27.50
//	 }
		mpu6050Bot.gyro.offset.data[x] =-79.0650024;// -91.6449966;//-128.712997;//-87.10
		mpu6050Bot.gyro.offset.data[y] = 20.6779995;//22.2159996;//25.8969994;//39.60
		mpu6050Bot.gyro.offset.data[z] = -0.8849999;//-2.8989993;//-4.5320006 ;//27.50 
}

//mpu6050数据准备
void Mpu6050Bot_Data_Prepare(void)
{	
	u8 i;
	int32_t FILT_TMP[ITEMS] = {0};
  static int16_t FILT_BUF[ITEMS][MPU6050_FILTER_NUM] = {0};

	/* 得出校准后的数据 */
	mpu6050Bot.acc.calibration.data[x] = mpu6050Bot.acc.origin.data[x]  - mpu6050Bot.acc.offset.data[x] ;
	mpu6050Bot.acc.calibration.data[y] = mpu6050Bot.acc.origin.data[y]  - mpu6050Bot.acc.offset.data[y] ;
	mpu6050Bot.acc.calibration.data[z] = mpu6050Bot.acc.origin.data[z]  - mpu6050Bot.acc.offset.data[z] ;
	mpu6050Bot.gyro.calibration.data[x] = mpu6050Bot.gyro.origin.data[x] - mpu6050Bot.gyro.offset.data[x] ;
	mpu6050Bot.gyro.calibration.data[y] = mpu6050Bot.gyro.origin.data[y] - mpu6050Bot.gyro.offset.data[y] ;
	mpu6050Bot.gyro.calibration.data[z] = mpu6050Bot.gyro.origin.data[z] - mpu6050Bot.gyro.offset.data[z] ;
	
  for(i=MPU6050_FILTER_NUM-1;i>=1;i--)
  {
    FILT_BUF[A_X][i] = FILT_BUF[A_X][i-1];
		FILT_BUF[A_Y][i] = FILT_BUF[A_Y][i-1];
		FILT_BUF[A_Z][i] = FILT_BUF[A_Z][i-1];
		FILT_BUF[G_X][i] = FILT_BUF[G_X][i-1];
		FILT_BUF[G_Y][i] = FILT_BUF[G_Y][i-1];
		FILT_BUF[G_Z][i] = FILT_BUF[G_Z][i-1];
  }

	  FILT_BUF[A_X][0] = mpu6050Bot.acc.calibration.data[x];
		FILT_BUF[A_Y][0] = mpu6050Bot.acc.calibration.data[y];
		FILT_BUF[A_Z][0] = mpu6050Bot.acc.calibration.data[z];
		FILT_BUF[G_X][0] = mpu6050Bot.gyro.calibration.data[x];
		FILT_BUF[G_Y][0] = mpu6050Bot.gyro.calibration.data[y];
		FILT_BUF[G_Z][0] = mpu6050Bot.gyro.calibration.data[z];

	for(i=0;i<MPU6050_FILTER_NUM;i++)
	{
		FILT_TMP[A_X] += FILT_BUF[A_X][i];
		FILT_TMP[A_Y] += FILT_BUF[A_Y][i];
		FILT_TMP[A_Z] += FILT_BUF[A_Z][i];
		FILT_TMP[G_X] += FILT_BUF[G_X][i];
		FILT_TMP[G_Y] += FILT_BUF[G_Y][i];
		FILT_TMP[G_Z] += FILT_BUF[G_Z][i];
	}

	mpu6050Bot.acc.filter.data[x] = (float)( FILT_TMP[A_X] )/(float)MPU6050_FILTER_NUM;
	mpu6050Bot.acc.filter.data[y] = (float)( FILT_TMP[A_Y] )/(float)MPU6050_FILTER_NUM;
	mpu6050Bot.acc.filter.data[z] = (float)( FILT_TMP[A_Z] )/(float)MPU6050_FILTER_NUM;

	mpu6050Bot.gyro.filter.data[x] = (float)( FILT_TMP[G_X] )/(float)MPU6050_FILTER_NUM;
	mpu6050Bot.gyro.filter.data[y] = (float)( FILT_TMP[G_Y] )/(float)MPU6050_FILTER_NUM;
	mpu6050Bot.gyro.filter.data[z] = (float)( FILT_TMP[G_Z] )/(float)MPU6050_FILTER_NUM;
	
	mpu6050Bot.gyro.dps.data[x] = mpu6050Bot.gyro.filter.data[x] * TO_ANGLE;
	mpu6050Bot.gyro.dps.data[y] = mpu6050Bot.gyro.filter.data[y] * TO_ANGLE;
	mpu6050Bot.gyro.dps.data[z] = mpu6050Bot.gyro.filter.data[z] * TO_ANGLE;
	
	mpu6050Bot.gyro.radps.data[x] = mpu6050Bot.gyro.dps.data[x] * ANGLE_TO_RAD;
	mpu6050Bot.gyro.radps.data[y] = mpu6050Bot.gyro.dps.data[y] * ANGLE_TO_RAD;
	mpu6050Bot.gyro.radps.data[z] = mpu6050Bot.gyro.dps.data[z] * ANGLE_TO_RAD;
	
	/*gyroz_tx = mpu6050Bot.gyro.filter.data[z] - 12.8;
	gyroz_tx = my_deathzoom(gyroz_tx,50);
	accz_tx  = mpu6050Bot.acc.calibration.data[z];*/
}
