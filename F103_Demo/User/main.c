#include "main.h"

//测得磁编码器对应的机械角0度，对应电角度=>42度（正负1度），机械角度是电角度的14倍。
//则机械角度与电角度的换算关系为：电角度=[（机械角度/（16384/14））*360+42]%360
//在while（1）中一次循环96us，移至schedule的100us中断里
int FOC_T=500;
long Change=0;
int main(void)
{

  Board_ALL_Init();


  
    while(1)
    {        
    

    
    }
}     

