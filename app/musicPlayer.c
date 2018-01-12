/**************************************************************
*FileName:       musicPlayer.h
*Description:    声明音乐播放任务得相关环境与接口
*Author:         Penny.liang.
*CreateDate:     2018-1-11
*Update:         ----
*Copyright:      596929003@qq.com
***************************************************************/
#include "musicPlayer.h"
#include "malloc.h"
//实现音乐播放任务得堆栈
OS_STK MUSIC_TASK_STACK[ MUSIC_STACK_SIZE ] __attribute__( (at(0x68100000)));
//歌曲存储路径
const static TCHAR * MUSIC_PATH = "0:/music";

//文件系统相关对象
DIR dir;        //目录对象
FIL file;       //文件对象
FILINFO fno;    //文件信息对象

/**********************************************
* FunctionName: musicPlayerTask
* Description:  音乐播放主要任务
* Input:        void *pdata
* Output:       void
***********************************************/

void musicPlayerTask(void * pdata)
{
    u8 res;
    printf("Music Player Task Is Run!\n");
    //=========================================
    //==========文件系统测试代码===============
    //1. 打开目录
    res = f_opendir(&dir, MUSIC_PATH);
    if(res != FR_OK)
    {
        printf("Dir Open Failed! ErrorCode : %d\n", res);
        while(1)
        {
            _delay_s(3);//系统内部简单得倒计时.......倒数
        }
    }
    
    //分配长文件名得内存空间
    fno.lfsize = _MAX_LFN;   //255
    fno.lfname = (TCHAR*) mymalloc(SRAMIN, _MAX_LFN);
    if( fno.lfname == NULL)
    {
        printf("Long File Name Create Failed!\n");
        OSTaskSuspend(OS_PRIO_SELF);
    }
    
    res = f_readdir(&dir, &fno);
    if( res != FR_OK)
    {
        printf("Read Dir Failed! ErrorCode:%d\n", res);
        OSTaskSuspend(OS_PRIO_SELF);
    }
    printf("File Name :%s\n", fno.lfname);
    
    while(1)
    {
        printf("Player Task Running ... \n");
        OSTimeDlyHMSM(0, 0, 5, 0);
    }
}