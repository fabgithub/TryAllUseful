//
//  testProductConsume.h
//  TestAllUseful
//
//  Created by liyoudi on 15/3/6.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__testProductConsume__
#define __TestAllUseful__testProductConsume__

#include "FakeThreadTemplate.h"
#include "ftProductConsume.h"

#include "ftduration.h"

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "cxxtest/TestSuite.h"

const char * NFGetFileNameFromPath(const char *szFilePath);

#ifdef WIN32
#define SLASH_CHAR '\\'
#else
#define SLASH_CHAR '/'
#endif

const char * NFGetFileNameFromPath(const char *szFilePath)
{
    const char *chRet = szFilePath;
    if(szFilePath)
    {
        while(*szFilePath)
        {
            if (SLASH_CHAR == *szFilePath)
            {
                chRet = szFilePath + 1;
            }
            ++szFilePath;
        }
    }
    return chRet;
}

#define _InfoLog( args ) // std::cout << " " << NFGetFileNameFromPath(__FILE__) << "(" << __LINE__ << ") " << __FUNCTION__ << ": " args << std::endl
#define _NoteLog( args ) std::cout << " " << NFGetFileNameFromPath(__FILE__) << "(" << __LINE__ << ") " << __FUNCTION__ << ": " args << std::endl
#define _ShowLog( args ) std::cout << " " << NFGetFileNameFromPath(__FILE__) << "(" << __LINE__ << ") " << __FUNCTION__ << ": " args << std::endl

class test_ftProductConsume : public CxxTest::TestSuite
{
    ftProductConsume1<int> intPC;
    ResumeFakeThreadObjT1<int> resumeProduct;
    ftProductConsume2<int, double> intDPC;
    ResumeFakeThreadObjT2<int, double> resumeDProduct;
    int mnConsumerFinishCount;
    int mnProductorFinishCount;
    int mnTimes;
    int mnCheckInConsume;
    int mnCheckInProduct;
public:
    void setUp()
    {
        mnConsumerFinishCount = 0;
        mnProductorFinishCount = 0;
        mnTimes = 50;
        mnCheckInConsume = 0;
        mnCheckInProduct = 0;
    }
    void tearDown()
    {
        intPC.Reset();
        intDPC.Reset();
    }
public:
    static void ProductProc1(test_ftProductConsume *pThis, int nTimes)
    {
        int nRet = 0;
        for(;pThis->mnCheckInProduct < nTimes;)
        {
            int i = pThis->mnCheckInProduct;
            ++pThis->mnCheckInProduct
            _InfoLog( << "before product " << i );
            nRet = ftProduct(pThis->intPC, i);
            TS_ASSERT_EQUALS(0, nRet);
            _InfoLog( << "after product " << i );
        }
        while(pThis->intPC.HasConsumer())
        {
            nRet = ftProduct(pThis->intPC, pThis->mnCheckInProduct);
            TS_ASSERT_EQUALS(0, nRet);
            ++pThis->mnCheckInProduct;
        }
        TS_ASSERT_EQUALS(0, nRet);
        _InfoLog( << "exit" );
        ++pThis->mnProductorFinishCount;
    }
    static void ProductProc2(test_ftProductConsume *pThis, int nTimes)
    {
        int nRet = 0;
        int iCur = 0;
        int i = -1;
        while(true)
        {
            nRet = YieldFromFakeThread(pThis->resumeProduct, &i);
            if(nRet)
            {
                break;
            }
            
            _InfoLog( << "before product " << i );
            nRet = ftProduct(pThis->intPC, i);
            TS_ASSERT_EQUALS(iCur, i);
            ++iCur;
            TS_ASSERT_EQUALS(0, nRet);
            _InfoLog( << "after product " << i );
            if(!pThis->intPC.HasConsumer())
            {
                break;
            }
        }
        _InfoLog( << "exit" );
        ++pThis->mnProductorFinishCount;
    }
    static void ConsumeProc(test_ftProductConsume *pThis, int nTimes)
    {
        int n = -1;
        _InfoLog( << "enter" );
        while(pThis->mnCheckInProduct <= nTimes && 0 == ftConsume(pThis->intPC, n))
        {
//            _ShowLog( << "total is " << nTimes << ": receive " << n );
            TS_ASSERT_EQUALS(n, pThis->mnCheckInConsume);
            ++pThis->mnCheckInConsume;
            if(n >= nTimes - 1)
            {
                break;
            }
        }
        _InfoLog( << "exit" );
        ++pThis->mnConsumerFinishCount;
    }
    void testCommon()
    {
        _InfoLog( << "" );
        ftduration d;
        RunInFakeThreadT(ProductProc1, this, mnTimes);
        RunInFakeThreadT(ProductProc1, this, mnTimes);
        RunInFakeThreadT(ProductProc1, this, mnTimes);
        RunInFakeThreadT(ConsumeProc, this, mnTimes);
        RunInFakeThreadT(ConsumeProc, this, mnTimes);
        
        while(intPC.HasConsumer())
        {
            int nRet = 0;
            nRet = ftProduct(intPC, mnCheckInProduct);
            TS_ASSERT_EQUALS(0, nRet);
            ++mnCheckInProduct;
        }
        TS_ASSERT_EQUALS(2, mnConsumerFinishCount);
        TS_ASSERT_EQUALS(3, mnProductorFinishCount);
        _InfoLog( << "exit" );
        _ShowLog( << "speed is " << mnTimes / d.durationSecond() << ", mnTimes is " << mnTimes << ", used " << d.durationSecond() << " seconds." );
    }
    void testProduct()
    {
        _InfoLog( << "" );
        RunInFakeThreadT(ProductProc2, this, mnTimes);
        RunInFakeThreadT(ProductProc2, this, mnTimes);
        RunInFakeThreadT(ConsumeProc, this, mnTimes);
        ftduration d;
        for (int i = 0; i < mnTimes; ++i)
        {
            TS_ASSERT_EQUALS(0, resumeProduct(i));
        }
        TS_ASSERT_EQUALS(1, mnConsumerFinishCount);
        TS_ASSERT_EQUALS(2, mnProductorFinishCount);
        _InfoLog( << "exit" );
        _ShowLog( << "speed is " << mnTimes / d.durationSecond() << ", mnTimes is " << mnTimes << ", used " << d.durationSecond() << " seconds." );
    }
    void testConsume()
    {
        _InfoLog( << "" );
        ftduration d;
        RunInFakeThreadT(ConsumeProc, this, mnTimes);
        RunInFakeThreadT(ConsumeProc, this, mnTimes);
        RunInFakeThreadT(ConsumeProc, this, mnTimes);
        RunInFakeThreadT(ConsumeProc, this, mnTimes);
        RunInFakeThreadT(ConsumeProc, this, mnTimes);
        RunInFakeThreadT(ProductProc1, this, mnTimes);
        RunInFakeThreadT(ProductProc1, this, mnTimes);
        RunInFakeThreadT(ProductProc1, this, mnTimes);

        TS_ASSERT_EQUALS(5, mnConsumerFinishCount);
        TS_ASSERT_EQUALS(3, mnProductorFinishCount);
        _InfoLog( << "exit" );
        _ShowLog( << "speed is " << mnTimes / d.durationSecond() << ", mnTimes is " << mnTimes << ", used " << d.durationSecond() << " seconds." );
    }
    void testProduct2()
    {
        _InfoLog( << "" );
        RunInFakeThreadT(ConsumeProc, this, mnTimes);
        RunInFakeThreadT(ConsumeProc, this, mnTimes);
        RunInFakeThreadT(ConsumeProc, this, mnTimes);
        RunInFakeThreadT(ConsumeProc, this, mnTimes);
        RunInFakeThreadT(ConsumeProc, this, mnTimes);
        RunInFakeThreadT(ProductProc2, this, mnTimes);
        ftduration d;
        for (int i = 0; i < mnTimes + 10; ++i)
        {
            resumeProduct(i);
        }
        TS_ASSERT_EQUALS(5, mnConsumerFinishCount);
        TS_ASSERT_EQUALS(1, mnProductorFinishCount);
        _InfoLog( << "exit" );
        _ShowLog( << "speed is " << mnTimes / d.durationSecond() << ", mnTimes is " << mnTimes << ", used " << d.durationSecond() << " seconds." );
    }

public:
    static void ProductDProc1(test_ftProductConsume *pThis, int nTimes)
    {
        int nRet = 0;
        for(;pThis->mnCheckInProduct < nTimes;)
        {
            int i = pThis->mnCheckInProduct;
            ++pThis->mnCheckInProduct
            _InfoLog( << "before product " << i );
            nRet = ftProduct(pThis->intDPC, i, i * 1.0);
            TS_ASSERT_EQUALS(0, nRet);
            _InfoLog( << "after product " << i );
        }
        while(pThis->intDPC.HasConsumer())
        {
            nRet = ftProduct(pThis->intDPC, pThis->mnCheckInProduct, pThis->mnCheckInProduct * 1.0);
            TS_ASSERT_EQUALS(0, nRet);
            ++pThis->mnCheckInProduct;
        }
        TS_ASSERT_EQUALS(0, nRet);
        ++pThis->mnProductorFinishCount;
        _NoteLog( << "exit " << pThis->mnProductorFinishCount );
    }
    static void ProductDProc2(test_ftProductConsume *pThis, int nTimes)
    {
        int nRet = 0;
        int iCur = 0;
        int i = -1;
        double d = -1.0;
        while(true)
        {
            nRet = YieldFromFakeThread(pThis->resumeDProduct, &i, &d);
            if(nRet)
            {
                break;
            }
            
            _InfoLog( << "before product " << i );
            nRet = ftProduct(pThis->intDPC, i, i * 1.0);
            TS_ASSERT_EQUALS(iCur, i);
            TS_ASSERT_EQUALS(iCur, d);
            ++iCur;
            TS_ASSERT_EQUALS(0, nRet);
            _InfoLog( << "after product " << i );
            if(!pThis->intDPC.HasConsumer())
            {
                break;
            }
        }
        ++pThis->mnProductorFinishCount;
        _NoteLog( << "exit " << pThis->mnProductorFinishCount );
    }
    static void ConsumeDProc(test_ftProductConsume *pThis, int nTimes)
    {
        int n = -1;
        double d = -1;
        _InfoLog( << "enter" );
        while(pThis->mnCheckInProduct <= nTimes && 0 == ftConsume(pThis->intDPC, n, d))
        {
            //            _ShowLog( << "total is " << nTimes << ": receive " << n );
            TS_ASSERT_EQUALS(n, pThis->mnCheckInConsume);
            TS_ASSERT_EQUALS(d, pThis->mnCheckInConsume);
            ++pThis->mnCheckInConsume;
            if(n >= nTimes - 1)
            {
                break;
            }
        }
        ++pThis->mnConsumerFinishCount;
        _NoteLog( << "exit " << pThis->mnConsumerFinishCount );
    }
    void testDCommon()
    {
        _InfoLog( << "" );
        ftduration d;
        RunInFakeThreadT(ProductDProc1, this, mnTimes);
        RunInFakeThreadT(ProductDProc1, this, mnTimes);
        RunInFakeThreadT(ProductDProc1, this, mnTimes);
        RunInFakeThreadT(ConsumeDProc, this, mnTimes);
        RunInFakeThreadT(ConsumeDProc, this, mnTimes);
        
        while(intDPC.HasConsumer())
        {
            int nRet = 0;
            nRet = ftProduct(intDPC, mnCheckInProduct, mnCheckInProduct * 1.0);
            TS_ASSERT_EQUALS(0, nRet);
            ++mnCheckInProduct;
        }
        TS_ASSERT_EQUALS(2, mnConsumerFinishCount);
        TS_ASSERT_EQUALS(3, mnProductorFinishCount);
        _InfoLog( << "exit" );
        _ShowLog( << "speed is " << mnTimes / d.durationSecond() << ", mnTimes is " << mnTimes << ", used " << d.durationSecond() << " seconds." );
    }
    void testDProduct()
    {
        _InfoLog( << "" );
        RunInFakeThreadT(ProductDProc2, this, mnTimes);
        RunInFakeThreadT(ProductDProc2, this, mnTimes);
        RunInFakeThreadT(ConsumeDProc, this, mnTimes);
        ftduration d;
        for (int i = 0; i < mnTimes; ++i)
        {
            TS_ASSERT_EQUALS(0, resumeDProduct(i, i));
        }
        TS_ASSERT_EQUALS(1, mnConsumerFinishCount);
        TS_ASSERT_EQUALS(2, mnProductorFinishCount);
        _InfoLog( << "exit" );
        _ShowLog( << "speed is " << mnTimes / d.durationSecond() << ", mnTimes is " << mnTimes << ", used " << d.durationSecond() << " seconds." );
    }
    void testDConsume()
    {
        _NoteLog( << "" );
        ftduration d;
        RunInFakeThreadT(ConsumeDProc, this, mnTimes);
        RunInFakeThreadT(ConsumeDProc, this, mnTimes);
        RunInFakeThreadT(ConsumeDProc, this, mnTimes);
        RunInFakeThreadT(ConsumeDProc, this, mnTimes);
        RunInFakeThreadT(ConsumeDProc, this, mnTimes);
        RunInFakeThreadT(ProductDProc1, this, mnTimes);
        RunInFakeThreadT(ProductDProc1, this, mnTimes);
        RunInFakeThreadT(ProductDProc1, this, mnTimes);
        
        TS_ASSERT_EQUALS(5, mnConsumerFinishCount);
        TS_ASSERT_EQUALS(3, mnProductorFinishCount);
        _InfoLog( << "exit" );
        _ShowLog( << "speed is " << mnTimes / d.durationSecond() << ", mnTimes is " << mnTimes << ", used " << d.durationSecond() << " seconds." );
    }
    void testDProduct2()
    {
        _InfoLog( << "" );
        RunInFakeThreadT(ConsumeDProc, this, mnTimes);
        RunInFakeThreadT(ConsumeDProc, this, mnTimes);
        RunInFakeThreadT(ConsumeDProc, this, mnTimes);
        RunInFakeThreadT(ConsumeDProc, this, mnTimes);
        RunInFakeThreadT(ConsumeDProc, this, mnTimes);
        RunInFakeThreadT(ProductDProc2, this, mnTimes);
        RunInFakeThreadT(ProductDProc2, this, mnTimes);
        ftduration d;
        int i = 0;
        for (i = 0; i < mnTimes; ++i)
        {
            TS_ASSERT_EQUALS(0, resumeDProduct(i, i));
        }
        while(intDPC.HasConsumer())
        {
            int nRet = 0;
            nRet = resumeDProduct(i, i * 1.0);
            TS_ASSERT_EQUALS(0, nRet);
            ++i;
        }
        TS_ASSERT_EQUALS(5, mnConsumerFinishCount);
        TS_ASSERT_EQUALS(2, mnProductorFinishCount);
        _InfoLog( << "exit" );
        _ShowLog( << "speed is " << mnTimes / d.durationSecond() << ", mnTimes is " << mnTimes << ", used " << d.durationSecond() << " seconds." );
    }
};

#endif /* defined(__TestAllUseful__testProductConsume__) */
