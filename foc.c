#include "foc.h"
#include "math.h"

#define SQRT3_2 0.866f
#define SQRT3 1.732f

/***********************************
*��ȡ�����Ƕ�
*�����Ƕ� = ��е�Ƕ� * �Ŷ���
************************************/
static void GetElectricAngle(FOC *foc)
{
    foc->Theat_ele = foc->Theat_mac * foc->polePairs;

}

/************************************
 * Clark���任
 * ͨ��ϵ��2/3��Ӧ�õȷ�ֵ�任
*************************************/
static void ClarkTransfer(FOC *foc)
{
    foc->I�� = foc->Ia;
    foc->I�� = (foc->Ia + 2.0f*foc->Ib)/SQRT3;
}

/*****************************************
 * Prak�任
 * ���нǶȾ�Ϊ��Ƕ�
*****************************************/
static void ParkTransfer(FOC *foc)
{
    foc->Id = foc->I�� * cos(foc->Theat_ele) + foc->I�� * sin(foc->Theat_ele);
    foc->Iq = -foc->I�� * sin(foc->Theat_ele) + foc->I�� * cos(foc->Theat_ele);
}

/****************************************
 * Park���任
 * �������SVPWM
******************************************/
static void PrakAntiTransfer(FOC *foc)
{
    foc->I��_sv = foc->Id * cos(foc->Theat_ele) - foc->Iq * sin(foc->Theat_ele);
    foc->I��_sv = foc->Iq * cos(foc->Theat_ele) - foc->Id * sin(foc->Theat_ele);
}

/******************************************
 *����PI������
 *����D��Q������Ŀ���
*******************************************/
static void CurrentPIControlId(FOC *foc,Kp,Ki,tar)
{
    //��������
    foc->D.Kp = Kp;
    foc->D.Ki = Ki;

    foc->D.pre = foc->Id;
    foc->D.tar = tar;

    //������
    foc->D.bias = foc->D.tar - foc->D.pre;

    //����PI�����������ֵ
    foc->D.out += foc->D.Kp * (foc->D.bias - foc->D.lastBias) + foc->D.Ki * foc->D.bias;

    //������
    foc->D.lastBias = foc->D.bias;

    //����޷�

}

static void CurrentPIControlIq(FOC *foc,Kp,Ki,tar)
{
    //��������
    foc->Q.Kp = Kp;
    foc->Q.Ki = Ki;

    foc->Q.pre = foc->Id;
    foc->Q.tar = tar;

    //������
    foc->Q.bias = foc->Q.tar - foc->Q.pre;

    //����PI�����������ֵ
    foc->Q.out += foc->Q.Kp * (foc->Q.bias - foc->Q.lastBias) + foc->Q.Ki * foc->Q.bias;

    //������
    foc->Q.lastBias = foc->Q.bias;

    //����޷�

}

