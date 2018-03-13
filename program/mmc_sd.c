#include "mmc_sd.h"
#include "spi.h"
#include "uart.h"

/*
使能SD片选信号
*/
void SD_Enable(void)
{
	u8 r1;
	SD_CS_CLR();
	do
	{
		r1 = SPI1_RW_Byte(0xff);
		UART1_Send(r1);
	}while(r1 != 0xff);
}


/*
除能SD片选信号
*/
void SD_Disable(void)
{
	SD_CS_SET();
	SPI1_RW_Byte(0xff);
}

void SD_Init(void)
{
//	u8 i;
//	u8 r1;
//	
//	RCC->APB2ENR|=1<<2;       //PORTA时钟使能 
//	GPIOA->CRL&=0xfffffff0; 
//	GPIOA->CRL|=0X00000003;//PA0 推挽 	    
//	GPIOA->ODR|=(1<<0);    //PA0上拉 
//	
//	UART1_Send(0xBB);
//	SD_Enable();
//	UART1_Send(0xCC);
//	SPI1_Init();			//SPI初始化
//	
//	
//	//SPI1_SetSpeed(SPI_SPEED_256);//设置到低速模式	
//	
//	UART1_Send(0xdd);
//	
//	for(i=0;i<10;i++)
//	{
//		SPI1_RW_Byte(0xff);
//	}
//	UART1_Send(0xcc);
//	
//	while(1)
//	{
//		r1 = SD_CMD(0,0,0x01);
//		UART1_Send(r1);
//	}
//		UART1_Send(0xaa);
//	if(r1 == 0x01)
//	{
//		UART1_Send('I');
//		UART1_Send('D');
//		UART1_Send('L');
//		UART1_Send('E');
//		UART1_Send('\n');
//	}
//	else
//	{
//		UART1_Send('S');
//		UART1_Send('D');
//		UART1_Send('I');
//		UART1_Send('O');
//		UART1_Send('\n');
//	}
//	//复位后至少74个时钟周期后才可工作
//	for(i=0;i<10;i++)
//	{
//		SPI1_RW_Byte(0xff);
//	}
//	
//	while(1)
//	{
//		r1 = SD_CMD(1,0x00,0x01);
//		UART1_Send(r1);
//		if((r1&0x01) == 0)
//		{
//			break;
//		}
//		else
//		{
//		}
//	}
////	UART1_Send(0xaa);
////	UART1_Send(SD_CMD(8,0x00,0x01));
//	while(1)
//	{
//		
//		if(SD_CMD(8,0x00,0x01)==1)
//		{
//			UART1_Send('V');
//			UART1_Send('2');
//			UART1_Send('.');
//			UART1_Send('0');
//			UART1_Send('\n');
//			break;
//		}
//	}
}

u8 SD_GetResponse(u8 Response)
{
	u16 Count=0xFFF;//等待次数	   						  
	while ((SPI1_RW_Byte(0XFF)!=Response)&&Count)Count--;//等待得到准确的回应  	  
	if (Count==0)return 0xff;//得到回应失败   
	else return 0x00;//正确回应
}

u8 SD_RecvData(u8*buf,u16 len)
{			  	  
	if(SD_GetResponse(0xFE))return 1;//等待SD卡发回数据起始令牌0xFE
    while(len--)//开始接收数据
    {
        *buf=SPI1_RW_Byte(0xFF);
        buf++;
    }
    //下面是2个伪CRC（dummy CRC）
    SPI1_RW_Byte(0xFF);
    SPI1_RW_Byte(0xFF);									  					    
    return 0;//读取成功
}

u8 SD_SendBlock(u8*buf,u8 cmd)
{	
	u16 t;		  	  
	SPI1_RW_Byte(cmd);
	if(cmd!=0XFD)//不是结束指令
	{
		for(t=0;t<512;t++)SPI1_RW_Byte(buf[t]);//提高速度,减少函数传参时间
	    SPI1_RW_Byte(0xFF);//忽略crc
	    SPI1_RW_Byte(0xFF);
		t=SPI1_RW_Byte(0xFF);//接收响应
		if((t&0x1F)!=0x05)return 2;//响应错误									  					    
	}						 									  					    
    return 0;//写入成功
}

u8 SD_RD_Disk(u8*buf,u32 sector,u8 cnt)
{
	u8 r1;
	//sector <<= 9;//转换为字节地址
	if(cnt==1)
	{
		r1=SD_CMD(17,sector,0X01);//读命令
		if(r1==0)//指令发送成功
		{
			r1=SD_RecvData(buf,512);//接收512个字节	   
		}
	}else
	{
		r1=SD_CMD(18,sector,0X01);//连续读命令
		do
		{
			r1=SD_RecvData(buf,512);//接收512个字节	 
			buf+=512;  
		}while(--cnt && r1==0); 	
		SD_CMD(12,0,0X01);	//发送停止命令
	}   
	//SD_DisSelect();//取消片选
	return r1;//
}



u8 SD_WR_Disk(u8*buf,u32 sector,u8 cnt)
{
	u8 r1;
	//if(SD_Type!=SD_TYPE_V2HC)sector *= 512;//转换为字节地址
	if(cnt==1)
	{
		r1=SD_CMD(24,sector,0X01);//读命令
		if(r1==0)//指令发送成功
		{
			r1=SD_SendBlock(buf,0xFE);//写512个字节	   
		}
	}else
	{
		//if(SD_Type!=SD_TYPE_MMC)
		{
			SD_CMD(55,0,0X01);	
			SD_CMD(23,cnt,0X01);//发送指令	
		}
 		r1=SD_CMD(25,sector,0X01);//连续读命令
		if(r1==0)
		{
			do
			{
				r1=SD_SendBlock(buf,0xFC);//接收512个字节	 
				buf+=512;  
			}while(--cnt && r1==0);
			r1=SD_SendBlock(0,0xFD);//接收512个字节 
		}
	}   
	//SD_Disable();//取消片选
	return r1;//
}

u8 SD_CMD(u8 cmd,u32 arg,u8 crc)
{
	u8 r1;	
	//发送
    SPI1_RW_Byte(cmd | 0x40);//分别写入命令
    SPI1_RW_Byte(arg >> 24);
    SPI1_RW_Byte(arg >> 16);
    SPI1_RW_Byte(arg >> 8);
    SPI1_RW_Byte(arg);	  
    SPI1_RW_Byte(crc); 
	UART1_Send(0xaa);
	do
	{
		r1 = SPI1_RW_Byte(0xFF);
		UART1_Send(r1);
	}while((r1&0X80));	 
UART1_Send(0xBB);
    return r1;
}


u8 SD_ACMD(u8 cmd,u32 arg,u8 crc)
{
	u8 r1;	

	//发送
    SPI1_RW_Byte(cmd | 0x40);//分别写入命令
    SPI1_RW_Byte(arg >> 24);
    SPI1_RW_Byte(arg >> 16);
    SPI1_RW_Byte(arg >> 8);
    SPI1_RW_Byte(arg);	  
    SPI1_RW_Byte(crc); 

	do
	{
		r1 = SPI1_RW_Byte(0xFF);
	}while(r1&0X80);	 

    return r1;
}


