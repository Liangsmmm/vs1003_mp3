/***********************************
* FileName:		briupKeyboard.h
* CreateDate:	2017-8-25
* Author:		Dennis Chen
* Description:	用于声明按键初始化接口函数
				与按键键值检测接口函数
* Update:		--------
***********************************/
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stm32f10x.h>

//中  上  下  左  右
//6   11   7   9   4	bit
//枚举类型定义
typedef  enum{
	KEY_NONE = 0,
	KEY_RGT = 1<<4,
	KEY_SEL = 1<<6,
	KEY_DOW = 1<<7,
	KEY_LFT = 1<<9,
	KEY_UP = 1<<11
}KEY_VALUE;

/******************
* FunctionName:	briupKeyInit(void);
* Desciption:	初始化五向按键的IO引脚
* Input:		NULL
* Output:		NULL
******************/
void briupKeyInit(void);

/******************
* FunctionName:	getKeyValue(void);
* Desciption:	检测按键，并返回按键键值
* Input:		NULL
* Output:		KEY_VALUE 返回按键对应键值
******************/
KEY_VALUE getKeyValue(void);

#endif
