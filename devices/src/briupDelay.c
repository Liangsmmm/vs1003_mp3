/***********************************
* FileName:		briupDelay.c
* CreateDate:	2017-8-24
* Author:		Dennis Chen
* Description:	实现延时的初始化接口
				及按us/ms/s进行延时的接口
* Update:		--------
***********************************/
#include "common.h"

//声明us和ms计数频率的全局静态变量
u32 fac_us,fac_ms;
u32 fac_ms_ucos;

/******************
* FunctionName:	_delay_init( u8 SYSCLK)
* Desciption:	初始化系统时钟的定时器
* Input:		SYSCLK	系统时钟频率(MHz)
* Output:		NULL
******************/
void _delay_init( u8 SYSCLK)
{
#ifdef	OS_MAX_TASKS
	//如果使用了操作系统，则需要计算
	//	"一次时钟节拍所需的重装载寄存器的值"
	u32 reload;
#endif
	SysTick->CTRL = 0;
//如果使用了uCOS-II系统，则进行如下配置
	//系统时钟8分频后得到系统节拍时钟
	reload = SYSCLK / 8;
	//计算1个uCOS-II节拍所需的定时器计数次数
	reload = reload * 1000000 / OS_TICKS_PER_SEC;
	
	
	//计算uCOS-II节拍的时间，即时钟分辨率
	fac_ms_ucos = 1000 / OS_TICKS_PER_SEC;
	
	//装载自动重装载寄存器的值
	SysTick->LOAD = reload;
	
	SysTick->CTRL |= 1<<0;
	fac_us = SYSCLK / 8;
	fac_ms = fac_us * 1000;
}

void _delay_os_start(void)
{
	//使能SysTick中断
	SysTick->CTRL |= 1<<1;
}


//按us延时
void _delay_us( u32 us)
{
	u32 temp;	//检测倒计时是否结束
#ifdef	OS_MAX_TASKS
	temp = SysTick->VAL;
	if( temp >= us)
	{
		temp = temp - us;
		while( SysTick->VAL > temp);
	}else
	{
		temp = SysTick->VAL + SysTick->LOAD - us;
		while( SysTick->VAL < us || SysTick->VAL > temp);
	}
#else
	SysTick->LOAD = fac_us * us;
	SysTick->VAL = 0;
	SysTick->CTRL |= 0x01;
	do
	{
		temp = SysTick->CTRL;
	}while( (temp & 0x01) && !(temp & (1<<16)) );
	SysTick->CTRL &= ~0x01;
#endif
}

//按ms延时	最大值不可超过1864ms
void _delay_ms( u32 ms)
{
	u32 temp, reload;	//检测倒计时是否结束
#ifdef	OS_MAX_TASKS
	if( OSRunning == 1)	//系统处于运行状态
	{
		if( ms >= fac_ms_ucos)	//延时时间大于系统时间分辨率
		{
			OSTimeDly( ms / fac_ms_ucos);
			ms = ms % fac_ms_ucos;
		}
		_delay_us( ms * 1000);
	}
	else
	{
		SysTick->CTRL &= ~0x01;
		reload = SysTick->LOAD;
		SysTick->LOAD = fac_ms * ms;
		SysTick->VAL = 0;
		SysTick->CTRL |= 0x01;
		do
		{
			temp = SysTick->CTRL;
		}while( (temp & 0x01) && !(temp & (1<<16)) );
		
		SysTick->CTRL &= ~0x01;
		SysTick->LOAD = reload;
		SysTick->CTRL |= 0x01;
	}
#else
	SysTick->LOAD = fac_ms * ms;
	SysTick->VAL = 0;
	SysTick->CTRL |= 0x01;
	do
	{
		temp = SysTick->CTRL;
	}while( (temp & 0x01) && !(temp & (1<<16)) );
	
#endif
}

//按s延时
void _delay_s( double s)
{
	u32 ms = (u32)(s * 1000);
	do
	{
		if( ms > 1000)
		{
			_delay_ms(1000);
			ms -= 1000;
		}else
		{
			_delay_ms( ms);
			ms = 0;
		}
	}while( ms > 0);
}

//实现系统节拍的中断服务函数
//	即SysTick的中断服务函数
void SysTick_Handler(void)
{
	//进入中断需要调用的OS API
	OSIntEnter();
	//驱动系统节拍
	OSTimeTick();
	//退出中断API
	OSIntExit();
}





