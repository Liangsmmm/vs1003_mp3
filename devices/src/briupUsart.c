/***********************************
* FileName:		briupUsart.c
* CreateDate:	2017-8-25
* Author:		Dennis Chen
* Description:	串口初始化接口、数据发送、接收接口
				的实现
* Update:		--------
***********************************/

#include "briupUsart.h"
#include "briupNVIC.h"

//用于开启或关闭printf输出重定向功能
#if 1

struct __FILE{
	int handle;
};

FILE __stdout;
_sys_exit( int x)
{
	x = x;
}

//fputc输出重定向操作
//ch 要发送的字符
//FILE * f	stdout 标准输出流
int fputc( int ch, FILE * f)
{
	while( !(USART1->SR & 1<<6));
	USART1->DR = ch;
	return ch;
}

#endif

/******************
* FunctionName:	briupUsart1Init(u32 bound);
* Desciption:	初始化基础外设函数接口
* Input:		u32 bound	需要设置的串口波特率
* Output:		NULL
******************/
void briupUsart1Init( u32 bound)
{
	//============分割线==================
	//		计算波特率寄存器的值
	double temp = 72000000.0/(16.0*bound);
	u16 mantissa = (u16)temp;
	u16 fraction = (temp - mantissa)*16;
	mantissa = (mantissa<<4) + fraction;
	//====================================
	//		IO端口  寄存器配置部分
	//使能GPIOA/USART1时钟控制
	RCC->APB2ENR |= 1<<2 | 1<<14;
	
	GPIOA->CRH &= 0xfffff00f;
	//Tx -> PA9  推挽复用输出
	GPIOA->CRH |= 0x0b<<4;
	
	//Rx -> PA10 上拉输入
	GPIOA->CRH |= 0x08<<8;
	GPIOA->ODR |= 1<<10;
	//=============================
	//		USART1	寄存器配置
	USART1->BRR = mantissa;
	USART1->CR1 = 1<<13 | 1<<3 | 1<<2;
	//=============================
	//		USART1中断配置
	USART1->CR1 |= 1<<5;	//使能 接收缓冲区非空中断
	briupNVICPriorityConfig( 3, 3, USART1_IRQn);
}

//通过串口向上位机发送一字节数据
void usart1PutChar( u8 ch)
{
	//通过状态位检测(前一次)是否发送完成
	while( !(USART1->SR & (1<<6)));
	//将待发送的数据放入串口数据寄存器
	USART1->DR = ch;
}

//通过串口向上位机发送一个字符串
void usart1PutStr( u8 * str)
{
	u8 * temp = str;
	while( *temp != 0 && *temp != '\n')
	{
		usart1PutChar(*temp);
		temp ++;
	}
	usart1PutChar('\n');
}

//通过串口接收一字节数据
u8 usart1GetChar( void)
{
	//判断当前数据寄存器中是否有未接收数据
	while( !(USART1->SR & 1<<5));
	//若有，则返回数据寄存器中的值（读取数据寄存器）
	return USART1->DR;
}

//串口接收中断处理函数
void USART1_IRQHandler()
{
	//判断是否 是接收缓冲器非空引发的中断
	if( USART1->SR & (1<<5))	//读数据寄存器非空
	{
		printf("From Computer:%c\n",usart1GetChar());
	}
}


