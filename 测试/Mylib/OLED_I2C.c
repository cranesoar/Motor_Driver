#include "OLED_I2C.h"

void WriteCmd(unsigned char I2C_Command)//写命令
{
//	I2C_WriteByte(0x00, I2C_Command);
	I2C2_Soft_Single_Write(OLED_ADDRESS,0x00,I2C_Command);
}

void WriteDat(unsigned char I2C_Data)//写数据
{
	I2C2_Soft_Single_Write(OLED_ADDRESS,0x40,I2C_Data);
}

void OLED_Init(void)
{
	DelayMs(100); //这里的延时很重要
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char ax, unsigned char ay) //设置起始点坐标
{ 
	WriteCmd(0xb0+ay);
	WriteCmd(((ax&0xf0)>>4)|0x10);
	WriteCmd((ax&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char ax, unsigned char ay, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char ax, unsigned char ay, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(ax > 126)
				{
					ax = 0;
					ay++;
				}
				OLED_SetPos(ax,ay);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				ax += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(ax > 120)
				{
					ax = 0;
					ay++;
				}
				OLED_SetPos(ax,ay);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(ax,ay+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				ax += 8;
				j++;
			}
		}break;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char ax, unsigned char ay, unsigned char N)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
// Description    : 显示codetab.h中的汉字,16*16点阵
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char ax, unsigned char ay, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(ax , ay);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(ax,ay + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char ax0,unsigned char ay0,unsigned char ax1,unsigned char ay1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char ax0,unsigned char ay0,unsigned char ax1,unsigned char ay1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char ax,ay;

  if(ay1%8==0)
		ay = ay1/8;
  else
		ay = ay1/8 + 1;
	  
	for(ay=ay0;ay<ay1;ay++)
	{
		OLED_SetPos(ax0,ay);
    for(ax=ax0;ax<ax1;ax++)
		{
			WriteDat(BMP[j++]);
		}
	}
}
void OLED_value(_Bool oled_cls ,float value,unsigned char x0 ,unsigned char y0, unsigned char textsize)
{
	unsigned char dat[6];
	u16 value_led;
	value_led=(unsigned int)(value);
	if(oled_cls)
	{OLED_CLS();}//OLED清屏	
	  if(value>=0)
	  {dat[0]= ' ';}
		else 
		{value=-value;
		value_led=(unsigned int)(value);	
			dat[0]= '-';
		}
		
		dat[1]=value_led/10000+0x30;
		dat[2]=value_led%10000/1000+0x30;
		dat[3]=value_led%1000/100+0x30;
		dat[4]=value_led%100/10+0x30;
		dat[5]=value_led%100%10+0x30;
	OLED_ShowStr(x0,y0,dat,textsize);				//测试8*16字符
	
}
