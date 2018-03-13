#ifndef __DV_H__
#define __DV_H__

#include "stm32f10x.h"
#include "uart.h"

void Disp_PV(u16 *pv,u16 *pv_set);			//��ʾ�¶Ⱥ������¶�
void Disp_Work_Sum(u16 *buf);				//��ʾ������
void Disp_Setting(void);					//��ʾ���ò�����λ�ã��ٶȣ�ʱ��
void Disp_Work_Cnt(u16 *buf);				//��ʾ��������
void Disp_Work_State(u8* work_flag);		//��ʾ����״̬
void Go_To_Page(u8 page);					//ת����Ӧҳ��
void Disp_C_Location(u16 *buf);				//��ʾ��ǰλ��
void Disp_Hand_Step(u16 *buf);				//��ʾ�ֶ�����
void Disp_Work_Step(u16 *buf);				//��ʾ��������
#endif

