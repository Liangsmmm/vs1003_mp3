
#include "common.h"
#include "main.h"
//����Ӳ������ú����ӿ�
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
    //LCDpingmu��Ļ��ͼ�ӿڲ���
    //���ƾ��Σ����ڲ���ʾ����
    briupLcdFastDrawDirection(160, 350, 140, 180, 0X457, 1);
    briupFontDrawText(165, 144, "~_~MP3", RED, 0X8430, GUI_FONT_32);
    
    //��ʼ��uC/OSϵͳ
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
//����Ӳ������ú������
u8 mp3DevicesInit(void)
{
    //��ʼ��NVIC�ж����ȼ�����
    briupNVICGroupInit(2);//2Bit��ռ  2Bit�Ƚ�
    //��ʼ����ʱ����
    _delay_init(72);  //��λ  MHz
    //��ʼ����������
    basePeriphInit();
    //��ʼ������  USART1   ->   57600
    //��ʼ���ⲿSRAM
    FSMC_SRAM_init();
    //��ʼ����Ƶ����оƬVS1003
    briupVs1003Init();
    
    briupUsart1Init(57600);
    //��ʼ��Һ����  ILI9486
    briupLcdInit();
    //��ʼ��������  spiTONGXINJIEKOUtongxͨ�Žӿ�
    tp_init();
    //��ʼ��SD��
    if(SD_OK == SD_Init())
        printf("SD Card Init Success!\n");
    else
    {
        printf("SD Card Init Error!Res:0X01\n");
        return 0x01;
    }
    //��ʼ���ļ�ϵͳ ������1���ļ�ϵͳָ��
    //    ����2�����ص��̷�
    //    ����3������ѡ��
    if(FR_OK == f_mount(&fs, "0", 1))
    {
        printf("FileSystem Mount Success !\n");
    }
    else
    {
        printf("FS Mount Error!Res:0x02\n");
        return 0x02;
    }
    //��ʼ��LCD�ļ��ֿ�
    briupFontInit();
    return 0;
    
    
}



void startTask(void* pdata)
{
    OS_CPU_SR cpu_sr = 0;
    
    _delay_os_start();
    
    OS_ENTER_CRITICAL();   //�����ٽ���
    
    #if LED_TASK_EN
    //led״ָ̬ʾ�Ƶ����񴴽�
    OSTaskCreate( LEDTask, (void*)0, &LED_TASK_STACK[ LED_STACK_SIZE-1 ], LED_TASK_PRIO);
    #endif
      
     OSTaskCreate(musicPlayerTask, (void * )0, &MUSIC_TASK_STACK[ MUSIC_STACK_SIZE-1 ], MUSIC_TASK_PRIO);
     OSTaskCreate(musicUITask, (void * )0, &MUI_TASK_STACK[ MUI_STACK_SIZE-1 ], MUI_TASK_PRIO);
    OSTaskCreate(musicControlTask, (void * )0, &MCTRL_TASK_STACK[ MCTRL_STACK_SIZE-1 ], MCTRL_TASK_PRIO);
    
    
    
    OS_EXIT_CRITICAL();//�˳��ٽ���
    
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
