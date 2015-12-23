//
//  FakeThreadLock.h
//  ft_framework
//
//  Created by liyoudi on 15/4/29.
//  Copyright (c) 2015�� liyoudi. All rights reserved.
//  ��������һ�δ����һ��������ʹ֮��ͬһʱ��ֻ��һ��Э���ܹ�������δ�������������

#ifndef __ft_framework__FakeThreadLock__
#define __ft_framework__FakeThreadLock__

#include "FakeThreadWrap.h"

#include <list>

class FakeThreadLock
{
public:
    FakeThreadLock();
    ~FakeThreadLock();
public:
    void Lock();
    void Unlock();
public:
    typedef std::list<FakeThreadResumeObject> ResumeObjList;
    ResumeObjList mWaitList;
    
    bool mbLocked;
};

class FakeThreadLockHelper
{
public:
    FakeThreadLockHelper(FakeThreadLock *pLock);
    ~FakeThreadLockHelper();
public:
    FakeThreadLock *mpLock;
};

// ��������ͬʱֻ��һ��Э��ִ�н���
#define FtProtectFunction() static FakeThreadLock __ftl; FakeThreadLockHelper lock(&__ftl)

#endif /* defined(__ft_framework__FakeThreadLock__) */
