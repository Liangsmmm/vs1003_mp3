
#include "common.h"


//VS1003 SPI 速率设置 （高速/低速）
void	briupVsSpeedLow(void)
{
	briupSpiSpeedSet( SPI_CLK_DIV_32);
}
void	briupVsSpeedHigh(void)
{
	briupSpiSpeedSet( SPI_CLK_DIV_8);
}

//VS1003 读取命令接口
//	Input:	addr	VS1003寄存器的地址
u16	briupVsReadReg( u8	addr)
{
	u16	temp = 0;
	//判断VS1003处于非忙碌状态
	while( VS_DREQ == 0);
	briupVsSpeedLow();
	VS_XDCS = 1;
	VS_XCS = 0;
	briupSpiWR( 0x03);
	briupSpiWR( addr);
	temp = briupSpiWR( 0xff);
	temp <<= 8;
	temp |= briupSpiWR( 0xff);
	VS_XCS = 1;
	briupVsSpeedHigh();
	return temp;
}

//VS1003 写命令接口
//	Inupt:	addr	VS1003寄存器的地址
//			data	要写入寄存器的16位数据
void briupVsWriteReg( u8 addr, u16 data)
{
	while( VS_DREQ == 0);
	briupVsSpeedLow();
	VS_XDCS = 1;
	VS_XCS = 0;
	briupSpiWR( 0x02);
	briupSpiWR( addr);
	briupSpiWR( data >> 8);
	briupSpiWR( data & 0xff);
	VS_XCS = 1;
	briupVsSpeedHigh();
	
}

//VS1003 读RAM接口
u16	briupVsReadRam( u16	addr)
{
	u16 temp = 0;
	//设置需要读取得RAM的基地址
	briupVsWriteReg( 0x07, addr);
	//通过0x06寄存器将RAM中的数据读取出来
	temp = briupVsReadReg( 0x06);
	return temp;
}

//VS1003 写RAM接口
void briupVsWriteRam( u16 addr, u16 data)
{
	briupVsWriteReg( 0x07, addr);
	briupVsWriteReg( 0x06, data);
}

//VS1003 写入要解码的音乐数据
void briupVsWriteData( u8 data)
{
	//等待VS1003处于非忙碌状态
	while( VS_DREQ == 0);
	VS_XCS = 1;
	VS_XDCS = 0;
	briupSpiWR( data);
	VS_XDCS = 1;
}

//VS1003硬件复位与软件复位接口
u8 briupVsHardReset(void)
{
	u8	count = 0;
	VS_RESET = 0;	//开始复位操作
	_delay_ms(20);
	VS_XDCS = 1;	//清除数据接口片选
	VS_XCS = 1;		//清除控制接口片选
	VS_RESET = 1;	//结束复位操作
	while( VS_DREQ == 0 && count < 100)
	{
		count ++;
		_delay_us(50);
	}
	_delay_ms(20);
	if( count >= 100)
		return 1;
	else
		return 0;
}

void briupVsSoftReset(void)
{
	u8	count = 0;	//超时退出
	while( VS_DREQ == 0);
	briupSpiWR( 0xff);
	//如果MODE寄存器中的值不等于初始值0x0800
	//则进行软件复位操作
	while( briupVsReadReg(0x00) != 0x0800)
	{
		//执行一次软件复位操作
		briupVsWriteReg( 0x00, 0x0804);
		_delay_ms(2);
		//如果复位成功，则读取0x00的返回值为0x0800
		//如果不成功，则重试，直到复位50次之后，超时退出
		if( ++count > 50)
			break;
	}
	while( VS_DREQ == 0);
	count = 0;
	//软件复位之后需将内部时钟进行3倍频
	while( briupVsReadReg( 0x03) != 0x9800)
	{
		briupVsWriteReg( 0x03, 0x9800);
		count ++;
		if( count > 50)
			break;
	}
	_delay_ms(20);
}

//VS1003初始化接口
u8 briupVs1003Init(void)
{
	//初始化PC13/PE6/PF6/PF7的GPIO引脚
	RCC->APB2ENR |= 1<<4 | 1<<6 | 1<<7;
	
	GPIOC->CRH &= 0xff0fffff;
	GPIOC->CRH |= 0x00800000;
	GPIOC->ODR |= 1<<13;
	
	GPIOE->CRL &= 0xf0ffffff;
	GPIOE->CRL |= 0x03000000;
	GPIOE->ODR |= 1<<6;
	
	GPIOF->CRL &= 0x00ffffff;
	GPIOF->CRL |= 0x33000000;
	GPIOF->ODR |= 3<<6;
	
	briupSpiInit();

	briupVsHardReset();		//硬件复位
	briupVsSoftReset();		//软件复位
	//低音/高音增强值
	briupVsWriteReg( 0x02, 0x7A7A);
	
	if( briupVsReadReg( 0x01) & (0x03<<4))
		return 0;
	else
		return 1;
	
}
//VS1003正弦测试接口
void briupVs1003Test(void)
{
	u16 temp, i;
	while( VS_DREQ == 0);
	temp = briupVsReadReg(0x00);
	temp |= 1<<5;	//SDI_TESTS置位
	briupVsWriteReg( 0x00, temp);
	for( i = 0;i < 40; i++)
	{
		while( VS_DREQ == 0);
		VS_XDCS = 0;	//拉低串行数据接口片选
		briupVsWriteData( 0x53);
		briupVsWriteData( 0xEF);
		briupVsWriteData( 0x6E);
		briupVsWriteData( i);
		briupVsWriteData( 0);
		briupVsWriteData( 0);
		briupVsWriteData( 0);
		briupVsWriteData( 0);
		_delay_ms(50);
		briupVsWriteData( 0x45);
		briupVsWriteData( 0x78);
		briupVsWriteData( 0x69);
		briupVsWriteData( 0x74);
		briupVsWriteData( 0);
		briupVsWriteData( 0);
		briupVsWriteData( 0);
		briupVsWriteData( 0);
		VS_XDCS = 1;
		_delay_ms(50);
	}
}
static u16 VOLUME_VAL[16] = {
	0xfefe, 0xfefe, 0xd0d0, 0xb0b0, 0x9090, 0x8080,
	0x7070, 0x6060, 0x5050, 0x4040, 0x3030, 0x2838,
	0x2020,	0x1010, 0x0000, 0x0000
};
void briupVs1003VolSet( u16 vol)
{
	vol &= 0xf;
	briupVsWriteReg( 0x0b, VOLUME_VAL[vol]);
}


//获取播放总时长
//	Input: 	文件大小
//	Output:	当前歌曲总时长
static	u16	VS_KBITS[2][16] = 
{
	{0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,0}, 
	{0,32,40,48,56,64,80,96,112,128,160,192,224,256,320,0}
};
u16	briupVsPlayTime( u32 fsize)
{
	u16	HDAT0, HDAT1;
	HDAT1 = briupVsReadReg( 0x09);
	HDAT0 = briupVsReadReg( 0x08);
	
	HDAT1 >>= 3;	//将ID[0:1]移动到最低位
	HDAT1 &= 0x03;	//只获取ID，不关心其他数据
	
	if( HDAT1 == 3)	HDAT1 = 1;	//根据不同ID，选用不同的比特率
	else			HDAT1 = 0;	//该判断是由于0/1/2比特率相同
	
	HDAT0 >>= 12;
	//此时,VS_KBITS[HDAT1][HDAT0]就是每秒钟传输位的个数(Kbit)
	return fsize / (VS_KBITS[HDAT1][HDAT0] * 125);
}

//获取当前解码时间，即当前播放时间
u16	briupVsPlayTimeNow(void)
{
	u16 temp;
	temp = briupVsReadReg( 0x04);
	return temp;
}


//防抖切歌
void briupVsRestart(void)
{
	u16 temp;
	u16 i, j;
	temp=briupVsReadReg(0x00);	//读取SPI_MODE的内容
	temp|=1<<3;					//设置SM_CANCEL位
	temp|=1<<2;					//设置SM_LAYER12位,允许播放MP1,MP2
	briupVsWriteReg(0x00,temp);	//设置取消当前解码指令
	for(i=0;i<2048;i += 32)			//发送2048个0,期间读取SM_CANCEL位.如果为0,则表示已经取消了当前解码
	{
		for( j = 0;j < 32;j++)
		{
			while( VS_DREQ == 0);
			VS_XDCS = 0;
			briupVsWriteData( 0);
			VS_XDCS = 1;
		}
		temp=briupVsReadReg(0x00);	//读取SPI_MODE的内容
 		if((temp&(1<<3))==0)break;	//成功取消了
	}
	if(i<2048)//SM_CANCEL正常
	{
		temp=briupVsReadRam(0X1E06)&0xff;//读取填充字节
		for(i=0;i<2052;i += 32)
		{
			for( j = 0;j < 32;j++)
			{
				while( VS_DREQ == 0);
				VS_XDCS = 0;
				briupVsWriteData( 0);
				VS_XDCS = 1;
			}
		}   	
	}else briupVsSoftReset();  	//SM_CANCEL不成功,坏情况,需要软复位 	  
	temp=briupVsReadReg(0x08); 
    temp+=briupVsReadReg(0x09);
	if(temp)					//软复位,还是没有成功取消,放杀手锏,硬复位
	{
		briupVsHardReset();		   	//硬复位
		briupVsSoftReset();  		//软复位 
	} 
}

