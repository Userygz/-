#ifndef __TIMER1_H
#define __TIMER1_H 			   
#include "sys.h"  

void TIM1_PWM_Set(uint8_t channel, uint16_t pwm_value);
void TIM1_PWM_Init(void);

#endif
