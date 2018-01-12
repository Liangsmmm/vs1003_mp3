
#include "common.h"
#include "main.h"
//驱动硬件外设得函数接口
FATFS  fs;//File System

u8 mp3DevicesInit(void);

int main(void)
{
    u8 count = 0;
    while(mp3DevicesInit())
    {
        printf("Devices Init Error ... Retry ... \n");
        count++;
        _delay_ms(200);
        if(count > 10)
            break;
    }
    
    if(count >10)
    {
        printf("Devices Init Failed!Please Restart and Try again!\n");
        setLed(LED_ALL);
        
        while(1);
    }
    //LCDpingmu屏幕绘图接口测试
    //绘制矩形，在内部显示文字
    briupLcdFastDrawDirection(160, 350, 140, 180, 0X457, 1);
    briupFontDrawText(165, 144, "~_~MP3", RED, 0X8430, GUI_FONT_32);
    
    //初始化uC/OS系统
    OSInit();
    OSTaskCreate(startTask, (void*)0, &START_TASK_STACK[ START_STACK_SIZE-1 ], START_TASK_PRIO);
    
    OSStart();
    while(1)
    {
        setLed(LED_G);
        _delay_ms(500);
        resetLed(LED_G);
        _delay_ms(500);
    }
	while(1);
}
//驱动硬件外设得函数借口
u8 mp3DevicesInit(void)
{
    //初始化NVIC中断优先级分组
    briupNVICGroupInit(2);//2Bit抢占  2Bit比较
    //初始化延时函数
    _delay_init(72);  //单位  MHz
    //初始化基础外设
    basePeriphInit();
    //初始化串口  USART1   ->   57600
    //初始化外部SRAM
    FSMC_SRAM_init();
    //初始化音频解码芯片VS1003
    briupVs1003Init();
    
    briupUsart1Init(57600);
    //初始化液晶屏  ILI9486
    briupLcdInit();
    //初始化触摸屏  spiTONGXINJIEKOUtongx通信接口
    tp_init();
    //初始化SD卡
    if(SD_OK == SD_Init())
        printf("SD Card Init Success!\n");
    else
    {
        printf("SD Card Init Error!Res:0X01\n");
        return 0x01;
    }
    //初始化文件系统 ，参数1：文件系统指针
    //    参数2：挂载的盘符
    //    参数3：挂载选项
    if(FR_OK == f_mount(&fs, "0", 1))
    {
        printf("FileSystem Mount Success !\n");
    }
    else
    {
        printf("FS Mount Error!Res:0x02\n");
        return 0x02;
    }
    //初始化LCD文件字库
    briupFontInit();
    return 0;
    
    
}



void startTask(void* pdata)
{
    OS_CPU_SR cpu_sr = 0;
    
    _delay_os_start();
    
    OS_ENTER_CRITICAL();   //进入临界区
    
    #if LED_TASK_EN
    //led状态指示灯得任务创建
    OSTaskCreate( LEDTask, (void*)0, &LED_TASK_STACK[ LED_STACK_SIZE-1 ], LED_TASK_PRIO);
    #endif
      
     OSTaskCreate(musicPlayerTask, (void * )0, &MUSIC_TASK_STACK[ MUSIC_STACK_SIZE-1 ], MUSIC_TASK_PRIO);
     OSTaskCreate(musicUITask, (void * )0, &MUI_TASK_STACK[ MUI_STACK_SIZE-1 ], MUI_TASK_PRIO);
    OSTaskCreate(musicControlTask, (void * )0, &MCTRL_TASK_STACK[ MCTRL_STACK_SIZE-1 ], MCTRL_TASK_PRIO);
    
    
    
    OS_EXIT_CRITICAL();//退出临界区
    
    OSTaskSuspend( OS_PRIO_SELF );
    while(1);
    
    

}

#if LED_TASK_EN
void LEDTask(void * pdata)
{
    while(1)
    {
        setLed(LED_G);
        _delay_ms(300);
        resetLed(LED_G);
        _delay_ms(300);
    }
}

#endif
