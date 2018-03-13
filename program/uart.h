#ifndef __UART_H__
#define __UART_H__
#include "stm32f10x.h"

#define Rx1_Buf_Length 32
#define Rx2_Buf_Length 32

void UART1_Init(u32 bound);
void UART1_Send(u8 tx_data);
void UART2_Init(u32 bound);
void UART2_Send(u8 tx_data);

#endif

