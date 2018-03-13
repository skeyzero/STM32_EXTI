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

void Loop_Fast(void)
{

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
	UART1_Send(0xaa);
}

void HW_Init(void)
{
	SysClk_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�趨�жϷ���2��2��ռ2�����ȼ�

	SysTick_Init();			//1ms�δ�ʱ��
	Open_SW();
	TIM3_Init();
	
	UART1_Init(9600);
	CAN_Init(0);
}



