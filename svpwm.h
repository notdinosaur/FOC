#ifndef svpwm_h
#define svpwm_h


typedef struct 
{
    float U��; //����ĵ�ѹ
    float U��; //�����ѹ
    float U1; //���������ж�
    float U2;
    float U3;
    float T0; //�˸�ʸ��������ʱ��
    float T1;
    float T2;
    float T3;
    float T4;
    float T5;
    float T6;
    float T7;
    float Ts; //��������
    float Udc; //ֱ��ĸ�ߵ�ѹ
    float ARR;
    float ccr1;
    float ccr2;
    float ccr3;
    uint8_t sector; //����

} SV;

void SvpwmSectorJudgment(SV *sv);
void GetVectorDuration(SV *sv);
void SvpwmGenerate(SV *sv);
uint8_t GetSVPWMSector(SV *sv);









#endif /*svpwm_h*/