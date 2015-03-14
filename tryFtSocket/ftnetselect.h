//
//  ftnetselect.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/27.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__ftnetselect__
#define __TestAllUseful__ftnetselect__

#include <stdio.h>

#include "ftoswrap.h"

/***
 网络事件回调函数
 nErrCode: 0表示成功有事件，-1表示超时
 pUserParam: 调置监控事件时传入的参数
 */
typedef void (*ftEventCallbackFn) ( int nErrCode, void *pUserParam );

int _ftnetAddReadFd_select(SOCKET sock, ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond = 0);
int _ftnetAddWriteFd_select(SOCKET sock, ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond = 0);

int ftnetSelect(unsigned int nTimeoutMs);

#endif /* defined(__TestAllUseful__ftnetselect__) */
