/**************************************************************
*FileName:       musicPlayer.h
*Description:    �������ֲ����������ػ�����ӿ�
*Author:         Penny.liang.
*CreateDate:     2018-1-11
*Update:         ----
*Copyright:      596929003@qq.com
***************************************************************/
#include "musicPlayer.h"
#include "malloc.h"
//ʵ�����ֲ�������ö�ջ
OS_STK MUSIC_TASK_STACK[ MUSIC_STACK_SIZE ] __attribute__( (at(0x68100000)));
//�����洢·��
const static TCHAR * MUSIC_PATH = "0:/music";

//�ļ�ϵͳ��ض���
DIR dir;        //Ŀ¼����
FIL file;       //�ļ�����
FILINFO fno;    //�ļ���Ϣ����

/**********************************************
* FunctionName: musicPlayerTask
* Description:  ���ֲ�����Ҫ����
* Input:        void *pdata
* Output:       void
***********************************************/

void musicPlayerTask(void * pdata)
{
    u8 res;
    printf("Music Player Task Is Run!\n");
    //=========================================
    //==========�ļ�ϵͳ���Դ���===============
    //1. ��Ŀ¼
    res = f_opendir(&dir, MUSIC_PATH);
    if(res != FR_OK)
    {
        printf("Dir Open Failed! ErrorCode : %d\n", res);
        while(1)
        {
            _delay_s(3);//ϵͳ�ڲ��򵥵õ���ʱ.......����
        }
    }
    
    //���䳤�ļ������ڴ�ռ�
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