/**************************************************************
*FileName:       musicControl.h
*Description:    绘制MP3界面的任务环境与接口
*Author:         Penny.liang.
*CreateDate:     2018-1-11
*Update:         ----
*Copyright:      596929003@qq.com
***************************************************************/
#include "musicControl.h"
//实现音乐播放任务得堆栈
OS_STK MCTRL_TASK_STACK[ MCTRL_STACK_SIZE ] __attribute__( (at(0x68120000)));


/**********************************************
* FunctionName: musicControlTask
* Description:   MP3界面主要任务
* Input:        void *pdata
* Output:       void
***********************************************/

void musicControlTask(void * pdata)
{
    printf("Control Task Is Run!\n");
    while(1)
    {
        printf("Control Task Running ... \n");
        OSTimeDlyHMSM(0, 0, 5, 0);
    }


    while(1);
}