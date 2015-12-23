//
//  FakeThreadTemplate.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/2/26.
//  Copyright (c) 2015�� liyoudi. All rights reserved.
//

#include "FakeThreadTemplate.h"

#include <map>

static std::string gstrNextTimerName;
void SetNextTimerNameForDebug(const std::string &name)
{
    gstrNextTimerName = name;
}

_FunctorForFakeThreadThreadProcParam::~_FunctorForFakeThreadThreadProcParam()
{
}
_FunctorForFakeThreadTimerProcParam::_FunctorForFakeThreadTimerProcParam()
: mdTimeoutSeconds(0.0)
{
    if(!gstrNextTimerName.empty())
    {
        mstrTimerName = gstrNextTimerName;
        gstrNextTimerName = "";
    }
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
typedef std::multimap<double, _FunctorForFakeThreadTimerProcParam *> FtTimerMap;
static FtTimerMap gFtTimerMap;
FTNET_API FtTimerMap & fttimer_get_all()
{
    return gFtTimerMap;
}

// ���һ����ʱ������������������ⲿ���á�
void _FakeThreadAddTimerFunc(_FunctorForFakeThreadTimerProcParam * pParam)
{
    double dExpires = pParam->whenExpire();
    gFtTimerMap.insert(std::make_pair(dExpires, pParam));
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
    FtTimerMap::iterator iter;
    while (!gFtTimerMap.empty())
    {
        iter = gFtTimerMap.begin();
        if(iter->first > dNow)
        {
            break;
        }
        pTimer = iter->second;
        gFtTimerMap.erase(iter);
        RunInFakeThreadT(ftTimerProc, pTimer);
    }
}
unsigned int fttimer_count() // ���ص�ǰ��ʱ������
{
    unsigned int nCount = 0;
    nCount = (unsigned int) gFtTimerMap.size();
    return nCount;
}
double fttimer_next_duration()
{
    double d = 1000000;
    if(!gFtTimerMap.empty())
    {
        d = gFtTimerMap.begin()->first;
    }
    return d;
}
void fttimer_free_all() // �ͷ�����δ��ʱ�Ķ�ʱ��
{
    
}
