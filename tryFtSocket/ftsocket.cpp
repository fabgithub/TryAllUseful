//
//  ftsocket.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/2/26.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#include "ftsocket.h"
#include "ftnetbuffer.h"

#include "FakeThreadWrap.h"
#include "FakeThreadTemplate.h"
#include "ftCheckValid.h"

#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <string>
#include <iostream>
#include <sstream>

#ifdef WIN32
#define socklen_t int
//#define errno WSAGetLastError()
#endif

typedef int (*_SockMgrFunction) (SOCKET sock, ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond);
typedef int (*_SockMgrFunction_OnClosed) (SOCKET sock);
typedef void (*_Cancel_event) (SOCKET sock);

static _SockMgrFunction _ftnetAddReadFd = _ftnetAddReadFd_select;
static _SockMgrFunction _ftnetAddWriteFd = _ftnetAddWriteFd_select;
//static _SockMgrFunction_OnClosed _ftnetOnFdClosed = _ftnetOnFdClosed_select;
static _Cancel_event _ftCancel_select = _ftnetCancel_select;
//static void _ftnetOnFdClosed(SOCKET)
//{
//}

typedef ResumeFakeThreadObjT1<int> ResumeNetOpObject;

static void _NetEventCb(int nErrCode, void *pParam)
{
    ResumeNetOpObject *pResumeObj = (ResumeNetOpObject *) pParam;
    (*pResumeObj)(nErrCode);
}

#ifdef WIN32
static int fn_for_init_win_sock()
{
	WSAData wsaD;
	WORD w;
	w = 0x0101;
	return WSAStartup(w, &wsaD);
}
static int gInitWinSock = fn_for_init_win_sock();
#endif

static int set_nonblocking(SOCKET fd, bool nonblock);

class _ftsocket_data
{
    _ftsocket_data(const _ftsocket_data &);
    _ftsocket_data & operator = (const _ftsocket_data &);
public:
    _ftsocket_data(int domain, int type, int protocol)
    : mnDomain(domain), mnType(type), mnProtocol(protocol)
    , mSock(INVALID_SOCKET), mnLastError(0), mstrLastErrInfo("")
    , mfnOnClosed(NULL), mpOnClosedParam(NULL)
    , mnRefCount(0)
    , mnReadTimeoutSecond(0), mnWriteTimeoutSecond(0)
    , mbHasPeekData(false), mnPeekBufPos(0)
    {
        mszLocalIp[0] = mszRemoteIp[0] = 0;
        mszLocal[0] = mszRemote[0] = 0;
        mnLocalPort = mnRemotePort = 0;
        mstrSockDebugInfo = "";
    }
    ~_ftsocket_data()
    {
    }
    SOCKET wrap_socket()
    {
        SetLastErrorInfo(0, "");
        return ::socket(mnDomain, mnType, mnProtocol);
    }
    void SetLastErrorInfo(int nError, const char *szInfo)
    {
        mnLastError = nError;
        mstrLastErrInfo = szInfo;
    }
    int AttachSock(SOCKET sock)
    {
        if(IsValidSocket(mSock))
        {
            mnLastError = 1;
            mstrLastErrInfo = "already init with socket";
            return -1;
        }
        mSock = sock;
        mnLastError = 0;
        mstrLastErrInfo = "";
        if(set_nonblocking(mSock, true))
        {
            return -2;
        }
        return 0;
    }
    int IncreaseRef()
    {
        ++mnRefCount;
        return mnRefCount;
    }
    int DecreaseRef(const ftsocket &fts)
    {
        if(mnRefCount == 1 && IsValidSocket(mSock)) // 最后一个引用时，若是有效的socket，就关闭它
        {
            SOCKET sock = mSock;
            mSock = INVALID_SOCKET;
            _ftCancel_select(sock);
            // _ftnetOnFdClosed(sock);
            // mResumeReadObj(-2);
            // mResumeWriteObj(-2);
            closesocket(sock);
            if(mfnOnClosed)
            {
                mfnOnClosed(mpOnClosedParam, fts, 0, "closed because this is the last reference.");
            }
            Reset();
        }
        --mnRefCount;
        if(mnRefCount <= 0)
        {
            delete this;
            return 0;
        }
        return mnRefCount;
    }
    void Close()
    {
        if(IsValidSocket(mSock))
        {
            SOCKET sock = mSock;
            mSock = INVALID_SOCKET;
            _ftCancel_select(sock);
            // mResumeReadObj(-2);
            // mResumeWriteObj(-2);
            // _ftnetOnFdClosed(sock);
            closesocket(sock);
        }
    }
    void Reset()
    {
        Close();
        //SetLastErrorInfo(0, "");
        mfnOnClosed = NULL;
        mpOnClosedParam = NULL;
        
        mszLocalIp[0] = mszRemoteIp[0] = 0;
        mszLocal[0] = mszRemote[0] = 0;
        mnLocalPort = mnRemotePort = 0;
        
        mstrSockDebugInfo = "";
        
        mnWriteTimeoutSecond = mnReadTimeoutSecond = 0;
        mbHasPeekData = false;
        mnPeekBufPos = 0;
    }
public:
    const char *localIp() const { InitAddrInfo(); return mszLocalIp; }
    unsigned int localPort() const { InitAddrInfo(); return mnLocalPort; }
    const char *remoteIp() const { InitAddrInfo(); return mszRemoteIp; }
    unsigned int remotePort() const { InitAddrInfo(); return mnRemotePort; }

    const char *local() const { InitAddrInfo(); return mszLocal; }
    const char *remote() const { InitAddrInfo(); return mszRemote; }
public:
    void SetSockDebugInfo(const char *szSockDebugInfo)
    {
        mstrSockDebugInfo = szSockDebugInfo;
    }
public:
    // 初始化地址信息, 在第一次需要此信息时调用
    void InitAddrInfo() const
    {
//        if(mnType != SOCK_STREAM) // 只有tcp可以获取地址
//        {
//            return ;
//        }
        if(!IsValidSocket(mSock) || mnRemotePort || mnLocalPort || mszLocalIp[0] || mszRemoteIp[0])
        {
            return ;
        }
        int nRet = 0;
        do
        {
            mszLocalIp[0] = mszRemoteIp[0] = 0;
            struct sockaddr_in addr;
            socklen_t nAddrLen = sizeof(addr);
            nRet = getsockname(mSock, (struct sockaddr *) &addr, &nAddrLen);
            if(!nRet)
            {
                const char * szFrom = inet_ntoa(addr.sin_addr);
                strcpy(mszLocalIp, szFrom);
                mnLocalPort = ntohs(addr.sin_port);
            }
            else
            {
                // break;
            }
            //
            nRet = getpeername(mSock, (struct sockaddr *) &addr, &nAddrLen);
            if(!nRet)
            {
                const char * szTo = inet_ntoa(addr.sin_addr);
                strcpy(mszRemoteIp, szTo);
                mnRemotePort = ntohs(addr.sin_port);
            }
            // else if(errno != ENOTCONN)
            {
                // std::cout << "getpeername failed with " << errno << ": " << strerror(errno) << std::endl;
                // break;
            }
            sprintf(mszLocal, "%s:%d", mszLocalIp, mnLocalPort);
            sprintf(mszRemote, "%s:%d", mszRemoteIp, mnRemotePort);
        }while(0);
    }
public:
    int mnDomain, mnType, mnProtocol;
    SOCKET mSock;
    int mnLastError;
    std::string mstrLastErrInfo;
    
    ftsocketCloseCB mfnOnClosed;
    void *mpOnClosedParam;
    
    int mnRefCount; // 引用计数
    /* 读写超时时间 */
    unsigned int mnReadTimeoutSecond;
    unsigned int mnWriteTimeoutSecond;
    //
    mutable char mszLocalIp[100];
    mutable unsigned int mnLocalPort;
    mutable char mszRemoteIp[100];
    mutable unsigned int mnRemotePort;
    
    mutable char mszLocal[100];
    mutable char mszRemote[100];
    
    std::string mstrSockDebugInfo;
    
    // 当为服务时，关闭时用于唤醒的对象
    ResumeNetOpObject mResumeReadObj; // 唤醒读
    // ResumeNetOpObject mResumeWriteObj; // 唤醒写
    
    // peek data
    bool mbHasPeekData;
    ftnetbuffer mPeekBufData;
    unsigned int mnPeekBufPos; /* 如果已经读取过，而这部分数据又没有一次读完，则需要记录上次读到的位置 */
};

static int set_nonblocking(SOCKET fd, bool nonblock)
{
#ifdef WIN32
    unsigned long nonblocking = nonblock ? 1 : 0;
    if (ioctlsocket(fd, FIONBIO, &nonblocking) == SOCKET_ERROR)
        return -1;
#else
    int flags;
    if ((flags = fcntl(fd, F_GETFL, NULL)) < 0)
        return -1;
    
    flags = nonblock ? (flags | O_NONBLOCK) : (flags &~O_NONBLOCK);
    
    if (fcntl(fd, F_SETFL, flags) == -1)
        return -1;
    
#endif
    return 0;
}

static int set_reuse(SOCKET fd, bool resue)
{
#ifdef WIN32
    int one = 1;
    return SOCKET_ERROR != ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof(int));
#else
    int on = 1;
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) ) != 0)
        return -1;
#endif
    return 0;
}
// ===============================
// ftsocket

ftsocket::ftsocket(int domain, int type, int protocol)
{
    mps = new _ftsocket_data(domain, type, protocol);
    mps->IncreaseRef();
}
ftsocket::~ftsocket()
{
    mps->DecreaseRef(*this);
}
ftsocket::ftsocket(const ftsocket &fts)
: mps(fts.mps)
{
    mps->IncreaseRef();
}
ftsocket & ftsocket::operator = (const ftsocket &fts)
{
    mps->DecreaseRef(*this);
    mps = fts.mps;
    mps->IncreaseRef();
    return *this;
}
bool ftsocket::IsValid() const
{
    return IsValidSocket(mps->mSock);
}
bool ftsocket::operator ! () const
{
    return !IsValidSocket(mps->mSock);
}
bool ftsocket::operator < (const ftsocket &fts) const
{
    return mps < fts.mps;
}
std::ostream & operator << (std::ostream &os, const ftsocket &fts)
{
    os << "ftsocket[";
    os << fts.mps->mSock << "(" << fts.mps << ")";
    if(fts.mps->mnType == SOCK_STREAM)
        os << "TCP";
    else if(fts.mps->mnType == SOCK_DGRAM)
        os << "UDP";
    else os << "UNKOWN";
    os << "('" << fts.mps->mstrSockDebugInfo << "') ";
    os << "L(" << fts.mps->localIp() << ":" << fts.mps->localPort() << "), ";
    os << "R(" << fts.mps->remoteIp() << ":" << fts.mps->remotePort() << ")";
    os << "]";
    return os;
}
int ftsocket::Bind(const char *szIp, unsigned int nPort, bool bListen)
{
    if(IsValidSocket(mps->mSock))
    {
        return -2;
    }
    int nRet = 0;
    sockaddr_in addr = {0};
    addr.sin_family = mps->mnDomain;
    addr.sin_port   = htons(nPort);
    addr.sin_addr.s_addr  = (szIp && szIp[0]) ? inet_addr(szIp) : htonl(INADDR_ANY);
    memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
    
    mps->mSock = mps->wrap_socket();
    set_reuse(mps->mSock, true);
    if( bind(mps->mSock,(const  sockaddr*)&addr, sizeof(addr)) < 0)
    {
        nRet = -1;
    }
    else
    {
        if(mps->mnType == SOCK_STREAM && bListen)
        {
            nRet = listen(mps->mSock, 15);
        }
        if(!nRet && set_nonblocking(mps->mSock, true))
        {
            nRet = -3;
        }
    }
    if(nRet)
    {
        mps->SetLastErrorInfo(errno, strerror(errno));
        closesocket(mps->mSock);
        mps->mSock = INVALID_SOCKET;
    }
    else
    {
        mps->InitAddrInfo();
    }
    return nRet;
}
int ftsocket::Accept(ftsocket &outCliSocket)
{
    if(!IsValidSocket(mps->mSock))
    {
        return -2;
    }
    if(IsValidSocket(outCliSocket.mps->mSock))
    {
        return -3;
    }
    outCliSocket.mps->Reset();
    //
    int nRet = 0;
    sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    memset(&addr, 0, sizeof(addr));
    SOCKET cliSock = INVALID_SOCKET;
    while(!IsValidSocket(cliSock))
    {
        cliSock = accept(mps->mSock, (sockaddr*)&addr, &addr_len);
        if(!IsValidSocket(cliSock))
        {
#ifdef WIN32
			int error = WSAGetLastError();
			if(WSAEWOULDBLOCK == error)
#else
            int error = errno;
            if(EWOULDBLOCK == error)
#endif
            {
                ResumeNetOpObject &resumeObj = mps->mResumeReadObj;
                _ftnetAddReadFd(mps->mSock, _NetEventCb, &resumeObj, mps->mnReadTimeoutSecond);
                int nNetOpRes = 0;
                nRet = YieldFromFakeThread(resumeObj, &nNetOpRes);
                if(!IsValidSocket(mps->mSock))
                {
                    nRet = -2;
                    mps->SetLastErrorInfo(0, "already closed.");
                    break;
                }
                if(nNetOpRes)
                {
                    nRet = -1;
                    std::stringstream ssErr;
                    ssErr << "net accept operation is timeout for " << mps->mnReadTimeoutSecond << " seconds.";
                    mps->SetLastErrorInfo(-3, ssErr.str().c_str());
                    break;
                }
                //
                if(0 == nRet)
                {
                    socklen_t nDataLen = sizeof(int);
                    nRet = getsockopt(mps->mSock, SOL_SOCKET, SO_ERROR, (char *) &error, &nDataLen);
                    mps->SetLastErrorInfo(error, strerror(error));
                    if (!nRet)
                    {
                        nRet = error;
                    }
                    else
                    {
                        mps->SetLastErrorInfo(errno, strerror(errno));
                        break;
                    }
                }
                else
                {
                    mps->SetLastErrorInfo(nRet, "Yield failed.");
                    break;
                }
            }
            else
            {
                nRet = -1;
                mps->SetLastErrorInfo(errno, strerror(errno));
                break;
            }
        }
    }
    if(IsValidSocket(cliSock))
    {
        nRet = outCliSocket.mps->AttachSock(cliSock);
        if(!nRet)
        {
            outCliSocket.mps->InitAddrInfo();
        }
    }
    return nRet;
}
int ftsocket::Connect(const char *szRemoteIp, unsigned int nRemotePort, const char *szLocalIp, unsigned int nLocalPort)
{
    if(IsValidSocket(mps->mSock))
    {
        return -2;
    }
    mps->SetLastErrorInfo(0, "");
    int nRet = 0;
    sockaddr_in srv_addr = {0};
    srv_addr.sin_family  = mps->mnDomain;
    srv_addr.sin_port    = htons(nRemotePort);
    srv_addr.sin_addr.s_addr = inet_addr(szRemoteIp);
    memset(srv_addr.sin_zero, 0, sizeof(srv_addr.sin_zero));
    
    if(szLocalIp && nLocalPort)
    {
        nRet = Bind(szLocalIp, nLocalPort, false);
        if(nRet)
        {
            return nRet;
        }
    }
    else
    {
        mps->mSock = mps->wrap_socket();
    }
    set_nonblocking(mps->mSock, true);
    set_reuse(mps->mSock, true);
    nRet = connect(mps->mSock, (const sockaddr*)&srv_addr, sizeof(srv_addr) );
    if( nRet < 0)
    {
#ifdef WIN32
		int error = WSAGetLastError();
		if(error != WSAEWOULDBLOCK)
#else
        int error = errno;
        if(EINPROGRESS != error)
#endif
        {
            mps->SetLastErrorInfo(errno, strerror(errno));
        }
        else // 需要yield
        {
            ResumeNetOpObject resumeObj;
            _ftnetAddWriteFd(mps->mSock, _NetEventCb, &resumeObj, mps->mnWriteTimeoutSecond);
            int nNetOpRes = 0;
            nRet = YieldFromFakeThread(resumeObj, &nNetOpRes);
            //
            if(nNetOpRes)
            {
                nRet = -1;
                std::stringstream ssErr;
                ssErr << "net connect operation is timeout for " << mps->mnWriteTimeoutSecond << " seconds.";
                mps->SetLastErrorInfo(-3, ssErr.str().c_str());
            }
            else if(0 == nRet)
            {
                socklen_t nDataLen = sizeof(int);
                nRet = getsockopt(mps->mSock, SOL_SOCKET, SO_ERROR, (char *) &error, &nDataLen);
                mps->SetLastErrorInfo(error, strerror(error));
                if (!nRet)
                {
                    nRet = error;
                }
                else
                {
                    mps->SetLastErrorInfo(errno, strerror(errno));
                }
            }
        }
    }
    if(nRet)
    {
        closesocket(mps->mSock);
        mps->mSock = INVALID_SOCKET;
    }
    else
    {
        mps->InitAddrInfo();
    }
    return nRet;
}
int ftsocket::Send(const void *szData, unsigned int nDataLen)
{
    int nRet = 0;
    if(!IsValidSocket(mps->mSock))
    {
        mps->SetLastErrorInfo(1, "invalid socket");
        return -1;
    }
    mps->SetLastErrorInfo(0, "");
    unsigned int nPos = 0;
    while (nPos < nDataLen)
    {
        nRet = (int) ::send(mps->mSock, ((char *) szData) + nPos, nDataLen - nPos, 0);
        if(nRet > 0)
        {
            nPos += nRet;
        }
        else
        {
#ifdef WIN32
			int error = WSAGetLastError();
            if(WSAEWOULDBLOCK == error)
#else
			int error = errno;
            if(EWOULDBLOCK == error)
#endif
            {
                ResumeNetOpObject resumeObj;
                _ftnetAddWriteFd(mps->mSock, _NetEventCb, &resumeObj, mps->mnWriteTimeoutSecond);
                int nNetOpRes = 0;
                nRet = YieldFromFakeThread(resumeObj, &nNetOpRes);
                if(nRet)
                {
                    mps->SetLastErrorInfo(2, "yield return error");
                    break;
                }
                if(nNetOpRes)
                {
                    nRet = -1;
                    std::stringstream ssErr;
                    ssErr << "net send operation is timeout for " << mps->mnWriteTimeoutSecond << " seconds.";
                    mps->SetLastErrorInfo(-3, ssErr.str().c_str());
                    break;
                }
            }
            else
            {
                mps->SetLastErrorInfo(error, strerror(error));
                break;
            }
        }
    }
    if(nPos >= nDataLen)
    {
        nRet = nPos;
    }
    return nRet;
}
static int ProcessPeekData(_ftsocket_data *mps, void *szData, unsigned int nDataLen)
{
    int nPos = 0;
    if(mps->mbHasPeekData && mps->mPeekBufData.bufferLen() > 0)
    {
        /*判断要接收的数据长度比已peek的小*/
        int nRemainPeek = mps->mPeekBufData.bufferLen() - mps->mnPeekBufPos;
        if(nRemainPeek <= 0)
        {}
        else if((int) nDataLen < nRemainPeek)
        {
            memcpy(szData, mps->mPeekBufData.buffer() + mps->mnPeekBufPos, nDataLen);
            mps->mnPeekBufPos += nDataLen;
            nPos = nDataLen;
        }
        else
        {
            memcpy(szData, mps->mPeekBufData.buffer() + mps->mnPeekBufPos, nRemainPeek);
            mps->mnPeekBufPos += nRemainPeek;
            mps->mbHasPeekData = false;
            nPos = nRemainPeek;
        }
    }
    return nPos;
}
int ftsocket::Recv(void *szData, unsigned int nDataLen, unsigned int nMinRecvSize)
{
    int nRet = 0;
    if(!IsValidSocket(mps->mSock))
    {
        mps->SetLastErrorInfo(1, "invalid socket");
        return -1;
    }
    mps->SetLastErrorInfo(0, "");
    unsigned int nPos = 0;
    if(nMinRecvSize > nDataLen)
    {
        nMinRecvSize = nDataLen;
    }
    if(mps->mbHasPeekData)
    {
        nPos = ProcessPeekData(mps, szData, nDataLen);
    }
    while (nPos < nMinRecvSize)
    {
        nRet = (int) ::recv(mps->mSock, ((char *) szData) + nPos, nDataLen - nPos, 0);
        if(nRet > 0)
        {
            nPos += nRet;
            if(mps->mnType == SOCK_DGRAM)
            {
                break;
            }
        }
        else if (nRet == 0)
        {
            break;
        }
        else
        {
#ifdef WIN32
			int error = WSAGetLastError();
            if(WSAEWOULDBLOCK == error)
#else
			int error = errno;
            if(EWOULDBLOCK == error)
#endif
            {
                ResumeNetOpObject resumeObj;
                _ftnetAddReadFd(mps->mSock, _NetEventCb, &resumeObj, mps->mnReadTimeoutSecond);
                int nNetOpRes = 0;
                nRet = YieldFromFakeThread(resumeObj, &nNetOpRes);
                if(nRet)
                {
                    mps->SetLastErrorInfo(2, "yield return error");
                    break;
                }
                if(nNetOpRes)
                {
                    nRet = -1;
                    std::stringstream ssErr;
                    ssErr << "net recv operation is timeout for " << mps->mnReadTimeoutSecond << " seconds.";
                    mps->SetLastErrorInfo(-3, ssErr.str().c_str());
                    break;
                }
            }
            else
            {
                mps->SetLastErrorInfo(errno, strerror(errno));
                break;
            }
        }
    }
    if(!mps->mnLastError)
    {
        nRet = nPos;
    }
    return nRet;
}
int ftsocket::Send(const ftnetbuffer &buf)
{
    return Send(buf.buffer(), buf.bufferLen());
}
int ftsocket::Send(int nBufCount, ...)
{
    int nRet = 0;
    va_list arg_ptr;
    va_start(arg_ptr, nBufCount);
    nRet = Send(nBufCount, arg_ptr);
    va_end(arg_ptr);
    return nRet;
}
int ftsocket::Send(int nBufCount, va_list arg_ptr)
{
    int nRet = 0;
    int nTmp = 0;
    for (; nBufCount > 0; --nBufCount)
    {
        ftnetbuffer *buf;
        buf = va_arg(arg_ptr, ftnetbuffer *);
        nTmp = Send(*buf);
        if(nTmp != buf->bufferLen())
        {
            nRet = nTmp;
            break;
        }
        nRet += nTmp;
    }
    return nRet;
}
int ftsocket::Recv(ftnetbuffer &buf, unsigned int nMinRecvSize)
{
    return Recv(buf.buffer(), buf.bufferLen(), nMinRecvSize);
}
int ftsocket::Recv(int nBufCount, ...)
{
    int nRet = 0;
    va_list arg_ptr;
    va_start(arg_ptr, nBufCount);
    nRet = Recv(nBufCount, arg_ptr);
    va_end(arg_ptr);
    return nRet;
}
int ftsocket::Recv(int nBufCount, va_list arg_ptr)
{
    int nRet = 0;
    int nTmp = 0;
    for (; nBufCount > 0; --nBufCount)
    {
        ftnetbuffer *buf;
        buf = va_arg(arg_ptr, ftnetbuffer *);
        nTmp = Recv(*buf);
        if(nTmp != buf->bufferLen())
        {
            nRet = nTmp;
            break;
        }
        nRet += nTmp;    }
    return nRet;
}
int ftsocket::SendTo(const void *szData, unsigned int nDataLen, const char *szIp, unsigned int nPort)
{
    int nRet = 0;
    if(!IsValidSocket(mps->mSock))
    {
        mps->mSock = mps->wrap_socket();
    }
    sockaddr_in srv_addr = {0};
    srv_addr.sin_family  = mps->mnDomain;
    srv_addr.sin_port    = htons(nPort);
    srv_addr.sin_addr.s_addr = inet_addr(szIp);
    memset(srv_addr.sin_zero, 0, sizeof(srv_addr.sin_zero));
    do
    {
        nRet = (int) ::sendto(mps->mSock, (const char *) szData, nDataLen, 0, (const struct sockaddr *) &srv_addr, sizeof(srv_addr));
        if(nRet < 0)
        {
#ifdef WIN32
			int error = WSAGetLastError();
            if(WSAEWOULDBLOCK != error)
#else
			int error = errno;
            if(EWOULDBLOCK != error)// 出错了
#endif
            {
                mps->SetLastErrorInfo(errno, strerror(errno));
                break;
            }
            else // 阻塞了，需要yield
            {
                ResumeNetOpObject resumeObj;
                _ftnetAddWriteFd(mps->mSock, _NetEventCb, &resumeObj, mps->mnWriteTimeoutSecond);
                int nNetOpRes = 0;
                nRet = YieldFromFakeThread(resumeObj, &nNetOpRes);
                if(nRet)
                {
                    mps->SetLastErrorInfo(2, "yield return error");
                    break;
                }
                if(nNetOpRes)
                {
                    nRet = -1;
                    std::stringstream ssErr;
                    ssErr << "net sendto operation is timeout for " << mps->mnWriteTimeoutSecond << " seconds.";
                    mps->SetLastErrorInfo(-3, ssErr.str().c_str());
                    break;
                }
            }
        }
    }while(nRet < 0);
    return nRet;
}
int ftsocket::SendTo(const ftnetbuffer &buf, const char *szIp, unsigned int nPort)
{
    int nRet = 0;
    nRet = SendTo(buf.buffer(), buf.bufferLen(), szIp, nPort);
    return nRet;
}

int ftsocket::RecvFrom(void *szData, unsigned int nDataLen, char *szIp, unsigned int nIpBufLen, unsigned int &nOutPort)
{
    if(nIpBufLen < 30)
    {
        return -9;
    }
    if(!IsValidSocket(mps->mSock))
    {
        if(mps->mnType == SOCK_DGRAM || mps->mnType == SOCK_STREAM)
        {
            return -1;
        }
        mps->mSock = mps->wrap_socket();
        if(!IsValidSocket(mps->mSock))
        {
            mps->SetLastErrorInfo(errno, strerror(errno));
            return -2;
        }
    }
    int nRet = 0;
    sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    memset(&addr, 0, sizeof(addr));
    do
    {
        nRet = (int) ::recvfrom(mps->mSock, (char *) szData, nDataLen, 0, (struct sockaddr *) &addr, &addr_len);
        if(nRet < 0)
        {
#ifdef WIN32
			int error = WSAGetLastError();
            if(WSAEWOULDBLOCK != error)
#else
			int error = errno;
            if(EWOULDBLOCK != error)// 出错了
#endif
            {
                mps->SetLastErrorInfo(errno, strerror(errno));
                break;
            }
            else // 阻塞了，需要yield
            {
                ResumeNetOpObject resumeObj;
                _ftnetAddReadFd(mps->mSock, _NetEventCb, &resumeObj, mps->mnReadTimeoutSecond);
                int nNetOpRes = 0;
                nRet = YieldFromFakeThread(resumeObj, &nNetOpRes);
                if(nRet)
                {
                    mps->SetLastErrorInfo(2, "yield return error");
                    break;
                }
                if(nNetOpRes)
                {
                    nRet = -1;
                    std::stringstream ssErr;
                    ssErr << "net recvfrom operation is timeout for " << mps->mnReadTimeoutSecond << " seconds.";
                    mps->SetLastErrorInfo(-3, ssErr.str().c_str());
                    break;
                }
                else if(!IsValidSocket(mps->mSock))
                {
                    break;
                }
            }
        }
    }while(nRet < 0);
    const char * szFrom = inet_ntoa(addr.sin_addr);
    if(nRet > 0 && szFrom)
    {
        strcpy(szIp, szFrom);
        nOutPort = ntohs(addr.sin_port);
    }
    return nRet;
}

int ftsocket::RecvFrom(ftnetbuffer &buf, char *szIp, unsigned int nIpBufLen, unsigned int &nOutPort)
{
    int nRet = 0;
    nRet = RecvFrom(buf.buffer(), buf.bufferLen(), szIp, nIpBufLen, nOutPort);
    return nRet;
}

int ftsocket::Close()
{
    int nRet = 0;
    if(!IsValidSocket(mps->mSock))
    {
        mps->SetLastErrorInfo(1, "invalid socket");
        return -1;
    }
    ftsocketCloseCB fn = mps->mfnOnClosed;
    void *pParam = mps->mpOnClosedParam;
    mps->Close();
    //
    if(fn)
    {
        fn(pParam, *this, LastErrorNo(), LastError());
    }
    mps->Reset();
    return nRet;
}
const char * ftsocket::LastError() const
{
    return mps->mstrLastErrInfo.c_str();
}
int ftsocket::LastErrorNo() const
{
    return mps->mnLastError;
}
const char *ftsocket::localIp() const
{
    return mps->localIp();
}
unsigned int ftsocket::localPort() const
{
    return mps->localPort();
}
const char *ftsocket::remoteIp() const
{
    return mps->remoteIp();
}
unsigned int ftsocket::remotePort() const
{
    return mps->remotePort();
}
const char *ftsocket::local() const
{
    return mps->local();
}
const char *ftsocket::remote() const
{
    return mps->remote();
}
void ftsocket::SetSockDebugInfo(const char *szSockDebugInfo)
{
    mps->SetSockDebugInfo(szSockDebugInfo);
}
void ftsocket::MonitorCloseEvent(ftsocketCloseCB fn, void *pParam)
{
    mps->mfnOnClosed = fn;
    mps->mpOnClosedParam = pParam;
}
void ftsocket::SetTimeout(unsigned int nReadTimeoutSecond, unsigned int nWriteTimeoutSecond)
{
    mps->mnReadTimeoutSecond = nReadTimeoutSecond;
    mps->mnWriteTimeoutSecond = nWriteTimeoutSecond;
}
void ftsocket::GetTimeout(unsigned int &nReadTimeoutSecond, unsigned int &nWriteTimeoutSecond)
{
    nReadTimeoutSecond = mps->mnReadTimeoutSecond;
    nWriteTimeoutSecond = mps->mnWriteTimeoutSecond;
}

int ftsocket::AddToReadMonitor(ftEventCallbackFn fn, void *pParam) const
{
    if(!IsValidSocket(mps->mSock))
    {
        return -1;
    }
    return _ftnetAddReadFd(mps->mSock, fn, pParam, mps->mnReadTimeoutSecond);
}
int ftsocket::recv_without_yield(void *szData, unsigned int nDataLen)
{
    mps->SetLastErrorInfo(0, "");
    int nPos = 0;
    if(mps->mbHasPeekData)
    {
        nPos = ProcessPeekData(mps, szData, nDataLen);
    }
    int nRet = nPos;
    if(nRet == 0)
    {
        nRet = (int) ::recv(mps->mSock, (char *) szData, nDataLen, 0);
        if(nRet)
        {
    #ifdef WIN32
                int error = WSAGetLastError();
                if(WSAEWOULDBLOCK != error)
    #else
                int error = errno;
                if(EWOULDBLOCK != error)// 出错了
    #endif
            {
                mps->SetLastErrorInfo(error, strerror(error));
            }
        }
    }
    return nRet;
}
int ftsocket::AddToSendMonitor(ftEventCallbackFn fn, void *pParam) const
{
    if(!IsValidSocket(mps->mSock))
    {
        return -1;
    }
    return _ftnetAddWriteFd(mps->mSock, fn, pParam, mps->mnWriteTimeoutSecond);
}
int ftsocket::send_without_yield(void *szData, unsigned int nDataLen)
{
    mps->SetLastErrorInfo(0, "");
    int nRet = (int) ::send(mps->mSock, (char *) szData, nDataLen, 0);
    if(nRet)
    {
#ifdef WIN32
			int error = WSAGetLastError();
            if(WSAEWOULDBLOCK != error)
#else
			int error = errno;
            if(EWOULDBLOCK != error)// 出错了
#endif
        {
            mps->SetLastErrorInfo(error, strerror(error));
        }
    }
    return nRet;
}
int ftsocket::PeekRecv(void *szData, unsigned int nDataLen)
{
    int nRet = 0;
    if(mps->mnType != SOCK_STREAM) /*只有TCP连接才能够使用此函数*/
    {
        return -1;
    }
    if(mps->mbHasPeekData)
    {
        return -2; // 如果已有peek data，则直接返回失败。需要用完已peek的数据才可以再调此函数。
    }
    mps->mbHasPeekData = false;
    nRet = Recv(szData, nDataLen);
    if(nRet > 0)
    {
        ftnetbuffer tmpBuf(szData, nDataLen);
        mps->mPeekBufData = tmpBuf;
        mps->mbHasPeekData = true;
        mps->mnPeekBufPos = 0;
    }
    return nRet;
}
const char * ftsocket::PeekRecv(unsigned int nMinRecvSize, unsigned int *pOutRealLen)
{
    ftnetbuffer tmpBuf(2048);
    int nRet = 0;
    mps->SetLastErrorInfo(0, "");
    if(pOutRealLen)
    {
        *pOutRealLen = 0;
    }
    if(mps->mnType != SOCK_STREAM) /*只有TCP连接才能够使用此函数*/
    {
        mps->SetLastErrorInfo(-1, "invalid socket");
        return NULL;
    }
    if(mps->mbHasPeekData)
    {
        mps->SetLastErrorInfo(-2, "there is some peek data that is not used, please use it first.");
        return NULL; // 如果已有peek data，则直接返回失败。需要用完已peek的数据才可以再调此函数。
    }
    mps->mbHasPeekData = false;
    nRet = Recv(tmpBuf, nMinRecvSize);
    if(nRet > 0)
    {
        tmpBuf.CutToRealSize(nRet);
        
        mps->mPeekBufData = tmpBuf;
        mps->mbHasPeekData = true;
        mps->mnPeekBufPos = 0;
        if(pOutRealLen)
        {
            *pOutRealLen = nRet;
        }
        return mps->mPeekBufData.buffer();
    }
    return NULL;
}
unsigned int ftsocket::HasPeekData(const char **pOutAddr) const
{
    if(pOutAddr)
    {
        *pOutAddr = NULL;
    }
    if(!mps->mbHasPeekData)
    {
        return 0;
    }
    if(mps->mPeekBufData.bufferLen() > mps->mnPeekBufPos)
    {
        unsigned int nRemain = mps->mPeekBufData.bufferLen() - mps->mnPeekBufPos;
        if(pOutAddr)
        {
            *pOutAddr = mps->mPeekBufData.buffer() + mps->mnPeekBufPos;
        }
        return nRemain;
    }
    mps->mbHasPeekData = false;
    return 0;
}

unsigned int ftsocket::UsePeekData(unsigned int nUseLen)
{
    if(!mps->mbHasPeekData)
    {
        return 0;
    }
    if(mps->mPeekBufData.bufferLen() > mps->mnPeekBufPos)
    {
        unsigned int nRemain = mps->mPeekBufData.bufferLen() - mps->mnPeekBufPos;
        mps->mnPeekBufPos += nUseLen;
        if(nRemain > nUseLen)
        {
            return nUseLen;
        }
        mps->mbHasPeekData = false;
        return nRemain;
    }
    mps->mbHasPeekData = false;
    return 0;
}

#ifdef HAS_FD_SEND_RECV
// 文件描述符相关函数

/*
 * 从fd中接收消息，并将文件描述符放在指针recvfd中
 */
static ssize_t recv_fd(int fd, void*data, size_t bytes, int*recvfd)
{
    struct msghdr msghdr_recv; /*建立一个消息，之后填充此消息的成员数据，并发送给fd*/
    struct iovec iov[1]; /*为向量，消息的数据在此项中保存*/
    size_t n;
    // int newfd;
    /*建立一个联合结构，便于进行消息的处理*/
    union{
        struct cmsghdr cm;
        char control[CMSG_SPACE(sizeof(int))];
    }control_un;
    struct cmsghdr*pcmsghdr; /*消息头部*/
    msghdr_recv.msg_control = control_un.control; /*控制消息*/
    msghdr_recv.msg_controllen = sizeof(control_un.control); /*控制消息的长度*/
    
    msghdr_recv.msg_name = NULL; /*消息的名称为空*/
    msghdr_recv.msg_namelen = 0; /*消息的长度为空*/
    
    iov[0].iov_base = data; /*向量的数据为传入的数据*/
    iov[0].iov_len = bytes; /*向量的长度为传入数据的长度*/
    msghdr_recv.msg_iov = iov; /*消息向量指针*/
    msghdr_recv.msg_iovlen = 1; /*消息向量的个数为1个*/
    if((n = recvmsg(fd, &msghdr_recv, 0))<=0) /*接收消息*/
        return n;
    
    if((pcmsghdr = CMSG_FIRSTHDR(&msghdr_recv))!= NULL && /*获得消息的头部*/
       pcmsghdr->cmsg_len == CMSG_LEN(sizeof(int))){ /*获得消息的长度为int*/
        if(pcmsghdr->cmsg_level != SOL_SOCKET) /*消息的level应该为SOL_SOCKET*/
            printf("control level != SOL_SOCKET\n");
        
        if(pcmsghdr->cmsg_type != SCM_RIGHTS) /*消息的类型判断*/
            printf("control type != SCM_RIGHTS\n");
        
        *recvfd =*((int*)CMSG_DATA(pcmsghdr)); /*获得打开文件的描述符*/
    }else
        *recvfd = -1;
    
    return n; /*返回接收消息的长度*/
}
#if 0
static ssize_t send_fd(int fd, void*data, size_t bytes, int sendfd)
{
    struct msghdr msghdr_send; /*发送消息,建立一个消息，之后填充此消息的成员数据，并发送给fd*/
    struct iovec iov[1]; /*为向量，消息的数据在此项两种保存*/
    //size_t n; /*大小*/
    // int newfd; /*文件描述符*/
    /*方便操作msg的结构 建立一个联合结构，便于进行消息的处理*/
    union{
        struct cmsghdr cm; /*control msg结构*/
        char control[CMSG_SPACE(sizeof(int))]; /*字符指针，方便控制*/
    }control_un;
    struct cmsghdr*pcmsghdr=NULL; /*控制头部的指针*/
    msghdr_send.msg_control = control_un.control; /*填充消息的控制部分*/
    msghdr_send.msg_controllen = sizeof(control_un.control); /*控制部分的长度*/
    
    pcmsghdr = CMSG_FIRSTHDR(&msghdr_send); /*取得第一个消息头*/
    pcmsghdr->cmsg_len = CMSG_LEN(sizeof(int)); /*获得长度 由于发送的是一个文件描述符，所以长度为一个int类型的长度*/
    pcmsghdr->cmsg_level = SOL_SOCKET; /*用于控制消息*/
    pcmsghdr->cmsg_type = SCM_RIGHTS;
    *((int*)CMSG_DATA(pcmsghdr))= sendfd; /*socket值*/
    
    
    msghdr_send.msg_name = NULL; /*名称*/
    msghdr_send.msg_namelen = 0; /*名称长度*/
    
    iov[0].iov_base = data; /*向量指针*/
    iov[0].iov_len = bytes; /*数据长度*/
    msghdr_send.msg_iov = iov; /*填充消息*/
    msghdr_send.msg_iovlen = 1;
    
    return (sendmsg(fd, &msghdr_send, 0)); /*发送消息*/
}
#else
//send_fd函数负责把取得的文件描述符传出去：

int send_fd(int fd, void *data, int bytes, int sendfd)
{
    struct msghdr msg;
    struct iovec iov[1];
    
    /*
     * Adapted from: W. Richard Stevens, UNIX Network Programming, Volume 1,
     * Second edition. Except that we use CMSG_LEN instead of CMSG_SPACE; the
     * latter breaks on LP64 systems.
     */
#if defined(CMSG_SPACE) && !defined(NO_MSGHDR_MSG_CONTROL)
    union {
        struct cmsghdr just_for_alignment;
        char control[CMSG_SPACE(sizeof(sendfd))];
    } control_un;
    struct cmsghdr *cmptr;
    
    memset((char *) &msg, 0, sizeof(msg)); /* Fix 200512 */
    msg.msg_control = control_un.control;
    msg.msg_controllen = CMSG_LEN(sizeof(sendfd)); /* Fix 200506 */
    
    cmptr = CMSG_FIRSTHDR(&msg);
    cmptr->cmsg_len = CMSG_LEN(sizeof(sendfd));
    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type = SCM_RIGHTS;
    *(int *) CMSG_DATA(cmptr) = sendfd;
#else
    msg.msg_accrights = (char *) &sendfd;
    msg.msg_accrightslen = sizeof(sendfd);
#endif
    
    msg.msg_name = 0;
    msg.msg_namelen = 0;
    
    /*
     * XXX We don't want to pass any data, just a file descriptor. However,
     * setting msg.msg_iov = 0 and msg.msg_iovlen = 0 causes trouble. See the
     * comments in the unix_recv_fd() routine.
     */
    iov->iov_base = (void *)"";
    iov->iov_len = 1;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    
    return (int) (sendmsg(fd, &msg, 0));
}
#endif
int ftsocket::SendSocket(const ftsocket &sock)
{
    int nRet = 0;
    if(!IsValidSocket(mps->mSock) || !IsValidSocket(sock.mps->mSock))
    {
        return -1;
    }
    static char nouseData[2] = "a";
    nRet = (int) send_fd(mps->mSock, nouseData, 1, sock.mps->mSock);
    if(nRet <= 0)
    {
        do
        {
#ifdef WIN32
            int error = WSAGetLastError();
            if(WSAEWOULDBLOCK == error)
#else
                int error = errno;
            if(EWOULDBLOCK == error)
#endif
            {
                ResumeNetOpObject resumeObj;
                _ftnetAddWriteFd(mps->mSock, _NetEventCb, &resumeObj, mps->mnWriteTimeoutSecond);
                int nNetOpRes = 0;
                nRet = YieldFromFakeThread(resumeObj, &nNetOpRes);
                if(nRet)
                {
                    mps->SetLastErrorInfo(2, "yield return error");
                    break;
                }
                if(nNetOpRes)
                {
                    nRet = -1;
                    std::stringstream ssErr;
                    ssErr << "net send_fd operation is timeout for " << mps->mnWriteTimeoutSecond << " seconds.";
                    mps->SetLastErrorInfo(-3, ssErr.str().c_str());
                    break;
                }
                nRet = (int) send_fd(mps->mSock, nouseData, 1, sock.mps->mSock);
            }
            else
            {
                mps->SetLastErrorInfo(error, strerror(error));
                break;
            }
        }while(0);
    }
    return nRet;
}
int ftsocket::RecvSocket(ftsocket &outSock)
{
    int nRet = 0;
    if(!IsValidSocket(mps->mSock) || IsValidSocket(outSock.mps->mSock))
    {
        return -1;
    }
    static char nouseData[2] = "a";
    int nOutSock = 0;
    for(int nTimes = 0;nTimes < 2;++nTimes)
    {
        nRet = (int) recv_fd(mps->mSock, nouseData, 1, &nOutSock);
        if(nRet > 0)
        {
            break;
        }
        else if(nTimes == 0)
        {
#ifdef WIN32
            int error = WSAGetLastError();
            if(WSAEWOULDBLOCK == error)
#else
                int error = errno;
            if(EWOULDBLOCK == error)
#endif
            {
                ResumeNetOpObject resumeObj;
                _ftnetAddReadFd(mps->mSock, _NetEventCb, &resumeObj, mps->mnReadTimeoutSecond);
                int nNetOpRes = 0;
                nRet = YieldFromFakeThread(resumeObj, &nNetOpRes);
                if(nRet)
                {
                    mps->SetLastErrorInfo(2, "yield return error");
                    break;
                }
                if(nNetOpRes)
                {
                    nRet = -1;
                    std::stringstream ssErr;
                    ssErr << "net recv_fd operation is timeout for " << mps->mnReadTimeoutSecond << " seconds.";
                    mps->SetLastErrorInfo(-3, ssErr.str().c_str());
                    break;
                }
            }
            else
            {
                mps->SetLastErrorInfo(errno, strerror(errno));
                break;
            }
        }
    }
    if(IsValidSocket(nOutSock))
    {
        outSock.mps->AttachSock(nOutSock);
    }
    else
    {
        mps->SetLastErrorInfo(errno, strerror(errno));
    }
    return nRet;
}
#endif // #ifdef HAS_FD_SEND_RECV
