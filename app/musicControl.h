/**************************************************************
*FileName:       musicControl.h
*Description:    ����MP3���Ƶ����񻷾���ӿ�
*Author:         Penny.liang.
*CreateDate:     2018-1-11
*Update:         ----
*Copyright:      596929003@qq.com
***************************************************************/
#ifndef __CONTROL_H__
#define __CONTROL_H__
#include "common.h"



/**********************************************
* FunctionName: musicControlTask
* Description:  MP3������Ҫ����
* Input:        void *pdata
* Output:       void
***********************************************/

#define MCTRL_TASK_PRIO 30
#define MCTRL_STACK_SIZE 2*1024
extern OS_STK MCTRL_TASK_STACK[ MCTRL_STACK_SIZE ];
void musicControlTask(void * pdata);




#endif
