#include "uart.h"
#include "mmc_sd.h"
#include "sys.h"
#include "hw.h"
#include "tim.h"
#include "can.h"
//JTAGģʽ����,��������JTAG��ģʽ
//mode:jtag,swdģʽ����;00,ȫʹ��;01,ʹ��SWD;10,ȫ�ر�;
//CHECK OK	



void Open_SW(void)
{
	RCC->APB2ENR|=1<<0;     //��������ʱ��	   
	AFIO->MAPR&=0XF8FFFFFF; //���MAPR��[26:24]
	AFIO->MAPR|=(2 << 24);       //����jtagģʽ
} 

void EXTI_Init(void)
{
	RCC->APB2ENR |= 1<<0;	//������AFIOʱ��
	RCC->APB2ENR |= 1<<3;	//ʹ��GPIOBʱ��
	
	GPIOB->CRL |= 4 << 4;	//PB0����Ϊ��������
	
	AFIO->EXTICR[0] |= 1 << 4;	//ѡ��PB1
	
	EXTI->RTSR |= 1<< 1;	//�����ش���
	EXTI->FTSR |= 1<< 1;	//�½��ش���
	
	NVIC_Set(1,1,7,2);		//���ô����ж�
	EXTI->IMR |= 1 << 1;	//�����ж���1���ж�����
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
	
	EXTI->PR |= 1<< 1;	//����жϱ�־λ
	
	//������
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
	//�½���
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
CAN�����ʼ��Ӧ����Ӳ�����ڳ�ʼ��ģʽʱ����
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�趨�жϷ���2��2��ռ2�����ȼ�
	SysTick_Init();			//1ms�δ�ʱ��
	Open_SW();
	TIM3_Init();
	UART1_Init(9600);
	EXTI_Init();
	CAN_Init(0);
	UART1_Send(0xbb);
}



