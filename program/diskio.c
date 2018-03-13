/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{

//	DSTATUS stat;
//	//int result;

//	switch (pdrv) {
//	case DEV_RAM :
//		//result = RAM_disk_status();

//		// translate the reslut code here

//		return stat;

//	case DEV_MMC :
//		//result = MMC_disk_status();

//		// translate the reslut code here

//		return stat;

//	case DEV_USB :
//		//result = USB_disk_status();

//		// translate the reslut code here

//		return stat;
//	}
//	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	/*
	初始化SD卡
	*/
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{

//	if ( count == 1 )		/* 1个sector的写操作 */
//  {/* 注意count不能为0，否则会出错 */		
//		SD_WriteBlock( sector << 9 ,(u32 *)(&buff[0]), SDCardInfo.CardBlockSize );
//	}
//	else							 /* 多个sector的写操作 */
//  {    
//		SD_WriteMultiBlocks( sector << 9 , (u32 *)(&buff[0]), SDCardInfo.CardBlockSize, count );
//	}        
//  return RES_OK;	
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	if ( count == 1 )		/* 1个sector的写操作 */
	{		/* 注意count不能为0，否则会出错 */		
		//SD_WriteBlock( sector << 9 ,(u32 *)(&buff[0]), SDCardInfo.CardBlockSize );
	}
	else							 /* 多个sector的写操作 */
	{    
		//SD_WriteMultiBlocks( sector << 9 , (u32 *)(&buff[0]), SDCardInfo.CardBlockSize, count );
	}        
  return 0;	
//	DRESULT res;
//	//int result;

//	switch (pdrv) {
//	case DEV_RAM :
//		// translate the arguments here

//		//result = RAM_disk_write(buff, sector, count);

//		// translate the reslut code here

//		return res;

//	case DEV_MMC :
//		// translate the arguments here

//		//result = MMC_disk_write(buff, sector, count);

//		// translate the reslut code here

//		return res;

//	case DEV_USB :
//		// translate the arguments here

//		//result = USB_disk_write(buff, sector, count);

//		// translate the reslut code here

//		return res;
//	}

//	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	
	return 0;
//	DRESULT res;
////	int result;

//	switch (pdrv) {
//	case DEV_RAM :

//		// Process of the command for the RAM drive

//		return res;

//	case DEV_MMC :

//		// Process of the command for the MMC/SD card

//		return res;

//	case DEV_USB :

//		// Process of the command the USB drive

//		return res;
//	}

//	return RES_PARERR;
}

