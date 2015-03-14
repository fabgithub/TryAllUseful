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

class _ftsocket_data;
class ftnetbuffer;

// 当连接被关闭时的回调通知函数
typedef void (*ftsocketCloseCB) (void *pParam, int nCode, const char *szInfo);

class ftsocket
{
public:
    ftsocket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
    ftsocket(const ftsocket &fts);
    ~ftsocket();
    
    ftsocket & operator = (const ftsocket &fts);
    bool operator ! () const;
    bool operator < (const ftsocket &fts) const;
public:
    int Bind(const char *szIp, unsigned int nPort);
    int Accept(ftsocket &outCliSocket);
    int Connect(const char *szIp, unsigned int nPort);
    int Send(const void *szData, unsigned int nDataLen);
    int Send(const ftnetbuffer &buf);
    int Send(int nBufCount, ...);
    int Send(int nBufCount, va_list arg_ptr);
    int Recv(void *szData, unsigned int nDataLen);
    int Recv(ftnetbuffer &buf);
    int Recv(int nBufCount, ...);
    int Recv(int nBufCount, va_list arg_ptr);
    
    int SendTo(const void *szData, unsigned int nDataLen, const char *szIp, unsigned int nPort);
    int SendTo(const ftnetbuffer &buf, const char *szIp, unsigned int nPort);
    int RecvFrom(void *szData, unsigned int nDataLen, char *szIp, unsigned int nIpBufLen, unsigned int &nOutPort);
    int RecvFrom(ftnetbuffer &buf, char *szIp, unsigned int nIpBufLen, unsigned int &nOutPort);
    
    int Close();
public:
    // 设置监控关闭事件的回调函数
    void MonitorCloseEvent(ftsocketCloseCB fn, void *pParam);
public:
    // only for advanced user
    int AddToReadMonitor(ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond = 0) const;
    int recv_without_yield(void *szData, unsigned int nDataLen);
    int AddToSendMonitor(ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond = 0) const;
    int send_without_yield(void *szData, unsigned int nDataLen);
public:
    const char * LastError() const;
    int LastErrorNo() const;
private:
    _ftsocket_data *mps;
};

#endif /* defined(__TestAllUseful__ftsocket__) */
