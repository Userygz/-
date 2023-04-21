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
#include "timer6.h"

void TIM6_Configuration(void)//1ms 的
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// 使能TIM6时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	// TIM6的计数时钟频率为72MHz
	TIM_TimeBaseStructure.TIM_Prescaler = 71;  // 分频系数
	TIM_TimeBaseStructure.TIM_Period = 999;      // 自动重载寄存器值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; // 时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	// 中断优先级为0，抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// 使能TIM6更新中断
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	// 启动TIM6
//	TIM_Cmd(TIM6, ENABLE);//开启定时器使能
	TIM_Cmd(TIM6,DISABLE);//关闭定时器使能
}
