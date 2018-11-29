#ifndef _MPU6050_H
#define	_MPU6050_H

#include "board.h"

#define ANGLE_TO_RAD 0.01745329f

#define TO_ANGLE     0.061036f

#define MPU6050_FILTER_NUM   10

#define A_X    0
#define A_Y    1
#define A_Z    2
#define G_X    3
#define G_Y    4
#define G_Z    5			

#define x 0
#define y 1
#define z 2

#define ITEMS  6

// MPU6050, 硬件I2c地址 0x68，模拟i2c地址0xD0
#define MPU6050_ADDRESS           0xD0	  // 0x68
#define DMP_MEM_START_ADDR        0x6E
#define DMP_MEM_R_W               0x6F

#define MPU_RA_XG_OFFS_TC         0x00    //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU_RA_YG_OFFS_TC         0x01    //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU_RA_ZG_OFFS_TC         0x02    //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU_RA_X_FINE_GAIN        0x03    //[7:0] X_FINE_GAIN
#define MPU_RA_Y_FINE_GAIN        0x04    //[7:0] Y_FINE_GAIN
#define MPU_RA_Z_FINE_GAIN        0x05    //[7:0] Z_FINE_GAIN
#define MPU_RA_XA_OFFS_H          0x06    //[15:0] XA_OFFS
#define MPU_RA_XA_OFFS_L_TC       0x07
#define MPU_RA_YA_OFFS_H          0x08    //[15:0] YA_OFFS
#define MPU_RA_YA_OFFS_L_TC       0x09
#define MPU_RA_ZA_OFFS_H          0x0A    //[15:0] ZA_OFFS
#define MPU_RA_ZA_OFFS_L_TC       0x0B
#define MPU_RA_PRODUCT_ID         0x0C    // Product ID Register
#define MPU_RA_XG_OFFS_USRH       0x13    //[15:0] XG_OFFS_USR
#define MPU_RA_XG_OFFS_USRL       0x14
#define MPU_RA_YG_OFFS_USRH       0x15    //[15:0] YG_OFFS_USR
#define MPU_RA_YG_OFFS_USRL       0x16
#define MPU_RA_ZG_OFFS_USRH       0x17    //[15:0] ZG_OFFS_USR
#define MPU_RA_ZG_OFFS_USRL       0x18
#define MPU_RA_SMPLRT_DIV         0x19
#define MPU_RA_CONFIG             0x1A
#define MPU_RA_GYRO_CONFIG        0x1B
#define MPU_RA_ACCEL_CONFIG       0x1C
#define MPU_RA_FF_THR             0x1D
#define MPU_RA_FF_DUR             0x1E
#define MPU_RA_MOT_THR            0x1F
#define MPU_RA_MOT_DUR            0x20
#define MPU_RA_ZRMOT_THR          0x21
#define MPU_RA_ZRMOT_DUR          0x22
#define MPU_RA_FIFO_EN            0x23
#define MPU_RA_I2C_MST_CTRL       0x24
#define MPU_RA_I2C_SLV0_ADDR      0x25
#define MPU_RA_I2C_SLV0_REG       0x26
#define MPU_RA_I2C_SLV0_CTRL      0x27
#define MPU_RA_I2C_SLV1_ADDR      0x28
#define MPU_RA_I2C_SLV1_REG       0x29
#define MPU_RA_I2C_SLV1_CTRL      0x2A
#define MPU_RA_I2C_SLV2_ADDR      0x2B
#define MPU_RA_I2C_SLV2_REG       0x2C
#define MPU_RA_I2C_SLV2_CTRL      0x2D
#define MPU_RA_I2C_SLV3_ADDR      0x2E
#define MPU_RA_I2C_SLV3_REG       0x2F
#define MPU_RA_I2C_SLV3_CTRL      0x30
#define MPU_RA_I2C_SLV4_ADDR      0x31
#define MPU_RA_I2C_SLV4_REG       0x32
#define MPU_RA_I2C_SLV4_DO        0x33
#define MPU_RA_I2C_SLV4_CTRL      0x34
#define MPU_RA_I2C_SLV4_DI        0x35
#define MPU_RA_I2C_MST_STATUS     0x36
#define MPU_RA_INT_PIN_CFG        0x37
#define MPU_RA_INT_ENABLE         0x38
#define MPU_RA_DMP_INT_STATUS     0x39
#define MPU_RA_INT_STATUS         0x3A
#define MPU_RA_ACCEL_XOUT_H       0x3B
#define MPU_RA_ACCEL_XOUT_L       0x3C
#define MPU_RA_ACCEL_YOUT_H       0x3D
#define MPU_RA_ACCEL_YOUT_L       0x3E
#define MPU_RA_ACCEL_ZOUT_H       0x3F
#define MPU_RA_ACCEL_ZOUT_L       0x40
#define MPU_RA_TEMP_OUT_H         0x41
#define MPU_RA_TEMP_OUT_L         0x42
#define MPU_RA_GYRO_XOUT_H        0x43
#define MPU_RA_GYRO_XOUT_L        0x44
#define MPU_RA_GYRO_YOUT_H        0x45
#define MPU_RA_GYRO_YOUT_L        0x46
#define MPU_RA_GYRO_ZOUT_H        0x47
#define MPU_RA_GYRO_ZOUT_L        0x48
#define MPU_RA_EXT_SENS_DATA_00   0x49
#define MPU_RA_MOT_DETECT_STATUS  0x61
#define MPU_RA_I2C_SLV0_DO        0x63
#define MPU_RA_I2C_SLV1_DO        0x64
#define MPU_RA_I2C_SLV2_DO        0x65
#define MPU_RA_I2C_SLV3_DO        0x66
#define MPU_RA_I2C_MST_DELAY_CTRL 0x67
#define MPU_RA_SIGNAL_PATH_RESET  0x68
#define MPU_RA_MOT_DETECT_CTRL    0x69
#define MPU_RA_USER_CTRL          0x6A
#define MPU_RA_PWR_MGMT_1         0x6B
#define MPU_RA_PWR_MGMT_2         0x6C
#define MPU_RA_BANK_SEL           0x6D
#define MPU_RA_MEM_START_ADDR     0x6E
#define MPU_RA_MEM_R_W            0x6F
#define MPU_RA_DMP_CFG_1          0x70
#define MPU_RA_DMP_CFG_2          0x71
#define MPU_RA_FIFO_COUNTH        0x72
#define MPU_RA_FIFO_COUNTL        0x73
#define MPU_RA_FIFO_R_W           0x74
#define MPU_RA_WHO_AM_I           0x75

#define MPU6050_SMPLRT_DIV        0       // 8000Hz

#define MPU6050_LPF_256HZ         0
#define MPU6050_LPF_188HZ         1
#define MPU6050_LPF_98HZ          2
#define MPU6050_LPF_42HZ          3
#define MPU6050_LPF_20HZ          4
#define MPU6050_LPF_10HZ          5
#define MPU6050_LPF_5HZ           6

#define MPU6050A_2mg              ((float)0.00006103f)  // 0.00006250 g/LSB
#define MPU6050A_4mg              ((float)0.00012207f)  // 0.00012500 g/LSB
#define MPU6050A_8mg              ((float)0.00024414f)  // 0.00025000 g/LSB

#define MPU6050G_s250dps          ((float)0.0076335f)  // 0.0087500 dps/LSB
#define MPU6050G_s500dps          ((float)0.0152671f)  // 0.0175000 dps/LSB
#define MPU6050G_s2000dps         ((float)0.0609756f)  // 0.0700000 dps/LSB
	

typedef struct _accdata{
												vec3int16 origin;  //原始值
											  vec3f offset_max;  //零偏值最大值
												vec3f offset_min;  //零偏值最小值	
	                      vec3f offset;      //零偏值 
												vec3f calibration; //校准值
												vec3f filter;      //滑动平均滤波值
											}accdata;

typedef struct _gyrodata{
												vec3int16 origin;  //原始值
												vec3f offset_max;  //零偏值最大值
												vec3f offset_min;  //零偏值最小值	
												vec3f offset;      //零偏值 
												vec3f calibration; //校准值
												vec3f filter;      //滑动平均滤波值
												vec3f dps;         //度每秒 
												vec3f radps;       //弧度每秒
											}gyrodata;

typedef struct _mpu{   
											accdata acc;
											gyrodata gyro;
									 }mpu;

extern mpu mpu6050Top;
extern mpu mpu6050Bot;


//初始化6050
void Mpu6050Top_Init(uint16_t sample_rate, uint16_t lpf);
//读取加速度
void Mpu6050Top_Read_Acc_Data(void);
//读取角速度
void Mpu6050Top_Read_Gyro_Data(void);
//读取加速度和角速度数据
void Mpu6050Top_Read(void);
//加速度零偏矫正
void Mpu6050Top_CalOffset_Acc(void);
//陀螺仪零偏矫正
void Mpu6050Top_CalOffset_Gyro(void);
//mpu6050数据准备
void Mpu6050Top_Data_Prepare(void);
									 
//初始化6050
void Mpu6050Bot_Init(uint16_t sample_rate, uint16_t lpf);
//读取加速度
void Mpu6050Bot_Read_Acc_Data(void);
//读取角速度
void Mpu6050Bot_Read_Gyro_Data(void);
//读取加速度和角速度数据
void Mpu6050Bot_Read(void);
//加速度零偏矫正
void Mpu6050Bot_CalOffset_Acc(void);
//陀螺仪零偏矫正
void Mpu6050Bot_CalOffset_Gyro(void);
//mpu6050数据准备
void Mpu6050Bot_Data_Prepare(void);


#endif
