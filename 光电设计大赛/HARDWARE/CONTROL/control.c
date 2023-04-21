#include "control.h"
#include "headfile.h"
#include "encoder.h"
#include "timer1.h"

/**********************************************************************************/
/************///结构体定义
/**********************************************************************************/

SpeedCrl_Typedef     CarSpeed_LF;
SpeedCrl_Typedef     CarSpeed_RF;
SpeedCrl_Typedef     CarSpeed_LB;
SpeedCrl_Typedef     CarSpeed_RB;
DirectionCrl_Typedef     CarDirection;

/**********************************************************************************/
/************///各参数初始化
/**********************************************************************************/
//编码值
uint16_t M_LF=0, M_LB=0;
uint16_t M_RF=0, M_RB=0;
//目标值
float Point_LF=0, Point_LB=500;
float Point_RF=0, Point_RB=0;
//输出值
int Car_Speed_out_LF=0, Car_Speed_out_LB=0;
int Car_Speed_out_RF=0, Car_Speed_out_RB=0;
//vofa数组
float data[2];
unsigned char tail[4]={0x00, 0x00, 0x80, 0x7f};
/**********************************************************************************/
/************///正反转引脚初始化
/**********************************************************************************/

void C5_C12_init() {
  GPIO_InitTypeDef GPIO_InitStructure;
	// 使能GPIOC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  // 初始化GPIO结构体
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 |
                                 GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 最大频率50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**********************************************************************************/
/************///PID参数的整定
/**********************************************************************************/

void Control_init(void)
{
    //速度环
      CarSpeed_LF.kd = 0;
      CarSpeed_LF.ki = 0;//2.5
      CarSpeed_LF.kp = 0;//2
      CarSpeed_LF.speed_out_max = 3500;
    //速度环
      CarSpeed_RF.kd = 0;
      CarSpeed_RF.ki = 0;//3
      CarSpeed_RF.kp = 0;//2
      CarSpeed_RF.speed_out_max = 3500;

	   CarSpeed_LB.kd = 0;
      CarSpeed_LB.ki = 0.01;//2.5
      CarSpeed_LB.kp = 1;//2
      CarSpeed_LB.speed_out_max = 3500;
    //速度环
      CarSpeed_RB.kd = 0;
      CarSpeed_RB.ki = 0;//3
      CarSpeed_RB.kp = 0;//2
      CarSpeed_RB.speed_out_max = 3500;

    //单环转向环  参数需要设置
      CarDirection.kp = 0;//
      CarDirection.kd = 0;//
      CarDirection.control_out_max = 0;
}

/**********************************************************************************/
/************///获取电机的编码值（速度）
/**********************************************************************************/

void Get_speed(void)
{
   //采集轮子的速度
//	Get_Encoder_Values(&M_LB, &M_RB, &M_RF, &M_LF);
	M_LB = -Read_Encoder(2);
	M_RB = Read_Encoder(3);
	M_RF = Read_Encoder(4);
	M_LF = Read_Encoder(5);
}

/**********************************************************************************/
/************///控制电机的转向
/**********************************************************************************/

/**********************************************************************************//***左前***/
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
/**********************************************************************************//***右前***/
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
/**********************************************************************************//***左后***/
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
/**********************************************************************************//***右后***/
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
/************///PID控制实现函数
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
/************///电机输出函数
/**********************************************************************************/

void Motor_Out(int16_t/*LF*/LF_outPWM,int16_t/*LB*/LB_outPWM,int16_t/*RF*/RF_outPWM,int16_t/*RB*/RB_outPWM)
{
    // 根据PWM值正负来控制电机的正反转
    if (LF_outPWM >= 0)
    {
        Left_Forward_Motor_Go();
    }
    else
    {
        Left_Forward_Motor_Back();
        LF_outPWM = -LF_outPWM; // 取绝对值
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

    // 应用PWM值到电机（这里需要替换为您的PWM设置函数）
    TIM1_PWM_Set(1,LB_outPWM);//PA8
    TIM1_PWM_Set(2,RB_outPWM);//PA9
    TIM1_PWM_Set(3,RF_outPWM);//PA10
    TIM1_PWM_Set(4,LF_outPWM);//PA11
}



/**********************************************************************************/
/************///总控函数
/**********************************************************************************/

void Control(void)
{
    Get_speed();//读取速度
	
//    Speedcontrol_LF();//速度环控制
    Speedcontrol_LB();//速度环控制
//	   Speedcontrol_RF();//速度环控制
//    Speedcontrol_RB();//速度环控制

	 //输入的参数pwm分别是：左前电机，左后电机，右前电机，右后电机的pwm值，这边默认就行，不用修改
    Motor_Out(Car_Speed_out_LF,Car_Speed_out_LB,Car_Speed_out_RF,Car_Speed_out_RB); //占空比输出
}














