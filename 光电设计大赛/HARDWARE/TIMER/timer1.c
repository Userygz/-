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
    // ����GPIOA��TIM1��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);

    // ��ʼ��GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ��ʼ����ʱ��
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1; // ��ʱ������Ϊ1000��������ʱ��
    TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1; // ��ʱ����ƵΪ72
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

    // ��ʼ��PWMͨ��
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 0; // ��ʼռ�ձ�Ϊ0
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    // ��ʼ������ͨ����PWM���
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    TIM_OC1Init(TIM1, &TIM_OCInitStruct);
    TIM_OC2Init(TIM1, &TIM_OCInitStruct);
    TIM_OC3Init(TIM1, &TIM_OCInitStruct);
    TIM_OC4Init(TIM1, &TIM_OCInitStruct);

    TIM_Cmd(TIM1, ENABLE);
	 TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	 TIM_CtrlPWMOutputs(TIM1,ENABLE);        //MOE �����ʹ��,�߼���ʱ�����뿪�����
}

void TIM1_PWM_Set(uint8_t channel, uint16_t pwm_value)
{
    // ����ѡ���ͨ��������PWM���
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
