//briupRTC.h
//声明RTC的接口及相关定义
#ifndef __RTC_H__
#define __RTC_H__

#include "stm32f10x.h"

//基准时间的宏定义
//需要在配置RTC时，修改成对应下载时间
#define	RTC_BASE_YEAR	2017
#define	RTC_BASE_MONTH	  12
#define	RTC_BASE_DAY	  1
#define	RTC_BASE_WEEK	  5
#define	RTC_BASE_HOUR	  17
#define	RTC_BASE_MINUTE	  20
#define	RTC_BASE_SECOND	  0

//自定义RTC时间的类型
typedef	struct rtcTimeDef{
	u16	year;
	u16	month;
	u16	day;
	u16 week;
	u16 hour;
	u16	minute;
	u16 second;
	u32	rtcClock;	//RTC计数器中的值
}RTCTimeDef;

//访问RTC寄存器时，需要确认RTC_CRL  RTOFF位为1
#define	RTC_WAIT_BUSY()		while( !(RTC->CRL & (1<<5)))

//RTC初始化配置接口
u8	briupRTCInit(void);

//RTC系统时间获取函数
u8	briupRTCGetClock(RTCTimeDef * time);

//RTC测试函数
void briupRTCTest(void);

#endif

