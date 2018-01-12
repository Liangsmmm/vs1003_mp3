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
//存储歌曲路径及名称得缓冲区
char musicNameBuf[ _MAX_LFN ];

//存储歌曲数据得缓冲区
#define FR_MAX_LEN 512
u8 musicDataBuf[ FR_MAX_LEN ];


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
    u8 res, temp;
    u32 i, j, resLen;
    OS_CPU_SR cpu_sr;
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
    
    //分配长文件名的内存空间
    fno.lfsize = _MAX_LFN;   //255
    fno.lfname = (TCHAR*) mymalloc(SRAMIN, _MAX_LFN);
    if( fno.lfname == NULL)
    {
        printf("Long File Name Create Failed!\n");
        OSTaskSuspend(OS_PRIO_SELF);
    }
    
    i = 0;
    do
    {
        res = f_readdir(&dir, &fno);
        if(fno.fname[0] == 0)
        {
            printf("歌曲读取完毕!");
            f_closedir(&dir);
            break;
        }
        i++;
        printf("%d.%s\n", i, fno.lfname);
        
        //拼接文件完整路径
        memset(musicNameBuf, 0, _MAX_LFN);
        strcat( musicNameBuf, MUSIC_PATH);
        strcat(musicNameBuf, "/");
        //长文件名为0拼接duan短文件名
        if( strlen( fno.lfname) == 0)
            strcat(musicNameBuf, fno.fname);
        else        //不为0则拼接长文件名
            strcat(musicNameBuf, fno.fname);
        printf("-->%s\n", musicNameBuf);
        res = f_open(&file, musicNameBuf, FA_READ);
        if(res != FR_OK)
        {
            printf("文件打开失败!跳过当前文件...\n");
            _delay_ms(100);
            continue;
        }
        while(1)  //播放音乐
        {
            res = f_read(&file, musicDataBuf, FR_MAX_LEN, &resLen);
            if( res != FR_OK || resLen <= 0)
            {
                printf("dangq当前歌曲播放完毕或文件错误...\n");
                f_close(&file);
                break;
            }
            OS_ENTER_CRITICAL();
            for( j = 0; j < resLen; j+=32)
            {
                //1.等待VS1003处在非忙碌状态
                while( VS_DREQ == 0);
                //2.拉低数据使能引脚
                VS_XDCS = 0;
                //3.VS1003最多一次性传输32Byte数据
                for( temp = 0; temp < (resLen -j >= 32 ?32:resLen - j) ; temp++)
                {
                    briupVsWriteData(musicDataBuf[j+temp]);
                }
                //4.拉高数据引脚
                VS_XDCS = 1;
                _delay_ms(10);
                    
                
            }
            OS_EXIT_CRITICAL();
        }
    }while( res == FR_OK);
    if(res != FR_OK)
        f_closedir(&dir);
#if 0
    res = f_readdir(&dir, &fno);
    if( res != FR_OK)
    {
        printf("Read Dir Failed! ErrorCode:%d\n", res);
        OSTaskSuspend(OS_PRIO_SELF);
    }
    printf("File Name :%s\n", fno.lfname);
#endif
    while(1)
    {
        printf("Player Task Running ... \n");
        OSTimeDlyHMSM(0, 0, 5, 0);
    }
}