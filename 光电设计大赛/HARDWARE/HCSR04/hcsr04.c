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
#include "hcsr04.h"
#include "timer6.h"
#include "exit.h"

int cont = 0;int n = 0;float time = 0;float lengthTemp = 0;


void hcsr04_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//先使能外设IO PORTA时钟 
	//触发引脚，推挽输出
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	 // 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);		 //默认拉低
	//回应引脚，浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	 // 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);		 //默认拉低
	
//	hcsr04_trig();
	TIM6_Configuration();
	EXTI_Configuration();
}

void hcsr04_trig(void)
{
	hcsr1_T = 1;
	delay_us(10);//10us
	hcsr1_T = 0;
}

void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		// 在此处添加需要执行的代码
		n ++;
		if(n==1)
		{
			TIM_Cmd(TIM6, ENABLE);//开启定时器使能
		}
		else if(n==2)
		{
			TIM_Cmd(TIM6, DISABLE);//关闭定时器使能
			time = (TIM_GetCounter(TIM6)/1000.0) + cont;
			lengthTemp = (time/2.0)*334.0;
//			printf("time : %f mm\n",time);
			printf("TIM_GetCounter : %d \n",TIM_GetCounter(TIM6));
			printf("cont : %d \n",cont);
			printf("lengthTemp : %f mm\n",lengthTemp);
			n = 0;cont = 0;TIM6->CNT=0;//将计数值清零
		}
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}
void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
			// 在此处添加需要执行的代码
			++ cont;
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}
