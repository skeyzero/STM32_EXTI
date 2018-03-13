#include "mmc_sd.h"
#include "spi.h"
#include "uart.h"

/*
ʹ��SDƬѡ�ź�
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
����SDƬѡ�ź�
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
//	RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ�� 
//	GPIOA->CRL&=0xfffffff0; 
//	GPIOA->CRL|=0X00000003;//PA0 ���� 	    
//	GPIOA->ODR|=(1<<0);    //PA0���� 
//	
//	UART1_Send(0xBB);
//	SD_Enable();
//	UART1_Send(0xCC);
//	SPI1_Init();			//SPI��ʼ��
//	
//	
//	//SPI1_SetSpeed(SPI_SPEED_256);//���õ�����ģʽ	
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
//	//��λ������74��ʱ�����ں�ſɹ���
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
	u16 Count=0xFFF;//�ȴ�����	   						  
	while ((SPI1_RW_Byte(0XFF)!=Response)&&Count)Count--;//�ȴ��õ�׼ȷ�Ļ�Ӧ  	  
	if (Count==0)return 0xff;//�õ���Ӧʧ��   
	else return 0x00;//��ȷ��Ӧ
}

u8 SD_RecvData(u8*buf,u16 len)
{			  	  
	if(SD_GetResponse(0xFE))return 1;//�ȴ�SD������������ʼ����0xFE
    while(len--)//��ʼ��������
    {
        *buf=SPI1_RW_Byte(0xFF);
        buf++;
    }
    //������2��αCRC��dummy CRC��
    SPI1_RW_Byte(0xFF);
    SPI1_RW_Byte(0xFF);									  					    
    return 0;//��ȡ�ɹ�
}

u8 SD_SendBlock(u8*buf,u8 cmd)
{	
	u16 t;		  	  
	SPI1_RW_Byte(cmd);
	if(cmd!=0XFD)//���ǽ���ָ��
	{
		for(t=0;t<512;t++)SPI1_RW_Byte(buf[t]);//����ٶ�,���ٺ�������ʱ��
	    SPI1_RW_Byte(0xFF);//����crc
	    SPI1_RW_Byte(0xFF);
		t=SPI1_RW_Byte(0xFF);//������Ӧ
		if((t&0x1F)!=0x05)return 2;//��Ӧ����									  					    
	}						 									  					    
    return 0;//д��ɹ�
}

u8 SD_RD_Disk(u8*buf,u32 sector,u8 cnt)
{
	u8 r1;
	//sector <<= 9;//ת��Ϊ�ֽڵ�ַ
	if(cnt==1)
	{
		r1=SD_CMD(17,sector,0X01);//������
		if(r1==0)//ָ��ͳɹ�
		{
			r1=SD_RecvData(buf,512);//����512���ֽ�	   
		}
	}else
	{
		r1=SD_CMD(18,sector,0X01);//����������
		do
		{
			r1=SD_RecvData(buf,512);//����512���ֽ�	 
			buf+=512;  
		}while(--cnt && r1==0); 	
		SD_CMD(12,0,0X01);	//����ֹͣ����
	}   
	//SD_DisSelect();//ȡ��Ƭѡ
	return r1;//
}



u8 SD_WR_Disk(u8*buf,u32 sector,u8 cnt)
{
	u8 r1;
	//if(SD_Type!=SD_TYPE_V2HC)sector *= 512;//ת��Ϊ�ֽڵ�ַ
	if(cnt==1)
	{
		r1=SD_CMD(24,sector,0X01);//������
		if(r1==0)//ָ��ͳɹ�
		{
			r1=SD_SendBlock(buf,0xFE);//д512���ֽ�	   
		}
	}else
	{
		//if(SD_Type!=SD_TYPE_MMC)
		{
			SD_CMD(55,0,0X01);	
			SD_CMD(23,cnt,0X01);//����ָ��	
		}
 		r1=SD_CMD(25,sector,0X01);//����������
		if(r1==0)
		{
			do
			{
				r1=SD_SendBlock(buf,0xFC);//����512���ֽ�	 
				buf+=512;  
			}while(--cnt && r1==0);
			r1=SD_SendBlock(0,0xFD);//����512���ֽ� 
		}
	}   
	//SD_Disable();//ȡ��Ƭѡ
	return r1;//
}

u8 SD_CMD(u8 cmd,u32 arg,u8 crc)
{
	u8 r1;	
	//����
    SPI1_RW_Byte(cmd | 0x40);//�ֱ�д������
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

	//����
    SPI1_RW_Byte(cmd | 0x40);//�ֱ�д������
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


