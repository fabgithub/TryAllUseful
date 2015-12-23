//
//  FakeThreadLock.cpp
//  ft_framework
//
//  Created by liyoudi on 15/4/29.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//

#include "FakeThreadLock.h"

FakeThreadLock::FakeThreadLock()
: mbLocked(false)
{
}
FakeThreadLock::~FakeThreadLock()
{
    
}

void FakeThreadLock::Lock()
{
    if(mbLocked)
    {
        FakeThreadResumeObject obj = GetResumeObjectForFakeThread();
        mWaitList.push_back(obj);
        YieldFromFakeThread();
    }
    else
    {
        mbLocked = true;
    }
}
void FakeThreadLock::Unlock()
{
    if(!mWaitList.empty())
    {
        mWaitList.front()();
        mWaitList.pop_front();
    }
    mbLocked = !mWaitList.empty();
}

FakeThreadLockHelper::FakeThreadLockHelper(FakeThreadLock *pLock)
: mpLock(pLock)
{
    mpLock->Lock();
}
FakeThreadLockHelper::~FakeThreadLockHelper()
{
    mpLock->Unlock();
}
