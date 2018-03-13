#ifndef __DV_H__
#define __DV_H__

#include "stm32f10x.h"
#include "uart.h"

void Disp_PV(u16 *pv,u16 *pv_set);			//显示温度和设置温度
void Disp_Work_Sum(u16 *buf);				//显示总周期
void Disp_Setting(void);					//显示设置参数，位置，速度，时间
void Disp_Work_Cnt(u16 *buf);				//显示工作周期
void Disp_Work_State(u8* work_flag);		//显示工作状态
void Go_To_Page(u8 page);					//转到相应页面
void Disp_C_Location(u16 *buf);				//显示当前位置
void Disp_Hand_Step(u16 *buf);				//显示手动步伐
void Disp_Work_Step(u16 *buf);				//显示工作步数
#endif

