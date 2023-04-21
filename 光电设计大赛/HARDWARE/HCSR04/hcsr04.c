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
#include "hcsr04.h"
#include "timer6.h"
#include "exit.h"

int cont = 0;int n = 0;float time = 0;float lengthTemp = 0;


void hcsr04_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//��ʹ������IO PORTAʱ�� 
	//�������ţ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	 // �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);		 //Ĭ������
	//��Ӧ���ţ���������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	 // �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);		 //Ĭ������
	
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
		// �ڴ˴������Ҫִ�еĴ���
		n ++;
		if(n==1)
		{
			TIM_Cmd(TIM6, ENABLE);//������ʱ��ʹ��
		}
		else if(n==2)
		{
			TIM_Cmd(TIM6, DISABLE);//�رն�ʱ��ʹ��
			time = (TIM_GetCounter(TIM6)/1000.0) + cont;
			lengthTemp = (time/2.0)*334.0;
//			printf("time : %f mm\n",time);
			printf("TIM_GetCounter : %d \n",TIM_GetCounter(TIM6));
			printf("cont : %d \n",cont);
			printf("lengthTemp : %f mm\n",lengthTemp);
			n = 0;cont = 0;TIM6->CNT=0;//������ֵ����
		}
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}
void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
			// �ڴ˴������Ҫִ�еĴ���
			++ cont;
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}
