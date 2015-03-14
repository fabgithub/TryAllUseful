//
//  FakeThreadWrap.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/2/11.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#include "FakeThreadWrap.h"

#include <string>
#include <pthread.h>
#include <unistd.h>
#include <list>

#include <stdio.h>
#include <stdlib.h>

class FakeThreadItem
{
public:
    FakeThreadItem(const std::string &strName, FakeThreadProc ftp, void *pParam)
    : mllRsp(0), mllOrignRsp(0), mllFromRsp(0), mbGotResumeObj(false), mnState(0), mbInitialised(false), mstrName(strName)
    , mftp(ftp), mpParam(pParam)
    , mpResumeParam(NULL)
    {}
public:
    void ResetForResue(const std::string &strName, FakeThreadProc ftp, void *pParam)
    {
        mllRsp = mllOrignRsp;
        mllFromRsp = 0;
        mftp = ftp;
        mpParam = pParam;
        mstrName = strName;
        mbInitialised = true;
        mbGotResumeObj = false;
        mnState = 0;
        mpResumeParam = NULL;
    }
    // 重置以备复用，注意，mllOrignRsp不能重置
    void Clear()
    {
        mllRsp = 0;
        mbGotResumeObj = false;
        mnState = 0;
        mllFromRsp = 0;
        mbInitialised = false;
        mstrName.clear();
        mftp = NULL;
        mpParam = NULL;
        mpResumeParam = NULL;
    }
public:
    long long mllRsp;
    long long mllOrignRsp;
    long long mllFromRsp; // 是从哪个协程的rsp过来的
    bool mbGotResumeObj; // 是否获取了用于resume的对象，如果没有，则不能yield
    int mnState; // 当前状态, 0表示未运行过，1表示被yield, 2表示正在运行，9表示运行结束
    
    bool mbInitialised;
    std::string mstrName;
    
    FakeThreadProc mftp;
    void *mpParam;
    
    // 当被resume时的参数，这个值会成为yield的返回值
    FakeThreadResumeParam *mpResumeParam;
};

// ==============
extern "C" FakeThreadItem * FakeThread_StartWithRsp(long long llNewRsp, void (*pfunc) (), FakeThreadItem *pItem, long long *pSaveMainRsp );
extern "C" FakeThreadResumeParam * FakeThread_YieldWithMainRsp(long long llMainRsp, long long *pThreadRsp);
extern "C" FakeThreadItem * FakeThread_GetItemPtr(long long llMainRsp);
extern "C" void FakeThread_ResumeWithThreadRsp(long long llThreadRsp, long long *pSaveMainRsp, FakeThreadItem *pItem, FakeThreadResumeParam *pResumeParam);
// ==============

extern "C" long long FakeThread_GetRSP();

// ================================
static long long gllRspForFakeThread = 0; // 保存从哪个rsp过来的

#define USE_REAL_THREAD_STACK 0

#if USE_REAL_THREAD_STACK
static void * FetchStackThreadProc(void *pParam)
{
    FakeThreadItem *pSI = (FakeThreadItem *) pParam;
    pSI->mllOrignRsp = pSI->mllRsp = FakeThread_GetRSP() - 1000;
    pSI->mbInitialised = true;
    while(true)
    {
        sleep(10 * 365 * 24 * 3600); // sleep ten years
    }
    return NULL;
}
#endif

typedef std::list<FakeThreadItem *> FakeThreadItemList;
static FakeThreadItemList gFakeThreadItemList;

FakeThreadItem * NewFakeThreadItem(const std::string &strName, FakeThreadProc ftp, void *pParam)
{
    FakeThreadItem *pRet = NULL;
    if(!gFakeThreadItemList.empty())
    {
        pRet = gFakeThreadItemList.front();
        gFakeThreadItemList.pop_front();
        pRet->ResetForResue(strName, ftp, pParam);
    }
    else
    {
        pRet = new FakeThreadItem(strName, ftp, pParam);
#if USE_REAL_THREAD_STACK
        pthread_t thr;
        pthread_create(&thr, NULL, FetchStackThreadProc, pRet);
        while(!pRet->mbInitialised)
        {
            usleep(1 * 1000);
        }
#else
        static int gnStackSize = 200 * 1024; // 分配100KB
        char *pNewStack = new char[gnStackSize];
        pRet->mllOrignRsp = pRet->mllRsp = (long long) (pNewStack + gnStackSize - 1024); // 实际使用99KB
        pRet->mbInitialised = true;
#endif
    }
    return pRet;
}

static void FakeThread_MainFunc()
{
    FakeThreadItem *pItem = FakeThread_GetItemPtr(gllRspForFakeThread);
    pItem->mftp(pItem->mpParam);
    
    pItem->mllRsp = pItem->mllOrignRsp; // 准备复用
    gllRspForFakeThread = pItem->mllFromRsp;
    
    pItem->Clear();
    gFakeThreadItemList.push_back(pItem);

    long long saveRsp = gllRspForFakeThread;
    gllRspForFakeThread = 0; // 表示已经回到了源线程
    
    long long tmpRsp = 0;
    FakeThread_YieldWithMainRsp(saveRsp, &tmpRsp);
}
/***
 在伪线程中执行一个函数对象
 返回9表示直接结束了。
 返回1表示被yield了。
 */
int RunFuncInFakeThread(FakeThreadProc ftp, void *pParam)
{
    FakeThreadItem * pSI = NULL;
    pSI = NewFakeThreadItem("first", ftp, pParam);
    
    long long oldPreRsp = gllRspForFakeThread; // 当前的前一协程的rsp
    long long savedRsp = pSI->mllRsp;
    pSI->mllRsp = 0; // 置成0表示正在运行中
    pSI->mllFromRsp = FakeThread_GetRSP(); // 从哪个rsp过来的
    FakeThread_StartWithRsp(savedRsp, FakeThread_MainFunc, pSI, &gllRspForFakeThread);

    gllRspForFakeThread = oldPreRsp; // 恢复当前的前一协程rsp
    return 0;
}

int YieldFromFakeThread(FakeThreadResumeParam **pOutResumeParam)
{
    if(!gllRspForFakeThread)
    {
        return -1;
    }
    long long saveRsp = gllRspForFakeThread;
    FakeThreadItem *pItem = FakeThread_GetItemPtr(saveRsp);
    if(!pItem->mbGotResumeObj)
    {
        return -2;
    }
    gllRspForFakeThread = pItem->mllFromRsp; // 设置从哪个协程过来的
    
    FakeThreadResumeParam *pYieldParam = FakeThread_YieldWithMainRsp(saveRsp, &pItem->mllRsp);
    pItem->mbGotResumeObj = false;
    if(pOutResumeParam)
    {
        *pOutResumeParam = pYieldParam;
    }
    return 0;
}

int ResumeToFakeThread(FakeThreadItem * pSI)
{
    if(!pSI || !pSI->mllRsp)
    {
        return -1;
    }
    // 当准备yield时，就不能再resume其它线程了
    if(pSI->mbGotResumeObj)
    {
        return -2;
    }
    // 保存本伪线程是从哪个rsp伪线程resume过来的，以便往回yield
    long long llSaveLastWhoResumeMe = gllRspForFakeThread;
    //
    long long savedRsp = pSI->mllRsp;
    pSI->mllRsp = 0; // 置成0表示正在运行中
    pSI->mllFromRsp = FakeThread_GetRSP();
    FakeThread_ResumeWithThreadRsp(savedRsp, &gllRspForFakeThread, pSI, pSI->mpResumeParam);
    // 恢复被谁resume的伪线程的rsp
    gllRspForFakeThread = llSaveLastWhoResumeMe;
    return 0;
}

class RealFkResumeObj
{
    int mnRef;
    FakeThreadItem *mpSI;
public:
    RealFkResumeObj(FakeThreadItem *pSI)
    : mnRef(0), mpSI(pSI)
    {
        if (mpSI)
        {
            mpSI->mbGotResumeObj = true;
        }
    }
    bool operator ! () const
    {
        return mpSI;
    }
public:
    void AddRef()
    {
        ++mnRef;
    }
    void DecreaseRef()
    {
        --mnRef;
        if(mnRef <= 0)
        {
            Resume(NULL);
            delete this;
        }
    }
    int Resume(FakeThreadResumeParam *pResumeParam)
    {
        if(!mpSI)
        {
            return -1;
        }
        mpSI->mbGotResumeObj = false;
        FakeThreadItem *pTmp = mpSI;
        pTmp->mpResumeParam = pResumeParam;
        mpSI = NULL;
        return ResumeToFakeThread(pTmp);
    }
};

FakeThreadResumeParam::~FakeThreadResumeParam() {}

FakeThreadResumeObject::FakeThreadResumeObject()
:mpResumeObj(NULL)
{}
FakeThreadResumeObject::FakeThreadResumeObject(RealFkResumeObj *pObj)
: mpResumeObj(pObj)
{
    if(mpResumeObj)
    {
        mpResumeObj->AddRef();
    }
}
FakeThreadResumeObject::~FakeThreadResumeObject()
{
    if(mpResumeObj)
    {
        mpResumeObj->DecreaseRef();
    }
}
FakeThreadResumeObject::FakeThreadResumeObject(const FakeThreadResumeObject &r)
: mpResumeObj(r.mpResumeObj)
{
    if(mpResumeObj)
    {
        mpResumeObj->AddRef();
    }
}
FakeThreadResumeObject & FakeThreadResumeObject::operator = (const FakeThreadResumeObject &r)
{
    if(mpResumeObj)
    {
        mpResumeObj->DecreaseRef();
    }
    mpResumeObj = r.mpResumeObj;
    if(mpResumeObj)
    {
        mpResumeObj->AddRef();
    }
    return *this;
}
bool FakeThreadResumeObject::operator == (const FakeThreadResumeObject &r) const
{
    return mpResumeObj == r.mpResumeObj;
}

int FakeThreadResumeObject::operator () (FakeThreadResumeParam *pResumeParam) const
{
    if(!mpResumeObj)
    {
        return -1;
    }
    return mpResumeObj->Resume(pResumeParam);
}
bool FakeThreadResumeObject::operator ! () const
{
    return !IsValid();
}
bool FakeThreadResumeObject::IsValid() const
{
    return mpResumeObj && !(*mpResumeObj);
}
void FakeThreadResumeObject::Detach()
{
    if (mpResumeObj)
    {
        mpResumeObj->DecreaseRef();
        mpResumeObj = NULL;
    }
}
FakeThreadResumeObject GetResumeObjectForFakeThread()
{
    if(!gllRspForFakeThread)
    {
        FakeThreadResumeObject invalid(NULL);
        return invalid;
    }
    long long saveRsp = gllRspForFakeThread;
    FakeThreadItem *pItem = FakeThread_GetItemPtr(saveRsp);
    FakeThreadResumeObject retObj(new RealFkResumeObj(pItem));
    return retObj;
}

#include <execinfo.h>
void FakeThreadPrintCallStack(void)
{
    void* callstack[128];
    int i, frames = backtrace(callstack, 128);
    char** strs = backtrace_symbols(callstack, frames);
    for (i = 0; i < frames; ++i)
    {
        printf("%s\n", strs[i]);
    }
    free(strs);
}
