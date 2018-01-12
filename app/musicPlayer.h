/**************************************************************
*FileName:       musicPlayer.h
*Description:    �������ֲ����������ػ�����ӿ�
*Author:         Penny.liang.
*CreateDate:     2018-1-11
*Update:         ----
*Copyright:      596929003@qq.com
***************************************************************/
#ifndef __MUSIC_H__
#define __MUSIC_H__
#include "common.h"



/**********************************************
* FunctionName: musicPlayerTask
* Description:  ���ֲ�����Ҫ����
* Input:        void *pdata
* Output:       void
***********************************************/

#define MUSIC_TASK_PRIO 32
#define MUSIC_STACK_SIZE 2*1024
extern OS_STK MUSIC_TASK_STACK[ MUSIC_STACK_SIZE ];
void musicPlayerTask(void * pdata);




#endif
