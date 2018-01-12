
#include "common.h"

#define FSMC_BASE ((u32)(0x68000000))

//FSMC初始化外部SRAM
void FSMC_SRAM_init(void)
{
	RCC->AHBENR |= 1<<8;		//FSMC时钟使能
	/*
 地址线：GPIOF（0~5 . 12~15）、GPIOG（0~5）、GPIOD（11.12.13） | 总共19跟地址线
	* 数据线：GPIOE（7~15）、GPIOD（0.1.8.9.10.14.15）		| 总共16跟数据线
	* 配置线：GPIOE（0.1）、GPIOD（4.5）、GPIOG（10）
	* 需要配置的IO口：
	* GPIOD（0/1/4/5/8-15）	12
	* GPIOE（0/1/7-15）			11
	* GPIOF（0-5/12-15）		10
	* GPIOG（0-5/10）				7
	*/
	RCC->APB2ENR |= 1<<5 | 1<<6 | 1<<7 | 1<<8;
	
	//将各个IO口置为复用推挽输出，配置前将寄存器清零
	GPIOD->CRH &= 0x00000000;
	GPIOD->CRH |= 0xBBBBBBBB;
	GPIOD->CRL &= 0xFF00FF00;
	GPIOD->CRL |= 0x00BB00BB;
	
	GPIOE->CRH &= 0x00000000;
	GPIOE->CRH |= 0xBBBBBBBB;
	GPIOE->CRL &= 0x0FFFFF00;
	GPIOE->CRL |= 0xB00000BB;
	
	GPIOF->CRH &= 0x0000FFFF;
	GPIOF->CRH |= 0xBBBB0000;
	GPIOF->CRL &= 0xFF000000;
	GPIOF->CRL |= 0x00BBBBBB;
	
	GPIOG->CRH &= 0xFFFFF0FF;
	GPIOG->CRH |= 0x00000B00;
	GPIOG->CRL &= 0xFF000000;
	GPIOG->CRL |= 0x00BBBBBB;
	
	//寄存器清零
	//Bank1共有NE1~4，每个端口都有一个BCR+TCR，所以共有8个寄存器
	//使用的是NE3，即对应BTCR[4]、BTCR[5]
	FSMC_Bank1->BTCR[4] = 0x00000000;
	FSMC_Bank1->BTCR[5] = 0x00000000;
	FSMC_Bank1E->BWTR[4] = 0x00000000;
	
	
	//操作BCR寄存器	使用异步模式，模式A（读写共用一个时序寄存器）
	//BTCR[偶数]：BCR寄存器；BTCR[奇数]：BTR寄存器
	FSMC_Bank1->BTCR[4] |= 1<< 14;
	FSMC_Bank1->BTCR[4] |= 1<< 12;			//存储器写使能
	FSMC_Bank1->BTCR[4] |= 1<< 4;				//存储器数据宽度为16bit
	FSMC_Bank1->BTCR[4] |= 0<< 2;
	
	/*
	FSMC_Bank1->BTCR[4] |= 1<<12;	//存储器 写 使能
	FSMC_Bank1->BTCR[4] |= 1<<14;	//读 写 使用不同时序
	FSMC_Bank1->BTCR[4] |= 1<<4;	//存储数据宽度为16位
	//写 控制
	FSMC_Bank1E->BWTR[4] |= 0<<28;	//MODE A
	FSMC_Bank1E->BWTR[4] |= 0<<0;	//地址建立时间为1*HCLK
	*/
	//操作BTR寄存器
	FSMC_Bank1->BTCR[5] |= 0<<28;
	FSMC_Bank1->BTCR[5] |= 0x2<<8;			//数据保存时间(DATAST) 为4个HCLK 4/72M = 55ns
	FSMC_Bank1->BTCR[5] |= 1<<0;				//地址建立时间ADDSET为1个HCLK
	//闪存写时序寄存器
	FSMC_Bank1E->BWTR[4] |= 3<<8;
	//使能BANK1区域3
	FSMC_Bank1->BTCR[4] |= 1<<0;
}

