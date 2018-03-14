#include "uart.h"
#include "sys.h"
u8 Rx1_Buf[Rx1_Buf_Length];
u8 Rx1_P = 0;
u8 Rx1_RP = 0;

u8 Rx2_Buf[Rx2_Buf_Length];
u8 Rx2_P = 0;


void UART1_Init(u32 bound)
{
	RCC->APB2ENR |= 1 << 2;//Enable PA clock
	
	GPIOA->CRH &= 0xfffff00f;//清除PA9,PA10设置位
	GPIOA->CRH |= 0x000004b0;//PA9 复用推皖输出，PA10浮空输入
	
	
	RCC->APB2ENR|=1<<14; //串口使能

    RCC->APB2RSTR|=1<<14;	//复位串口
    RCC->APB2RSTR&=~(1<<14);//
	
    USART1->BRR=0x1D4C;//设置波特率
    USART1->CR1|=0x200c;//设置停止位
}

void UART2_Init(u32 bound)
{
	RCC->APB2ENR |= 1 << 2;//Enable PA clock
	
	GPIOA->CRL &= 0xffff00ff;//清除PA2,PA3设置位
	GPIOA->CRL |= 0x00004b00;//PA2复用推皖输出,PA3浮空输入
	
	RCC->APB1ENR|=1<<17; //串口使能

    RCC->APB1RSTR|=1<<17;	//复位串口
    RCC->APB1RSTR&=~(1<<17);//
	
    USART2->BRR=0x0EA6;//设置波特率
    USART2->CR1|=0x202c;//设置停止位,设置接受中断使能
	
	NVIC_Set(2,1,0x26,2);//设置串口中断
}

void UART1_Send(u8 tx_data)
{
	while((USART1->SR & (0x40)) == 0)//判断发送缓冲区为空
	{
		
	}
	 USART1->DR = tx_data;
}

void UART2_Send(u8 tx_data)
{
	while((USART2->SR & (0x40)) == 0)//判断发送缓冲区为空
	{
		
	}
	 USART2->DR = tx_data;
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 rx_data;
	
	rx_data = USART1->DR;
	Rx1_Buf[Rx1_P] = rx_data;
	Rx1_P ++;
	Rx1_P = Rx1_P % Rx1_Buf_Length;
} 

void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	u8 rx_data;
	
	rx_data = USART2->DR;
	
	Rx2_Buf[Rx2_P] = rx_data;
	Rx2_P ++;
	Rx2_P = Rx2_P % Rx2_Buf_Length;
} 
	
