#ifndef __HCSR04_H
#define __HCSR04_H 			   
#include "sys.h"  

#define hcsr1_T PAout(4)
#define hcsr1_E PAout(5)

void hcsr04_init(void);
void hcsr04_trig(void);


#endif
