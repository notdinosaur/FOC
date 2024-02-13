#include "svpwm.h"
#include "math.h"
#include "stm32f4xx_tim.h"

#define SQRT3_2 0.866f
#define SQRT3 1.732f

/********************************************************
 * 扇区判断
 * 利用Uα，Uβ来引入三个电压量U1,U2,U3来判断扇区
 * *sv为结构体指针，指向SVPWM结构体相关量
 * 
********************************************************/

void SvpwmSectorJudgment(SV *sv)
{
    uint8_t a=0;
    uint8_t b=0;
    uint8_t c=0;
    uint8_t N;

    sv->U1 = sv->Uβ;
    sv->U2 = -sv->Uα * SQRT3_2 - sv->Uβ / 2.0f;
	sv->U3 = sv->Uα * SQRT3_2 - sv->Uβ / 2.0f;

    if(sv->U1 > 0)  a = 1;
    if(sv->U2 > 0)  b = 1;
    if(sv->U3 > 0)  c = 1;

    N = 4*c + 2*b + a;

    switch(N)
    {
        case 1: sv->sector = 6; break;
        case 2: sv->sector = 4; break;
        case 3: sv->sector = 5; break;
        case 4: sv->sector = 2; break;
        case 5: sv->sector = 1; break;
        case 6: sv->sector = 3; break;

    }

}

/******************************************************
 * 矢量时间计算
 * 通过伏秒平衡及相关数学变换推导得出作用时间与α，β轴上电压的关系
 * K为调制比，U1,U2,U3涵义同上
**********************************************************/
void GetVectorDuration(SV*sv)
{
    float k = (SQRT3 * sv->Ts) /Udc;
    switch(sv->sector)
    {
        case 1:
            sv->T4 = k*sv->U3;
            sv->T6 = k*sv->U1;
            sv->T0 = sv->T7 = (sv->Ts - sv->T4 - sv->T6)/2.0f;
            break;
        case 2:
            sv->T2 = -k*sv->U3;
            sv->T6 = -k*sv->U2;
            sv->T0 = sv->T7 = (sv->Ts - sv->T2 - sv->T6)/2.0f;
            break;
        case 3:
            sv->T2 = k*sv->U1;
            sv->T3 = k*sv->U;
            sv->T0 = sv->T7 = (sv->Ts - sv->T2 - sv->T3)/2.0f;
            break;
        case 4:
            sv->T1 = -k*sv->U1;
            sv->T3 = -k*sv->U3;
            sv->T0 = sv->T7 = (sv->Ts = sv->T1 - sv->T3)/2.0f;
            break;
        case 5:
            sv->T1 = k*sv->U2;
            sv->T5 = k*sv->U3;
            sv->T0 = sv->T7 = (sv->Ts - sv->T1 - sv->T5)/2.0f;
            break;
        case 6:
            sv->T4 = -k*sv->U2;
            sv->T5 = -k*sv->U1;
            sv->T0 = sv->T7 = (sv->Ts - sv->T4 - sv->T5)2.0f;
            break;

    }
}
/****************************************************
 * 七段式分配矢量时间
 * 计算得到CCR
 * 
******************************************************/
void SvpwmGenerate(SV *sv)
{
    float Ta,Tb,Tc;
    float CCR1,CCR2,CCR3;
    switch(sv->sector)
    {
        case 1:
            Ta = sv->T4 + sv->T6 + sv->T7;
            Tb = sv->T6 + sv->T7;
            Tc = sv->T7;
            break;
        case 2:
            Ta = sv->T6 + sv->T7;
            Tb = sv->T2 + sv->T6 + sv->T7;
            Tc = sv->T7;
            break;
        case 3:
            Ta = sv->T7;
            Tb = sv->T2 + sv->T3 + sv->T7;
            Tc = sv->T3 + sv->T7;
            break;
        case 4:
            Ta = sv->T7;
            Tb = sv->T3 + sv->T7;
            Tc = sv->T1 + sv->T3 +sv->T7;
            break;
        case 5:
            Ta = sv->T5 + sv->T7;
            Tb = sv->T7;
            Tc = sv->T1 + sv->T5 +sv->T7;      
            break;
        case 6:
            Ta = sv->T4 + sv->T5 + sv->T7;
            Tb = sv->T7;
            Tc = sv->sv->T5 +sv->T7;    
            break;                  
    }


    CCR1 = Ta/sv->Ts * sv->ARR;
    CCR2 = Tb/sv->Ts * sv->ARR;
    CCR3 = Tc/sv->Ts * sv->ARR;

    sv->ccr1 = CCR1;
    sv->ccr2 = CCR2;
    sv->ccr3 = CCR3;

}

/*****************************************************
 * 输入α与β轴电压，开关时间，自动重装载值
 * 通过以上三步实现SVpem调制
 * 最终通过TIM_SetCompare函数实现发波
 * 
*******************************************************/
void SvpwmControl(SV *sv,Uα,Uβ,Ts,ARR,Udc)
{
    sv->Uα = Uα;
    sv->Uβ = Uβ;
    sv->ARR = ARR;
    sv->Ts = Ts;
    sv->Udc = Udc;

    SvpwmSectorJudgment(SV *sv);
    GetVectorDuration(SV*sv);
    SvpwmGenerate(SV*sv);

    TIM_SetCompare1(TIM3,sv->ccr1);
    TIM_SetCompare2(TIM3,sv->ccr2);
    TIM_SetCompare3(TIM3,sv->ccr3);


}
/*****************************************************
 * 获取扇区
 * 
 * 
******************************************************/
uint8_t GetSVPWMSector(SV *sv)
{
    return sv->sector;
}