#ifndef __CONTROL_H
#define __CONTROL_H 			   
#include "sys.h"  
/**********************************************************************************/

extern uint16_t M_LF, M_LB;
extern uint16_t M_RF, M_RB;

typedef struct//����
{
    float err_new;
    float err_old;
    float err_pre_old;

    float kp;
    float ki;
    float kd;

    float P;
    float I;
    float D;

    int speed_out_new;
    int speed_out_old;
    int speed_out_max;
}SpeedCrl_Typedef;

typedef struct//����
{
    int err_new;
    int err_old;
    float kp;
    float kd;
    int control_out_old;
    int control_out_new;
    int control_out_max;
}DirectionCrl_Typedef;
/**********************************************************************************/
void Left_Forward_Motor_Go(void); //������ת AIN����෴��ƽ  BINҲ����෴��ƽ

void Left_Forward_Motor_Back(void);  //������ת

void Left_Forward_Motor_Stop(void);  //����ֹͣ AIN��BIN�����ͬ��ƽ

void Right_Forward_Motor_Go(void); //�ҵ����ת AIN����෴��ƽ  BINҲ����෴��ƽ

void Right_Forward_Motor_Back(void); //�ҵ����ת

void Right_Forward_Motor_Stop(void);  //�ҵ��ֹͣ AIN��BIN�����ͬ��ƽ

void Left_Behind_Motor_Go(void); //������ת AIN����෴��ƽ  BINҲ����෴��ƽ

void Left_Behind_Motor_Back(void);  //������ת

void Left_Behind_Motor_Stop(void);  //����ֹͣ AIN��BIN�����ͬ��ƽ

void Right_Behind_Motor_Go(void); //�ҵ����ת AIN����෴��ƽ  BINҲ����෴��ƽ

void Right_Behind_Motor_Back(void); //�ҵ����ת

void Right_Behind_Motor_Stop(void);  //�ҵ��ֹͣ AIN��BIN�����ͬ��ƽ
/**********************************************************************************/
void Speedcontrol_RF(void);

void Speedcontrol_RB(void);

void Speedcontrol_LF(void);

void Speedcontrol_LB(void);
/**********************************************************************************/
void Get_speed(void);
/**********************************************************************************/
void Motor_Out(int16_t LF_outPWM,int16_t LB_outPWM,int16_t RF_outPWM,int16_t RB_outPWM);
/**********************************************************************************/
void C5_C12_init(void);
/**********************************************************************************/
void Control(void);
/**********************************************************************************/
void Control_init(void);
/**********************************************************************************/
#endif
