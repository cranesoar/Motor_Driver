#include "imu.h"

eulerAngle gimbalTopAngle;
eulerAngle gimbalBotAngle;

#define Kp 0.6f                           // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.1f                           // integral gain governs rate of convergence of gyroscope biases
float Start_GyroYaw=0,Position_GyroYaw;
_Bool Start_GyroYaw_flag=0;
void TDT_IMUTopupdate(float half_T, vec3f* gyro, vec3f* acc)
{
	float vx, vy, vz;//(r系到b系的第三列)
	
  float norm;
  float ex, ey, ez;
	
	float gx = gyro->data[x];
	float gy = gyro->data[y];
	float gz = gyro->data[z];
	float ax = acc->data[x];
	float ay = acc->data[y];
	float az = acc->data[z];
	
	static float q0 = 1, q1 = 0, q2 = 0, q3 = 0;     // quaternion elements representing the estimated orientation
	static float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error

		
	//acc数据归一化
  norm = my_sqrt(ax*ax + ay*ay + az*az);       
  ax = ax / norm;
  ay = ay / norm;
  az = az / norm;

  // estimated direction of gravity and flux (v and w)              估计重力方向和流量/变迁
  vx = 2*(q1*q3 - q0*q2);												//四元素中xyz的表示
  vy = 2*(q0*q1 + q2*q3);
  vz = 1 - 2*(q1*q1 + q2*q2);
	
  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) ;                           					 //向量外积在相减得到差分就是误差
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;

  exInt = exInt + ex *Ki *2 *half_T;								  //对误差进行积分
  eyInt = eyInt + ey *Ki *2 *half_T;
  ezInt = ezInt + ez *Ki *2 *half_T;
	
  // 积分限幅
	exInt = LIMIT(exInt, - IMU_INTEGRAL_LIM ,IMU_INTEGRAL_LIM );
	exInt = LIMIT(exInt, - IMU_INTEGRAL_LIM ,IMU_INTEGRAL_LIM );
	exInt = LIMIT(exInt, - IMU_INTEGRAL_LIM ,IMU_INTEGRAL_LIM );
	
  // adjusted gyroscope measurements
  gx = gx + Kp *(ex + exInt);					   						
  gy = gy + Kp *(ey + eyInt);				   							
  gz = gz + Kp *(ez + ezInt);					   					  							
	
  // integrate quaternion rate and normalise						   //四元素的微分方程
  q0 = q0 + (-q1*gx - q2*gy - q3*gz) *half_T;
  q1 = q1 + ( q0*gx + q2*gz - q3*gy) *half_T;
  q2 = q2 + ( q0*gy - q1*gz + q3*gx) *half_T;
  q3 = q3 + ( q0*gz + q1*gy - q2*gx) *half_T;

  // normalise quaternion
  norm = my_sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;
 
  gimbalTopAngle.yaw = fast_atan2(2*q1*q2+2*q0*q3, -2*q2*q2-2*q3*q3+1) *57.3f;
  gimbalTopAngle.roll = fast_atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1) *57.3f;
  gimbalTopAngle.pitch = asin(-2*q1*q3 + 2*q0*q2) *57.3f; 		
	
  gimbalTopAngle.yaw= (gimbalTopAngle.yaw>=0) ?  gimbalTopAngle.yaw : (gimbalTopAngle.yaw+360);
  
  if((gimbalTopAngle.yaw-gimbalTopAngle.lastyaw)>180)
  gimbalTopAngle.yawsum+=gimbalTopAngle.yaw-gimbalTopAngle.lastyaw-360;
  else if ((gimbalTopAngle.yaw-gimbalTopAngle.lastyaw)<-180)  
  gimbalTopAngle.yawsum+=gimbalTopAngle.yaw-gimbalTopAngle.lastyaw+360;  
  else
  gimbalTopAngle.yawsum+=gimbalTopAngle.yaw-gimbalTopAngle.lastyaw;  
  
  gimbalTopAngle.lastyaw=gimbalTopAngle.yaw;
}

void TDT_IMUBotupdate(float half_T, vec3f* gyro, vec3f* acc)
{
	float vx, vy, vz;//(r系到b系的第三列)
	
  float norm;
  float ex, ey, ez;
	
	float gx = gyro->data[x];
	float gy = gyro->data[y];
	float gz = gyro->data[z];
	float ax = acc->data[x];
	float ay = acc->data[y];
	float az = acc->data[z];
	
	static float q0 = 1, q1 = 0, q2 = 0, q3 = 0;     // quaternion elements representing the estimated orientation
	static float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error

		
	//acc数据归一化
  norm = my_sqrt(ax*ax + ay*ay + az*az);       
  ax = ax / norm;
  ay = ay / norm;
  az = az / norm;

  // estimated direction of gravity and flux (v and w)              估计重力方向和流量/变迁
  vx = 2*(q1*q3 - q0*q2);												//四元素中xyz的表示
  vy = 2*(q0*q1 + q2*q3);
  vz = 1 - 2*(q1*q1 + q2*q2);
	
  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) ;                           					 //向量外积在相减得到差分就是误差
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;

  exInt = exInt + ex *Ki *2 *half_T;								  //对误差进行积分
  eyInt = eyInt + ey *Ki *2 *half_T;
  ezInt = ezInt + ez *Ki *2 *half_T;
	
  // 积分限幅
	exInt = LIMIT(exInt, - IMU_INTEGRAL_LIM ,IMU_INTEGRAL_LIM );
	exInt = LIMIT(exInt, - IMU_INTEGRAL_LIM ,IMU_INTEGRAL_LIM );
	exInt = LIMIT(exInt, - IMU_INTEGRAL_LIM ,IMU_INTEGRAL_LIM );
	
  // adjusted gyroscope measurements
  gx = gx + Kp *(ex + exInt);					   						
  gy = gy + Kp *(ey + eyInt);				   							
  gz = gz + Kp *(ez + ezInt);					   					  							
	
  // integrate quaternion rate and normalise						   //四元素的微分方程
  q0 = q0 + (-q1*gx - q2*gy - q3*gz) *half_T;
  q1 = q1 + ( q0*gx + q2*gz - q3*gy) *half_T;
  q2 = q2 + ( q0*gy - q1*gz + q3*gx) *half_T;
  q3 = q3 + ( q0*gz + q1*gy - q2*gx) *half_T;

  // normalise quaternion
  norm = my_sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;

  gimbalBotAngle.yaw = fast_atan2(2*q1*q2+2*q0*q3, -2*q2*q2-2*q3*q3+1) *57.3f;
  gimbalBotAngle.roll = fast_atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1) *57.3f;
  gimbalBotAngle.pitch = asin(-2*q1*q3 + 2*q0*q2) *57.3f; 
   
   gimbalBotAngle.yaw= (gimbalBotAngle.yaw>=0) ?  gimbalBotAngle.yaw : (gimbalBotAngle.yaw+360);
  
  if((gimbalBotAngle.yaw-gimbalBotAngle.lastyaw)>=180)
  gimbalBotAngle.yaw-=360;
  else if ((gimbalBotAngle.yaw-gimbalBotAngle.lastyaw)<-180)  
  gimbalBotAngle.yaw+=360;  
  
  gimbalBotAngle.lastyaw=gimbalBotAngle.yaw;
	
}
