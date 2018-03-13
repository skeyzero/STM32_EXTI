#include "adc.h"
#include "uart.h"
#include "sys.h"
#include "tim.h"
#include "mmc_sd.h"
#include "hw.h"


u8 Time_5ms_Flag = 0;
u8 Time_20ms_Flag = 0;
u8 Time_50ms_Flag = 0;
u8 Time_500ms_Flag = 0;
u8 Time_100ms_Flag = 0;


extern void HW_Init(void);
extern void Loop_Fast(void);
extern void Loop_5ms(void);
extern void Loop_20ms(void);
extern void Loop_50ms(void);
extern void Loop_100ms(void);
extern void Loop_500ms(void);


int main(void)
{
	HW_Init();
	while(1)
	{
		
		Loop_Fast();
		if(Time_5ms_Flag == 1)
		{
			Time_5ms_Flag = 0;
			Loop_5ms();
		}
		
		if(Time_20ms_Flag == 1)
		{
			Time_20ms_Flag = 0;
			Loop_20ms();
		}
		
		if(Time_50ms_Flag == 1)
		{
			Time_50ms_Flag = 0;
			Loop_50ms();
			
		}
		
		if(Time_100ms_Flag == 1)
		{
			Time_100ms_Flag = 0;
			Loop_100ms();
			
		}
		
		if(Time_500ms_Flag == 1)
		{
			Time_500ms_Flag = 0;
			Loop_500ms();
		}
	}
}

