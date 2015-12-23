//
//  ftsocket.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/26.
//  Copyright (c) 2015�� liyoudi. All rights reserved.
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

// �����ӱ��ر�ʱ�Ļص�֪ͨ����
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
     �󶨶˿ڣ����bListenΪtrue, �����listen, ����Ͳ���listen��Ŀ���ǰ�ĳ��IP�˿������ӶԷ���
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
public: // �ļ�����������
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
    // ��socket����һ�����ƣ����ڵ���ʱ֪�����������ĵ�socket����
    void SetSockDebugInfo(const char *szSockDebugInfo);
public:
    // ���ü�عر��¼��Ļص�����
    void MonitorCloseEvent(ftsocketCloseCB fn, void *pParam);
    /* ���ö�д�ĳ�ʱʱ�䡣ע�⣺ֻ����ִ�ж���д����ʱ�Ż��ʱ��ʱ��Ϊ0��ʾ���Ƴ�ʱ */
    void SetTimeout(unsigned int nReadTimeoutSecond, unsigned int nWriteTimeoutSecond);
    /* ��ȡ��ǰ�ĳ�ʱ���� */
    void GetTimeout(unsigned int &nReadTimeoutSecond, unsigned int &nWriteTimeoutSecond);
public:
    // only for advanced user
    int AddToReadMonitor(ftEventCallbackFn fn, void *pParam) const;
    int recv_without_yield(void *szData, unsigned int nDataLen);
    int AddToSendMonitor(ftEventCallbackFn fn, void *pParam) const;
    int send_without_yield(void *szData, unsigned int nDataLen);
    /***
     PeekRecv
     ��tcp�Ͻ���һ�����ݣ��������´ε���Recv����ʱ�����ǻ᷵����Щ���ݵ�.
     ע�⣺����������ô˺�������ᶪʧ��һ���յ������ݡ�
     
     �˺�������ҪĿ������һ������˿���֧�ֶ���Э�̡�
     ��ʱ��Ҫ����һ�����Ӻ�����һ��ʼ�Ĳ������ݣ�
     ���ж���ʲôЭ�飬Ȼ���ٽ��벻ͬ��Э�鴦��������ȥ��
     */
    int PeekRecv(void *szData, unsigned int nDataLen);
    /***
     PeekRecv
     ��ȡ����nMinRecvSize�ֽڵ�peek����.
     �ɹ��򷵻ػ�����ָ�롣
     ʧ�ܷ���NULL, ������LastErrorNo��ȡ
     pOutRealLen����ʵ���յ����ݵ��ܳ���
     */
    const char * PeekRecv(unsigned int nMinRecvSize, unsigned int *pOutRealLen);
    /***
     HasPeekData
     ���ص�ǰpeek data���ֽ���
     ���pOutAddr��NULL������peek data���򷵻�peek data��һ�ֽڵ�ַ��ע�⣺�Ⲣ���ܱ�֤���ֽڶ���ĵ�ַ��
     */
    unsigned int HasPeekData(const char **pOutAddr) const;
    /***
     UsePeekData
     ʹ��һ���ֽ�����peek data
     ����ʵ��ʹ�õ�peek data�ֽ���������������������ʵ������ʱ���򷵻�ʵ�ʴ�С�������peek data
     ����0��ʾpeek data������
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
