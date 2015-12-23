//
//  FakeThreadWrap.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/11.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__FakeThreadWrap__
#define __TestAllUseful__FakeThreadWrap__

#include "ftnet_export.h"
#include <stdio.h>
#include <string>
#include <list>

typedef void (*FakeThreadProc) ( void * );

class RealFkResumeObj;

// resume时的参数基类
class FTNET_API FakeThreadResumeParam
{
public:
    virtual ~FakeThreadResumeParam();
    virtual const char *FtsParamType() const = 0;
};

class FTNET_API FakeThreadResumeObject
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
    mutable RealFkResumeObj *mpResumeObj;
};

FTNET_API int RunFuncInFakeThread(FakeThreadProc ftp, void *pParam);
FTNET_API int YieldFromFakeThread(FakeThreadResumeParam **pOutResumeParam = NULL);
FTNET_API FakeThreadResumeObject GetResumeObjectForFakeThread();

FTNET_API int FakeThreadSetName(const char *szFakeThreadName);
FTNET_API const char *FakeThreadGetName(bool bNullWhenInMain = false);

class FTNET_API FakeThreadStackHelper
{
    FakeThreadStackHelper(const FakeThreadStackHelper &);
    FakeThreadStackHelper & operator = (const FakeThreadStackHelper &);
public:
    FakeThreadStackHelper(const char *szFuncName, const char *szFile, int nLine);
    ~FakeThreadStackHelper();
};

class FTNET_API FtStackInfo
{
public:
    FtStackInfo();
    FtStackInfo(const std::string &strName, const std::string &strFile, int nLine);
public:
    std::string mstrName;
    std::string mstrFile;
    int mnLine;
};

typedef std::list<FtStackInfo> FtStackInfoList;
typedef std::list<FtStackInfoList> FakeThreadInfoList;

FTNET_API FtStackInfoList FakeThreadGetStack();
FTNET_API FakeThreadInfoList FakeThreadGetAllStack();

FTNET_API unsigned int GetRunningFakeThreadCount();

FTNET_API void FakeThreadPrintCallStack(void);

#if FAKE_THREAD_NO_STACK_RECORDER
#define FakeThreadRecordStack()
#else
#define FakeThreadRecordStack() FakeThreadStackHelper _ft_stack_recorder(__FUNCTION__, __FILE__, __LINE__)
#endif

#endif /* defined(__TestAllUseful__FakeThreadWrap__) */
