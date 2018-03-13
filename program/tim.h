#ifndef __TIM_H__
#define __TIM_H__

#include "stm32f10x.h"

#define M_DIR_S 	1
#define M_DIR_N		0


void TIM3_Init(void);
void TIM2_Init(void);
u8 Set_Motor_Circle(u8 motor,u8 dir,u16 cycle);
u8 Set_Motor_Circle_Time(u8 motor,u8 dir,u16 cycle,u8 time);
void Set_Motor_Angle(u8 motor,u8 dir,u32 angle);
void M_Dir_Handle(u8 m,u8 dir);
#endif

