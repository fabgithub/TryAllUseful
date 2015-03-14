//
//  FakeThreadWrap.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/2/11.
//  Copyright (c) 2015�� liyoudi. All rights reserved.
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
    // �����Ա����ã�ע�⣬mllOrignRsp��������
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
    long long mllFromRsp; // �Ǵ��ĸ�Э�̵�rsp������
    bool mbGotResumeObj; // �Ƿ��ȡ������resume�Ķ������û�У�����yield
    int mnState; // ��ǰ״̬, 0��ʾδ���й���1��ʾ��yield, 2��ʾ�������У�9��ʾ���н���
    
    bool mbInitialised;
    std::string mstrName;
    
    FakeThreadProc mftp;
    void *mpParam;
    
    // ����resumeʱ�Ĳ��������ֵ���Ϊyield�ķ���ֵ
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
static long long gllRspForFakeThread = 0; // ������ĸ�rsp������

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
        static int gnStackSize = 200 * 1024; // ����100KB
        char *pNewStack = new char[gnStackSize];
        pRet->mllOrignRsp = pRet->mllRsp = (long long) (pNewStack + gnStackSize - 1024); // ʵ��ʹ��99KB
        pRet->mbInitialised = true;
#endif
    }
    return pRet;
}

static void FakeThread_MainFunc()
{
    FakeThreadItem *pItem = FakeThread_GetItemPtr(gllRspForFakeThread);
    pItem->mftp(pItem->mpParam);
    
    pItem->mllRsp = pItem->mllOrignRsp; // ׼������
    gllRspForFakeThread = pItem->mllFromRsp;
    
    pItem->Clear();
    gFakeThreadItemList.push_back(pItem);

    long long saveRsp = gllRspForFakeThread;
    gllRspForFakeThread = 0; // ��ʾ�Ѿ��ص���Դ�߳�
    
    long long tmpRsp = 0;
    FakeThread_YieldWithMainRsp(saveRsp, &tmpRsp);
}
/***
 ��α�߳���ִ��һ����������
 ����9��ʾֱ�ӽ����ˡ�
 ����1��ʾ��yield�ˡ�
 */
int RunFuncInFakeThread(FakeThreadProc ftp, void *pParam)
{
    FakeThreadItem * pSI = NULL;
    pSI = NewFakeThreadItem("first", ftp, pParam);
    
    long long oldPreRsp = gllRspForFakeThread; // ��ǰ��ǰһЭ�̵�rsp
    long long savedRsp = pSI->mllRsp;
    pSI->mllRsp = 0; // �ó�0��ʾ����������
    pSI->mllFromRsp = FakeThread_GetRSP(); // ���ĸ�rsp������
    FakeThread_StartWithRsp(savedRsp, FakeThread_MainFunc, pSI, &gllRspForFakeThread);

    gllRspForFakeThread = oldPreRsp; // �ָ���ǰ��ǰһЭ��rsp
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
    gllRspForFakeThread = pItem->mllFromRsp; // ���ô��ĸ�Э�̹�����
    
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
    // ��׼��yieldʱ���Ͳ�����resume�����߳���
    if(pSI->mbGotResumeObj)
    {
        return -2;
    }
    // ���汾α�߳��Ǵ��ĸ�rspα�߳�resume�����ģ��Ա�����yield
    long long llSaveLastWhoResumeMe = gllRspForFakeThread;
    //
    long long savedRsp = pSI->mllRsp;
    pSI->mllRsp = 0; // �ó�0��ʾ����������
    pSI->mllFromRsp = FakeThread_GetRSP();
    FakeThread_ResumeWithThreadRsp(savedRsp, &gllRspForFakeThread, pSI, pSI->mpResumeParam);
    // �ָ���˭resume��α�̵߳�rsp
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
