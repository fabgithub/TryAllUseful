//
//  FakeThreadTemplate.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/2/26.
//  Copyright (c) 2015�� liyoudi. All rights reserved.
//

#include "FakeThreadTemplate.h"

#include <map>

_FunctorForFakeThreadThreadProcParam::~_FunctorForFakeThreadThreadProcParam()
{
}
_FunctorForFakeThreadTimerProcParam::~_FunctorForFakeThreadTimerProcParam()
{
}
double _FunctorForFakeThreadTimerProcParam::whenExpire()
{
    return md.whenStart() + mdTimeoutSeconds;
}

void _FakeThreadProcForTemplate ( void * param)
{
    _FunctorForFakeThreadThreadProcParam *p = (_FunctorForFakeThreadThreadProcParam *) param;
    (*p) ();
}
// ================================================
// for timer
typedef std::multimap<double, _FunctorForFakeThreadTimerProcParam *> TimerMap;
static TimerMap gTimerMap;

// ���һ����ʱ������������������ⲿ���á�
void _FakeThreadAddTimerFunc(_FunctorForFakeThreadTimerProcParam * pParam)
{
    double dExpires = pParam->whenExpire();
    gTimerMap.insert(std::make_pair(dExpires, pParam));
}
static void ftTimerProc(_FunctorForFakeThreadTimerProcParam *pTimer)
{
    (*pTimer) ();
    delete pTimer;
}
// ִ�ж�ʱ��, ���ⲿ����
void fttimer_do()
{
    ftduration now;
    double dNow = now.whenStart();
    _FunctorForFakeThreadTimerProcParam *pTimer = NULL;
    TimerMap::iterator iter;
    while (!gTimerMap.empty())
    {
        iter = gTimerMap.begin();
        if(iter->first > dNow)
        {
            break;
        }
        pTimer = iter->second;
        gTimerMap.erase(iter);
        RunInFakeThreadT(ftTimerProc, pTimer);
    }
}
unsigned int fttimer_count() // ���ص�ǰ��ʱ������
{
    unsigned int nCount = 0;
    nCount = (unsigned int) gTimerMap.size();
    return nCount;
}
void fttimer_free_all() // �ͷ�����δ��ʱ�Ķ�ʱ��
{
    
}
