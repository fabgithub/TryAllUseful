//
//  test_ftrpc.h
//  TestAllUseful
//
//  Created by liyoudi on 15/3/2.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__test_ftrpc__
#define __TestAllUseful__test_ftrpc__

#include <stdio.h>

#include "ftnetbuffer.h"
#include "ftrpc_callable_manager.h"

#include "ftsocket.h"
#include "FakeThreadTemplate.h"
#include "ftnetselect.h"
#include "ftduration.h"

#include "cxxtest/TestSuite.h"

const char * NFGetFileNameFromPath(const char *szFilePath);

#define InfoLog( args )  // std::cout << " " << NFGetFileNameFromPath(__FILE__) << "(" << __LINE__ << ") " << __FUNCTION__ << ": " args << std::endl

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

struct tmpStruct
{
    tmpStruct() : n(0) {}
    tmpStruct(int i) : n(i) {}
    double d;
    int n;
    char c;
};

class HasStCls
{
public:
    HasStCls() : m(0) {}
    void SetValues()
    {
        st1.n = 1;
        st2.n = 2;
        st1.c = 3;
        st2.c = 4;
        st1.d = 5;
        st2.d = 98.1;
        
        nums.insert(0x100);
        nums.insert(0x200);
        nums.insert(0x300);
        
        idm[2] = 100;
        idm[3] = 300;
        
        m = 0x12345678;
    }
    int sum()
    {
        static int gnOccorError = 0;
//        ++gnOccorError;
        int nRet = 0;
        nRet = st1.n + st2.n + st1.c + st2.c + st1.d + st2.d + gnOccorError;
        std::set<int>::iterator iter;
        for(iter = nums.begin();iter != nums.end();++iter)
        {
            nRet += *iter;
        }
        
        std::map<int, double>::iterator iterIdm;
        for (iterIdm = idm.begin(); iterIdm != idm.end(); ++iterIdm)
        {
            nRet += iterIdm->first + iterIdm->second;
        }
        nRet += m;
        return nRet;
    }
    tmpStruct st1;
    tmpStruct st2;
    std::set<int> nums;
    std::map<int, double> idm;
    int m;
};

typedef std::set<int> NumSet;
typedef std::vector<const char *> StrSet;
typedef std::map<int, double> IntDoubleMap;
ftrpc_DECLARE_TYPE_PACK(NumSet);
ftrpc_DECLARE_TYPE_PACK(StrSet);
ftrpc_DECLARE_TYPE_PACK(IntDoubleMap)

#include "ftrpc_pack_container.h"
#include "ftrpc.h"

ftrpc_IMPLEMENT_CONTAINER_TYPE(NumSet);
ftrpc_IMPLEMENT_CONTAINER_TYPE(StrSet);
ftrpc_IMPLEMENT_CONTAINER_TYPE(IntDoubleMap);
// =======================================
class ts__ : public tmpStruct
{
public:
    std::set<int> nn;
    IntDoubleMap idm;
};
ftrpc_IMPLEMENT_STRUCE_PACK(ts__, 2, idm, nn)
ftrpc_IMPLEMENT_STRUCE_PACK(tmpStruct, 3, n, c, d)
ftrpc_IMPLEMENT_STRUCE_PACK(HasStCls, 5, nums, m, idm, st1, st2)

class test_ftrpc : public CxxTest::TestSuite
{
    int mnFinish;
public:
    void setUp()
    {
        signal(SIGPIPE, SIG_IGN);
        mnFinish = 0;
    }
public:
    static int PushDeviceList(StrSet ss)
    {
        return (int) ss.size();
    }
    static int AddForRpc(int n, int m)
    {
        return n + m;
    }
    static int FuncForHasStCls(HasStCls hsc1, HasStCls hsc2)
    {
        return hsc1.sum() + hsc2.sum();
    }
    static int FuncForRpc(int n)
    {
        return n * n * 8;
    }
    static tmpStruct FuncForRpc_WithStruct(tmpStruct st1, tmpStruct st2, tmpStruct st3, tmpStruct st4, tmpStruct st5)
    {
        tmpStruct ret ( st1.n + st2.n + st3.n + st4.n + st5.n );
        return ret;
    }
    static double FuncForRpc2(int n, double d)
    {
        return n * d;
    }
    static int ConvertToNumByRemoteCall(const char *szNum, ftnetbuffer numBuf)
    {
        return atoi(szNum) + atoi(numBuf.buffer());
    }
    static int FuncForRpcServer(const char *szFuncName, const _ftBufArray &parameters, void *pParam)
    {
        int nRet = 0;
//        test_ftrpc *pThis = (test_ftrpc *) pParam;
        InfoLog( << "received '" << szFuncName << "' call request." );
        return nRet;
    }
    static void ServerFunc(test_ftrpc *pThis, const char *szIp, int nPort)
    {
        int nRet = 0;
        ftsocket svrSock;
        nRet = svrSock.Bind(szIp, nPort);
        TS_ASSERT_EQUALS(nRet, 0);
        while(true)
        {
            ftsocket cliSock;
            nRet = svrSock.Accept(cliSock);
            TS_ASSERT_EQUALS(nRet, 0);
            // 设置处理函数
            ftrpc rpc(cliSock);
            ftrpc_callable_manager mgr;
            mgr.RegisterFunc("PushDeviceList", PushDeviceList);
            mgr.RegisterFunc("AddForRpc", AddForRpc);
            mgr.RegisterFunc("FuncForRpc_WithStruct", FuncForRpc_WithStruct);
            mgr.RegisterFunc("FuncForRpc", FuncForRpc);
            mgr.RegisterFunc("FuncForRpc2", FuncForRpc2);
            mgr.RegisterFunc("ConvertToNumByRemoteCall", ConvertToNumByRemoteCall);
            mgr.RegisterFunc("FuncForHasStCls", FuncForHasStCls);
            rpc.SetCallableMgr(mgr);
        }
    }
    static void ClientFunc(test_ftrpc *pThis, const char *szIp, int nPort, int *pnTimes)
    {
        int nRet = 0;
        ftsocket sock;
        ftrpc rpc(sock);
        nRet = sock.Connect(szIp, nPort);
        TS_ASSERT_EQUALS(nRet, 0);
        if(nRet)
        {
            InfoLog( << "sock error " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
        int nRpcRet = 0;
        int nInParam = 5;
        double dNum = 3.2;
        double dRet = 0.0;
        const char szNumber[] = "129876521";
        tmpStruct retSt, st1(2), st2(3);
        ftnetbuffer numBuf(1024);
        strcpy(numBuf.buffer(), "292929");
        HasStCls hsc;
        hsc.SetValues();
        while((*pnTimes) > 0)
        {
            --(*pnTimes);

            nRet = rpc.RemoteCall("FuncForHasStCls", nRpcRet, FuncForHasStCls, hsc, hsc);
            TS_ASSERT_EQUALS(nRet, 0);
            if(nRet)
            {
                InfoLog( << "last error is " << sock.LastErrorNo() << ": " << sock.LastError() );
            }
            TS_ASSERT_EQUALS(nRpcRet, FuncForHasStCls(hsc, hsc))
            
            nRet = rpc.RemoteCall("AddForRpc", nRpcRet, AddForRpc, nInParam, nInParam + 1);
            TS_ASSERT_EQUALS(nRet, 0);
            if(nRet)
            {
                InfoLog( << "last error is " << sock.LastErrorNo() << ": " << sock.LastError() );
            }
            TS_ASSERT_EQUALS(nRpcRet, AddForRpc(nInParam, nInParam + 1))
            
            nRet = rpc.RemoteCall("FuncForRpc_WithStruct", retSt, FuncForRpc_WithStruct, st1, st2, st1, st2, st1);
            TS_ASSERT_EQUALS(nRet, 0);
            if(nRet)
            {
                InfoLog( << "last error is " << sock.LastErrorNo() << ": " << sock.LastError() );
            }
            TS_ASSERT_EQUALS(retSt.n, FuncForRpc_WithStruct(st1, st2, st1, st2, st1).n)
            
            nRet = rpc.RemoteCall("ConvertToNumByRemoteCall", nRpcRet, ConvertToNumByRemoteCall, szNumber, numBuf);
            TS_ASSERT_EQUALS(nRet, 0);
            if(nRet)
            {
                InfoLog( << "last error is " << sock.LastErrorNo() << ": " << sock.LastError() );
            }
            TS_ASSERT_EQUALS(nRpcRet, ConvertToNumByRemoteCall(szNumber, numBuf))
            
            nRet = rpc.RemoteCall("FuncForRpc", nRpcRet, FuncForRpc, nInParam);
            TS_ASSERT_EQUALS(nRet, 0);
            if(nRet)
            {
                InfoLog( << "last error is " << sock.LastErrorNo() << ": " << sock.LastError() );
            }
            TS_ASSERT_EQUALS(nRpcRet, FuncForRpc(nInParam))
            
            nRet = rpc.RemoteCall("FuncForRpc2", dRet, FuncForRpc2, nInParam, dNum);
            TS_ASSERT_EQUALS(nRet, 0);
            if(nRet)
            {
                InfoLog( << "last error is " << sock.LastErrorNo() << ": " << sock.LastError() );
            }
            TS_ASSERT_EQUALS(dRet, FuncForRpc2(nInParam, dNum))
        }
        pThis->mnFinish = 1;
    }
    void testCommon()
    {
        const char *szIp = "127.0.0.1";
        int nPort = 32111;
        int nTimes = 1 * 100;
        int nTmpTime = nTimes;
        RunInFakeThreadT(ServerFunc, this, szIp, nPort);
        int nClientCount = 1;
        while(nClientCount > 0)
        {
            RunInFakeThreadT(ClientFunc, this, szIp, nPort, &nTmpTime);
            --nClientCount;
        }
        
        ftduration d;
        while(!mnFinish)
        {
            ftnetSelect(100);
        }
        TS_ASSERT_EQUALS(mnFinish, 1);
        std::cout <<  __func__ << " speed is " << nTimes / d.durationSecond() << " rpc/s, " << nTimes << " call used " << d.durationSecond() << " seconds." << std::endl;
        std::cout << "ftnetbuffer::TotalInstanceCount() = " << ftnetbuffer::TotalInstanceCount() << std::endl;
    }
    static void Func1(ResumeFakeThreadObjT1<const char *> *pResumeObj)
    {
        const char *n = 0;
        InfoLog( << "before yield -----" );
        TS_ASSERT_EQUALS(0, YieldFromFakeThread(*pResumeObj, &n));
        InfoLog( << "after yield " << n );
        InfoLog( << "before yield ******" );
        TS_ASSERT_EQUALS(0, YieldFromFakeThread(*pResumeObj, &n));
        InfoLog( << "after yield " << n );
    }
    static void Func2(ResumeFakeThreadObjT1<const char *> *pResumeObj1, ResumeFakeThreadObjT1<const char *> *pResumeObj2)
    {
        InfoLog( << "before run fake thread in fake thread." );
        TS_ASSERT_EQUALS(0, RunInFakeThreadT(Func1, pResumeObj1));
        InfoLog( << "after run fake thread in fake thread." );
        
        const char *n = 0;
        InfoLog( << "before yield <<<<<" );
        TS_ASSERT_EQUALS(0, YieldFromFakeThread(*pResumeObj2, &n));
        InfoLog( << "after yield >>>>>> " << n );
        
        InfoLog( << "before resume" );
        (*pResumeObj1)("3333333333333333333");
        InfoLog( << "after resume" );

        InfoLog( << "before yield ~~~~~~" );
        TS_ASSERT_EQUALS(0, YieldFromFakeThread(*pResumeObj2, &n));
        InfoLog( << "after yield ####### " << n );
        
    }
    void testRemoteCall()
    {
        ResumeFakeThreadObjT1<const char *> resumeObj1, resumeObj2;
        TS_ASSERT_EQUALS(0, RunInFakeThreadT(Func2, &resumeObj1, &resumeObj2));
        InfoLog( << "before resume" );
        TS_ASSERT_EQUALS(0, resumeObj1("111111111111111111"));
        InfoLog( << "after resume" );

        InfoLog( << "before resume" );
        TS_ASSERT_EQUALS(0, resumeObj2("222222222222222222"));
        InfoLog( << "after resume" );
        
        InfoLog( << "before resume" );
        TS_ASSERT_EQUALS(0, resumeObj2("444444444444444444"));
        InfoLog( << "after resume" );
        
        InfoLog( << "end run" );
    }

    static void ClientFuncForDeviceList(test_ftrpc *pThis, const char *szIp, int nPort, int *pnTimes, int nDeviceCount)
    {
        StrSet ips, ids;
        static char szTmpStr[] = "_111111111122222222222333333333311";
        for (int nn = 0; nn < nDeviceCount; ++nn)
        {
            ips.push_back(szTmpStr);
            ids.push_back(szTmpStr);
//            ips.insert(szTmpStr);
//            ids.insert(szTmpStr);
        }
        int nRet = 0;
        ftsocket sock;
        ftrpc rpc(sock);
        nRet = sock.Connect(szIp, nPort);
        TS_ASSERT_EQUALS(nRet, 0);
        if(nRet)
        {
            InfoLog( << "sock error " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
        int nRpcRet = 0;
        HasStCls hsc;
        hsc.SetValues();
        while((*pnTimes) > 0)
        {
            --(*pnTimes);
            
            nRet = rpc.RemoteCall("PushDeviceList", nRpcRet, PushDeviceList, ids);
            TS_ASSERT_EQUALS(nRet, 0);
            if(nRet)
            {
                InfoLog( << "last error is " << sock.LastErrorNo() << ": " << sock.LastError() );
            }
            TS_ASSERT_EQUALS(nRpcRet, PushDeviceList(ids))
        }
        pThis->mnFinish = 1;
    }
    
    void testPerform()
    {
        int nRet = 0;
        const char *szIp = "127.0.0.1";
        int nPort = 32111;
        int nTimes = 1 * 1000;
        int nDeviceCount = 200;
        int nTmpTime = nTimes;
//        RunInFakeThreadT(ServerFunc, this, szIp, nPort);
        int nClientCount = 1;
        while(nClientCount > 0)
        {
            nRet = RunInFakeThreadT(ClientFuncForDeviceList, this, szIp, nPort, &nTmpTime, nDeviceCount);
            TS_ASSERT_EQUALS(nRet, 0);
            --nClientCount;
        }
        
        ftduration d;
        while(!mnFinish)
        {
            ftnetSelect(100);
        }
        TS_ASSERT_EQUALS(mnFinish, 1);
        std::cout << __func__ << " speed is " << nTimes / d.durationSecond() << " rpc/s, " << nTimes << " call used " << d.durationSecond() << " seconds." << std::endl;
        std::cout << "ftnetbuffer::TotalInstanceCount() = " << ftnetbuffer::TotalInstanceCount() << std::endl;
    }
};

#endif /* defined(__TestAllUseful__test_ftrpc__) */
