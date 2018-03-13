#include "sys.h"
#include "tim.h"
#include "uart.h"


void TIM3_Init()
{
	RCC->APB1ENR |= 1<< 1;			//ʹ��TIM3ʱ��
	TIM3->ARR = 9999;				//�Զ���װֵ 10000 /36000000 = 1/3600�� һ�����3600������
	TIM3->CR1 |= 1 << 7;			//����ARRװ�뻺����
	
	TIM3->PSC = 0;					//����Ƶ
	TIM3->DIER|=1<<0;   			//��������ж� �����ж�ָ���� �������		
	TIM3->CR1 |= 1;					//ʹ�ܼ���

	NVIC_Set(1,3,0x1D,2);			//��ռ1�������ȼ�3����2		
}


void TIM2_Init(void)
{
	//�˲������ֶ��޸�IO������
	RCC->APB1ENR|=1<<0;       //TIM3ʱ��ʹ��    
	
	//PA0 
	GPIOA->CRL&=0XFFFFFFF0;//PA7���
	GPIOA->CRL|=0X0000000B;//���ù������ 	  
	GPIOA->ODR|=1<<0;//PA7����	

	RCC->APB1ENR |= 1<< 0;			//ʹ��TIM2ʱ��
	TIM2->CR1 |= 1 << 7;			//����ARRװ�뻺����
	TIM2->ARR = 17999;				//�Զ���װֵ 18M /18000 = 0.001��
	TIM2->PSC = 0;					//����Ƶ
	
	TIM2->DIER |= 1<0;
	TIM2->DIER |= 1<<6;			//�������ж�
	
	TIM2->CR1 |= 1;		

	//TIM2->DIER|=1<<0;   			//��������ж� �����ж�ָ���� �������		
	TIM2->CCMR1 |= 1<< 2;//OC1FE = 1
	TIM2->CCMR1 |= 1<< 3;//OC1PE = 1ʹ��Ԥװ��
	TIM2->CCMR1 |= 7<< 4;//PWMģʽ2//ʹ�ܼ���
	
	NVIC_Set(0,3,28,2);			//��ռ1�������ȼ�3����2		
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	TIM3->SR&=~(1<<0);//����жϱ�־λ 
}


void TIM2_IRQHandler(void)   //TIM3�ж�
{
	TIM2->SR&=~(1<<0);//����жϱ�־λ
}

