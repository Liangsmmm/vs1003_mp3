/***********************************
* FileName:		briupBasePeriph.h
* CreateDate:	2017-8-24
* Author:		Dennis Chen
* Description:	用于声明基础外设初始化接口
				及相关宏定义、操作接口
* Update:		--------
***********************************/
#ifndef __BASEPERIPH_H__
#define __BASEPERIPH_H__

#include <stm32f10x.h>

//基础颜色 位定义
#define	LED_R	(1<<2)
#define	LED_G	(1<<1)
#define	LED_B	(1<<0)
#define	LED_ALL	(0x07)
//混合颜色
#define LED_Y	( LED_R | LED_G )
#define LED_P	( LED_R | LED_B )
#define LED_C	( LED_B | LED_G )
#define LED_W	( LED_ALL )

/******************
* FunctionName:	basePeriphInit();
* Desciption:	初始化基础外设函数接口
* Input:		NULL
* Output:		NULL
******************/
void basePeriphInit(void);

/******************
* FunctionName:	setLed(u8 ledStat);
* Desciption:	根据ledStat点亮对应LED
* Input:		ledStat  用低3位对应3个LED灯的状态
* Output:		NULL
******************/
void setLed(u8 ledStat);
//根据ledStat熄灭对应LED灯
void resetLed(u8 ledStat);


void setBeep(void);
void resetBeep(void);

void setRelay(u8 choose);
void resetRelay(u8 choose);

#endif

