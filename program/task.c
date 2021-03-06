#include "uart.h"
#include "mmc_sd.h"
#include "sys.h"
#include "hw.h"
#include "tim.h"
#include "can.h"
//JTAG模式设置,用于设置JTAG的模式
//mode:jtag,swd模式设置;00,全使能;01,使能SWD;10,全关闭;
//CHECK OK	



void Open_SW(void)
{
	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
	AFIO->MAPR&=0XF8FFFFFF; //清除MAPR的[26:24]
	AFIO->MAPR|=(2 << 24);       //设置jtag模式
} 

void EXTI_Init(void)
{
	RCC->APB2ENR |= 1<<0;	//开启辅AFIO时钟
	RCC->APB2ENR |= 1<<3;	//使能GPIOB时钟
	
	GPIOB->CRL |= 4 << 4;	//PB0设置为浮空输入
	
	AFIO->EXTICR[0] |= 1 << 4;	//选择PB1
	
	EXTI->RTSR |= 1<< 1;	//上升沿触发
	EXTI->FTSR |= 1<< 1;	//下降沿触发
	
	NVIC_Set(1,1,7,2);		//设置串口中断
	EXTI->IMR |= 1 << 1;	//开放中断线1的中断请求
}

u8 exti_flag = 0;

u8 exit_cnt = 0;
u8 exit_low_cnt = 0;

u8 RF433_Buf[128];
u8 RF433_Pt = 0;
void EXTI1_IRQHandler(void)
{
	static u8 begin = 0;
	static u8 len = 0;
	
	EXTI->PR |= 1<< 1;	//清除中断标志位
	
	//上升沿
	if(GPIOB->IDR &0x02)
	{
		exit_cnt = 0;
		len = 0;
		if(begin == 0)
		{
			if(exit_low_cnt > 13)
			{
				begin = 1;
				UART1_Send(0xaa);	
			}
		}
		else
		{
			if(exit_low_cnt > 13)
			begin = 0;
			//UART1_Send(0xbb);	
		}
	}
	//下降沿
	else
	{
		exit_low_cnt = 0;
		if(begin == 1)
		{
			//if(((exit_cnt >= 3) && (exit_cnt <=5))||((exit_cnt >= 9) && (exit_cnt <=10)))
			if(((exit_cnt >= 2) && (exit_cnt <=5)))	
			{
				//RF433_Buf[RF433_Pt] = exit_cnt;
				UART1_Send(0x00);	
			}
			else if(((exit_cnt >= 8) && (exit_cnt <=11)))
			{
				UART1_Send(0x01);	
			}
			else
			{
				begin = 0;
				//UART1_Send(0xff);
			}
		}
	}
	//exti_flag = 1;
}


void Loop_Fast(void)
{
//	if(exti_flag == 1)
//	{
//		exti_flag = 0;
//		UART1_Send(0xbb);
//	}
}

/*
CAN软件初始化应该在硬件处于初始化模式时进行
*/
void Loop_5ms(void)
{
	
}

void Loop_20ms(void)
{
	
}

void Loop_50ms(void)
{

}

void Loop_100ms(void)
{
	
}

void Loop_500ms(void)
{
	
}

void HW_Init(void)
{
	SysClk_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设定中断分组2，2抢占2子优先级
	SysTick_Init();			//1ms滴答时钟
	Open_SW();
	TIM3_Init();
	UART1_Init(9600);
	EXTI_Init();
	CAN_Init(0);
	UART1_Send(0xbb);
}



