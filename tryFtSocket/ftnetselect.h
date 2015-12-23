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

FTNET_API int _ftnetAddReadFd_select(SOCKET sock, ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond = 0);
FTNET_API int _ftnetAddWriteFd_select(SOCKET sock, ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond = 0);
FTNET_API int _ftnetOnFdClosed_select(SOCKET sock);
FTNET_API void _ftnetCancel_select(SOCKET sock);

FTNET_API int ftnetSelect(unsigned int nTimeoutMs);
FTNET_API void ftnetstop();

FTNET_API void ftnet_set_max_select_fd_count(unsigned int nMax);
FTNET_API unsigned int ftnet_get_max_select_fd_count(unsigned int nMax);

#endif /* defined(__TestAllUseful__ftnetselect__) */
