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
	// 使能定时器7时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
  
  // 初始化定时器7
  TIM_TimeBaseStructure.TIM_Period = Period; // 定时器周期为1s
  TIM_TimeBaseStructure.TIM_Prescaler = 7200; // 定时器分频为7200
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
  
  // 使能定时器7更新中断
  TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
  
  // 使能定时器7
  TIM_Cmd(TIM7, ENABLE);
  
  // 配置定时器7中断优先级
  NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

