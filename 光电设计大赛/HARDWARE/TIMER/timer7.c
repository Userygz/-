#include "timer7.h"
#include "sys.h"
#include "stdint.h"
#include "stdio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "usart.h"
#include "delay.h"
#include "control.h"

void TIM7_IRQHandler(void) {
  if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) {
    Control();
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
  }
}

void TIM7_Init(u32 Period) {
  
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	// ʹ�ܶ�ʱ��7ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
  
  // ��ʼ����ʱ��7
  TIM_TimeBaseStructure.TIM_Period = Period; // ��ʱ������Ϊ1s
  TIM_TimeBaseStructure.TIM_Prescaler = 7200; // ��ʱ����ƵΪ7200
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
  
  // ʹ�ܶ�ʱ��7�����ж�
  TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
  
  // ʹ�ܶ�ʱ��7
  TIM_Cmd(TIM7, ENABLE);
  
  // ���ö�ʱ��7�ж����ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

