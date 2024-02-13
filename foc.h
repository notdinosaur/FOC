#ifndef foc_h
#define foc_h

typedef struct 
{
    int polePairs;  //������
    float Ia; //�������ֵ
    float Ib;
    float Ic;
    float Theat_ele; //�����Ƕ�
    float Theat_mac; //��е�Ƕ�

    //����任
    float I��; //Clark�任
    float I��;
    float Id; //Park�任
    float Iq;

    float I��_sv;//Park���任
    float I��_sv;

    float Id_tar;//����Ŀ��ֵ
    float Iq_tar;

    struct PI Q;
    struct PI D;

    
} FOC;


typedef struct 
{
    float Kp;
    float Ki;
    float Kd;

    float pre;
    float tar;
    float bias;
    float lastBias;
    float out;
    float outMax;

} PI;


