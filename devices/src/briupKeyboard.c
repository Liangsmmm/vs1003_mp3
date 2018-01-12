/***********************************
* FileName:		briupKeyboard.c
* CreateDate:	2017-8-25
* Author:		Dennis Chen
* Description:	实现按键初始化操作和获取按键键值
* Update:		--------
************************************/

#include "briupKeyboard.h"
#include "briupNVIC.h"
#include "briupBasePeriph.h"
#include "briupDelay.h"
#include "briupTIM2.h"

/******************
* FunctionName:	briupKeyInit(void);
* Desciption:	初始化五向按键的IO引脚
* Input:		NULL
* Output:		NULL
******************/
void briupKeyInit(void)
{
	//开启GPIOC/GPIOG的时钟
	//RCC->APB2ENR = RCC->APB2ENR | ( 1<<4 | 1<<8);
	RCC->APB2ENR |= 1<<4 | 1<<8;
	
	//配置按键对应的IO引脚的模式
		//清除复位值
	//GPIOG Pin.6/7/9/11  ---  GPIOC Pin.4
	GPIOG->CRL &= 0x00ffffff;	//清除对应引脚配置
	GPIOG->CRH &= 0xffff0f0f;
	
	//将按键引脚配置为输入(上/下拉输入)
	GPIOG->CRL |= 0x88000000;
	GPIOG->CRH |= 0x00008080;
	GPIOG->ODR |= 1<<6 | 1<<7 | 1<<9 | 1<<11;
	
	GPIOC->CRL &= ~(0xf<<16);
	GPIOC->CRL |= 0x08<<16;
	
	GPIOC->ODR |= 1<<4;
	/*
	//select键中断配置
	//调用GPIO映射接口，确定对应引脚和外部中断线的映射
	briupExNVICInit( GPIO_G, 6, 0x02);	//SELECT
	briupExNVICInit( GPIO_G, 7, 0x02);	//DOWN
	briupExNVICInit( GPIO_G, 9, 0x02);	//LEFT
	//设置外部中断线6的优先级和中断源
	briupNVICPriorityConfig( 3, 1, EXTI9_5_IRQn);
	
	//up键中断配置
	briupExNVICInit( GPIO_G, 11, 0x02);
	briupNVICPriorityConfig( 3, 1, EXTI15_10_IRQn);
	
	briupExNVICInit( GPIO_C, 4, 0x02);	//RIGHT
	briupNVICPriorityConfig( 3, 1, EXTI4_IRQn);*/
}

/******************
* FunctionName:	getKeyValue(void);
* Desciption:	检测按键，并返回按键键值
* Input:		NULL
* Output:		KEY_VALUE 返回按键对应键值
******************/
KEY_VALUE getKeyValue(void)
{
	KEY_VALUE key = KEY_NONE;
	
	if(!(GPIOG->IDR & KEY_SEL))	//选择键
	{
		_delay_ms(4);
		if(!(GPIOG->IDR & KEY_SEL))
			key |= (1<<4);
	}
		
	if(!(GPIOG->IDR & KEY_UP))	//上
	{
		_delay_ms(4);
		if(!(GPIOG->IDR & KEY_UP))
			key |= (1<<0);
	}
	if(!(GPIOG->IDR & KEY_DOW))	//下
	{
		_delay_ms(4);
		if(!(GPIOG->IDR & KEY_DOW))
			key |= (1<<1);
	}
	if(!(GPIOG->IDR & KEY_LFT))	//左
	{
		_delay_ms(4);
		if(!(GPIOG->IDR & KEY_LFT))
			key |= (1<<2);
	}
	if(!(GPIOC->IDR & KEY_RGT))	//右
	{
		_delay_ms(4);
		if(!(GPIOG->IDR & KEY_RGT))
			key |= (1<<3);
	}
	
	return key;
}
/*
//select中断服务函数
void EXTI9_5_IRQHandler()
{
	//端口引脚输入寄存器判断
	if( !(GPIOG->IDR & 1<<6))	//SEL
	{
		//挂起对应外部中断
		EXTI->PR |= 1<<6;
	}
	if( !(GPIOG->IDR & 1<<7))	//DOWN
	{
		EXTI->PR |= 1<<7;
	}
	if( !(GPIOG->IDR & 1<<9))	//LEFT
	{
		EXTI->PR |= 1<<9;
	}
}

//up键中断服务函数
void EXTI15_10_IRQHandler()
{
	EXTI->PR |= 1<<11;
}

//RIGHT
void EXTI4_IRQHandler()
{
	EXTI->PR |= 1<<4;
}

*/


