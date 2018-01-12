/***********************************
* FileName:		briupBasePeriph.c
* CreateDate:	2017-8-24
* Author:		Dennis Chen
* Description:	对基础外设接口的实现文件
* Update:		--------
***********************************/
#include "briupBasePeriph.h"


/******************
* FunctionName:	basePeriphInit();
* Desciption:	初始化基础外设函数接口
* Input:		NULL
* Output:		NULL
******************/
void basePeriphInit(void)
{
	//开启GPIOC的时钟
	RCC->APB2ENR |= 1<<4 | 1<<8;
	
	//设置GPIOC Pin.0/1/2引脚为推挽输出50MHz
	GPIOC->CRL &= ~0xffff;
	GPIOC->CRL |= 0x3333;
	
	GPIOG->CRH &= 0xf00fffff;
	GPIOG->CRH |= 0x33<<20;
	
	resetLed( LED_ALL);
	resetBeep();
	resetRelay(0x03);
}

/******************
* FunctionName:	setLed(u8 ledStat);
* Desciption:	根据ledStat点亮对应LED
* Input:		ledStat  用低3位对应3个LED灯的状态
* Output:		NULL
******************/
void setLed(u8 ledStat)
{
	//使GPIOC Pin.0/1/2输出低电平
	//ODR	BSRR	BRR
	GPIOC->ODR &= ~ledStat;
}

//根据ledStat熄灭对应LED灯
void resetLed(u8 ledStat)
{
	GPIOC->ODR |= ledStat;
}

void setBeep()
{
	GPIOG->ODR |= 1<<14;
}
void resetBeep()
{
	GPIOG->ODR &= ~(1<<14);
}

void setRelay(u8 choose)
{
	if( choose & 0x01)
		GPIOG->ODR |= 1<<13;
	if( choose & 0x02)
		GPIOC->ODR |= 1<<3;
}
void resetRelay(u8 choose)
{
	if( choose & 0x01)
		GPIOG->ODR &= ~(1<<13);
	if( choose & 0x02)
		GPIOC->ODR &= ~(1<<3);
}
