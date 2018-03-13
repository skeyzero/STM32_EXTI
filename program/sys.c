#include "sys.h"

/*
: 设定中断优先级

*/


extern u8 Time_5ms_Flag;
extern u8 Time_50ms_Flag;
extern u8 Time_20ms_Flag;
extern u8 Time_100ms_Flag;
extern u8 Time_500ms_Flag;

/* 使用CRC检验请删除注释 */

unsigned int wCRCTable[] = {
0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };



//系统时钟72M
//AHB不分频，AHB输出也是72M
//APB1分频必须2或以上，因为APB1最大允许36M 这里36M
//定时器2-7频率位72M，因为APB1不为1，因此频率这里需要翻倍AHB / 8
//cortex 系统滴答时钟为72M/8=9M

#define APB1_DIV_1  0x00000000
#define APB1_DIV_2	0x00000400
#define APB1_DIV_4  0x00000500
#define APB1_DIV_8 	0x00000600
#define APB1_DIV_16	0x00000700


#define APB2_DIV_1  ((u32)0 << 11)
#define APB2_DIV_2	((u32)4 << 11)
#define APB2_DIV_4  ((u32)5 << 11)
#define APB2_DIV_8 	((u32)6 << 11)
#define APB2_DIV_16	((u32)7 << 11)


//SYSCLK = 72M
//AHB	 = 72M
//APB1 	 = 18M
//APB2	 = 72M
//USB	 = 78M

void SysClk_Init(void)
{
	RCC->CR|=1<<16;					//HSEON=1
	while((RCC->CR&(1<<17))==0);	//等待外部晶振就绪
	
	RCC->CFGR &= 0xFFFFF8FF;		//清除APB1分频位
	RCC->CFGR|=  APB1_DIV_2;		//APB1输出必须小于36MHz

	RCC->CFGR&=~(1<<17);			//
	RCC->CFGR|=1<<16;				//    ??HSE??PLL???
	FLASH->ACR|=0x32;				//根据FLASH编程手册，这里应该是延时两个时钟，如果配置错误程序读写就不会正常，程序就不能正常运行
	
	RCC->CFGR|=7<<18;				//9倍频
	
	RCC->CFGR&=0xffffff0c;			//清除AHB位，使AHB不分频
	RCC->CFGR|=0x00000002;//PLL作为系统时钟

	RCC->CR|=1<<24;//enable pll
	while((RCC->CR&1<<25)==0);//pll ready
}

void NVIC_Set(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	
	u8 IPRADDR=NVIC_Channel/4;  //每组只能存4个,得到组地址 
	u8 IPROFFSET=NVIC_Channel%4;//在组内的偏移
	IPROFFSET=IPROFFSET*8+4;    //得到偏移的确切位置
	//MY_NVIC_PriorityGroupConfig(NVIC_Group);//设置分组
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;//取低四位

	if(NVIC_Channel<32)NVIC->ISER[0]|=1<<NVIC_Channel;//使能中断位(要清除的话,相反操作就OK)
	else NVIC->ISER[1]|=1<<(NVIC_Channel-32);    
	NVIC->IP[IPRADDR]|=temp<<IPROFFSET;//设置响应优先级和抢断优先级   	    	  				   
}

/*
滴答始终作为定时基准，每一毫秒产生一个中断


*/

void SysTick_Init(void)
{
	SysTick->CTRL |= 1<<1;//计数器倒数到0是产生SysTick异常请求
	SysTick->LOAD = 900;//9M/9000,1ms中断
	SysTick->CTRL |= 1;//使能
}

/*
滴答始终倒数到0异常处理

*/
void SysTick_Handler(void)
{
	static u16 systick_cnt = 0;
	systick_cnt ++;
	if((systick_cnt % 50) == 0)
	{
		Time_5ms_Flag = 1;
	}
	if((systick_cnt % 200) == 0)
	{
		Time_20ms_Flag = 1;
	}
	if((systick_cnt % 500) == 0)
	{
		Time_50ms_Flag = 1;
	}
	if((systick_cnt % 1000) == 0)
	{
		Time_100ms_Flag = 1;
	}
	if((systick_cnt % 5000) == 0)
	{
		Time_500ms_Flag = 1;
	}
	
	systick_cnt = systick_cnt % 5000;
}


u16 CRC16(u8 *buffer,u8 num)
{
//0x06,0x03,0x00,0x00,0x00,0x1A,[0xC5,0xB6]	
//0x03,0x03,0x00,0x00,0x00,0x1A,[0xC5,0xE3]
//	03,03,00,30,00,07,[05,E5]

	unsigned char nTemp;
	unsigned int wCRCWord = 0xFFFF;
	
	   while (num--)
	   {
		  nTemp = *buffer++ ^ wCRCWord;
		  wCRCWord >>= 8;
		  wCRCWord ^= wCRCTable[nTemp];
	   }
	   return wCRCWord;
}	


