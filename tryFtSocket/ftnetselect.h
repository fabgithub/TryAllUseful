//
//  ftnetselect.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/27.
//  Copyright (c) 2015�� liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__ftnetselect__
#define __TestAllUseful__ftnetselect__

#include <stdio.h>

#include "ftoswrap.h"

/***
 �����¼��ص�����
 nErrCode: 0��ʾ�ɹ����¼���-1��ʾ��ʱ
 pUserParam: ���ü���¼�ʱ����Ĳ���
 */
typedef void (*ftEventCallbackFn) ( int nErrCode, void *pUserParam );

int _ftnetAddReadFd_select(SOCKET sock, ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond = 0);
int _ftnetAddWriteFd_select(SOCKET sock, ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond = 0);

int ftnetSelect(unsigned int nTimeoutMs);

#endif /* defined(__TestAllUseful__ftnetselect__) */
