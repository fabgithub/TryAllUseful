//
//  FakeThreadWrap.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/11.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__FakeThreadWrap__
#define __TestAllUseful__FakeThreadWrap__

#include <stdio.h>

typedef void (*FakeThreadProc) ( void * );

class RealFkResumeObj;

// resume时的参数基类
class FakeThreadResumeParam
{
public:
    virtual ~FakeThreadResumeParam();
    virtual const char *FtsParamType() const = 0;
};

class FakeThreadResumeObject
{
public:
    FakeThreadResumeObject();
    FakeThreadResumeObject(const FakeThreadResumeObject &r);
    FakeThreadResumeObject(RealFkResumeObj *pObj);
    ~FakeThreadResumeObject();

    FakeThreadResumeObject & operator = (const FakeThreadResumeObject &r);
    bool operator == (const FakeThreadResumeObject &r) const;

    int operator () (FakeThreadResumeParam *pResumeParam = NULL) const;
    bool operator ! () const;
public:
    bool IsValid() const;
    void Detach();
private:
    RealFkResumeObj *mpResumeObj;
};

int RunFuncInFakeThread(FakeThreadProc ftp, void *pParam);
int YieldFromFakeThread(FakeThreadResumeParam **pOutResumeParam = NULL);
FakeThreadResumeObject GetResumeObjectForFakeThread();

void FakeThreadPrintCallStack(void);

#endif /* defined(__TestAllUseful__FakeThreadWrap__) */
