#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f10x.h"

#define SPI_SPEED_2   0
#define SPI_SPEED_8   1
#define SPI_SPEED_16  2
#define SPI_SPEED_256 3
void SPI1_Init(void);
u8 SPI1_RW_Byte(u8 TxData);
void SPI1_SetSpeed(u8 SpeedSet);

#endif

