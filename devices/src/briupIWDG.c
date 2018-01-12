
#include "common.h"

//独立看门狗初始化接口
//	pr  - 预分频系数
//	rlr - 重装载值
// 独立看门狗的输入时钟为LSI，默认40KHz，但不精确
void briupIWDGInit(u16 pr, u16 rlr)
{
	//1.开启并就绪LSI时钟
	RCC->CSR |= 0x01;
	while( !(RCC->CSR & 0x02));
	
	//2.除能PR/RLR的写保护
	IWDG->KR = 0x5555;
	
	IWDG->PR = pr;
	IWDG->RLR = rlr;
	//使能IWDG计数器
	IWDG->KR = 0xCCCC;
	IWDG->KR = 0xAAAA;
}

//执行一次喂狗操作
void briupIWDGFeed(void)
{
	IWDG->KR = 0xAAAA;
}

void briupIWDGTest(void)
{
	u8 key = 0;
	resetLed( LED_ALL);
	_delay_ms(1000);
	setLed( LED_G);
	while(1)
	{
		key = getKeyValue();
		if( key == KEY_SEL)
			briupIWDGFeed();
	}
}

