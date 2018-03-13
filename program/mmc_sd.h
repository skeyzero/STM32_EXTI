#ifndef __MMC_SD__
#define __MMC_SD__

#include "stm32f10x.h"
#include "spi.h"


#define SD_CS_SET()		GPIOA->BSRR |= 1<<0
#define SD_CS_CLR()		GPIOA->BSRR |= 1<<16

void SD_Init(void);
void SD_Reset(void);
u8 SD_CMD(u8 cmd,u32 arg,u8 crc);


u8 SD_RD_Disk(u8*buf,u32 sector,u8 cnt);
u8 SD_WR_Disk(u8*buf,u32 sector,u8 cnt);

#endif

