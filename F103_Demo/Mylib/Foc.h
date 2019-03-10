#ifndef __FOC_H
#define __FOC_H
#include "main.h"

typedef struct 
{
    int16_t Ia;     //输入，A相定子电流
    int16_t Ib;     //输入，B相定子电流
    int32_t Ic;     //输入，C相定子电流
    int16_t Ia_last;     //输入，上次A相定子电流
    int16_t Ib_last;     //输入，上次B相定子电流
    int32_t Ic_last;     //输入，上次C相定子电流  
    int32_t IAlpha;  //输出，静止坐标系Alpha轴定子电流
    int32_t IBeta;   //输出，静止坐标系Beta轴定子电流
    void (*calcClark)();    
    void (*calcAntiClark)();
}_CLARK;

typedef struct 
{
    int32_t Id;     //输出，旋转坐标系下的D坐标值电流
    int32_t Iq;         //输出，旋转坐标系下的Q坐标值电流
    int32_t IAlpha;  //输入，静止坐标系Alpha轴定子电流
    int32_t IBeta;   //输入，静止坐标系Beta轴定子电流
    float Ud;     //输出，旋转坐标系下的D坐标值电压
    float Uq;          //输出，旋转坐标系下的Q坐标值电压
    long UAlpha;  //输入，静止坐标系Alpha轴定子电压
    long UBeta;   //输入，静止坐标系Beta轴定子电压
    long Theta;    //旋转坐标角度
    float ActId;  //实际D轴电流
    float ActIq;  //实际Q轴电流
    void (*calcPark)();      
    void (*calcAntiPark)();  
}_PARK;

typedef struct 
{
    float UAlpha; //输入，静止坐标系Alpha轴定子电压
    float UBeta;  //输入，静止坐标系Beta轴定子电压
    long Ua;      //
    long Ub;      //
    long Uc;      //
    long Tx;        //
    long Ty;        //
    long Tz;        //
    long taOn;    //A相时间
    long tbOn;      //B相时间
    long tcOn;      //C相时间
} _SVPWM;


typedef struct {
                 u16 MOTA;
                 u16 MOTB;
                 u16 MOTC;  
}_duty;

typedef struct {
                 u16 PhaseAOffset;
                 u16 PhaseBOffset;
                 u16 PhaseCOffset;  
}_Offset;

typedef struct {
                 int Id_Result;
                 int Iq_Result; 
                 int SetValue;
                 int Feedback;  
}_Speed;

typedef struct 
{
  float Kp;
  float Ki;
  float Kc;
}Parameter;

typedef struct  
{
  Parameter Id;
  Parameter Iq;
  Parameter Speed;
}_PID;

typedef struct{
               _duty  Duty;
               _SVPWM Svpwm;
               _CLARK Clack;
               _PARK  Park;
               _Offset Offset; 
               _Speed  Speed;
               _PID    PID;
}_DRV8305;

extern _DRV8305 DRV8305;
extern u8 Step_Flag;
extern float Outmax_Id,Outmax_Iq,Outmax_Speed;
void Clarke_Trans(void);
void Park_Trans(void);
void Id_PI_Controller(void);
void Iq_PI_Controller(void);
void Speed_Controller(void);
#endif

