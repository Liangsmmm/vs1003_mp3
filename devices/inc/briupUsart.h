/***********************************
* FileName:		briupUsart.h
* CreateDate:	2017-8-25
* Author:		Dennis Chen
* Description:	串口初始化接口、数据发送、接收接口
				的声明
* Update:		--------
***********************************/

#ifndef __USART_H__
#define __USART_H__

#include <stm32f10x.h>
#include <stdio.h>

/******************
* FunctionName:	briupUsart1Init(u32 bound);
* Desciption:	初始化基础外设函数接口
* Input:		u32 bound	需要设置的串口波特率
* Output:		NULL
******************/
void briupUsart1Init( u32 bound);
//通过串口向上位机发送一字节数据
void usart1PutChar( u8 ch);
//通过串口向上位机发送一个字符串
void usart1PutStr( u8 * str);
//通过串口接收一字节数据
u8 usart1GetChar( void);

#endif
