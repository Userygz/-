#include "timer1.h"
#include "sys.h"
#include "stdint.h"
#include "stdio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "usart.h"
#include "delay.h"

void TIM1_PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    // 开启GPIOA、TIM1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);

    // 初始化GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 初始化定时器
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1; // 定时器周期为1000个计数器时钟
    TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1; // 定时器分频为72
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

    // 初始化PWM通道
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 0; // 初始占空比为0
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    // 初始化所有通道的PWM输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    TIM_OC1Init(TIM1, &TIM_OCInitStruct);
    TIM_OC2Init(TIM1, &TIM_OCInitStruct);
    TIM_OC3Init(TIM1, &TIM_OCInitStruct);
    TIM_OC4Init(TIM1, &TIM_OCInitStruct);

    TIM_Cmd(TIM1, ENABLE);
	 TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	 TIM_CtrlPWMOutputs(TIM1,ENABLE);        //MOE 主输出使能,高级定时器必须开启这个
}

void TIM1_PWM_Set(uint8_t channel, uint16_t pwm_value)
{
    // 根据选择的通道来设置PWM输出
    switch (channel) {
        case 1:
            TIM_SetCompare1(TIM1, pwm_value);
            break;
        case 2:
            TIM_SetCompare2(TIM1, pwm_value);
            break;
        case 3:
            TIM_SetCompare3(TIM1, pwm_value);
            break;
        case 4:
            TIM_SetCompare4(TIM1, pwm_value);
            break;
        default:
            break;
    }
}
