#ifndef __FOC_H
#define __FOC_H
#include "main.h"

typedef struct 
{
    u16 Ia;     //输入，A相定子电流
    u16 Ib;     //输入，B相定子电流
    u16 Ic;     //输入，C相定子电流
    u16 IAlpha;  //输出，静止坐标系Alpha轴定子电流
    u16 IBeta;   //输出，静止坐标系Beta轴定子电流
    void (*calcClark)();    
    void (*calcAntiClark)();
}_CLARK;

typedef struct 
{
    long Id;     //输出，旋转坐标系下的D坐标值电流
    long Iq;         //输出，旋转坐标系下的Q坐标值电流
    long IAlpha;  //输入，静止坐标系Alpha轴定子电流
    long IBeta;   //输入，静止坐标系Beta轴定子电流
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

typedef struct{
               _duty  Duty;
               _SVPWM Svpwm;
               _CLARK Clack;
               _PARK  Park;
               _Offset Offset; 
}_DRV8305;

extern _DRV8305 DRV8305;
extern u8 Step_Flag;


#endif

