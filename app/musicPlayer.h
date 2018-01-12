/**************************************************************
*FileName:       musicPlayer.h
*Description:    声明音乐播放任务得相关环境与接口
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
* Description:  音乐播放主要任务
* Input:        void *pdata
* Output:       void
***********************************************/

#define MUSIC_TASK_PRIO 32
#define MUSIC_STACK_SIZE 2*1024
extern OS_STK MUSIC_TASK_STACK[ MUSIC_STACK_SIZE ];
void musicPlayerTask(void * pdata);




#endif
