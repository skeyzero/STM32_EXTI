#ifndef __CAN_H__
#define __CAN_H__

#include "stm32f10x.h"

void CAN_Init(u8 mode);
u8 CAN1_TX(u16 id, u8* tx_buf,u8 tx_len);
#endif

