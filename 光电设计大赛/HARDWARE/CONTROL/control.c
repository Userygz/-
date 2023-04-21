#include "control.h"
#include "headfile.h"
#include "encoder.h"
#include "timer1.h"

/**********************************************************************************/
/************///�ṹ�嶨��
/**********************************************************************************/

SpeedCrl_Typedef     CarSpeed_LF;
SpeedCrl_Typedef     CarSpeed_RF;
SpeedCrl_Typedef     CarSpeed_LB;
SpeedCrl_Typedef     CarSpeed_RB;
DirectionCrl_Typedef     CarDirection;

/**********************************************************************************/
/************///��������ʼ��
/**********************************************************************************/
//����ֵ
uint16_t M_LF=0, M_LB=0;
uint16_t M_RF=0, M_RB=0;
//Ŀ��ֵ
float Point_LF=0, Point_LB=500;
float Point_RF=0, Point_RB=0;
//���ֵ
int Car_Speed_out_LF=0, Car_Speed_out_LB=0;
int Car_Speed_out_RF=0, Car_Speed_out_RB=0;
//vofa����
float data[2];
unsigned char tail[4]={0x00, 0x00, 0x80, 0x7f};
/**********************************************************************************/
/************///����ת���ų�ʼ��
/**********************************************************************************/

void C5_C12_init() {
  GPIO_InitTypeDef GPIO_InitStructure;
	// ʹ��GPIOCʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  // ��ʼ��GPIO�ṹ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 |
                                 GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // ���Ƶ��50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**********************************************************************************/
/************///PID����������
/**********************************************************************************/

void Control_init(void)
{
    //�ٶȻ�
      CarSpeed_LF.kd = 0;
      CarSpeed_LF.ki = 0;//2.5
      CarSpeed_LF.kp = 0;//2
      CarSpeed_LF.speed_out_max = 3500;
    //�ٶȻ�
      CarSpeed_RF.kd = 0;
      CarSpeed_RF.ki = 0;//3
      CarSpeed_RF.kp = 0;//2
      CarSpeed_RF.speed_out_max = 3500;

	   CarSpeed_LB.kd = 0;
      CarSpeed_LB.ki = 0.01;//2.5
      CarSpeed_LB.kp = 1;//2
      CarSpeed_LB.speed_out_max = 3500;
    //�ٶȻ�
      CarSpeed_RB.kd = 0;
      CarSpeed_RB.ki = 0;//3
      CarSpeed_RB.kp = 0;//2
      CarSpeed_RB.speed_out_max = 3500;

    //����ת��  ������Ҫ����
      CarDirection.kp = 0;//
      CarDirection.kd = 0;//
      CarDirection.control_out_max = 0;
}

/**********************************************************************************/
/************///��ȡ����ı���ֵ���ٶȣ�
/**********************************************************************************/

void Get_speed(void)
{
   //�ɼ����ӵ��ٶ�
//	Get_Encoder_Values(&M_LB, &M_RB, &M_RF, &M_LF);
	M_LB = -Read_Encoder(2);
	M_RB = Read_Encoder(3);
	M_RF = Read_Encoder(4);
	M_LF = Read_Encoder(5);
}

/**********************************************************************************/
/************///���Ƶ����ת��
/**********************************************************************************/

/**********************************************************************************//***��ǰ***/
void Left_Forward_Motor_Go(void) 
{
    GPIO_SetBits(GPIOC,GPIO_Pin_11);
    GPIO_ResetBits(GPIOC,GPIO_Pin_12);
}
void Left_Forward_Motor_Back(void)  
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_11);
    GPIO_SetBits(GPIOC,GPIO_Pin_12);
}
void Left_Forward_Motor_Stop(void)  
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_11);
    GPIO_ResetBits(GPIOC,GPIO_Pin_12);
}
/**********************************************************************************//***��ǰ***/
void Right_Forward_Motor_Go(void) 
{
    GPIO_SetBits(GPIOC,GPIO_Pin_9);
    GPIO_ResetBits(GPIOC,GPIO_Pin_10);
}
void Right_Forward_Motor_Back(void)  
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_9);
    GPIO_SetBits(GPIOC,GPIO_Pin_10);
}
void Right_Forward_Motor_Stop()  
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_9);
    GPIO_ResetBits(GPIOC,GPIO_Pin_10);
}
/**********************************************************************************//***���***/
void Left_Behind_Motor_Go(void) 
{
    GPIO_SetBits(GPIOC,GPIO_Pin_5);
    GPIO_ResetBits(GPIOC,GPIO_Pin_6);
}
void Left_Behind_Motor_Back(void)  
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_5);
    GPIO_SetBits(GPIOC,GPIO_Pin_6);
}
void Left_Behind_Motor_Stop(void)  
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_5);
    GPIO_ResetBits(GPIOC,GPIO_Pin_6);
}
/**********************************************************************************//***�Һ�***/
void Right_Behind_Motor_Go(void) 
{
    GPIO_SetBits(GPIOC,GPIO_Pin_7);
    GPIO_ResetBits(GPIOC,GPIO_Pin_8);
}
void Right_Behind_Motor_Back(void)  
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_7);
    GPIO_SetBits(GPIOC,GPIO_Pin_8);
}
void Right_Behind_Motor_Stop(void)  
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_7);
    GPIO_ResetBits(GPIOC,GPIO_Pin_8);
}

/**********************************************************************************/
/************///PID����ʵ�ֺ���
/**********************************************************************************/

/**********************************************************************************/
void Speedcontrol_RF(void)
{
    CarSpeed_RF.err_new = Point_RF - M_RF;
    CarSpeed_RF.P = CarSpeed_RF.kp*( CarSpeed_RF.err_new-CarSpeed_RF.err_old );

    CarSpeed_RF.err_pre_old = CarSpeed_RF.err_old;
    CarSpeed_RF.err_old     = CarSpeed_RF.err_new;

    CarSpeed_RF.I = CarSpeed_RF.ki*CarSpeed_RF.err_new;

    CarSpeed_RF.D = CarSpeed_RF.kd*(CarSpeed_RF.err_new - 2*CarSpeed_RF.err_old + CarSpeed_RF.err_pre_old );

    CarSpeed_RF.speed_out_new += CarSpeed_RF.P + CarSpeed_RF.I + CarSpeed_RF.D;

    Car_Speed_out_RF = CarSpeed_RF.speed_out_new;

    if (Car_Speed_out_RF >= CarSpeed_RF.speed_out_max)
        Car_Speed_out_RF = CarSpeed_RF.speed_out_max;
    else if (Car_Speed_out_RF <= -CarSpeed_RF.speed_out_max)
        Car_Speed_out_RF = -CarSpeed_RF.speed_out_max;
}

/**********************************************************************************/
void Speedcontrol_RB(void)
{
    CarSpeed_RB.err_new = Point_RB - M_RB;
    CarSpeed_RB.P = CarSpeed_RB.kp*( CarSpeed_RB.err_new-CarSpeed_RB.err_old );

    CarSpeed_RB.err_pre_old = CarSpeed_RB.err_old;
    CarSpeed_RB.err_old     = CarSpeed_RB.err_new;

    CarSpeed_RB.I = CarSpeed_RB.ki*CarSpeed_RB.err_new;

    CarSpeed_RB.D = CarSpeed_RB.kd*(CarSpeed_RB.err_new - 2*CarSpeed_RB.err_old + CarSpeed_RB.err_pre_old );

    CarSpeed_RB.speed_out_new += CarSpeed_RB.P + CarSpeed_RB.I + CarSpeed_RB.D;

    Car_Speed_out_RB = CarSpeed_RB.speed_out_new;

    if (Car_Speed_out_RB >= CarSpeed_RB.speed_out_max)
        Car_Speed_out_RB = CarSpeed_RB.speed_out_max;
    else if (Car_Speed_out_RB <= -CarSpeed_RB.speed_out_max)
        Car_Speed_out_RB = -CarSpeed_RB.speed_out_max;
}

/**********************************************************************************/
void Speedcontrol_LF(void)
{
    CarSpeed_LF.err_new = Point_LF - M_LF;

    CarSpeed_LF.P = CarSpeed_LF.kp * (CarSpeed_LF.err_new - CarSpeed_LF.err_old);

    CarSpeed_LF.err_pre_old = CarSpeed_LF.err_old;
    CarSpeed_LF.err_old = CarSpeed_LF.err_new;

    CarSpeed_LF.I = CarSpeed_LF.ki * CarSpeed_LF.err_new;

    CarSpeed_LF.D = CarSpeed_LF.kd * (CarSpeed_LF.err_new - 2 * CarSpeed_LF.err_old + CarSpeed_LF.err_pre_old);

    CarSpeed_LF.speed_out_new += CarSpeed_LF.P + CarSpeed_LF.I + CarSpeed_LF.D;

    Car_Speed_out_LF = CarSpeed_LF.speed_out_new;

     if (Car_Speed_out_LF >= CarSpeed_LF.speed_out_max)
         Car_Speed_out_LF = CarSpeed_LF.speed_out_max;
     else if (Car_Speed_out_LF <= -CarSpeed_LF.speed_out_max)
         Car_Speed_out_LF = -CarSpeed_LF.speed_out_max;
}

/**********************************************************************************/
void Speedcontrol_LB(void)
{
    CarSpeed_LB.err_new = Point_LB - M_LB;

    CarSpeed_LB.P = CarSpeed_LB.kp * (CarSpeed_LB.err_new - CarSpeed_LB.err_old);

    CarSpeed_LB.err_pre_old = CarSpeed_LB.err_old;
    CarSpeed_LB.err_old = CarSpeed_LB.err_new;

    CarSpeed_LB.I = CarSpeed_LB.ki * CarSpeed_LB.err_new;

    CarSpeed_LB.D = CarSpeed_LB.kd * (CarSpeed_LB.err_new - 2 * CarSpeed_LB.err_old + CarSpeed_LB.err_pre_old);

    CarSpeed_LB.speed_out_new += CarSpeed_LB.P + CarSpeed_LB.I + CarSpeed_LB.D;

    Car_Speed_out_LB = CarSpeed_LB.speed_out_new;

     if (Car_Speed_out_LB >= CarSpeed_LB.speed_out_max)
         Car_Speed_out_LB = CarSpeed_LB.speed_out_max;
     else if (Car_Speed_out_LB <= -CarSpeed_LB.speed_out_max)
         Car_Speed_out_LB = -CarSpeed_LB.speed_out_max;
}

/**********************************************************************************/
/************///����������
/**********************************************************************************/

void Motor_Out(int16_t/*LF*/LF_outPWM,int16_t/*LB*/LB_outPWM,int16_t/*RF*/RF_outPWM,int16_t/*RB*/RB_outPWM)
{
    // ����PWMֵ���������Ƶ��������ת
    if (LF_outPWM >= 0)
    {
        Left_Forward_Motor_Go();
    }
    else
    {
        Left_Forward_Motor_Back();
        LF_outPWM = -LF_outPWM; // ȡ����ֵ
    }

    if (LB_outPWM >= 0)
    {
        Left_Behind_Motor_Go();
    }
    else
    {
        Left_Behind_Motor_Back();
        LB_outPWM = -LB_outPWM;
    }

    if (RF_outPWM >= 0)
    {
        Right_Forward_Motor_Go();
    }
    else
    {
        Right_Forward_Motor_Back();
        RF_outPWM = -RF_outPWM;
    }

    if (RB_outPWM >= 0)
    {
        Right_Behind_Motor_Go();
    }
    else
    {
        Right_Behind_Motor_Back();
        RB_outPWM = -RB_outPWM;
    }

    // Ӧ��PWMֵ�������������Ҫ�滻Ϊ����PWM���ú�����
    TIM1_PWM_Set(1,LB_outPWM);//PA8
    TIM1_PWM_Set(2,RB_outPWM);//PA9
    TIM1_PWM_Set(3,RF_outPWM);//PA10
    TIM1_PWM_Set(4,LF_outPWM);//PA11
}



/**********************************************************************************/
/************///�ܿغ���
/**********************************************************************************/

void Control(void)
{
    Get_speed();//��ȡ�ٶ�
	
//    Speedcontrol_LF();//�ٶȻ�����
    Speedcontrol_LB();//�ٶȻ�����
//	   Speedcontrol_RF();//�ٶȻ�����
//    Speedcontrol_RB();//�ٶȻ�����

	 //����Ĳ���pwm�ֱ��ǣ���ǰ��������������ǰ������Һ�����pwmֵ�����Ĭ�Ͼ��У������޸�
    Motor_Out(Car_Speed_out_LF,Car_Speed_out_LB,Car_Speed_out_RF,Car_Speed_out_RB); //ռ�ձ����
}














