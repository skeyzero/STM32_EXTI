#include "sys.h"
#include "tim.h"
#include "uart.h"


void TIM3_Init()
{
	RCC->APB1ENR |= 1<< 1;			//使能TIM3时钟
	TIM3->ARR = 9999;				//自动重装值 10000 /36000000 = 1/3600秒 一秒产生3600个脉冲
	TIM3->CR1 |= 1 << 7;			//允许ARR装入缓冲器
	
	TIM3->PSC = 0;					//不分频
	TIM3->DIER|=1<<0;   			//允许更新中断 更新中断指的是 计数溢出		
	TIM3->CR1 |= 1;					//使能计数

	NVIC_Set(1,3,0x1D,2);			//抢占1，子优先级3，组2		
}


void TIM2_Init(void)
{
	//此部分需手动修改IO口设置
	RCC->APB1ENR|=1<<0;       //TIM3时钟使能    
	
	//PA0 
	GPIOA->CRL&=0XFFFFFFF0;//PA7输出
	GPIOA->CRL|=0X0000000B;//复用功能输出 	  
	GPIOA->ODR|=1<<0;//PA7上拉	

	RCC->APB1ENR |= 1<< 0;			//使能TIM2时钟
	TIM2->CR1 |= 1 << 7;			//允许ARR装入缓冲器
	TIM2->ARR = 17999;				//自动重装值 18M /18000 = 0.001秒
	TIM2->PSC = 0;					//不分频
	
	TIM2->DIER |= 1<0;
	TIM2->DIER |= 1<<6;			//允许触发中断
	
	TIM2->CR1 |= 1;		

	//TIM2->DIER|=1<<0;   			//允许更新中断 更新中断指的是 计数溢出		
	TIM2->CCMR1 |= 1<< 2;//OC1FE = 1
	TIM2->CCMR1 |= 1<< 3;//OC1PE = 1使能预装载
	TIM2->CCMR1 |= 7<< 4;//PWM模式2//使能计数
	
	NVIC_Set(0,3,28,2);			//抢占1，子优先级3，组2		
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	TIM3->SR&=~(1<<0);//清除中断标志位 
}


void TIM2_IRQHandler(void)   //TIM3中断
{
	TIM2->SR&=~(1<<0);//清除中断标志位
}

