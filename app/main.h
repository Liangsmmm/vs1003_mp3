/**************************************************************
*FileName:       main.h
*Description:    用于声明任务所需的堆栈、函数等
*Author:         Penny.liang.
*CreateDate:     2018-1-11
*Update:         ----
*Copyright:      596929003@qq.com
***************************************************************/
#ifndef __MAIN_H__
#define __MAIN_H__
#include "common.h"
#include "musicPlayer.h"
#include "musicUI.h"
#include "musicControl.h"

//LED状态灯得任务 条件编译开关
#define LED_TASK_EN 1

/**********************************************
* FunctionName: startTask
* Description:  uC/OS启动任务
* Input:        void *pdata
* Output:       
***********************************************/
#define START_TASK_PRIO  50
#define START_STACK_SIZE 256
static OS_STK START_TASK_STACK[ START_STACK_SIZE ];
void startTask( void * pdata);

#if LED_TASK_EN
/**********************************************
* FunctionName: ledTask
* Description:  状态指示灯得任务相关接口
* Input:        void *pdata
* Output:       void
***********************************************/
#define LED_TASK_PRIO 45
#define LED_STACK_SIZE 64
static OS_STK LED_TASK_STACK[ LED_STACK_SIZE ];
void LEDTask( void * pdata);
#endif

#endif





