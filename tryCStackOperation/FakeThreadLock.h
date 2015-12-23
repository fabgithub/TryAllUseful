//
//  FakeThreadLock.h
//  ft_framework
//
//  Created by liyoudi on 15/4/29.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//  用于锁定一段代码或一个函数，使之在同一时刻只有一个协程能够进入这段代码或这个函数。

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

// 保护函数同时只有一个协程执行进入
#define FtProtectFunction() static FakeThreadLock __ftl; FakeThreadLockHelper lock(&__ftl)

#endif /* defined(__ft_framework__FakeThreadLock__) */
