/**************************************************************
*FileName:       musicUI.h
*Description:    绘制MP3界面的任务环境与接口
*Author:         Penny.liang.
*CreateDate:     2018-1-11
*Update:         ----
*Copyright:      596929003@qq.com
***************************************************************/
#include "musicUI.h"
//实现音乐播放任务得堆栈
OS_STK MUI_TASK_STACK[ MUI_STACK_SIZE ] __attribute__( (at(0x68110000)));


/**********************************************
* FunctionName: musicUITask
* Description:   MP3界面主要任务
* Input:        void *pdata
* Output:       void
***********************************************/

void musicUITask(void * pdata)
{
    
    //LCD屏幕绘图接口测试
    //绘制矩形，在内部显示文字
    //参数： x1 x2 y1 y2 color full
    briupLcdFastDrawDirection(160,350,140,180, 0x07E0, 1);
    briupFontDrawText(190, 144, "*_*", RED, YELLOW, GUI_FONT_32);
    printf("Music UI Task Is Run!\n");
    while(1)
    {
        printf("MUI Task Running ... \n");
        OSTimeDlyHMSM(0, 0, 5, 0);
    }
}
