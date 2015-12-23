//
//  testFakeThreadWrap.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/11.
//  Copyright (c) 2015 liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__testFakeThreadWrap__
#define __TestAllUseful__testFakeThreadWrap__

#include "FakeThreadWrap.h"
#include "FakeThreadTemplate.h"
#include "FakeThreadLock.h"

#include "ftduration.h"

#include <stdio.h>
#ifdef WIN32
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#include "cxxtest/TestSuite.h"

const char * NFGetFileNameFromPath(const char *szFilePath);

#ifndef InfoLog
#define InfoLog( args )  std::cout << " " << NFGetFileNameFromPath(__FILE__) << "(" << __LINE__ << ") " << __FUNCTION__ << ": " args << std::endl
#endif

class testFakeThreadWrap : public CxxTest::TestSuite
{
    std::string mstrCurFunc;
    FakeThreadResumeObject mResumeForCommon;
    FakeThreadResumeObject mResumeForYieldWithParam;
    FakeThreadResumeObject mResumeFortestReuseFakeThread;
public:
    static void SomeFunc(void *pParam)
    {
        testFakeThreadWrap *pTS = (testFakeThreadWrap *) pParam;
        std::string funcName;
        funcName = pTS->mstrCurFunc;
        
        InfoLog( << "before YieldFromFakeThread from " << funcName );
        if(funcName == "testCommon")
        {
            pTS->mResumeForCommon = GetResumeObjectForFakeThread();
        }
        else if(funcName == "testYieldWithParam")
        {
            pTS->mResumeForYieldWithParam = GetResumeObjectForFakeThread();
        }
        else
        {
        }
//        GetResumeObjectForFakeThread();
        if(YieldFromFakeThread())
        {
            InfoLog( << "YieldFromFakeThread failed. " );
        }
        InfoLog( << "after YieldFromFakeThread from " << funcName );
    }
    void testCommon()
    {
        mstrCurFunc = "testCommon";
        RunFuncInFakeThread(SomeFunc, this);
    }
    void testYieldWithParam()
    {
        mstrCurFunc = "testYieldWithParam";
        RunFuncInFakeThread(SomeFunc, this);
    }
    void testResumeWithParam()
    {
        mResumeForCommon();
        mResumeForYieldWithParam();

        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
    void testReuseFakeThread()
    {
        mstrCurFunc = "testReuseFakeThread";
        RunFuncInFakeThread(SomeFunc, this);
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
    
    class ResumeParam : public FakeThreadResumeParam
    {
    public:
        std::string mstrName;
        ResumeParam() : mstrName("ResumeParam") {}
        virtual const char *FtsParamType() const
        {
//            static char gszType[] = "ResumeParam";
            return mstrName.c_str();
        }
    };
    class ResumeParam2 : public FakeThreadResumeParam
    {
    public:
        std::string mstrName;
        ResumeParam2() : mstrName("ResumeParam2") {}
        virtual const char *FtsParamType() const
        {
//            static char gszType[] = "ResumeParam2";
            return mstrName.c_str();
        }
    };
    // =================================================
    // test resume another fake thread in some fake thread
    FakeThreadResumeObject gResume_testResumeInFakeThread_func1_Obj;
    FakeThreadResumeObject gResume_testResumeInFakeThread_func1_Obj_nouse;
    
    static void SomeDemoFunc()
    {
//        FakeThreadPrintCallStack();
    }
    static void ResumeInFakeThread_func1(void *pParam)
    {
        SomeDemoFunc();

        FakeThreadResumeParam *pResumeParam = NULL;
        int nYieldRes = 0;
        testFakeThreadWrap *pTS = (testFakeThreadWrap *) pParam;
        
        InfoLog( << "before yield 1" );
        pTS->gResume_testResumeInFakeThread_func1_Obj = GetResumeObjectForFakeThread();
        TS_ASSERT_EQUALS(pTS->gResume_testResumeInFakeThread_func1_Obj.IsValid(), true);
        nYieldRes = YieldFromFakeThread(&pResumeParam);
        TS_ASSERT_EQUALS(pTS->gResume_testResumeInFakeThread_func1_Obj.IsValid(), false);
        TS_ASSERT(pResumeParam && strcmp(pResumeParam->FtsParamType(), "ResumeParam") == 0);
        TS_ASSERT_EQUALS(nYieldRes, 0);
        InfoLog( << "after yield 1" );
        
//        FakeThreadPrintCallStack();
        
        InfoLog( << "before yield 2" );
        pTS->gResume_testResumeInFakeThread_func1_Obj = GetResumeObjectForFakeThread();
        pTS->gResume_testResumeInFakeThread_func1_Obj_nouse = GetResumeObjectForFakeThread();
        TS_ASSERT_EQUALS(pTS->gResume_testResumeInFakeThread_func1_Obj.IsValid(), true);
        pResumeParam = NULL;
        nYieldRes = YieldFromFakeThread(&pResumeParam);
        TS_ASSERT_EQUALS(pTS->gResume_testResumeInFakeThread_func1_Obj.IsValid(), false);
        TS_ASSERT(pResumeParam && strcmp(pResumeParam->FtsParamType(), "ResumeParam2") == 0);
        TS_ASSERT_EQUALS(nYieldRes, 0);
        InfoLog( << "after yield 2" );
    }
    static void ResumeInFakeThread_func2(void *pParam)
    {
        ResumeParam rp1;
        ResumeParam2 rp2;
        int nResumeRes = 0;
        testFakeThreadWrap *pTS = (testFakeThreadWrap *) pParam;
        InfoLog( << "before resume 1" );
        nResumeRes = pTS->gResume_testResumeInFakeThread_func1_Obj(&rp1);
        TS_ASSERT_EQUALS(nResumeRes, 0);
        InfoLog( << "after resume 1" );

        InfoLog( << "before resume 2" );
        nResumeRes = pTS->gResume_testResumeInFakeThread_func1_Obj(&rp2);
        TS_ASSERT_EQUALS(nResumeRes, 0);
        InfoLog( << "after resume 2" );
    }
    void testResumeInFakeThread()
    {
        ResumeParam rp;
        
//        FakeThreadPrintCallStack();

        std::cout << std::endl;
        std::cout << std::endl;
        RunFuncInFakeThread(ResumeInFakeThread_func1, this);
        RunFuncInFakeThread(ResumeInFakeThread_func2, this);
        int nResumeRes = gResume_testResumeInFakeThread_func1_Obj(&rp);
        TS_ASSERT_EQUALS(nResumeRes, -1);

        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
    static void ForTestMemory(void *pParam)
    {
        if(pParam)
        {
            std::list<FakeThreadResumeObject> *pAllFakeObj = (std::list<FakeThreadResumeObject> *) pParam;
            (*pAllFakeObj).push_back(GetResumeObjectForFakeThread());
            YieldFromFakeThread();
        }
    }
    void testMemorySizeForFakeThread()
    {
        // alloc 100 thread, see memory
        {
            std::list<FakeThreadResumeObject> allFakeObj;
            int nCount = 20;
            for (; nCount > 0; --nCount)
            {
                RunFuncInFakeThread(ForTestMemory, (void *) &allFakeObj);
            }
            InfoLog( << "finish." );
            std::cout << std::endl;
            std::cout << std::endl;
        }
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
    void testMemorySizeForFakeThread2()
    {
        ftduration d;
        int nTotalCount = 10 * 1000;
        for(int i = 0;i < nTotalCount;i++)
        {
            RunFuncInFakeThread(ForTestMemory, NULL);
        }
        InfoLog( << "finish." );
        InfoLog( << "RunFuncInFakeThread call can be " << nTotalCount / d.durationSecond() << " per second." );
        std::cout << std::endl;
        std::cout << std::endl;
        
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
    // test resume performence
    class ResumeParamForCount : public FakeThreadResumeParam
    {
    public:
        ResumeParamForCount() : mnIndex(0) {}
    public:
        virtual const char *FtsParamType() const
        {
            static char gszType[] = "ResumeParam";
            return gszType;
        }
        
        int mnIndex;
    };
    static void ForTestResumeCount(void *pParam)
    {
        FakeThreadResumeParam *pRp = NULL;
        //pthread_attr_t attr;
        //pthread_attr_init(&attr);
        //size_t nStackSize = 0;
        //pthread_attr_getstacksize(&attr, &nStackSize);
//        char szFillStack[10 * 50 * 1024] = "";
//        szFillStack[0] = 0;
        FakeThreadResumeObject *pResumeObj = (FakeThreadResumeObject *) pParam;
        while (true)
        {
            *pResumeObj = GetResumeObjectForFakeThread();
            int nYieldRes = YieldFromFakeThread(&pRp);
            TS_ASSERT(nYieldRes == 0);
            
            if(pRp)
            {
                ResumeParamForCount *pRpc = (ResumeParamForCount *) pRp;
                if (pRpc->mnIndex % (1000 * 1000) == 0)
                {
                    std::cout << pRpc->mnIndex << std::endl;
                }
            }
            else
            {
                break;
            }
        }
    }
    void testResumeCount()
    {
        {
            ResumeParamForCount rp;
            FakeThreadResumeObject resumeObj;
            RunFuncInFakeThread(ForTestResumeCount, &resumeObj);
            ftduration d;
            int nTotalCount = 10;
    //         nTotalCount = 10 * 1000 * 1000;
            for (int i = 0; i < nTotalCount; ++i)
            {
                rp.mnIndex = i;
                resumeObj(&rp);
            }
    //        resumeObj();
            InfoLog( << "yield resume call can be " << nTotalCount / d.durationSecond() << " per second." );
        }
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
    // test template function group performence
    ResumeFakeThreadObjT1<int> mResumeObj_ForTestResumeCountT;
    static void ForTestResumeCountT(testFakeThreadWrap *pThis, const std::string info, ResumeFakeThreadObjT1<int> *pResumeObj)
    {
        while (true)
        {
            int nIndex = 0;
            int nYieldRes = YieldFromFakeThread(*pResumeObj, &nIndex);
            TS_ASSERT(nYieldRes == 0);
            if(nYieldRes == 0 && nIndex >= 0)
            {
                if (nIndex % (1000 * 1000) == 0)
                {
                    std::cout << nIndex << std::endl;
                }
            }
           else
           {
               break;
           }
        }
    }
    void testResumeCountT()
    {
//        return;
        const std::string info("t");
        ResumeFakeThreadObjT1<int> resumeObj;
        RunInFakeThreadT(ForTestResumeCountT, this, info, &resumeObj);
        ftduration d;
        int nTotalCount = 10;
//         nTotalCount = 10 * 1000 * 1000;
        for (int i = 0; i < nTotalCount; ++i)
        {
            resumeObj(i);
        }
        resumeObj(-1);
        InfoLog( << "yield resume call can be " << nTotalCount / d.durationSecond() << " per second." );
        
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
    
    static void FuncForTemplate_Common(double d)
    {
        TS_ASSERT_EQUALS(d, 123.56);
        InfoLog( << "end" );
    }
    
    static void FuncForTemplate_Common2(double d, const char *szStr)
    {
        TS_ASSERT_EQUALS(d, 123.56);
        TS_ASSERT_EQUALS(std::string(szStr), "abc");
        InfoLog( << "end" );
    }
    
    ResumeFakeThreadObjT1<std::string> mResumeT1;
    static void FuncForTemplate_Common3(testFakeThreadWrap *pThis, std::string strText, int m)
    {
        TS_ASSERT_EQUALS(strText, "in string param");

        int nRet = 0;
        std::string strFromResume;
        nRet = YieldFromFakeThread(pThis->mResumeT1, &strFromResume);
        TS_ASSERT_EQUALS(nRet, 0);
        TS_ASSERT_EQUALS(strFromResume, "Resume with parameter");

        TS_ASSERT_EQUALS(m, 987);
        InfoLog( << "end" );
    }
    
    void testFakeThreadTemplate_Common()
    {
        RunInFakeThreadT(.15, FuncForTemplate_Common, 123.56);
        RunInFakeThreadT(FuncForTemplate_Common2, 123.56, "abc");
        RunInFakeThreadT(.1, FuncForTemplate_Common3, this, std::string("in string param"), 987);
        
        while(fttimer_count() > 0)
        {
#ifdef WIN32
            Sleep(10);
#else
            usleep(1000);
#endif
            fttimer_do();
        }
        InfoLog( << "end" );
        
        TS_ASSERT_EQUALS(1, GetRunningFakeThreadCount());
    }
    void testFakeThreadTemplate_Start()
    {
        mResumeT1("Resume with parameter");
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
    void testFakeThreadTemplate_Yield()
    {
        
    }
    // test fake thread lock
    static void FuncForLockTest(FakeThreadResumeObject * resumeObj, int *nTestNum, int nIndex)
    {
        InfoLog( << "[" << nIndex << "] before protected" );
        FtProtectFunction();
        InfoLog( << "[" << nIndex << "] after protected" );
        
        *resumeObj = GetResumeObjectForFakeThread();
        InfoLog( << "[" << nIndex << "] before yield" );
        YieldFromFakeThread();
        InfoLog( << "[" << nIndex << "] after yield" );
    }
    void testFakeThreadTemplate_Resume()
    {
        {
            FakeThreadResumeObject resumeObj1, resumeObj2, resumeObj3, resumeObj4;
            int nTestNum = 0;
            RunInFakeThreadT(FuncForLockTest, &resumeObj1, &nTestNum, 1);
            RunInFakeThreadT(FuncForLockTest, &resumeObj2, &nTestNum, 2);
            RunInFakeThreadT(FuncForLockTest, &resumeObj3, &nTestNum, 3);
            RunInFakeThreadT(FuncForLockTest, &resumeObj4, &nTestNum, 4);
            
            TS_ASSERT(resumeObj1.IsValid());
            TS_ASSERT(!resumeObj2.IsValid());
            TS_ASSERT(!resumeObj3.IsValid());
            TS_ASSERT(!resumeObj4.IsValid());
            resumeObj1();
            TS_ASSERT(resumeObj2.IsValid());
            TS_ASSERT(!resumeObj3.IsValid());
            TS_ASSERT(!resumeObj4.IsValid());
            resumeObj2();
            TS_ASSERT(resumeObj3.IsValid());
            TS_ASSERT(!resumeObj4.IsValid());
            resumeObj3();
            TS_ASSERT(resumeObj4.IsValid());
        }
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
};

#endif /* defined(__TestAllUseful__testFakeThreadWrap__) */
