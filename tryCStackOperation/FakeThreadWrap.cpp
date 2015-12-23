//
//  FakeThreadWrap.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/2/11.
//  Copyright (c) 2015�� liyoudi. All rights reserved.
//

#include "FakeThreadWrap.h"

#include <string>
#ifdef WIN32
    #include <Windows.h>
#else
    #include <pthread.h>
    #include <unistd.h>
#endif

#include <list>
#include <set>

#include <stdio.h>
#include <stdlib.h>

#if __x86_64__ || defined(_WIN64)
    typedef long long FtRspType;
#else
    typedef unsigned int FtRspType;
#endif

FtStackInfo::FtStackInfo()
: mnLine(-1)
{}
FtStackInfo::FtStackInfo(const std::string &strName, const std::string &strFile, int nLine)
: mstrName(strName), mstrFile(strFile), mnLine(nLine)
{}

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
        mStackList.clear();
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
        mStackList.clear();
    }
public:
    FtRspType mllRsp;
    FtRspType mllOrignRsp;
    FtRspType mllFromRsp; // �Ǵ��ĸ�Э�̵�rsp������
    bool mbGotResumeObj; // �Ƿ��ȡ������resume�Ķ������û�У�����yield
    int mnState; // ��ǰ״̬, 0��ʾδ���й���1��ʾ��yield, 2��ʾ�������У�9��ʾ���н���
    
    bool mbInitialised;
    std::string mstrName;
    FtStackInfoList mStackList;
    
    FakeThreadProc mftp;
    void *mpParam;
    
    // ����resumeʱ�Ĳ��������ֵ���Ϊyield�ķ���ֵ
    FakeThreadResumeParam *mpResumeParam;
};

// ==============
extern "C" FakeThreadItem * FakeThread_StartWithRsp(FtRspType llNewRsp, void (*pfunc) (), FakeThreadItem *pItem, FtRspType *pSaveMainRsp );
extern "C" FakeThreadResumeParam * FakeThread_YieldWithMainRsp(FtRspType llMainRsp, FtRspType *pThreadRsp);
extern "C" FakeThreadItem * FakeThread_GetItemPtr(FtRspType llMainRsp);
extern "C" void FakeThread_ResumeWithThreadRsp(FtRspType llThreadRsp, FtRspType *pSaveMainRsp, FakeThreadItem *pItem, FakeThreadResumeParam *pResumeParam);
// ==============

extern "C" FtRspType FakeThread_GetRSP();

// ================================
static FtRspType gllRspForFakeThread = 0; // ������ĸ�rsp������

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
static FakeThreadItemList gFakeThreadItemFreeList;

typedef std::set<FakeThreadItem *> FakeThreadItemSet;
static FakeThreadItemSet gFakeThreadRunningSet;

#define USE_REAL_THREAD_STACK 0

FakeThreadItem * NewFakeThreadItem(const std::string &strName, FakeThreadProc ftp, void *pParam)
{
    FakeThreadItem *pRet = NULL;
    if(!gFakeThreadItemFreeList.empty())
    {
        pRet = gFakeThreadItemFreeList.front();
        gFakeThreadItemFreeList.pop_front();
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
        static int gnStackSize = 100 * 1024; // ����100KB
        char *pNewStack = new char[gnStackSize];
        pRet->mllOrignRsp = pRet->mllRsp = (FtRspType) (pNewStack + gnStackSize - 1024); // ʵ��ʹ��99KB
        pRet->mbInitialised = true;
#endif
    }
    gFakeThreadRunningSet.insert(pRet);
    pRet->mStackList.clear();
    pRet->mStackList.push_back(FtStackInfo(strName.c_str(), "", -1));
    return pRet;
}

static void FakeThread_MainFunc()
{
    FakeThreadItem *pItem = FakeThread_GetItemPtr(gllRspForFakeThread);
    pItem->mllFromRsp = gllRspForFakeThread; // �������������

    pItem->mftp(pItem->mpParam);
    
    pItem->mllRsp = pItem->mllOrignRsp; // ׼������
    gllRspForFakeThread = pItem->mllFromRsp;
    
    pItem->Clear();
    gFakeThreadItemFreeList.push_back(pItem);
    gFakeThreadRunningSet.erase(pItem);

    FtRspType saveRsp = gllRspForFakeThread;
    gllRspForFakeThread = 0; // ��ʾ�Ѿ��ص���Դ�߳�
    
    FtRspType tmpRsp = 0;
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
    pSI = NewFakeThreadItem("", ftp, pParam);
    
    FtRspType oldPreRsp = gllRspForFakeThread; // ��ǰ��ǰһЭ�̵�rsp
    FtRspType savedRsp = pSI->mllRsp;
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
    FtRspType saveRsp = gllRspForFakeThread;
    FakeThreadItem *pItem = FakeThread_GetItemPtr(saveRsp);
    if(!pItem->mbGotResumeObj)
    {
        return -2;
    }
    gllRspForFakeThread = pItem->mllFromRsp; // ���ô��ĸ�Э�̹�����
    
    FakeThreadResumeParam *pYieldParam = FakeThread_YieldWithMainRsp(saveRsp, &pItem->mllRsp);
    pItem->mllFromRsp = gllRspForFakeThread; // �������������
    
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
    FtRspType llSaveLastWhoResumeMe = gllRspForFakeThread;
    //
    FtRspType savedRsp = pSI->mllRsp;
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
        return mpSI == NULL;
    }
	bool IsValid() const
	{
		return mpSI != NULL;
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
    RealFkResumeObj *pTmp = mpResumeObj;
    mpResumeObj = NULL;
    int nRet = pTmp->Resume(pResumeParam);
    pTmp->DecreaseRef();
    return nRet;
}
bool FakeThreadResumeObject::operator ! () const
{
    return !IsValid();
}
bool FakeThreadResumeObject::IsValid() const
{
	return mpResumeObj && (mpResumeObj->IsValid());
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
    FtRspType saveRsp = gllRspForFakeThread;
    FakeThreadItem *pItem = FakeThread_GetItemPtr(saveRsp);
    FakeThreadResumeObject retObj(new RealFkResumeObj(pItem));
    return retObj;
}
unsigned int GetRunningFakeThreadCount()
{
    return (unsigned int) gFakeThreadRunningSet.size();
}

#ifndef WIN32
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
#endif

// -------------------------------------------------
// for debug
FTNET_API int FakeThreadSetName(const char *szFakeThreadName)
{
    if(!gllRspForFakeThread)
    {
        return -1;
    }
    FakeThreadItem *pItem = FakeThread_GetItemPtr(gllRspForFakeThread);
    if(pItem->mstrName.size() > 0 && pItem->mstrName.size() < 50)
    {
        pItem->mstrName += szFakeThreadName;
    }
    else
    {
        pItem->mstrName = szFakeThreadName;
    }
    if(!pItem->mStackList.empty())
    {
        pItem->mStackList.front().mstrName = pItem->mstrName;
    }
    else
    {
        pItem->mStackList.push_front(FtStackInfo(pItem->mstrName, "", -1));
    }
    return 0;
}
FTNET_API const char *FakeThreadGetName(bool bNullWhenInMain)
{
    if(!gllRspForFakeThread)
    {
        return bNullWhenInMain ? NULL : "";
    }
    FakeThreadItem *pItem = FakeThread_GetItemPtr(gllRspForFakeThread);
    return pItem->mstrName.c_str();
}
FakeThreadStackHelper::FakeThreadStackHelper(const char *szFuncName, const char *szFile, int nLine)
{
    if(!gllRspForFakeThread)
    {
        return ;
    }
    FakeThreadItem *pItem = FakeThread_GetItemPtr(gllRspForFakeThread);
    FtStackInfo si(szFuncName, szFile, nLine);
    pItem->mStackList.push_back(si);
}
FakeThreadStackHelper::~FakeThreadStackHelper()
{
    if(!gllRspForFakeThread)
    {
        return ;
    }
    FakeThreadItem *pItem = FakeThread_GetItemPtr(gllRspForFakeThread);
    if(pItem->mStackList.size() <= 1)
    {
        return ;
    }
    pItem->mStackList.pop_back();
}
FTNET_API FtStackInfoList FakeThreadGetStack()
{
    FtStackInfoList st;
    if(!gllRspForFakeThread)
    {
        return st;
    }
    FakeThreadItem *pItem = FakeThread_GetItemPtr(gllRspForFakeThread);
    return pItem->mStackList;
}
FTNET_API FakeThreadInfoList FakeThreadGetAllStack()
{
    FakeThreadInfoList ftil;
    FakeThreadItemSet::iterator iter;
    for (iter = gFakeThreadRunningSet.begin(); iter != gFakeThreadRunningSet.end(); ++iter)
    {
        ftil.push_back((*iter)->mStackList);
    }
    return ftil;
}
