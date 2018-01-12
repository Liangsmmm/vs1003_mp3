
#include "briupTIM2.h"

//定时器2初始化接口
//传入参数：预分频系数
void briupTIM2Init(u16 psc)
{
	RCC->APB1ENR |= 0x01;
	
	TIM2->PSC = psc-1;
	//使定时器为向下计数
	TIM2->CR1 |= 1<<4;
	//使能定时器更新中断
	TIM2->DIER |= 0x01;
	
	//注册定时器2中断向量
	briupNVICPriorityConfig( 2,1, TIM2_IRQn);
}

//开启定时器操作
void briupTIM2Start( u16 arr)
{
	TIM2->ARR = arr;
	TIM2->CR1 |= 0x01;
}

//关闭定时器操作
void briupTIM2Stop(void)
{
	TIM2->CR1 &= ~0x01;
}

//编写中断服务函数
static volatile u8 BeepFlag = 0;
void TIM2_IRQHandler()
{
	if(BeepFlag)
	{
		setBeep();
		BeepFlag = 0;
	}else
	{
		resetBeep();
		BeepFlag = 1;
	}
	//挂起定时器2的更新中断
	TIM2->SR &= ~0x01;
}

