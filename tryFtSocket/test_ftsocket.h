//
//  test_ftsocket.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/27.
/*  Copyright (c) 2015 liyoudi. All rights reserved. */
//

#ifndef TestAllUseful_test_ftsocket_h
#define TestAllUseful_test_ftsocket_h

#include "FakeThreadTemplate.h"
#include "ftsocket.h"
#include "ftnetselect.h"

#include "ftduration.h"
#include "ftnetbuffer.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
    #include <windows.h>
#else
    #include <sys/time.h>
    #include <unistd.h>
#include <sys/signal.h>
#endif

#include "cxxtest/TestSuite.h"

const char * NFGetFileNameFromPath(const char *szFilePath);
#ifndef InfoLog
#define InfoLog( args ) std::cout << " " << NFGetFileNameFromPath(__FILE__) << "(" << __LINE__ << ") " << __FUNCTION__ << ": " args << std::endl
#endif

class test_ftsocket : public CxxTest::TestSuite
{
    int mnFinished;
public:
    void setUp()
    {
#ifndef WIN32
        signal(SIGPIPE, SIG_IGN);
        signal(SIGPIPE, SIG_IGN);
#endif
        mnFinished = 0;
        
        mbHasConnected = 0;
        mnHasSvrOk = 0;

        std::string & strText = mstrText;
        if(strText.empty())
        {
            for (int nIdx = 0; nIdx < 10000; ++nIdx)
            {
                char szTmp[20] = "";
                sprintf(szTmp, "%08d ", nIdx);
                strText += szTmp;
            }
            strText += "exit here";
            mnClientSendSize = (int) strText.length();
        }
        for (int n = 0;n < sizeof(mszSendBuf); n += sizeof(int))
        {
            *((int *) (mszSendBuf + n)) = n;
        }
    }
public:
    std::string mstrText;
    int mbHasConnected;
    int mnHasSvrOk;
    int mnClientSendSize;
    char mszSendBuf[1 * 1024];
public:
    static void SvrProc(test_ftsocket *pThis, const char *szBindIp, unsigned int nBindPort)
    {
        int nRet = 0;
        ftsocket svrSock, cliSock;
        nRet = svrSock.Bind(szBindIp, nBindPort);
        TS_ASSERT_EQUALS(nRet, 0);
        if(nRet)
        {
            InfoLog( << "bind on " << szBindIp << ":" << nBindPort << " failed with " << svrSock.LastErrorNo() << ": " << svrSock.LastError() );
            pThis->mnHasSvrOk = -2;
            return;
        }
        nRet = svrSock.Accept(cliSock);
        TS_ASSERT_EQUALS(nRet, 0);
        InfoLog( << "accept a client " << cliSock );
        // recv
        char *buf = new char[pThis->mnClientSendSize + 10];
        memset(buf, 0, pThis->mnClientSendSize + 10);
        int nIdx = 0;
        bool bSvrOk = true;
        while(true)
        {
            nRet = cliSock.Recv(buf, pThis->mnClientSendSize);
            TS_ASSERT_EQUALS(nRet, pThis->mnClientSendSize);
            if(strcmp(buf + pThis->mnClientSendSize - 9, "exit here") == 0)
            {
                std::cout << (buf + pThis->mnClientSendSize - 9) << std::endl;
                break;
            }
            else
            {
                bSvrOk = false;
            }
            for (int n = 0; n < 8; ++n)
            {
                TS_ASSERT_LESS_THAN_EQUALS(buf[n], '9');
                TS_ASSERT_LESS_THAN_EQUALS('0', buf[n]);
                if(buf[n] > '9' || buf[n] < '0')
                {
                    InfoLog( << "recv data is error: " << buf );
                    bSvrOk = false;
                    break;
                }
            }
            if(!bSvrOk)
            {
                break;
            }
            if(nIdx != atoi(buf))
            {
                TS_ASSERT_EQUALS(nIdx, atoi(buf));
                bSvrOk = false;
                break;
            }
            ++nIdx;
        }
        InfoLog( << "server recv data " << (bSvrOk ? "ok" : "error") );
        
        //
        nRet = cliSock.Send(buf, pThis->mnClientSendSize);
        TS_ASSERT_EQUALS(nRet, pThis->mnClientSendSize);
        bSvrOk = nRet == pThis->mnClientSendSize;
        InfoLog( << "server send data " << (bSvrOk ? "ok" : "error") );
        //
        
        delete [] buf;
        pThis->mnHasSvrOk = bSvrOk ? 1 : -1;
    }
    static void ConnectProc(test_ftsocket *pThis, const char *szConnectIp, unsigned int nConnectPort)
    {
        int nRet = 0;
        ftsocket sock;
        InfoLog( << "before connect" );
        nRet = sock.Connect(szConnectIp, nConnectPort);
        InfoLog( << "after connect" );
        TS_ASSERT_EQUALS(nRet, 0);
        if(!nRet)
        {
            InfoLog( << "begin send ************" );
            nRet = sock.Send(pThis->mstrText.c_str(), (int) pThis->mstrText.length());
            TS_ASSERT_EQUALS(nRet, pThis->mstrText.length());
            InfoLog( << "send returns " << nRet << ", with code " << sock.LastErrorNo() << ": " << sock.LastError() );
            int nDataLen = pThis->mnClientSendSize;
            char *pData = new char[nDataLen + 10];
            pData[nDataLen] = 0;
            nRet = sock.Recv(pData, nDataLen);
            TS_ASSERT_EQUALS(nDataLen, nRet);
            if(nDataLen == nRet)
            {
                TS_ASSERT_EQUALS(pThis->mstrText, pData);
            }
            InfoLog( << "recv returns " << nRet << ", with code " << sock.LastErrorNo() << ": " << sock.LastError() );
            delete [] pData;
            pThis->mbHasConnected = 1;
        }
        else
        {
            pThis->mbHasConnected = -1;
            InfoLog( << "connect return failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
        }
    }
    void testCommon()
    {
        int nRet = 0;
        nRet = RunInFakeThreadT(SomeFuncTmp, this);
        TS_ASSERT_EQUALS(0, nRet);
        //
        mbHasConnected = 0;
        const char * szIp = "127.0.0.1";
        unsigned int nPort = 23456;
        RunInFakeThreadT(SvrProc, this, szIp, nPort);
        RunInFakeThreadT(ConnectProc, this, szIp, nPort);
        int nCount = 0;
        InfoLog( << "enter loop" );
        while(/* nCount < 200 && */ !mbHasConnected)
        {
            if(ftnetSelect(100) <= 0)
            {
                ++nCount;
            }
        }
        TS_ASSERT_EQUALS(mbHasConnected, 1);
        TS_ASSERT_EQUALS(mnHasSvrOk, 1);
        
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
    class InfoForConnect
    {
    public:
        InfoForConnect()
        {
            mstrSvrIp = mstrCliIp = "127.0.0.1";
            mnSvrPort = 9999;
            mnCliPort = 9998;
            mbFinished = false;
        }
    public:
        std::string mstrSvrIp, mstrCliIp;
        unsigned int mnSvrPort, mnCliPort;
        bool mbFinished;
    };
    static void FuncServer_ForConnect(InfoForConnect *pIFC)
    {
        ftsocket sock;
        int nRet = 0;
        nRet = sock.Bind(pIFC->mstrSvrIp.c_str(), pIFC->mnSvrPort);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            InfoLog( << "bind failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
        ftsocket cliSock;
        nRet = sock.Accept(cliSock);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            InfoLog( << "accept failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
        InfoLog( << "accepted socket is " << cliSock );
        TS_ASSERT_EQUALS(pIFC->mnCliPort, cliSock.remotePort());
        pIFC->mbFinished = true;
    }
    static void FuncClient_ForConnect(InfoForConnect *pIFC)
    {
        ftsocket sock;
        int nRet = 0;
        nRet = sock.Connect(pIFC->mstrSvrIp.c_str(), pIFC->mnSvrPort, pIFC->mstrCliIp.c_str(), pIFC->mnCliPort);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            InfoLog( << "connect failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
    }
    void testConnect()
    {
        /* 使用本机特定端口向外发起连接 */
        InfoForConnect ifc;
        RunInFakeThreadT(FuncServer_ForConnect, &ifc);
        RunInFakeThreadT(FuncClient_ForConnect, &ifc);
        while(!ifc.mbFinished)
        {
            ftnetSelect(100);
        }
        InfoLog( << "finished." );
        
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
#ifdef HAS_FD_SEND_RECV
    /* 测试收发文件描述符 */
    class InfoForTestFd
    {
    public:
        InfoForTestFd()
        {
            mstrIp = "127.0.0.1";
            mnTransFdPort = 9999;
            mnDataPort = 9998;
            
            mstrTestData = "This data is for test.";
        }
    public:
        std::string mstrIp;
        unsigned int mnTransFdPort; /* 传输fd的服务端端口 */
        unsigned int mnDataPort; /* 用来传输数据的服务端端口 */
        
        std::string mstrTestData;
        
        bool mbFinished;
    };
    /* 绑定端口，accept一个连接，然后接收一个文件描述符，并向其发送一段数据，再接收一段数据 */
    static void FuncFor_FdTransServer(InfoForTestFd *pIftf)
    {
        ftsocket sock;
        int nRet = 0;
        nRet = sock.Bind(pIftf->mstrIp.c_str(), pIftf->mnTransFdPort);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            InfoLog( << "bind failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
        ftsocket cliSock;
        nRet = sock.Accept(cliSock);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            InfoLog( << "accept failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
        InfoLog( << "accepted socket is " << cliSock );
        //
        ftsocket receivedSock;
        nRet = cliSock.RecvSocket(receivedSock);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            InfoLog( << "receive socket failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
        InfoLog( << "received socket is " << cliSock );
        //
        pIftf->mbFinished = true;
    }
    // for receive data connection
    static void FuncFor_FdDataServer(InfoForTestFd *pIftf)
    {
        ftsocket sock;
        int nRet = 0;
        nRet = sock.Bind(pIftf->mstrIp.c_str(), pIftf->mnDataPort);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            InfoLog( << "bind failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
        ftsocket cliSock;
        nRet = sock.Accept(cliSock);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            InfoLog( << "accept failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
        InfoLog( << "accepted socket is " << cliSock );
        //
        ftsocket fdsock;
        nRet = fdsock.Connect(pIftf->mstrIp.c_str(), pIftf->mnTransFdPort);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            InfoLog( << "connect failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
        nRet = fdsock.SendSocket(cliSock);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            InfoLog( << "send socket failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
    }
    // data client
    static void FuncFor_DataClient(InfoForTestFd *pIftf)
    {
        ftsocket sock;
        int nRet = 0;
        nRet = sock.Connect(pIftf->mstrIp.c_str(), pIftf->mnDataPort);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            InfoLog( << "connect failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
        nRet = sock.Send(pIftf->mstrTestData.c_str(), (unsigned int) pIftf->mstrTestData.length());
        TS_ASSERT_EQUALS(nRet, pIftf->mstrTestData.length());
    }
#endif // HAS_FD_SEND_RECV
    void testSend()
    {
#if 0
        InfoForTestFd iftf;
        RunInFakeThreadT(FuncFor_FdTransServer, &iftf);
        RunInFakeThreadT(FuncFor_FdDataServer, &iftf);
        RunInFakeThreadT(FuncFor_DataClient, &iftf);
        
        while(!iftf.mbFinished)
        {
            ftnetSelect(100);
        }
        InfoLog("finished.");
#endif
        
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
    //
    class InfoForContinue
    {
    public:
        InfoForContinue()
        {
            mstrIp = "127.0.0.1";
            mnPort = 9999;
            mnMaxInt = 1 * 10 * 1024;
            mbFinished = false;
        }
    public:
        std::string mstrIp;
        unsigned int mnPort;
        int mnMaxInt;
        bool mbFinished;
    };
    static void FuncFor_ContinueServer(InfoForContinue *pIfc)
    {
        ftsocket sock;
        int nRet = 0;
        nRet = sock.Bind(pIfc->mstrIp.c_str(), pIfc->mnPort);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            InfoLog( << "bind failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
        ftsocket cliSock;
        nRet = sock.Accept(cliSock);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            InfoLog( << "accept failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
        int nTmp = 0;
        int nIndex = 0;
        for(nIndex = 0;nIndex < pIfc->mnMaxInt;++nIndex)
        {
            nRet = cliSock.Recv(&nTmp, sizeof(nTmp));
            TS_ASSERT_EQUALS(sizeof(nTmp), nRet);
            TS_ASSERT_EQUALS(nIndex, nTmp);
            if(nRet != sizeof(nTmp) || nIndex != nTmp)
            {
                break;
            }
        }
        pIfc->mbFinished = true;
    }
    static void FuncFor_ContinueClient(InfoForContinue *pIfc)
    {
        ftsocket sock;
        int nRet = 0;
        nRet = sock.Connect(pIfc->mstrIp.c_str(), pIfc->mnPort);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            InfoLog( << "connect failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            return ;
        }
        int nIndex = 0;
        for(nIndex = 0;nIndex < pIfc->mnMaxInt;++nIndex)
        {
            nRet = sock.Send(&nIndex, sizeof(nIndex));
            TS_ASSERT_EQUALS(sizeof(nIndex), nRet);
            if(sizeof(nIndex) != nRet)
            {
                break;
            }
        }
    }
    void testRecv()
    {
        InfoForContinue ifc;
        RunInFakeThreadT(FuncFor_ContinueServer, &ifc);
        RunInFakeThreadT(FuncFor_ContinueClient, &ifc);
        
        while(!ifc.mbFinished)
        {
            ftnetSelect(100);
        }
        
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
    // test performance
    static void ServerForPerform(test_ftsocket *pThis, const char *szBindIp, unsigned int nBindPort, int nSendTimes)
    {
        int nRet = 0;
        ftsocket svrSock, cliSock, tmpSock;
        nRet = tmpSock.Bind(szBindIp, nBindPort);
        TS_ASSERT_EQUALS(nRet, 0);
        if(nRet)
        {
            InfoLog( << "bind failed." );
            return;
        }
        svrSock = tmpSock;
        tmpSock = cliSock;
        nRet = svrSock.Accept(tmpSock);
        TS_ASSERT_EQUALS(nRet, 0);
        InfoLog( << "accept a client." );
        // start sending data, format: len, data
        int nSendLen = sizeof(pThis->mszSendBuf);
        bool bOk = true;
        ftduration d;
        double dSendBytes = 0;
        for (int nTimes = 0; nTimes < nSendTimes; ++nTimes)
        {
            nRet = cliSock.Send(&nSendLen, sizeof(nSendLen));
            //TS_ASSERT_EQUALS(nRet, sizeof(nSendLen));
            if(nRet != sizeof(nSendLen))
            {
                bOk = false;
                break;
            }
            dSendBytes += nRet;
            nRet = cliSock.Send(pThis->mszSendBuf, nSendLen);
            //TS_ASSERT_EQUALS(nRet, (nSendLen));
            if(nRet != (nSendLen))
            {
                bOk = false;
                break;
            }
            dSendBytes += nRet;
        }
        nSendLen = 0;
        nRet = cliSock.Send(&nSendLen, sizeof(nSendLen));
        TS_ASSERT_EQUALS(nRet, sizeof(nSendLen));
        InfoLog( << "send speed is " << (dSendBytes / (1024 * 1024)) / (d.durationSecond() + 0.000001) << " MB/s" );
        pThis->mnHasSvrOk = bOk ? 1 : -1;
    }
    static void ClientForPerform(test_ftsocket *pThis, const char *szConnectIp, unsigned int nConnectPort)
    {
        int nRet = 0;
        ftsocket sock;
        InfoLog( << "before connect" );
        nRet = sock.Connect(szConnectIp, nConnectPort);
        InfoLog( << "after connect" );
        TS_ASSERT_EQUALS(nRet, 0);
        int nRecvLen = sizeof(pThis->mszSendBuf);
        bool bOk = false;
        char *pRecvBuf = new char[nRecvLen];
        ftduration d;
        double dRecvBytes = 0;
        while (true)
        {
            nRet = sock.Recv(&nRecvLen, sizeof(nRecvLen));
            TS_ASSERT_EQUALS(nRet, sizeof(nRecvLen));
            if(nRet != sizeof(nRecvLen))
            {
                break;
            }
            if(nRecvLen == 0)
            {
                bOk = true;
                break;
            }
            dRecvBytes += nRet;
            nRet = sock.Recv(pRecvBuf, nRecvLen);
            int nn = 0;
            for (nn = 0; nn < nRecvLen; nn += 4)
            {
                //TS_ASSERT_EQUALS(nn, *((int *) (pRecvBuf + nn)));
                if(nn != *((int *) (pRecvBuf + nn)))
                {
                    break;
                }
            }
            if(nn < nRecvLen)
            {
                break;
            }
            //TS_ASSERT_EQUALS(nRet, (nRecvLen));
            if(nRet != (nRecvLen))
            {
                break;
            }
            dRecvBytes += nRet;
        }
        delete []pRecvBuf;
        InfoLog( << "recv speed is " << (dRecvBytes / (1024 * 1024)) / (d.durationSecond() + 0.000001) << " MB/s" );
        pThis->mbHasConnected = bOk ? 1 : -1;
    }
    void testPerform() // test performance
    {
        const char * szIp = "127.0.0.1";
        unsigned int nPort = 23456;
        int nSendTimes = 1 * 10 * 1024; // times of sending
        RunInFakeThreadT(ServerForPerform, this, szIp, nPort, nSendTimes);
        RunInFakeThreadT(ClientForPerform, this, szIp, nPort);
        int nCount = 0;
        InfoLog( << "enter loop" );
        while(nCount < 200 && !mbHasConnected)
        {
            if(ftnetSelect(100) <= 0)
            {
                ++nCount;
            }
        }
        TS_ASSERT_EQUALS(mbHasConnected, 1);
        TS_ASSERT_EQUALS(mnHasSvrOk, 1);
        
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
    
    // the following is for UDP test
    static void UdpServerCommon(test_ftsocket *pThis, const char *szBindIp, int nBindPort)
    {
        ftsocket svrSock(AF_INET, SOCK_DGRAM);
        int nRet = 0;
        nRet = svrSock.Bind(szBindIp, nBindPort);
        TS_ASSERT_EQUALS(0, nRet)
        if(nRet)
        {
            InfoLog( << "bind error " << svrSock.LastErrorNo() << ": " << svrSock.LastError() );
            return ;
        }
        while(true)
        {
            char buf[2048] = "";
            char szIp[30] = "";
            unsigned int nCliPort = 0;
            nRet = svrSock.RecvFrom(buf, sizeof(buf) - 1, szIp, sizeof(szIp), nCliPort);
            TS_ASSERT_LESS_THAN_EQUALS(0, nRet);
            if(nRet < 0)
            {
                InfoLog( << "RecvFrom error " << svrSock.LastErrorNo() << ": " << svrSock.LastError() );
                break;
            }
            InfoLog( << "receive " << nRet << " bytes data from '" << szIp << ":" << nCliPort << "': " << buf );
            if(nRet == 1)
            {
                break;
            }
        }
        pThis->mnFinished = 1;
    }
    static void UdpClientCommon(test_ftsocket *pThis, ftsocket cliSock, const char *szIp, int nPort, int nSendBytes)
    {
        char szData[1000] = "This is from test program by fab.";
        if(nSendBytes > sizeof(szData))
        {
            nSendBytes = (int) sizeof(szData);
        }
        int nRet = 0;
        int nCount = 1;
        while(nCount-- > 0)
        {
            nRet = cliSock.SendTo(szData, nSendBytes, szIp, nPort);
            TS_ASSERT_EQUALS(nSendBytes, nRet);
            if(nRet < 0)
            {
                pThis->mnFinished = -1;
                InfoLog( << "sendto failed with " << cliSock.LastErrorNo() << ": " << cliSock.LastError() );
            }
            else
            {
//                InfoLog( << "sendto out " << nRet << " bytes." );
            }
        }
//        ResumeFakeThreadObjT1<int> resumeObj;
//        YieldFromFakeThread(resumeObj, (int *) NULL);
    }
    static void SomeFuncTmp(test_ftsocket *pThis)
    {
        InfoLog( << "" );
    }
    void testUdpCommon()
    {
        std::cout << "***************************************" << std::endl;
        int nRet = 0;
        const char szIp[] = "127.0.0.1";
//        const char szIp[] = "192.168.4.42";
        int nPort = 60000;
        ftsocket cliSock(AF_INET, SOCK_DGRAM);
        nRet = cliSock.Bind("0.0.0.0", 11210);
        TS_ASSERT_EQUALS(0, nRet);
        ftsocket cliSock2(AF_INET, SOCK_DGRAM);
        nRet = cliSock2.Bind("0.0.0.0", 11211);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet < 0)
        {
            InfoLog( << "Bind failed with " << cliSock.LastErrorNo() << ": " << cliSock.LastError() );
            return ;
        }
        nRet = RunInFakeThreadT(UdpServerCommon, this, szIp, nPort);
        TS_ASSERT_EQUALS(0, nRet);
        nRet = RunInFakeThreadT(UdpClientCommon, this, cliSock, szIp, nPort, 111);
        TS_ASSERT_EQUALS(0, nRet);
        nRet = RunInFakeThreadT(.1, UdpClientCommon, this, cliSock, szIp, nPort, 222);
        TS_ASSERT_EQUALS(0, nRet);
        for (int nThread = 0; nThread < 10; ++nThread)
        {
            nRet = RunInFakeThreadT(.1, UdpClientCommon, this, cliSock2, szIp, nPort, 333);
            TS_ASSERT_EQUALS(0, nRet);
        }
        nRet = RunInFakeThreadT(.05, UdpClientCommon, this, cliSock, szIp, nPort, 55);
        TS_ASSERT_EQUALS(0, nRet);
        nRet = RunInFakeThreadT(.1, UdpClientCommon, this, cliSock2, szIp, nPort, 666);
        TS_ASSERT_EQUALS(0, nRet);
        nRet = RunInFakeThreadT(.5, UdpClientCommon, this, cliSock, szIp, nPort, 1);
        TS_ASSERT_EQUALS(0, nRet);
        
        while(mnFinished == 0)
        {
            fttimer_do();
            ftnetSelect(100);
        }
        nRet = 0;
        
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
    /* 测试超时功能 */
    static void TrySocketTimeout_Server(ftsocket svrSock)
    {
        ftsocket cliSock;
        int nRet = 0;
        nRet = svrSock.Accept(cliSock);
        TS_ASSERT_EQUALS(0, nRet);
        if(nRet)
        {
            return ;
        }
        cliSock.SetTimeout(2, 2);
        ftnetbuffer buf(10000 * 1024);
        nRet = cliSock.Send(buf);
        if(nRet < 0)
        {
            TS_ASSERT_EQUALS(-3, cliSock.LastErrorNo());
            InfoLog( << "send to " << cliSock << " failed with " << cliSock.LastErrorNo() << ": " << cliSock.LastError() );
        }
    }
    static void TrySocketTimeout(bool *pbExit)
    {
        const char *szIp = "119.121.211.99";
        unsigned int nPort = 12121;
        do
        {
            ftsocket sock;
            int nRet = 0;
            sock.SetTimeout(1, 1);
            nRet = sock.Connect(szIp, nPort);
            if(nRet)
            {
//                TS_ASSERT_EQUALS(-3, sock.LastErrorNo());
                InfoLog( << "connect to " << szIp << ":" << nPort << " failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
            }
            sock.Close();
            //
            szIp = "127.0.0.1";
            nPort = 12918;
            ftsocket svrSock;
            nRet = svrSock.Bind(szIp, nPort);
            TS_ASSERT_EQUALS(0, nRet);
            if(nRet)
            {
                InfoLog( << "bind on " << szIp << ":" << nPort << " failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
                break;
            }
            RunInFakeThreadT(TrySocketTimeout_Server, svrSock);
            sock.SetTimeout(3, 3);
            nRet = sock.Connect(szIp, nPort);
            if(nRet)
            {
                InfoLog( << "connect to " << szIp << ":" << nPort << " failed with " << sock.LastErrorNo() << ": " << sock.LastError() );
                break;
            }
            ftsocket sock2;
            sock2.SetTimeout(3, 3);
            nRet = sock2.Connect(szIp, nPort);
            if(nRet)
            {
                InfoLog( << "connect to " << szIp << ":" << nPort << " failed with " << sock2.LastErrorNo() << ": " << sock2.LastError() );
                break;
            }
            ftnetbuffer recvBuf(100*1024);
            nRet = sock2.Recv(recvBuf);
            TS_ASSERT_EQUALS(-3, sock2.LastErrorNo());
            if(nRet < 0)
            {
                InfoLog( << "receive from " << szIp << ":" << nPort << " failed with " << sock2.LastErrorNo() << ": " << sock2.LastError() );
                break;
            }
        }while(false);
        //
        *pbExit = true;
    }
    void testUdpPerform()
    {
        std::cout << "***************************************" << std::endl;
        bool bExit = false;
        RunInFakeThreadT(TrySocketTimeout, &bExit);
        while(!bExit)
        {
            ftnetSelect(100);
        }
        
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
    
    static void FuncForProtocol(test_ftsocket *pThis)
    {

    }
    void testUdpProtocol()
    {
        int nTimes = 1000 * 1000 * 10;
        ftduration d;
        int nIndex = 0;
        while(nIndex < nTimes)
        {
            ++nIndex;
        }
        InfoLog( << "while speed is " << (nTimes) / (d.durationSecond() + 0.000001) << " times/s" );
        
        TS_ASSERT_EQUALS(0, GetRunningFakeThreadCount());
    }
};

#endif
