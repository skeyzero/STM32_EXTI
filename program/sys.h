#ifndef __SYS_H__
#define __SYS_H__

#include "stm32f10x.h"


void SysClk_Init(void);
void NVIC_Set(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);
void SysTick_Init(void);
u16 CRC16(u8 *buffer,u8 num);
#endif

