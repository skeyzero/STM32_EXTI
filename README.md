﻿# STM32_EXTI
STM32 EXTI
   
   
*非互联型产品有19个能产生事件/中断清流的边沿检测器。   
*每个输入线可配置独立的输入类型(脉冲/挂起)和对应触发事件(上下双边触发);
*每个输入线可独立被屏蔽。
挂起寄存器保存着状态线的中断请求。    

主要特性：   

设置中断：
1.配置并使能中断线
2.设置触发方式
3.中断屏蔽寄存器中写1允许中断

中断响应：
1.外部输入产生中断请求，挂起位置1
2.挂起寄存器中相应位写1清除请求


事件雷同：


外部中断线EXTI0-19
其中0-15每个中断线都可以链接对应的Pxn;例如：EXTI0对应PA0-PG0    
其他线连接到别的地方，暂时不用管

寄存器描述：
中断屏蔽寄存器 EXTI->IMR
0-19bit 每一位代表一条中断屏蔽线。写1来开放中断请求。   

事件屏蔽寄存器 EXTI->EMR

上升触发事件选择寄存器 EXTI->RTSR  
下降触发事件选择寄存器 EXTI->FTSR  
0-19bit 每一位标识一条屏蔽线使能触发。1使能

软件中断/事件寄存器
0-19bit 每一位代表一个中断/事件，通过软件写1可以使事件**软件产生**(这里可以用做一些用户自定义的事件，产生后挂起然后进入中断处理，能够快速响应)  


挂起寄存器 EXTI->PR
0-19bit 每一位代表一个中断/事件已经产生，通过写1清除中断位

外部中断寄存器 AFIO_EXTICR1-4  
总共4个32位寄存器。每个寄存器只使用低16位，每4位控制一个EXTIx外部中断输入源。例如：
AFIO->EXTICR1 = 0 |(1<<4)|(2<<8)|(6 <<12);//表示 PA0作为EXTI0外部中断输入，PB1作为EXTI1外部中断输入，PG3作为EXTI3外部中断输入。





