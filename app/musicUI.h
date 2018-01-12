/**************************************************************
*FileName:       musicUI.h
*Description:    绘制MP3界面的任务环境与接口
*Author:         Penny.liang.
*CreateDate:     2018-1-11
*Update:         ----
*Copyright:      596929003@qq.com
***************************************************************/
#ifndef __UI_H__
#define __UI_H__
#include "common.h"



/**********************************************
* FunctionName: musicUITask
* Description:  MP3界面主要任务
* Input:        void *pdata
* Output:       void
***********************************************/

#define MUI_TASK_PRIO 31
#define MUI_STACK_SIZE 5*1024
extern OS_STK MUI_TASK_STACK[ MUI_STACK_SIZE ];
void musicUITask(void * pdata);




#endif
