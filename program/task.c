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

void Loop_Fast(void)
{

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
	UART1_Send(0xaa);
}

void HW_Init(void)
{
	SysClk_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设定中断分组2，2抢占2子优先级

	SysTick_Init();			//1ms滴答时钟
	Open_SW();
	TIM3_Init();
	
	UART1_Init(9600);
	CAN_Init(0);
}



