/***********************************
* FileName:		briupDelay.h
* CreateDate:	2017-8-24
* Author:		Dennis Chen
* Description:	声明延时的初始化接口
				及按us/ms/s进行延时的接口
* Update:		--------
***********************************/
#ifndef __DELAY_H__
#define __DELAY_H__

#include <stm32f10x.h>

/******************
* FunctionName:	_delay_init( u8 SYSCLK)
* Desciption:	初始化系统时钟的定时器
* Input:		SYSCLK	系统时钟频率(MHz)
* Output:		NULL
******************/
void _delay_init( u8 SYSCLK);

//启动系统滴答定时器
void _delay_os_start( void );

//按us延时
void _delay_us( u32 us);
//按ms延时
void _delay_ms( u32 ms);
//按s延时
void _delay_s( double s);

#endif
