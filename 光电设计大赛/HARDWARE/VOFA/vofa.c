#include "sys.h"
#include "stdint.h"
#include "stdio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "usart.h"
#include "delay.h"
//工程文件的头文件
#include "vofa.h"


void vofa_printmpu6050(float p,float r,float y)
{
	//printf("pitch,roll,yaw: %f,%f,%f\n",p,r,y);
	printf("yaw: %f\n",y);
}
void vofa_printhcsr04(float q,float h,float z,float y)
{
	printf("qian: %f\n",q);
}
