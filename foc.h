#ifndef foc_h
#define foc_h

typedef struct 
{
    int polePairs;  //极对数
    float Ia; //三项电流值
    float Ib;
    float Ic;
    float Theat_ele; //电气角度
    float Theat_mac; //机械角度

    //坐标变换
    float Iα; //Clark变换
    float Iβ;
    float Id; //Park变换
    float Iq;

    float Iα_sv;//Park反变换
    float Iβ_sv;

    float Id_tar;//电流目标值
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


