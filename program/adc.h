#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f10x.h"

void  Adc_Init(void);
u16 Get_Adc(u8 ch);

#endif

