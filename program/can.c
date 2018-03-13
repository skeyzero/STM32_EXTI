#include "can.h"

/*
mode: 0����ģʽ  1�ػ�ģʽ
*/

void CAN_Init(u8 mode)
{
	//��λCAN
	RCC->APB1RSTR |= (u32)1 <<25;
	RCC->APB1RSTR &= ~((u32)1 << 25);
	
	RCC->APB2ENR |= (u32)1 << 2;//Enable PA clock
	RCC->APB1ENR |= (u32)1 << 25;	//ʹ��CAN1ʱ��
	
	GPIOA->CRH &= 0xfff00fff;//���PA11,PA12����λ
	GPIOA->CRH |= 0x000b8000;//PA12 �������������PA11��������
	GPIOA->ODR |= 1<<11;//PA11��������
	
	CAN1->MCR &= ~((u32)0x0002);		//�˳�˯��ģʽ
	CAN1->MCR |= 1<< 0;					//����CAN��ʼ����INRQ = 1

	while((CAN1->MSR & 0x01)==0x00);	//�ȴ������ʼ��ģʽȷ�� CAN-MSR.INAKȷ��

	if(mode == 0)
	{
		CAN1->BTR = (u32)(4 << 16) |/* ((u32)1 << 30)|*/0xff;		//	����ỻģʽ
	}
	else
	{
		CAN1->BTR = (u32)(4 << 16) | ((u32)1 << 30)| 0xff;		//	����ỻģʽ
	}
	CAN1->BTR = (u32)(4 << 16) |/* ((u32)1 << 30)|*/0xff;		//	����ỻģʽ
	CAN1->MCR &= ~(0x00000001);// b7=0��ֹʱ�����  b6=0 ���Զ�����
	
	CAN1->FA1R = 0;//����,0-27��Ӧ28�����˼Ĵ������ʵ��ֻ��0-13��14�����˼Ĵ���
	
	
	
	//CAN1->IER |= (1 << 1)|(1<<4);//����FIFO 0/1 �Һ�ʹ��
						//�ȴ�INAKλ��1ȷ�Ͻ����ʼ��ģʽ
	CAN1->MCR |= 1<<4;	//b4 = 1����ֻ��һ�Σ����ط� b4=0 һֱ�ط�
	CAN1->MCR &= ~(1 <<1);	//���CAN_MCR��INRQλ�����˳���ʼ��ģʽ,CAN-MSR INAKλΪȷ���˳���ʼ��ģʽ
	
	//�ڶ�bxCAN�Ĺ�����(ģʽ��λ��FIFO�������������ֵ)����
	//��ʼ��ǰ�����Ҫ��CAN_FMR�Ĵ���FINITλ��λ���Թ�������ʼ�������ڷǳ�ʼ��ģʽ�½��У�FINT = 1ʱ����ֹ���½���
	
	CAN1->MCR &= ~((u32)0x01);//�˳���ʼ��ģʽ
	
	while((CAN1->MSR & 0x01)==0x01);	//��INAK SLAK bit = 0,��������ģʽ
	
	
	//CAN����������
	CAN1->FMR |= 0x01;	//FINIT = 1 ������������ʼ��ģʽ��������������Ҫ�ڳ�ʼ��ģʽ������
	
	CAN1->FS1R = 0;		//FSCx ���й�����Ϊ2*16λ����14�������
	CAN1->FM1R |= 0x00;	//FBMx =1 �������ʶ���б�ģʽ 0 λ����ģʽ
	CAN1->FFA1R = 0;	//���˽������FIFO0/1,��Ӧλ���ö�Ӧ��
	CAN1->FA1R = 0x0001;//���������1��ÿһλ��Ӧһ��������
	CAN1->sFilterRegister[0].FR1 = 0x00;//14��������,���ﲻ�����ˣ��������б�ʶ������
	
	CAN1->FMR = 0;	//�������������ģʽ
}

/*
����:
	u16 id: ��ʶ�� 11 bit��ʶ��
	u8 *tx_buf:���ݵ�ַ
	u8 tx_len:���͵����ݳ��� 0-8�ֽ�

����:
	�ɹ�װ�ط���0,���򷵻�1
*/
u8 CAN1_TX(u16 id, u8* tx_buf,u8 tx_len)
{
	u8 i;
	u8 can_txbox = 0xff;
	u32 tx_data[2];
	
	for(i=0;i<3;i++)
	{
		if(CAN1->TSR |(1 << (26 + i)))//Ѱ�ҿ�����
		{
			can_txbox = i;
			
			break;
		}
		
	}
	
	if(can_txbox!= 0xff)//���ڿ�����
	{
		tx_data[0] = 0;
		tx_data[1] = 0;
		
		//û�����ݷ���
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
		
		CAN1->sTxMailBox[can_txbox].TDTR = 0;			//b31-b16����ʱ��� b8ʱ���ʹ��λ b3-0 �������ݳ���0-8
		CAN1->sTxMailBox[can_txbox].TDTR |= tx_len;		//���÷������ݳ���
		
		//��Ҫ���͵���������
		CAN1->sTxMailBox[can_txbox].TDLR = tx_data[0];
		CAN1->sTxMailBox[can_txbox].TDHR = tx_data[1];

		
		//CAN��������Ĵ�������
		CAN1->sTxMailBox[can_txbox].TIR = 0;	//�����ʶ�Ĵ��� b28-b18 11bit��ʶ����չ�ĸ��ֽڱ�ʶ�� b1=0 ����֡ b1=1Զ��֡ b0����1����������
		CAN1->sTxMailBox[can_txbox].TIR |= ((u32)id << 18);//���ñ�ʶ��λid,����λ����֡,������
		
		CAN1->sTxMailBox[can_txbox].TIR |= 0x01;
		
		return 0;	//�ɹ�װ������׼������
	}
	else
	{
		return 1;	//û�п����䣬�޷�����
	}
	
}
