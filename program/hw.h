#ifndef __HW_H__
#define __HW_H__

#include "stm32f10x.h"

void HW_Input_Init(void);
u16 Input_State(void);
void HW_Output_Init(void);
void Open_SW(void);
void HW_Output(u32 dat);
#endif

