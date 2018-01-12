
#include "briupNVIC.h"

static volatile u8 NVIC_Group;	//保存分组号

//NVIC中断分组初始化 !!注意！该函数只能执行一次！
void briupNVICGroupInit(u8 group)
{
	//SCB->AIRCR
	//System Control Block	系统控制块(区)
	u32 temp;
	NVIC_Group = group;
	//通过STM32组号获取CM3内核中断分组号
	group = ~group & 0x07;
	
	temp = SCB->AIRCR;
	temp &= 0x0000f8ff;
	temp |= group << 8;
	temp |= 0x05fa0000;
	SCB->AIRCR = temp;
}

//NVIC外设优先级设置
//channel -> 中断源  stm32f10x.h 中定义的
void briupNVICPriorityConfig( u8 preemptionPriority,\
							u8 subPriority, u8 channel)
{
	//NVIC->IP寄存器组
	//根据中断分组指定4位有效位的抢占优先级位数和...
	u8 tmp;
	//指定有效位数的临时变量
	u8 tmpptr, tmpsub;
	tmpptr = 0x04 - NVIC_Group;
	tmpsub = 0x0f >> NVIC_Group;
	
	tmp = preemptionPriority << tmpptr;
	tmp |= subPriority & tmpsub;
	tmp <<= 4;
	NVIC->IP[channel] = tmp;
	NVIC->ISER[channel>>0x05] |= 1 << (channel & 0x1f);
}

//GPIO外部中断引脚配置
//						端口		位		边沿
void briupExNVICInit( u8 GPIOx, u8 Bitx, u8 TRIM)
{
	//计算AFIO->EXTICR寄存器组的位置
	u8 exAddr, exOffset;
	exAddr = Bitx/4;
	exOffset = (Bitx%4)*4;
	//开启AFIO时钟
	RCC->APB2ENR |= 0x01;
	//将传入的IO引脚映射到对应EXTI线上
	AFIO->EXTICR[exAddr] |= GPIOx << exOffset;
	//开放IO引脚对应外部中断线上的中断屏蔽位
	EXTI->IMR |= 1<<Bitx;
	EXTI->EMR |= 1<<Bitx;
	
	if(TRIM & 0x01)	//上升沿触发
		EXTI->RTSR |= 1<<Bitx;
	if(TRIM & 0x02)	//下降沿触发
		EXTI->FTSR |= 1<<Bitx;
}

