//briupIWDG.h
//独立看门狗接口声明文件
#ifndef __IWDG_H__
#define	__IWDG_H__

#include "stm32f10x.h"

//独立看门狗初始化接口
//	pr  - 预分频系数
//	rlr - 重装载值
void briupIWDGInit(u16 pr, u16 rlr);

//执行一次喂狗操作
void briupIWDGFeed(void);
//独立看门狗测试程序
void briupIWDGTest(void);

#endif

