//briupVS1003.h
//用于声明和定义VS1003相关引脚与接口

#ifndef __VS1003_H__
#define __VS1003_H__

#include "common.h"

//PC13 -> DREQ		PE6	->	XRESET
//PF6  -> XDCS		PF7 ->	XCS
#define	VS_DREQ		PCxIn(13)
#define	VS_RESET	PExOut(6)
#define	VS_XDCS		PFxOut(6)
#define	VS_XCS		PFxOut(7)

//VS1003 SPI 速率设置 （高速/低速）
void	briupVsSpeedLow(void);
void	briupVsSpeedHigh(void);

//VS1003 读取命令接口
//	Input:	addr	VS1003寄存器的地址
u16	briupVsReadReg( u8	addr);

//VS1003 写命令接口
//	Inupt:	addr	VS1003寄存器的地址
//			data	要写入寄存器的16位数据
void briupVsWriteReg( u8 addr, u16 data);

//VS1003 读RAM接口
u16 briupVsReadRam( u16	addr);
//VS1003 写RAM接口
void briupVsWriteRam( u16 addr, u16 data);

//VS1003 写入要解码的音乐数据
void briupVsWriteData( u8 data);

//VS1003硬件复位与软件复位接口
u8 briupVsHardReset(void);
void briupVsSoftReset(void);

//VS1003初始化接口
u8 briupVs1003Init(void);
//VS1003正弦测试接口
void briupVs1003Test(void);

//================================================
//			播放相关接口代码
//	音量调整

void briupVs1003VolSet( u16 vol);

//获取播放总时长
//	Input: 	文件大小
//	Output:	当前歌曲总时长
u16	briupVsPlayTime( u32 fsize);

//获取当前解码时间，即当前播放时间
u16	briupVsPlayTimeNow(void);

//防抖切歌
void briupVsRestart(void);

#endif

