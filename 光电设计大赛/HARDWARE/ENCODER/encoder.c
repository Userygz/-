#include "encoder.h"
#include "headfile.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


void Encoder_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

	 // 打开AFIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // 打开时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM5, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	 // 释放JTAG引脚，以便使用PA15作为普通I/O
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // 重映射PB3
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
	
    // 初始化GPIO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 设置为上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // 编码器1：A15，B3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 编码器2：A6，A7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 编码器3：B6，B7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 编码器4：A0，A1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 初始化定时器
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 0x00;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    // 初始化定时器输入捕获
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1 | TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x00;
	 
	 TIM_ICInit(TIM2, &TIM_ICInitStructure);
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
	 TIM_ICInit(TIM4, &TIM_ICInitStructure);
	 TIM_ICInit(TIM5, &TIM_ICInitStructure);

	 // 使能定时器
	 TIM_Cmd(TIM2, ENABLE);
	 TIM_Cmd(TIM3, ENABLE);
	 TIM_Cmd(TIM4, ENABLE);
	 TIM_Cmd(TIM5, ENABLE);

	 // 使能定时器的编码器接口
	 TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
	 TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
	 TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
	 TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
}

void Get_Encoder_Values(uint16_t *encoder1, uint16_t *encoder2, uint16_t *encoder3, uint16_t *encoder4)
{
    *encoder1 = TIM_GetCounter(TIM2);
    *encoder2 = TIM_GetCounter(TIM3);
    *encoder3 = TIM_GetCounter(TIM4);
    *encoder4 = TIM_GetCounter(TIM5);
}

int Read_Encoder(u8 TIMX)
{
   int Encoder_TIM;    
   switch(TIMX)
	 {
	    case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
		 case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;
		 case 5:  Encoder_TIM= (short)TIM5 -> CNT;  TIM5 -> CNT=0;break;		 
		 default: Encoder_TIM=0;
	 }
	return Encoder_TIM;
}

