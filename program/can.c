#include "can.h"

/*
mode: 0正常模式  1回环模式
*/

void CAN_Init(u8 mode)
{
	//复位CAN
	RCC->APB1RSTR |= (u32)1 <<25;
	RCC->APB1RSTR &= ~((u32)1 << 25);
	
	RCC->APB2ENR |= (u32)1 << 2;//Enable PA clock
	RCC->APB1ENR |= (u32)1 << 25;	//使能CAN1时钟
	
	GPIOA->CRH &= 0xfff00fff;//清除PA11,PA12设置位
	GPIOA->CRH |= 0x000b8000;//PA12 复用推皖输出，PA11上拉输入
	GPIOA->ODR |= 1<<11;//PA11上拉输入
	
	CAN1->MCR &= ~((u32)0x0002);		//退出睡眠模式
	CAN1->MCR |= 1<< 0;					//请求CAN初始化，INRQ = 1

	while((CAN1->MSR & 0x01)==0x00);	//等待进入初始化模式确认 CAN-MSR.INAK确认

	if(mode == 0)
	{
		CAN1->BTR = (u32)(4 << 16) |/* ((u32)1 << 30)|*/0xff;		//	进入会换模式
	}
	else
	{
		CAN1->BTR = (u32)(4 << 16) | ((u32)1 << 30)| 0xff;		//	进入会换模式
	}
	CAN1->BTR = (u32)(4 << 16) |/* ((u32)1 << 30)|*/0xff;		//	进入会换模式
	CAN1->MCR &= ~(0x00000001);// b7=0禁止时间出发  b6=0 不自动离线
	
	CAN1->FA1R = 0;//过滤,0-27对应28个过滤寄存器激活。实际只有0-13共14个过滤寄存器
	
	
	
	//CAN1->IER |= (1 << 1)|(1<<4);//设置FIFO 0/1 挂号使能
						//等待INAK位置1确认进入初始化模式
	CAN1->MCR |= 1<<4;	//b4 = 1报文只发一次，不重发 b4=0 一直重发
	CAN1->MCR &= ~(1 <<1);	//清除CAN_MCR的INRQ位请求退出初始化模式,CAN-MSR INAK位为确认退出初始化模式
	
	//在对bxCAN的过滤组(模式，位宽，FIFO关联，激活，过滤值)进行
	//初始化前，软件要多CAN_FMR寄存器FINIT位置位，对过滤器初始化可以在非初始化模式下进行，FINT = 1时，禁止保温接受
	
	CAN1->MCR &= ~((u32)0x01);//退出初始化模式
	
	while((CAN1->MSR & 0x01)==0x01);	//当INAK SLAK bit = 0,进入正常模式
	
	
	//CAN过滤组配置
	CAN1->FMR |= 0x01;	//FINIT = 1 过滤器组进入初始化模式，过滤器配置需要在初始化模式下配置
	
	CAN1->FS1R = 0;		//FSCx 所有过滤组为2*16位，共14组过滤组
	CAN1->FM1R |= 0x00;	//FBMx =1 过滤组标识符列表模式 0 位屏蔽模式
	CAN1->FFA1R = 0;	//过滤结果放在FIFO0/1,对应位设置对应组
	CAN1->FA1R = 0x0001;//激活过滤组1，每一位对应一个过滤组
	CAN1->sFilterRegister[0].FR1 = 0x00;//14个过滤组,这里不做过滤，接受所有标识符数据
	
	CAN1->FMR = 0;	//过滤组进入正常模式
}

/*
参数:
	u16 id: 标识符 11 bit标识符
	u8 *tx_buf:数据地址
	u8 tx_len:发送的数据长多 0-8字节

返回:
	成功装载返回0,否则返回1
*/
u8 CAN1_TX(u16 id, u8* tx_buf,u8 tx_len)
{
	u8 i;
	u8 can_txbox = 0xff;
	u32 tx_data[2];
	
	for(i=0;i<3;i++)
	{
		if(CAN1->TSR |(1 << (26 + i)))//寻找空邮箱
		{
			can_txbox = i;
			
			break;
		}
		
	}
	
	if(can_txbox!= 0xff)//存在空邮箱
	{
		tx_data[0] = 0;
		tx_data[1] = 0;
		
		//没有数据发送
		if(tx_len == 0)
		{
			tx_data[0] = 0;
			tx_data[1] = 0;
		}
		else
		{
			if(tx_len <= 4)
			{
				for(i=0;i<tx_len;i++)
				{
					tx_data[0] |= (u32)(*(tx_buf + i)) << (i * 8);
				}
			}
			else
			{
				for(i=0;i<tx_len;i++)
				{
					if(i < 4)
					{
						tx_data[0] |= (u32)(*(tx_buf + i)) << (i * 8);
					}
					else
					{
						tx_data[1] |= (u32)(*(tx_buf + i)) << ((i-4) * 8);
					}
				}
			}
		}
		
		CAN1->sTxMailBox[can_txbox].TDTR = 0;			//b31-b16报文时间戳 b8时间戳使能位 b3-0 发送数据长度0-8
		CAN1->sTxMailBox[can_txbox].TDTR |= tx_len;		//设置发送数据长度
		
		//将要发送的数据填入
		CAN1->sTxMailBox[can_txbox].TDLR = tx_data[0];
		CAN1->sTxMailBox[can_txbox].TDHR = tx_data[1];

		
		//CAN发送邮箱寄存器配置
		CAN1->sTxMailBox[can_txbox].TIR = 0;	//邮箱标识寄存器 b28-b18 11bit标识或拓展的高字节标识符 b1=0 数据帧 b1=1远程帧 b0设置1请求发送数据
		CAN1->sTxMailBox[can_txbox].TIR |= ((u32)id << 18);//设置标识符位id,设置位数据帧,请求发送
		
		CAN1->sTxMailBox[can_txbox].TIR |= 0x01;
		
		return 0;	//成功装载数据准备发送
	}
	else
	{
		return 1;	//没有空邮箱，无法发送
	}
	
}
