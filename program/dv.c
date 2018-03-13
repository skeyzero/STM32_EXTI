#include "dv.h"

extern u16 M_Road[6][8];
extern u16 M_Time[6][8];
extern u8 M_Speed_Div[6];

/*
显示温度
*/
void Disp_PV(u16 *pv,u16 *pv_set)
{
	u8 i;
	static u16 pv_last[6] = {0,0,0,0,0,0};
	static u16 pv_set_last[6] = {0,0,0,0,0,0};
	
	for(i=0;i<6;i++)
	{
		if(*(pv + i)!= pv_last[i])
		{
			pv_last[i] = *(pv + i);
			
			UART2_Send(0x5A);
			UART2_Send(0xA5);
			UART2_Send(0x05);
			UART2_Send(0x82);
			UART2_Send(0x01 + i);
			UART2_Send(0xA0);
			UART2_Send((pv_last[i]/256)%256);
			UART2_Send(pv_last[i]%256);	
		}
		
		if(*(pv_set + i)!= pv_set_last[i])
		{
			pv_set_last[i] = *(pv_set + i);
			
			UART2_Send(0x5A);
			UART2_Send(0xA5);
			UART2_Send(0x05);
			UART2_Send(0x82);
			UART2_Send(0x01 + i);
			UART2_Send(0xA4);
			UART2_Send((pv_set_last[i]/256)%256);
			UART2_Send(pv_set_last[i]%256);	
		}
	}
}


/*显示总周期*/
void Disp_Work_Sum(u16 *buf)
{
	u8 i;
	static u16 buf_last[6];
	for(i=0;i<6;i++)
	{
		if(buf_last[i] != (*(buf + i)))
		{
			buf_last[i] = (*(buf + i));
			UART2_Send(0x5A);
			UART2_Send(0xA5);
			UART2_Send(0x05);
			UART2_Send(0x82);
			UART2_Send(0x01 + i);
			UART2_Send(0xAC);
			UART2_Send((buf_last[i]/256)%256);
			UART2_Send(buf_last[i]%256);	
		}
	}
}

void Disp_C_Location(u16 *buf)
{
	u8 i;
	static u16 buf_last[6];
	for(i=0;i<6;i++)
	{
		if(buf_last[i] != (*(buf + i)))
		{
			buf_last[i] = (*(buf + i));
			UART2_Send(0x5A);
			UART2_Send(0xA5);
			UART2_Send(0x05);
			UART2_Send(0x82);
			UART2_Send(0x01 + i);
			UART2_Send(0xA8);
			UART2_Send((buf_last[i]/256)%256);
			UART2_Send(buf_last[i]%256);	
		}
	}
}


void Disp_Hand_Step(u16 *buf)
{
	u8 i;
	static u16 buf_last[6];
	for(i=0;i<6;i++)
	{
		if(buf_last[i] != (*(buf + i)))
		{
			buf_last[i] = (*(buf + i));
			UART2_Send(0x5A);
			UART2_Send(0xA5);
			UART2_Send(0x05);
			UART2_Send(0x82);
			UART2_Send(0x01 + i);
			UART2_Send(0xB8);
			UART2_Send((buf_last[i]/256)%256);
			UART2_Send(buf_last[i]%256);	
		}
	}
}

void Disp_Reset_State(u8 *state)
{
	u8 i;
	static u8 last_state[6];
	
	for(i=0;i<6;i++)
	{
		if(last_state[i] != (*(state + i)))
		{
			last_state[i] = (*(state + i));
			UART2_Send(0x5a);
			UART2_Send(0xa5);
			UART2_Send(0x05);
			UART2_Send(0x82);
			UART2_Send(0x00);
			UART2_Send(0x50 + i);
			UART2_Send(0x00);
			UART2_Send(last_state[i]);
		}
	}
}

void Disp_Work_Step(u16 *buf)
{
	u8 i;
	static u16 buf_last[6];
	for(i=0;i<6;i++)
	{
		if(buf_last[i] != (*(buf + i)))
		{
			buf_last[i] = (*(buf + i));
			UART2_Send(0x5A);
			UART2_Send(0xA5);
			UART2_Send(0x05);
			UART2_Send(0x82);
			UART2_Send(0x01 + i);
			UART2_Send(0xB0);
			UART2_Send((buf_last[i]/256)%256);
			UART2_Send(buf_last[i]%256);	
		}
	}
}


void Disp_Setting(void)
{
	
	u8 i,j;
	
	static u16 m_road_last[6][8];
	static u16 m_time_last[6][8];
	static u8 m_speed_div[6];
	
	for(i=0;i<6;i++)
	for(j=0;j<8;j++)
	{
		if(m_road_last[i][j] != M_Road[i][j]);
		{
			m_road_last[i][j] = M_Road[i][j];
			UART2_Send(0x5A);
			UART2_Send(0xA5);
			UART2_Send(0x05);
			UART2_Send(0x82);
			UART2_Send(0x01 + i);
			UART2_Send(0x00 + (j*4));
			UART2_Send((m_road_last[i][j]/256)%256);
			UART2_Send(m_road_last[i][j]%256);	
		}
		
		if(m_time_last[i][j] != M_Time[i][j]);
		{
			m_time_last[i][j] = M_Time[i][j];
			UART2_Send(0x5A);
			UART2_Send(0xA5);
			UART2_Send(0x05);
			UART2_Send(0x82);
			UART2_Send(0x01 + i);
			UART2_Send(0x60 + (j*4));
			UART2_Send((m_time_last[i][j]/256)%256);
			UART2_Send(m_time_last[i][j]%256);	
		}
	}
	
	for(i=0;i<6;i++)
	{
		if(m_speed_div[i] != M_Speed_Div[i]);
		{
			m_speed_div[i] = M_Speed_Div[i];
			UART2_Send(0x5A);
			UART2_Send(0xA5);
			UART2_Send(0x05);
			UART2_Send(0x82);
			UART2_Send(0x01 + i);
			UART2_Send(0x30 + (i*4));
			UART2_Send((m_speed_div[i]/256)%256);
			UART2_Send(m_speed_div[i]%256);	
		}	
	}
}


/*显示完成周期*/
void Disp_Work_Cnt(u16 *buf)
{
	u8 i;
	static u16 buf_last[6];
	for(i=0;i<6;i++)
	{
		//if(buf_last[i] != (*(buf + i)))
		{
			buf_last[i] = (*(buf + i));
			UART2_Send(0x5A);
			UART2_Send(0xA5);
			UART2_Send(0x05);
			UART2_Send(0x82);
			UART2_Send(0x01 + i);
			UART2_Send(0xB0);
			UART2_Send((buf_last[i]/256)%256);
			UART2_Send(buf_last[i]%256);	
		}
	}
}

//
void Disp_Work_State(u8* work_flag)
{
	u8 i;
	static u8 last_work_flag[6];
	
	for(i=0;i<6;i++)
	{
		if(last_work_flag[i] != (*(work_flag + i)))
		{
			last_work_flag[i] = (*(work_flag + i));
			UART2_Send(0x5a);
			UART2_Send(0xa5);
			UART2_Send(0x05);
			UART2_Send(0x82);
			UART2_Send(0x00);
			UART2_Send(0x50 + i);
			UART2_Send(0x00);
			UART2_Send(last_work_flag[i]);
		}
	}
}


void Go_To_Page(u8 page)
{
	static u8 last_page = 0;
	if(last_page != page)
	{
		UART2_Send(0x5a);
		UART2_Send(0xa5);
		UART2_Send(0x04);
		UART2_Send(0x80);

		UART2_Send(0x03);
		UART2_Send(0x00);
		UART2_Send(page);	
		last_page = page;
	}
}
