/***********************************
* FileName:		briupDelay.c
* CreateDate:	2017-8-24
* Author:		Dennis Chen
* Description:	ʵ����ʱ�ĳ�ʼ���ӿ�
				����us/ms/s������ʱ�Ľӿ�
* Update:		--------
***********************************/
#include "common.h"

//����us��ms����Ƶ�ʵ�ȫ�־�̬����
u32 fac_us,fac_ms;
u32 fac_ms_ucos;

/******************
* FunctionName:	_delay_init( u8 SYSCLK)
* Desciption:	��ʼ��ϵͳʱ�ӵĶ�ʱ��
* Input:		SYSCLK	ϵͳʱ��Ƶ��(MHz)
* Output:		NULL
******************/
void _delay_init( u8 SYSCLK)
{
#ifdef	OS_MAX_TASKS
	//���ʹ���˲���ϵͳ������Ҫ����
	//	"һ��ʱ�ӽ����������װ�ؼĴ�����ֵ"
	u32 reload;
#endif
	SysTick->CTRL = 0;
//���ʹ����uCOS-IIϵͳ���������������
	//ϵͳʱ��8��Ƶ��õ�ϵͳ����ʱ��
	reload = SYSCLK / 8;
	//����1��uCOS-II��������Ķ�ʱ����������
	reload = reload * 1000000 / OS_TICKS_PER_SEC;
	
	
	//����uCOS-II���ĵ�ʱ�䣬��ʱ�ӷֱ���
	fac_ms_ucos = 1000 / OS_TICKS_PER_SEC;
	
	//װ���Զ���װ�ؼĴ�����ֵ
	SysTick->LOAD = reload;
	
	SysTick->CTRL |= 1<<0;
	fac_us = SYSCLK / 8;
	fac_ms = fac_us * 1000;
}

void _delay_os_start(void)
{
	//ʹ��SysTick�ж�
	SysTick->CTRL |= 1<<1;
}


//��us��ʱ
void _delay_us( u32 us)
{
	u32 temp;	//��⵹��ʱ�Ƿ����
#ifdef	OS_MAX_TASKS
	temp = SysTick->VAL;
	if( temp >= us)
	{
		temp = temp - us;
		while( SysTick->VAL > temp);
	}else
	{
		temp = SysTick->VAL + SysTick->LOAD - us;
		while( SysTick->VAL < us || SysTick->VAL > temp);
	}
#else
	SysTick->LOAD = fac_us * us;
	SysTick->VAL = 0;
	SysTick->CTRL |= 0x01;
	do
	{
		temp = SysTick->CTRL;
	}while( (temp & 0x01) && !(temp & (1<<16)) );
	SysTick->CTRL &= ~0x01;
#endif
}

//��ms��ʱ	���ֵ���ɳ���1864ms
void _delay_ms( u32 ms)
{
	u32 temp, reload;	//��⵹��ʱ�Ƿ����
#ifdef	OS_MAX_TASKS
	if( OSRunning == 1)	//ϵͳ��������״̬
	{
		if( ms >= fac_ms_ucos)	//��ʱʱ�����ϵͳʱ��ֱ���
		{
			OSTimeDly( ms / fac_ms_ucos);
			ms = ms % fac_ms_ucos;
		}
		_delay_us( ms * 1000);
	}
	else
	{
		SysTick->CTRL &= ~0x01;
		reload = SysTick->LOAD;
		SysTick->LOAD = fac_ms * ms;
		SysTick->VAL = 0;
		SysTick->CTRL |= 0x01;
		do
		{
			temp = SysTick->CTRL;
		}while( (temp & 0x01) && !(temp & (1<<16)) );
		
		SysTick->CTRL &= ~0x01;
		SysTick->LOAD = reload;
		SysTick->CTRL |= 0x01;
	}
#else
	SysTick->LOAD = fac_ms * ms;
	SysTick->VAL = 0;
	SysTick->CTRL |= 0x01;
	do
	{
		temp = SysTick->CTRL;
	}while( (temp & 0x01) && !(temp & (1<<16)) );
	
#endif
}

//��s��ʱ
void _delay_s( double s)
{
	u32 ms = (u32)(s * 1000);
	do
	{
		if( ms > 1000)
		{
			_delay_ms(1000);
			ms -= 1000;
		}else
		{
			_delay_ms( ms);
			ms = 0;
		}
	}while( ms > 0);
}

//ʵ��ϵͳ���ĵ��жϷ�����
//	��SysTick���жϷ�����
void SysTick_Handler(void)
{
	//�����ж���Ҫ���õ�OS API
	OSIntEnter();
	//����ϵͳ����
	OSTimeTick();
	//�˳��ж�API
	OSIntExit();
}





