//
//  ftrpc.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/3/1.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#include "ftrpc.h"
#include "FakeThreadTemplate.h"
#include "ftrpc_callable_manager.h"

#include <string.h>
#include <stdarg.h>
#include <set>
#include <map>
#include <vector>
#include <iostream>

#define _InfoLog(args) // std::cout << "ftrpc.cpp (" << __LINE__ << "): " args << std::endl

class ftrpc_head
{
public:
    ftrpc_head() : mHeadBuf(24)
    { strcpy(mHeadBuf.buffer(), "ftrpc"); majorVer() = 0; minorVer() = 0; cmd() = CMD_REQUEST, paramterCount() = 0; reserved1() = 0; seq() = 0; bodyLen() = 0; }
    ~ftrpc_head() {}
    void Reset()
    {
        ftrpc_head h;
        mHeadBuf = h.mHeadBuf;
    }
public:
    enum
    {
        CMD_REQUEST = 0,
        CMD_RETURN,
        CMD_QUERY_FUNCTION_LIST,
        CMD_QUERY_FUNCTION_DETAIL,
    };
public:
    const char * protocol() const { ((char *)mHeadBuf.buffer())[5] = 0; return (mHeadBuf.buffer()); }
    unsigned char &majorVer() { return *(unsigned char *)(mHeadBuf.buffer() + 6); }
    const unsigned char majorVer() const { return *(unsigned char *)(mHeadBuf.buffer() + 6); }
    unsigned char &minorVer() { return *(unsigned char *)(mHeadBuf.buffer() + 7); }
    const unsigned char minorVer() const { return *(unsigned char *)(mHeadBuf.buffer() + 7); }
    // 指令位：0表示请求，1表示响应，2表示查询支持的函数名列表，3表示查询函数参数的信息
    unsigned char &cmd() { return *((unsigned char *)(mHeadBuf.buffer() + 8)); }
    const unsigned char cmd() const { return *((unsigned char *)(mHeadBuf.buffer() + 8)); }
    // 参数个数，如果大于0,表示是请求的参数个数，如果小于0,表示返回值的个数, 所以最大参数个数为127个
    unsigned char &paramterCount() { return *((unsigned char *)(mHeadBuf.buffer() + 9)); }
    const unsigned char paramterCount() const { return *((unsigned char *)(mHeadBuf.buffer() + 9)); }
    
    unsigned short &reserved1() { return *((unsigned short *)(mHeadBuf.buffer() + 10)); }
    const unsigned short reserved1() const { return *((unsigned short *)(mHeadBuf.buffer() + 10)); }
    unsigned int &seq() { return *((unsigned int *)(mHeadBuf.buffer() + 12)); }
    const unsigned int seq() const { return *((unsigned int *)(mHeadBuf.buffer() + 12)); }
    // 响应码，只是返回消息中表示响应码，请求消息中填0
    int &res() { return *((int *)(mHeadBuf.buffer() + 16)); }
    const int res() const { return *((int *)(mHeadBuf.buffer() + 16)); }
    // 包体字节长度
    unsigned int &bodyLen() { return *((unsigned int *)(mHeadBuf.buffer() + 20)); }
    const unsigned int bodyLen() const { return *((unsigned int *)(mHeadBuf.buffer() + 20)); }

    ftnetbuffer &buf() { return mHeadBuf; }
    const ftnetbuffer &buf() const { return mHeadBuf; }
protected:
    ftnetbuffer mHeadBuf;
};

std::ostream & operator << (std::ostream &os, const ftrpc_head & h)
{
    os << "ftrpc_head[" << h.protocol() << ", " << (unsigned int) h.majorVer() << "." << (unsigned int) h.minorVer() << ", cmd(" << (unsigned int)h.cmd() << "), " << (int) h.res() << ", params(" << (int) h.paramterCount() << "), seq(" << h.seq() << ")]";
    return os;
}
// ==================================================================
typedef int (*ftrpc_server_cb) (const ftrpc_head &head, _ftBufArray &retBufArray, const char *szFuncName, const _ftBufArray &parameters, void *pUserData);
// ==================================================================
typedef ResumeFakeThreadObjT1<_ftBufArray > _ResumeObjForReturn;
typedef void (*_ftReturnProcessorFn) ( const _ftBufArray &retBufList, _ResumeObjForReturn *pResumeObj );
class _ftReturnProcessorInfo
{
public:
    _ftReturnProcessorInfo(unsigned int seq = 0, _ftReturnProcessorFn fn = NULL, _ResumeObjForReturn *pResumeObj = NULL)
    : mnSeq(seq), mfn(fn), mpResumeObj(pResumeObj)
    {}
    bool operator < (const _ftReturnProcessorInfo &rpi) const
    {
        return mnSeq < rpi.mnSeq;
    }
public:
    unsigned int mnSeq;
    _ftReturnProcessorFn mfn;
    _ResumeObjForReturn *mpResumeObj;
};
typedef std::set<_ftReturnProcessorInfo> _ftReturnProcessorInfoSet;

static void _NetEventCb_ForRpc(int nErrCode, void *pParam);

class _ftrpcProcessorInfo
{
public:
    enum
    {
        INIT = 0,
        RECV_HEAD,
        RECV_BUFFER_LEN,
        RECV_BUFFER,
        FINISHED,
    };
    explicit _ftrpcProcessorInfo(const ftsocket &fts)
    : mfts(fts)
    , mnState(RECV_HEAD), mpRecvBuf(NULL), mnRecvLen(0), mnShouldRecvLen(0)
    {
        mnUsedToSaveParameterBufLen[0] = mnUsedToSaveParameterBufLen[1] = 0;
        mpRecvBuf = mHead.buf().buffer();
        mnShouldRecvLen = mHead.buf().bufferLen();
    }
public:
    int OnRecvEvent()
    {
        int nRet = 0;
        while(mRpcMgr.IsValid() || !mReturnProcessorSet.empty())
        {
            int nRecvRes = 0;
            nRecvRes = mfts.recv_without_yield(mpRecvBuf + mnRecvLen, mnShouldRecvLen - mnRecvLen);
            if(nRecvRes <= 0)
            {
                if(errno != EWOULDBLOCK)
                {
                    OnClosed();
                    nRet = -1;
                }
                else
                {
                    mfts.AddToReadMonitor(_NetEventCb_ForRpc, this);
                }
                break;
            }
            else
            {
                mnRecvLen += nRecvRes;
                if(mnRecvLen >= mnShouldRecvLen) // 本阶段完成，转换状态
                {
                    char nParamCount = 0;
                    switch (mnState)
                    {
                        case RECV_HEAD:
                            mnState = RECV_BUFFER_LEN;
                            mpRecvBuf = (char *) mnUsedToSaveParameterBufLen;
                            mnRecvLen = 0;
                            mnShouldRecvLen = (sizeof(mnUsedToSaveParameterBufLen));
                            _InfoLog( << "RECV_HEAD completed " << mHead );
                            break;
                        case RECV_BUFFER_LEN:
                            mnState = RECV_BUFFER;
                            if(mFuncNameBuffer.bufferLen() == 0) // 还没接收函数名
                            {
                                _InfoLog( << "start recv function name, length is " << mnUsedToSaveParameterBufLen[0] << ", " << mHead );
                                ftnetbuffer buf(mnUsedToSaveParameterBufLen[0]);
                                mFuncNameBuffer = buf;
                                mpRecvBuf = buf.buffer();
                            }
                            else
                            {
                                ftnetbuffer buf(mnUsedToSaveParameterBufLen[0]);
                                mParameters.push_back(buf);
                                mpRecvBuf = mParameters[mParameters.size() - 1].buffer();
                                _InfoLog( << "start recv " << mParameters.size() << "th parameter buffer, length is " << mnUsedToSaveParameterBufLen[0] << ", " << mHead );
                            }
                            mnRecvLen = 0;
                            mnShouldRecvLen = mnUsedToSaveParameterBufLen[0];
                            break;
                        case RECV_BUFFER:
                            nParamCount = (mHead.paramterCount());
                            if(nParamCount <= mParameters.size()) // 接收完毕
                            {
                                _InfoLog( << "recv rpc " << (mHead.cmd() != ftrpc_head::CMD_RETURN ? "request" : "return") << " commplete for '" << mFuncNameBuffer.buffer() << "'" << ", " << mHead );
                                OnRecvRpcMsg();
                                //
                                mHead.Reset();
                                mFuncNameBuffer.Reset();
                                mParameters.clear();
                                mnState = RECV_HEAD;
                                mnRecvLen = 0;
                                mpRecvBuf = mHead.buf().buffer();
                                mnShouldRecvLen = mHead.buf().bufferLen();
                            }
                            else // 继续接收下一个缓冲区
                            {
                                mnState = RECV_BUFFER_LEN;
                                mpRecvBuf = (char *) mnUsedToSaveParameterBufLen;
                                mnRecvLen = 0;
                                mnShouldRecvLen = (sizeof(mnUsedToSaveParameterBufLen));
                                if(mParameters.size() > 0)
                                {
                                    _InfoLog( << "recv " << mParameters.size() << "th parameter buffer completed, length is " << mParameters[mParameters.size() - 1].bufferLen() << ", " << mHead );
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        return nRet;
    }
    static void _DoRemoteCallInFakeThread(_ftrpcProcessorInfo *pThis)
    {
        int nRet = 0;
        pThis->mFuncNameBuffer.buffer()[pThis->mFuncNameBuffer.bufferLen() - 1] = 0; // 保证字符串字结束符
        _ftBufArray retBufArray;
        unsigned int seq = pThis->mHead.seq(); // 注意：必须保存，因为mHead在下面这个函数返回时，可能应已经变化了。
        ftnetbuffer nameBuf(pThis->mFuncNameBuffer); // 注意：必须保存
        ftrpc rpc(pThis->mfts);
        nRet = pThis->mRpcMgr.CallFunc(retBufArray, pThis->mFuncNameBuffer.buffer(), pThis->mParameters);
        rpc.SendRpcReturnPackage(seq, nameBuf.buffer(), nRet, retBufArray);
    }
    int OnRecvRpcMsg()
    {
        int nRet = 0;
        if(mHead.cmd() != ftrpc_head::CMD_RETURN) // 收到外部发来的远程调用
        {
            if(mRpcMgr.IsValid())
            {
                if(mFuncNameBuffer.bufferLen() > 0)
                {
                    RunInFakeThreadT(_DoRemoteCallInFakeThread, this);
                }
                else
                {
                    // not found the function, error
                    nRet = -1;
                }
            }
        }
        else // 收到对外部调用的返回消息
        {
            _ftReturnProcessorInfo rpi(mHead.seq());
            _ftReturnProcessorInfoSet::iterator iterFound;
            iterFound = mReturnProcessorSet.find(rpi);
            if(iterFound != mReturnProcessorSet.end())
            {
                _ftReturnProcessorFn fn = iterFound->mfn;
                _ResumeObjForReturn *pResumeObj = iterFound->mpResumeObj;
                mReturnProcessorSet.erase(iterFound);
                fn(mParameters, pResumeObj);
            }
            else
            {
                _InfoLog( << "ERROR return message. " << mHead );
            }
        }
        if(nRet)
        {
            OnClosed();
        }
        return nRet;
    }
    void ResponseAllByDefaule()
    {
        static const char szFailedBody[] = "conection is closed.";
        static ftnetbuffer failedBodyBuf(szFailedBody, (unsigned int) sizeof(szFailedBody));
//        _ResProcessorInfoSet_forRequest::iterator iter;
//        while(!mResProcessorSet.empty())
//        {
//            iter = mResProcessorSet.begin();
//            ftMegaResponse resp(iter->mnCmd, iter->mnSeq, -6000000, failedBodyBuf);
//            _ftmMegaResponseProcessor_forRequest fn = iter->mfn;
//            void *pParam = iter->mpParam;
//            mResProcessorSet.erase(iter);
//            fn(resp, pParam);
//        }
    }
    void OnClosed()
    {
    }
public:
    ftsocket mfts;
    _ftReturnProcessorInfoSet mReturnProcessorSet;
    ftrpc_callable_manager mRpcMgr;
    
    int mnState; // 0 - receiving head, 1 - receiving parameter length, 2 - receiving parameter buffer
    char *mpRecvBuf;
    unsigned int mnRecvLen;
    unsigned int mnShouldRecvLen;
    
    unsigned int mnUsedToSaveParameterBufLen[2]; // 用于在接收参数缓冲区的长度, 要8字节
    
    ftrpc_head mHead;
    ftnetbuffer mFuncNameBuffer;
    _ftBufArray mParameters;
};

typedef std::map<ftsocket, _ftrpcProcessorInfo *> ftrpcProcessorMap;
static ftrpcProcessorMap gftrpcProcessorMap;
// ==================================================================
static void FreeRpcProcessInfo(const ftsocket fts);

static void _NetEventCb_ForRpc(int nErrCode, void *pParam)
{
    //    std::cout << "---------------" << std::endl;
    _ftrpcProcessorInfo *pProcessor = (_ftrpcProcessorInfo *) pParam;
    // 在这里收数据，直到有一个完整的megaeyes消息生成
    int nRet = 0;
    nRet = pProcessor->OnRecvEvent();
    if(nRet)
    {
        FreeRpcProcessInfo(pProcessor->mfts);
    }
}// ==================================================================
static _ftrpcProcessorInfo * GetRpcProcessInfo(const ftsocket &fts)
{
    _ftrpcProcessorInfo *pProcessor = NULL;
    std::pair<ftrpcProcessorMap::iterator, bool> ib = gftrpcProcessorMap.insert(std::make_pair(fts, pProcessor));
    if (ib.second) // 若插入成功，则说明是新的，需要开始接收
    {
        pProcessor = new _ftrpcProcessorInfo(fts);
        ib.first->second = pProcessor;
        // 在这里要添加读事件监测
        fts.AddToReadMonitor(_NetEventCb_ForRpc, pProcessor);
    }
    else
    {
        pProcessor = ib.first->second;
    }
    return pProcessor;
}
static void FreeRpcProcessInfo(const ftsocket fts)
{
    ftrpcProcessorMap::iterator iter;
    iter = gftrpcProcessorMap.find(fts);
    if(iter != gftrpcProcessorMap.end())
    {
        delete iter->second;
        gftrpcProcessorMap.erase(iter);
    }
}
// ==================================================================
static void _ftSetRpcMgr(const ftsocket &fts, const ftrpc_callable_manager &rpcMgr)
{
    _ftrpcProcessorInfo * pProcessorInfo = GetRpcProcessInfo(fts);
    pProcessorInfo->mRpcMgr = rpcMgr;
}
static void _ftAddReturnProcessor(const ftsocket &fts, unsigned int nSeq, _ftReturnProcessorFn fn, _ResumeObjForReturn *pResumeObj)
{
    _ftrpcProcessorInfo * pProcessorInfo = GetRpcProcessInfo(fts);
    _ftReturnProcessorInfo rpi(nSeq, fn, pResumeObj);
    pProcessorInfo->mReturnProcessorSet.insert(rpi);
}
// ==================================================================
void _ftReturnProcessorCB(const _ftBufArray &retBufArray, _ResumeObjForReturn *pResumeObj)
{
    (*pResumeObj)(retBufArray);
}
// ==================================================================
// ==================================================================
// ==================================================================

ftrpc::ftrpc()
{
}
ftrpc::ftrpc(const ftrpc &rpc)
{
}
ftrpc::ftrpc(const ftsocket &fts)
: mfts(fts)
{
    
}
ftrpc::~ftrpc()
{
}
ftrpc & ftrpc::operator = (const ftrpc &rpc)
{
    mfts = rpc.mfts;
    return *this;
}
void ftrpc::SetCallableMgr(const ftrpc_callable_manager &mgr)
{
    _ftSetRpcMgr(mfts, mgr);
}
static unsigned int ComputeRpcBodyLen(const char *szFuncName, _ftBufArray &retBufArray)
{
    unsigned int nRet = 0;
    nRet += ftrpc_pack_GetAlignLen((unsigned int) strlen(szFuncName) + 1);
    _ftBufArray::iterator iter;
    for (iter = retBufArray.begin(); iter != retBufArray.end(); ++iter)
    {
        nRet += ftrpc_pack_GetAlignLen(iter->bufferLen());
    }
    nRet += 8 * (1 + (unsigned int) retBufArray.size());
    return nRet;
}
static int SendRpcReturnPackage_Common(ftsocket mfts, unsigned int seq, const char *szFuncName, char nResult, _ftBufArray &retBufArray, unsigned char cmd)
{
    int nRet = 0;
    if(!mfts)
    {
        nRet = -1;
    }
//    else if(cmd == ftrpc_head::CMD_RETURN) // 远程调用必须有参数
//    {
//        return -2;
//    }
    else
    {
        do
        {
            unsigned int nBodyLen = ComputeRpcBodyLen(szFuncName, retBufArray);
            char nParameterCount = (char) retBufArray.size();
            ftrpc_head head;
            head.paramterCount() = nParameterCount;
            head.cmd() = cmd;
            head.seq() = seq;
            head.bodyLen() = nBodyLen;
            
            nRet = mfts.Send(head.buf());
            if(nRet != head.buf().bufferLen())
            {
                break;
            }
            //
            ftnetbuffer nameBuf(ftrpc_pack_GetAlignLen(strlen(szFuncName) + 1));
            memset(nameBuf.buffer(), 0, nameBuf.bufferLen());
            strcpy(nameBuf.buffer(), szFuncName);
            ftnetbuffer nameLenBuf(8);
            *((unsigned int *) (nameLenBuf.buffer() + 0)) = nameBuf.bufferLen();
            *((unsigned int *) (nameLenBuf.buffer() + 4)) = 0;
            nRet = mfts.Send(nameLenBuf);
            if(nRet != nameLenBuf.bufferLen())
            {
                break;
            }
            nRet = mfts.Send(nameBuf);
            if(nRet != nameBuf.bufferLen())
            {
                break;
            }
            //
            for (_ftBufArray::iterator iter = retBufArray.begin();iter != retBufArray.end();++iter)
            {
                ftnetbuffer paramBuf = (*iter);
                if(!paramBuf)
                {
                    nRet = -99999996;
                    break;
                }
                ftnetbuffer lenBuf(8);
                *((unsigned int *) (lenBuf.buffer() + 0)) = paramBuf.bufferLen();
                *((unsigned int *) (lenBuf.buffer() + 4)) = 0;
                nRet = mfts.Send(lenBuf);
                if(nRet != lenBuf.bufferLen())
                {
                    break;
                }
                nRet = mfts.Send(paramBuf);
                if(nRet != (paramBuf).bufferLen())
                {
                    break;
                }
            }
            if(nRet <= 0)
            {
                break;
            }
            nRet = 0;
        }while(false);
    }
    return nRet;
}
int ftrpc::SendRpcReturnPackage(unsigned int seq, const char *szFuncName, char nResult, _ftBufArray &retBufArray)
{
    return SendRpcReturnPackage_Common(mfts, seq, szFuncName, nResult, retBufArray, ftrpc_head::CMD_RETURN);
}
int ftrpc::SendRpcRequestPackage(const char *szFuncName, _ftBufArray &retBufArray, unsigned int nParameterCount, ...)
{
    int nRet = 0;
    if(!mfts)
    {
        nRet = -1;
    }
    else
    {
        do
        {
            static unsigned int gnSeq = 0;
            ++gnSeq;
            unsigned int nThisSeq = gnSeq;
            _ftBufArray paramArray;
            //
            va_list arg_ptr;
            va_start(arg_ptr, nParameterCount);
            for (; nParameterCount > 0; --nParameterCount)
            {
                ftnetbuffer *pParamBuf = va_arg(arg_ptr, ftnetbuffer *);
                if(!pParamBuf)
                {
                    nRet = -99999996;
                    break;
                }
                paramArray.push_back(*pParamBuf);
            }
            va_end(arg_ptr);
            if(nRet < 0)
            {
                break;
            }
            nRet = SendRpcReturnPackage_Common(mfts, nThisSeq, szFuncName, 0, paramArray, ftrpc_head::CMD_REQUEST);
            if(nRet < 0)
            {
                break;
            }
            // 在这里需要yield
            _ResumeObjForReturn resumeObj;
            _ftAddReturnProcessor(mfts, nThisSeq, _ftReturnProcessorCB, &resumeObj);
            nRet = YieldFromFakeThread(resumeObj, &retBufArray);
        }while (false);
    }
    return nRet;
}

#if 0
static int SendRpcReturnPackage_Common_body_send_once(ftsocket mfts, unsigned int seq, const char *szFuncName, char nResult, _ftBufArray &retBufArray, unsigned char cmd)
{
    int nRet = 0;
    if(!mfts)
    {
        nRet = -1;
    }
    else if(retBufArray.size() == 0 && cmd == ftrpc_head::CMD_RETURN) // 远程调用必须有参数
    {
        return -2;
    }
    else
    {
        do
        {
            unsigned int nBodyLen = ComputeRpcBodyLen(szFuncName, retBufArray);
            char nParameterCount = (char) retBufArray.size();
            ftrpc_head head;
            head.paramterCount() = nParameterCount;
            head.cmd() = cmd;
            head.seq() = seq;
            head.bodyLen() = nBodyLen;
            
            nRet = mfts.Send(head.buf());
            if(nRet != head.buf().bufferLen())
            {
                break;
            }
            //
            ftnetbuffer bodyBuf(nBodyLen);
            unsigned nBufPos = 0;
            int nNameLen = ftrpc_pack_GetAlignLen(strlen(szFuncName) + 1);
            // store name len
            ftnetbuffer nameLenBuf(bodyBuf.buffer() + nBufPos, 8, false);
            *((unsigned int *) (nameLenBuf.buffer() + 0)) = nNameLen;
            *((unsigned int *) (nameLenBuf.buffer() + 4)) = 0;
            nBufPos += 8;
            // store name string
            ftnetbuffer nameBuf(bodyBuf.buffer() + nBufPos, nNameLen, false);
            memset(nameBuf.buffer(), 0, nameBuf.bufferLen());
            strcpy(nameBuf.buffer(), szFuncName);
            nBufPos += nNameLen;
            
            //
            for (_ftBufArray::iterator iter = retBufArray.begin();iter != retBufArray.end();++iter)
            {
                ftnetbuffer paramBuf = (*iter);
                if(!paramBuf)
                {
                    nRet = -99999996;
                    break;
                }
                unsigned int nRealBufLen = ftrpc_pack_GetAlignLen(paramBuf.bufferLen());
                ftnetbuffer lenBuf(bodyBuf.buffer() + nBufPos, 8, false);
                *((unsigned int *) (lenBuf.buffer() + 0)) = nRealBufLen;
                *((unsigned int *) (lenBuf.buffer() + 4)) = 0;
                nBufPos += 8;
                memcpy(bodyBuf.buffer() + nBufPos, paramBuf.buffer(), paramBuf.bufferLen());
                nBufPos += nRealBufLen;
            }
            if(nRet <= 0)
            {
                break;
            }
            nRet = mfts.Send(bodyBuf);
            if(nRet != (bodyBuf).bufferLen())
            {
                break;
            }
            if(nRet <= 0)
            {
                break;
            }
            nRet = 0;
        }while(false);
    }
    return nRet;
}
#endif // 0
