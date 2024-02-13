#include "foc.h"
#include "math.h"

#define SQRT3_2 0.866f
#define SQRT3 1.732f

/***********************************
*获取电气角度
*电气角度 = 机械角度 * 磁对数
************************************/
static void GetElectricAngle(FOC *foc)
{
    foc->Theat_ele = foc->Theat_mac * foc->polePairs;

}

/************************************
 * Clark正变换
 * 通过系数2/3，应用等幅值变换
*************************************/
static void ClarkTransfer(FOC *foc)
{
    foc->Iα = foc->Ia;
    foc->Iβ = (foc->Ia + 2.0f*foc->Ib)/SQRT3;
}

/*****************************************
 * Prak变换
 * 其中角度均为电角度
*****************************************/
static void ParkTransfer(FOC *foc)
{
    foc->Id = foc->Iα * cos(foc->Theat_ele) + foc->Iβ * sin(foc->Theat_ele);
    foc->Iq = -foc->Iα * sin(foc->Theat_ele) + foc->Iβ * cos(foc->Theat_ele);
}

/****************************************
 * Park反变换
 * 结果用于SVPWM
******************************************/
static void PrakAntiTransfer(FOC *foc)
{
    foc->Iα_sv = foc->Id * cos(foc->Theat_ele) - foc->Iq * sin(foc->Theat_ele);
    foc->Iβ_sv = foc->Iq * cos(foc->Theat_ele) - foc->Id * sin(foc->Theat_ele);
}

/******************************************
 *两个PI控制器
 *用于D，Q轴电流的控制
*******************************************/
static void CurrentPIControlId(FOC *foc,Kp,Ki,tar)
{
    //参数传递
    foc->D.Kp = Kp;
    foc->D.Ki = Ki;

    foc->D.pre = foc->Id;
    foc->D.tar = tar;

    //误差计算
    foc->D.bias = foc->D.tar - foc->D.pre;

    //计算PI控制器的输出值
    foc->D.out += foc->D.Kp * (foc->D.bias - foc->D.lastBias) + foc->D.Ki * foc->D.bias;

    //误差更迭
    foc->D.lastBias = foc->D.bias;

    //输出限幅

}

static void CurrentPIControlIq(FOC *foc,Kp,Ki,tar)
{
    //参数传递
    foc->Q.Kp = Kp;
    foc->Q.Ki = Ki;

    foc->Q.pre = foc->Id;
    foc->Q.tar = tar;

    //误差计算
    foc->Q.bias = foc->Q.tar - foc->Q.pre;

    //计算PI控制器的输出值
    foc->Q.out += foc->Q.Kp * (foc->Q.bias - foc->Q.lastBias) + foc->Q.Ki * foc->Q.bias;

    //误差更迭
    foc->Q.lastBias = foc->Q.bias;

    //输出限幅

}

