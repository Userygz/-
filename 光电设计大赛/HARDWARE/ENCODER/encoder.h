#ifndef __ENCODER_H
#define __ENCODER_H 			   
#include "sys.h"  

void Encoder_Init(void);
void Get_Encoder_Values(uint16_t *encoder1, uint16_t *encoder2, uint16_t *encoder3, uint16_t *encoder4);
int Read_Encoder(u8 TIMX);	
#endif
