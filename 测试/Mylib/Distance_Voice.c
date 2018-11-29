#include "Distance_Voice.h"


//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 KS103_ReadOneByte(u8 address, u8 reg)
{				  
	u8 temp=0;		  	    																 
    I2C2_Soft_Start();  
    I2C2_Soft_SendByte(address);   //发送低地址
	I2C2_Soft_WaitAck();	 
	I2C2_Soft_SendByte(reg);   //发送低地址
	I2C2_Soft_WaitAck();	   
	I2C2_Soft_Start();  	 	   
	I2C2_Soft_SendByte (address + 1);           //进入接收模式			   
	I2C2_Soft_WaitAck();	 

	I2C2_Soft_Delay();
//	delay_us(50);	   //增加此代码通信成功！！！
    temp=I2C2_Soft_ReadByte();	  //读寄存器3	   
    I2C2_Soft_Stop();//产生一个停止条件	    
	return temp;
}

void KS103_WriteOneByte(u8 address,u8 reg,u8 command)
{				   	  	    																 
  I2C2_Soft_Start();   
	I2C2_Soft_SendByte(address);	    //发送写命令
	I2C2_Soft_WaitAck();
	I2C2_Soft_SendByte(reg);//发送高地址	  
	I2C2_Soft_WaitAck();	   
  I2C2_Soft_SendByte(command);   //发送低地址
	I2C2_Soft_WaitAck(); 	 										  		   
  I2C2_Soft_Stop();//产生一个停止条件 
//	delay_ms(80);	 
}
