//
//  ftsocket.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/26.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__ftsocket__
#define __TestAllUseful__ftsocket__

#include "ftnetselect.h"

#include <stdio.h>
#include <iosfwd>

class _ftsocket_data;
class ftnetbuffer;

class ftsocket;

#ifndef WIN32
#define HAS_FD_SEND_RECV
#endif

// 当连接被关闭时的回调通知函数
typedef void (*ftsocketCloseCB) (void *pParam, const ftsocket &sock, int nCode, const char *szInfo);

class FTNET_API ftsocket
{
public:
    ftsocket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
    ftsocket(const ftsocket &fts);
    ~ftsocket();
    
    bool IsValid() const;
    ftsocket & operator = (const ftsocket &fts);
    bool operator ! () const;
    bool operator < (const ftsocket &fts) const;
public:
    /***
     绑定端口，如果bListen为true, 则调用listen, 否则就不调listen；目的是绑定某个IP端口来连接对方。
     */
    int Bind(const char *szIp, unsigned int nPort, bool bListen = true);
    int Accept(ftsocket &outCliSocket);
    int Connect(const char *szIp, unsigned int nPort, const char *szLocalIp = NULL, unsigned int nLocalPort = 0);
    int Send(const void *szData, unsigned int nDataLen);
    int Send(const ftnetbuffer &buf);
    int Send(int nBufCount, ...);
    int Send(int nBufCount, va_list arg_ptr);
    int Recv(void *szData, unsigned int nDataLen, unsigned int nMinRecvSize = 0xFFFFFFFF);
    int Recv(ftnetbuffer &buf, unsigned int nMinRecvSize = 0xFFFFFFFF);
    int Recv(int nBufCount, ...);
    int Recv(int nBufCount, va_list arg_ptr);
    
    int SendTo(const void *szData, unsigned int nDataLen, const char *szIp, unsigned int nPort);
    int SendTo(const ftnetbuffer &buf, const char *szIp, unsigned int nPort);
    int RecvFrom(void *szData, unsigned int nDataLen, char *szIp, unsigned int nIpBufLen, unsigned int &nOutPort);
    int RecvFrom(ftnetbuffer &buf, char *szIp, unsigned int nIpBufLen, unsigned int &nOutPort);
    
    //
    int Close();
#ifdef HAS_FD_SEND_RECV
public: // 文件描述符函数
    int SendSocket(const ftsocket &sock);
    int RecvSocket(ftsocket &outSock);
#endif
public:
    const char *localIp() const;
    unsigned int localPort() const;
    const char *remoteIp() const;
    unsigned int remotePort() const;

    const char *local() const;
    const char *remote() const;
public:
    // 给socket设置一个名称，用于调试时知道是哪里生的的socket对象
    void SetSockDebugInfo(const char *szSockDebugInfo);
public:
    // 设置监控关闭事件的回调函数
    void MonitorCloseEvent(ftsocketCloseCB fn, void *pParam);
    /* 设置读写的超时时间。注意：只有在执行读或写动作时才会计时。时间为0表示不计超时 */
    void SetTimeout(unsigned int nReadTimeoutSecond, unsigned int nWriteTimeoutSecond);
    /* 获取当前的超时设置 */
    void GetTimeout(unsigned int &nReadTimeoutSecond, unsigned int &nWriteTimeoutSecond);
public:
    // only for advanced user
    int AddToReadMonitor(ftEventCallbackFn fn, void *pParam) const;
    int recv_without_yield(void *szData, unsigned int nDataLen);
    int AddToSendMonitor(ftEventCallbackFn fn, void *pParam) const;
    int send_without_yield(void *szData, unsigned int nDataLen);
    /***
     PeekRecv
     从tcp上接收一点数据，但是在下次调用Recv函数时，还是会返回这些数据的.
     注意：如果连续调用此函数，则会丢失上一次收到的数据。
     
     此函数的主要目的是在一个服务端口上支持多种协程。
     这时需要接受一个连接后，先收一开始的部分数据，
     以判断是什么协议，然后再进入不同的协议处理流程中去。
     */
    int PeekRecv(void *szData, unsigned int nDataLen);
    /***
     PeekRecv
     收取至少nMinRecvSize字节的peek数据.
     成功则返回缓冲区指针。
     失败返回NULL, 错误用LastErrorNo获取
     pOutRealLen返回实际收到数据的总长度
     */
    const char * PeekRecv(unsigned int nMinRecvSize, unsigned int *pOutRealLen);
    /***
     HasPeekData
     返回当前peek data的字节数
     如果pOutAddr非NULL，且有peek data，则返回peek data下一字节地址。注意：这并不能保证有字节对齐的地址。
     */
    unsigned int HasPeekData(const char **pOutAddr) const;
    /***
     UsePeekData
     使用一定字节数的peek data
     返回实际使用的peek data字节数，即如果传入参数大于实际数据时，则返回实际大小，并清空peek data
     返回0表示peek data用完了
     */
    unsigned int UsePeekData(unsigned int nUseLen);
public:
    const char * LastError() const;
    int LastErrorNo() const;
public :
    _ftsocket_data *mps;
};

FTNET_API std::ostream & operator << (std::ostream &os, const ftsocket &fts);

#endif /* defined(__TestAllUseful__ftsocket__) */
