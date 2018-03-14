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
	
	GPIOA->CRH &= 0xfffff00f;//���PA9,PA10����λ
	GPIOA->CRH |= 0x000004b0;//PA9 �������������PA10��������
	
	
	RCC->APB2ENR|=1<<14; //����ʹ��

    RCC->APB2RSTR|=1<<14;	//��λ����
    RCC->APB2RSTR&=~(1<<14);//
	
    USART1->BRR=0x1D4C;//���ò�����
    USART1->CR1|=0x200c;//����ֹͣλ
}

void UART2_Init(u32 bound)
{
	RCC->APB2ENR |= 1 << 2;//Enable PA clock
	
	GPIOA->CRL &= 0xffff00ff;//���PA2,PA3����λ
	GPIOA->CRL |= 0x00004b00;//PA2�����������,PA3��������
	
	RCC->APB1ENR|=1<<17; //����ʹ��

    RCC->APB1RSTR|=1<<17;	//��λ����
    RCC->APB1RSTR&=~(1<<17);//
	
    USART2->BRR=0x0EA6;//���ò�����
    USART2->CR1|=0x202c;//����ֹͣλ,���ý����ж�ʹ��
	
	NVIC_Set(2,1,0x26,2);//���ô����ж�
}

void UART1_Send(u8 tx_data)
{
	while((USART1->SR & (0x40)) == 0)//�жϷ��ͻ�����Ϊ��
	{
		
	}
	 USART1->DR = tx_data;
}

void UART2_Send(u8 tx_data)
{
	while((USART2->SR & (0x40)) == 0)//�жϷ��ͻ�����Ϊ��
	{
		
	}
	 USART2->DR = tx_data;
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 rx_data;
	
	rx_data = USART1->DR;
	Rx1_Buf[Rx1_P] = rx_data;
	Rx1_P ++;
	Rx1_P = Rx1_P % Rx1_Buf_Length;
} 

void USART2_IRQHandler(void)                	//����1�жϷ������
{
	u8 rx_data;
	
	rx_data = USART2->DR;
	
	Rx2_Buf[Rx2_P] = rx_data;
	Rx2_P ++;
	Rx2_P = Rx2_P % Rx2_Buf_Length;
} 
	
