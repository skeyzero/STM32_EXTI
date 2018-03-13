#include "hw.h"


#define ST_H() GPIOE->BSRR |= 1 << 3
#define ST_L() GPIOE->BSRR |= 1 << 19

#define SH_H() GPIOE->BSRR |= 1 << 2
#define SH_L() GPIOE->BSRR |= 1 << 18

#define DS_H() GPIOE->BSRR |= 1 << 4
#define DS_L() GPIOE->BSRR |= 1 << 20



void Delayss(void)
{
	u16 i,j;
	for(i=0;i<1000;i++)
	for(j=0;j<1000;j++);
}

/*

*/



u16 Input_State(void)
{
	u16 state = 0;
	state = /*(GPIOD->IDR&0xff)|*/(((GPIOB->IDR >> 3)&0x7F))|((GPIOE->IDR << 15)&(0x8000));
	return state;
}


void HW_Output_Init(void)
{
	RCC->APB2ENR |= 1 << 6;//Enable PE clock
	GPIOE->CRL &= 0xfff000ff;//PE2-4清除配置位
	GPIOE->CRL |= 0x00033300;//推挽输出
	GPIOE->ODR |= 7 << 2;	//PE2-4输出高
}


void HW_Output(u32 dat)
{
	u8 i;
	u8 dat_buf;
	
	ST_L();
	dat_buf = dat >> 24;

	for(i=0;i<8;i++)
	{
	 	SH_L();
		Delayss();
		if(dat_buf&0x01)
		{
			DS_H();
		}
		else
		{
			DS_L();
		}
		Delayss();
		dat_buf = dat_buf >> 1;
		SH_H();
		Delayss();
	}
	dat_buf = dat >> 16;
	for(i=0;i<8;i++)
	{
	 	SH_L();
		Delayss();
		if(dat_buf&0x01)
		{
			DS_H();
		}
		else
		{
			DS_L();
		}
		Delayss();
		dat_buf = dat_buf >> 1;
		SH_H();
		Delayss();
	}
	dat_buf = dat >> 8;
	for(i=0;i<8;i++)
	{
	 	SH_L();
		Delayss();
		if(dat_buf&0x01)
		{
			DS_H();
		}
		else
		{
			DS_L();
		}
		Delayss();
		dat_buf = dat_buf >> 1;
		SH_H();
		Delayss();
	}
	dat_buf = dat;
	for(i=0;i<8;i++)
	{
	 	SH_L();
		Delayss();
		if(dat_buf&0x01)
		{
			DS_H();
		}
		else
		{
			DS_L();
		}
		Delayss();
		dat_buf = dat_buf >> 1;
		SH_H();
		Delayss();
	}
	ST_H();
}





