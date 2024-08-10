#include "k210.h"
#define Len 20
int code_Len = 0;
char code_Arr[20] = {0};
int n = 0;
int xx;

void k210_Receive(uint8_t Com_Data)
{
    uint8_t i;
    static uint8_t RxCounter1=0;
    static uint16_t RxBuffer1[8]={0};
    static uint8_t RxState = 0;

    if(RxState==0&&Com_Data==0x2C)
    {
        RxState=1;
        RxBuffer1[RxCounter1++]=Com_Data;
    }
    else if(RxState==1&&Com_Data==0x12)
    {
        RxState=2;
        RxBuffer1[RxCounter1++]=Com_Data;
    }
    else if(RxState==2)
    {
        RxBuffer1[RxCounter1++]=Com_Data;
        if(RxCounter1>=4||Com_Data == 0x5B)
        {
            RxState=3;
            xx = RxBuffer1[RxCounter1-2];
            //printf("%d\r\n",xx);
        }
    }
    else if(RxState==3)
    {
        if(RxBuffer1[RxCounter1-1] == 0x5B)
        {
            RxCounter1 = 0;
            RxState = 0;
        }
        else
        {
            RxState = 0;
            RxCounter1=0;
            for(i=0;i<8;i++)
            {
                RxBuffer1[i]=0x00;
            }
        }
    }
    else
    {
        RxState = 0;
        RxCounter1=0;
        for(i=0;i<8;i++)
        {
            RxBuffer1[i]=0x00;
        }
    }

}

