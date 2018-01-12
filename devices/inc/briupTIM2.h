
#ifndef __TIM2_H__
#define __TIM2_H__

#include <stm32f10x.h>
#include "briupUsart.h"
#include "briupBasePeriph.h"
#include "briupDelay.h"
#include "briupNVIC.h"

//定时器2初始化接口
//传入参数：预分频系数
void briupTIM2Init(u16 psc);

//开启定时器操作
void briupTIM2Start( u16 arr);
//关闭定时器操作
void briupTIM2Stop(void);

#endif
