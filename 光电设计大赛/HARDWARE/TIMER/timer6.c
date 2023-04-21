#include "sys.h"
#include "stdint.h"
#include "stdio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "usart.h"
#include "delay.h"
//�����ļ���ͷ�ļ�
#include "timer6.h"

void TIM6_Configuration(void)//1ms ��
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// ʹ��TIM6ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	// TIM6�ļ���ʱ��Ƶ��Ϊ72MHz
	TIM_TimeBaseStructure.TIM_Prescaler = 71;  // ��Ƶϵ��
	TIM_TimeBaseStructure.TIM_Period = 999;      // �Զ����ؼĴ���ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; // ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	// �ж����ȼ�Ϊ0����ռ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// ʹ��TIM6�����ж�
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	// ����TIM6
//	TIM_Cmd(TIM6, ENABLE);//������ʱ��ʹ��
	TIM_Cmd(TIM6,DISABLE);//�رն�ʱ��ʹ��
}
