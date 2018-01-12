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
//�洢����·�������Ƶû�����
char musicNameBuf[ _MAX_LFN ];

//�洢�������ݵû�����
#define FR_MAX_LEN 512
u8 musicDataBuf[ FR_MAX_LEN ];


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
    u8 res, temp;
    u32 i, j, resLen;
    OS_CPU_SR cpu_sr;
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
    
    i = 0;
    do
    {
        res = f_readdir(&dir, &fno);
        if(fno.fname[0] == 0)
        {
            printf("������ȡ���!");
            f_closedir(&dir);
            break;
        }
        i++;
        printf("%d.%s\n", i, fno.lfname);
        
        //ƴ���ļ�����·��
        memset(musicNameBuf, 0, _MAX_LFN);
        strcat( musicNameBuf, MUSIC_PATH);
        strcat(musicNameBuf, "/");
        //���ļ���Ϊ0ƴ��duan���ļ���
        if( strlen( fno.lfname) == 0)
            strcat(musicNameBuf, fno.fname);
        else        //��Ϊ0��ƴ�ӳ��ļ���
            strcat(musicNameBuf, fno.fname);
        printf("-->%s\n", musicNameBuf);
        res = f_open(&file, musicNameBuf, FA_READ);
        if(res != FR_OK)
        {
            printf("�ļ���ʧ��!������ǰ�ļ�...\n");
            _delay_ms(100);
            continue;
        }
        while(1)  //��������
        {
            res = f_read(&file, musicDataBuf, FR_MAX_LEN, &resLen);
            if( res != FR_OK || resLen <= 0)
            {
                printf("dangq��ǰ����������ϻ��ļ�����...\n");
                f_close(&file);
                break;
            }
            OS_ENTER_CRITICAL();
            for( j = 0; j < resLen; j+=32)
            {
                //1.�ȴ�VS1003���ڷ�æµ״̬
                while( VS_DREQ == 0);
                //2.��������ʹ������
                VS_XDCS = 0;
                //3.VS1003���һ���Դ���32Byte����
                for( temp = 0; temp < (resLen -j >= 32 ?32:resLen - j) ; temp++)
                {
                    briupVsWriteData(musicDataBuf[j+temp]);
                }
                //4.������������
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