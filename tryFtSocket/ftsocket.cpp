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

#include <errno.h>
#include <string.h>
#include <stdarg.h>

typedef int (*_SockMgrFunction) (SOCKET sock, ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond);

static _SockMgrFunction _ftnetAddReadFd = _ftnetAddReadFd_select;
static _SockMgrFunction _ftnetAddWriteFd = _ftnetAddWriteFd_select;

static void _NetEventCb(int nErrCode, void *pParam)
{
    FakeThreadResumeObject *pResumeObj = (FakeThreadResumeObject *) pParam;
    (*pResumeObj)();
}

class _ftsocket_data
{
public:
    _ftsocket_data(int domain, int type, int protocol)
    : mnDomain(domain), mnType(type), mnProtocol(protocol)
    , mSock(INVALID_SOCKET), mnLastError(0), mszLastErrInfo("")
    , mfnOnClosed(NULL), mpOnClosedParam(NULL)
    , mnRefCount(0)
    {}
    ~_ftsocket_data()
    {
        if(IsValidSocket(mSock))
        {
            closesocket(mSock);
            if(mfnOnClosed)
            {
                mfnOnClosed(mpOnClosedParam, 0, "closed because no reference");
            }
        }
    }
    SOCKET wrap_socket()
    {
        return ::socket(mnDomain, mnType, mnProtocol);
    }
    void SetLastErrorInfo(int nError, const char *szInfo)
    {
        mnLastError = nError;
        mszLastErrInfo = szInfo;
    }
    int AttachSock(SOCKET sock)
    {
        if(IsValidSocket(mSock))
        {
            mnLastError = 1;
            mszLastErrInfo = "already init with socket";
            return -1;
        }
        mSock = sock;
        mnLastError = 0;
        mszLastErrInfo = "";
        return 0;
    }
    int IncreaseRef()
    {
        ++mnRefCount;
        return mnRefCount;
    }
    int DecreaseRef()
    {
        --mnRefCount;
        if(mnRefCount <= 0)
        {
            delete this;
            return 0;
        }
        return mnRefCount;
    }
    void Reset()
    {
        closesocket(mSock);
        mSock = INVALID_SOCKET;
        SetLastErrorInfo(0, "");
        mfnOnClosed = NULL;
        mpOnClosedParam = NULL;
    }
public:
    int mnDomain, mnType, mnProtocol;
    SOCKET mSock;
    int mnLastError;
    const char *mszLastErrInfo;
    
    ftsocketCloseCB mfnOnClosed;
    void *mpOnClosedParam;
    
    int mnRefCount; // 引用计数
};

int set_nonblocking(SOCKET fd, bool nonblock)
{
#ifdef __WINDOWS__
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

int set_reuse(SOCKET fd, bool resue)
{
#ifdef __WINDOWS__
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
    mps->DecreaseRef();
}
ftsocket::ftsocket(const ftsocket &fts)
: mps(fts.mps)
{
    mps->IncreaseRef();
}
ftsocket & ftsocket::operator = (const ftsocket &fts)
{
    mps->DecreaseRef();
    mps = fts.mps;
    mps->IncreaseRef();
    return *this;
}
bool ftsocket::operator ! () const
{
    return !IsValidSocket(mps->mSock);
}
bool ftsocket::operator < (const ftsocket &fts) const
{
    return mps < fts.mps;
}
int ftsocket::Bind(const char *szIp, unsigned int nPort)
{
    if(IsValidSocket(mps->mSock))
    {
        return -2;
    }
    int nRet = 0;
    sockaddr_in addr = {0};
    addr.sin_family = mps->mnDomain;
    addr.sin_port   = htons(nPort);
    addr.sin_addr.s_addr  = szIp ? inet_addr(szIp) : htonl(INADDR_ANY);
    memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
    
    mps->mSock = mps->wrap_socket();
    set_reuse(mps->mSock, true);
    if( bind(mps->mSock,(const  sockaddr*)&addr, sizeof(addr)) < 0)
    {
        nRet = -1;
    }
    else
    {
        if(mps->mnType == SOCK_STREAM)
        {
            nRet = listen(mps->mSock, 15);
        }
        set_nonblocking(mps->mSock, true);
    }
    if(nRet)
    {
        mps->SetLastErrorInfo(errno, strerror(errno));
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
            int error = errno;
            if(EWOULDBLOCK == error)
            {
                FakeThreadResumeObject resumeObj = GetResumeObjectForFakeThread();
                _ftnetAddReadFd(mps->mSock, _NetEventCb, &resumeObj, 0);
                nRet = YieldFromFakeThread();
                //
                if(0 == nRet)
                {
                    socklen_t nDataLen = sizeof(int);
                    nRet = getsockopt(mps->mSock, SOL_SOCKET, SO_ERROR, &error, &nDataLen);
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
    }
    return nRet;
}
int ftsocket::Connect(const char *szIp, unsigned int nPort)
{
    if(IsValidSocket(mps->mSock))
    {
        return -2;
    }
    mps->SetLastErrorInfo(0, "");
    int nRet = 0;
    sockaddr_in srv_addr = {0};
    srv_addr.sin_family  = mps->mnDomain;
    srv_addr.sin_port    = htons(nPort);
    srv_addr.sin_addr.s_addr = inet_addr(szIp);
    memset(srv_addr.sin_zero, 0, sizeof(srv_addr.sin_zero));
    
    mps->mSock = mps->wrap_socket();
    set_nonblocking(mps->mSock, true);
    set_reuse(mps->mSock, true);
    nRet = connect(mps->mSock, (const sockaddr*)&srv_addr, sizeof(srv_addr) );
    if( nRet < 0)
    {
        int error = errno;
        if(EINPROGRESS != error)
        {
            mps->SetLastErrorInfo(errno, strerror(errno));
        }
        else // 需要yield
        {
            FakeThreadResumeObject resumeObj = GetResumeObjectForFakeThread();
            _ftnetAddWriteFd(mps->mSock, _NetEventCb, &resumeObj, 0);
            nRet = YieldFromFakeThread();
            //
            if(0 == nRet)
            {
                socklen_t nDataLen = sizeof(int);
                nRet = getsockopt(mps->mSock, SOL_SOCKET, SO_ERROR, &error, &nDataLen);
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
            if(EWOULDBLOCK == errno)
            {
                FakeThreadResumeObject resumeObj = GetResumeObjectForFakeThread();
                _ftnetAddWriteFd(mps->mSock, _NetEventCb, &resumeObj, 0);
                nRet = YieldFromFakeThread();
                if(nRet)
                {
                    mps->SetLastErrorInfo(2, "yield return error");
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
    if(nPos >= nDataLen)
    {
        nRet = nPos;
    }
    return nRet;
}
int ftsocket::Recv(void *szData, unsigned int nDataLen)
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
        nRet = (int) ::recv(mps->mSock, ((char *) szData) + nPos, nDataLen - nPos, 0);
        if(nRet > 0)
        {
            nPos += nRet;
            if(mps->mnType == SOCK_DGRAM)
            {
                break;
            }
        }
        else
        {
            if(EWOULDBLOCK == errno)
            {
                FakeThreadResumeObject resumeObj = GetResumeObjectForFakeThread();
                _ftnetAddReadFd(mps->mSock, _NetEventCb, &resumeObj, 0);
                nRet = YieldFromFakeThread();
                if(nRet)
                {
                    mps->SetLastErrorInfo(2, "yield return error");
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
int ftsocket::Recv(ftnetbuffer &buf)
{
    return Recv(buf.buffer(), buf.bufferLen());
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
        nRet = (int) ::sendto(mps->mSock, szData, nDataLen, 0, (const struct sockaddr *) &srv_addr, sizeof(srv_addr));
        if(nRet < 0)
        {
            if(errno != EWOULDBLOCK) // 出错了
            {
                mps->SetLastErrorInfo(errno, strerror(errno));
                break;
            }
            else // 阻塞了，需要yield
            {
                FakeThreadResumeObject resumeObj = GetResumeObjectForFakeThread();
                _ftnetAddReadFd(mps->mSock, _NetEventCb, &resumeObj, 0);
                nRet = YieldFromFakeThread();
                if(nRet)
                {
                    mps->SetLastErrorInfo(2, "yield return error");
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
        nRet = (int) ::recvfrom(mps->mSock, szData, nDataLen, 0, (struct sockaddr *) &addr, &addr_len);
        if(nRet < 0)
        {
            if(errno != EWOULDBLOCK) // 出错了
            {
                mps->SetLastErrorInfo(errno, strerror(errno));
                break;
            }
            else // 阻塞了，需要yield
            {
                FakeThreadResumeObject resumeObj = GetResumeObjectForFakeThread();
                _ftnetAddReadFd(mps->mSock, _NetEventCb, &resumeObj, 0);
                nRet = YieldFromFakeThread();
                if(nRet)
                {
                    mps->SetLastErrorInfo(2, "yield return error");
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
    mps->Reset();
    
    //
    ftsocketCloseCB fn = mps->mfnOnClosed;
    void *pParam = mps->mpOnClosedParam;
    if(fn)
    {
        fn(pParam, 0, "");
    }
    return nRet;
}
const char * ftsocket::LastError() const
{
    return mps->mszLastErrInfo;
}
int ftsocket::LastErrorNo() const
{
    return mps->mnLastError;
}
void ftsocket::MonitorCloseEvent(ftsocketCloseCB fn, void *pParam)
{
    mps->mfnOnClosed = fn;
    mps->mpOnClosedParam = pParam;
}
int ftsocket::AddToReadMonitor(ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond) const
{
    if(!IsValidSocket(mps->mSock))
    {
        return -1;
    }
    return _ftnetAddReadFd(mps->mSock, fn, pParam, 0);
}
int ftsocket::recv_without_yield(void *szData, unsigned int nDataLen)
{
    mps->SetLastErrorInfo(0, "");
    int nRet = (int) ::recv(mps->mSock, szData, nDataLen, 0);
    if(nRet)
    {
        if(errno != EWOULDBLOCK)
        {
            mps->SetLastErrorInfo(errno, strerror(errno));
        }
    }
    return nRet;
}
int ftsocket::AddToSendMonitor(ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond) const
{
    if(!IsValidSocket(mps->mSock))
    {
        return -1;
    }
    return _ftnetAddWriteFd(mps->mSock, fn, pParam, 0);
}
int ftsocket::send_without_yield(void *szData, unsigned int nDataLen)
{
    mps->SetLastErrorInfo(0, "");
    int nRet = (int) ::send(mps->mSock, szData, nDataLen, 0);
    if(nRet)
    {
        if(errno != EWOULDBLOCK)
        {
            mps->SetLastErrorInfo(errno, strerror(errno));
        }
    }
    return nRet;
}
