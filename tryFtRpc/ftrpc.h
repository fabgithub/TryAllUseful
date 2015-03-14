//
//  ftrpc.h
//  TestAllUseful
//
//  Created by liyoudi on 15/3/1.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__ftrpc__
#define __TestAllUseful__ftrpc__

#include "ftsocket.h"
#include "ftnetbuffer.h"
#include "ftrpc_pack_basic.h"
#include "FakeThreadTemplate.h"

#include <stdio.h>

class ftrpc_callable_manager;

class ftrpc
{
public:
    ftrpc();
    ftrpc(const ftrpc &rpc);
    explicit ftrpc(const ftsocket &fts);
    ~ftrpc();
    
    ftrpc & operator = (const ftrpc &rpc);
public: // 注册可被远端调用的函数
    void SetCallableMgr(const ftrpc_callable_manager &mgr);
protected:
    int SendRpcReturnPackage(unsigned int seq, const char *szFuncName, char nResult, _ftBufArray &retBufArray);
    int SendRpcRequestPackage(const char *szFuncName, _ftBufArray &retBufArray, unsigned int nParameterCount, ...);
    
    friend class _ftrpcProcessorInfo;
public: // 调用对方函数
//    template <class Ret, class T1>
//    int RemoteCall(const char *szFuncName, Ret &ret, Ret (*fn) (T1), T1);
#define _DECLARE_REMOTE_CALL(n) \
    template <class Ret, __ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
    int RemoteCall(const char *szFuncName, Ret &ret, Ret (*fn) (__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)), __ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T));
    
    _DECLARE_REMOTE_CALL(1)
    _DECLARE_REMOTE_CALL(2)
    _DECLARE_REMOTE_CALL(3)
    _DECLARE_REMOTE_CALL(4)
    _DECLARE_REMOTE_CALL(5)
    _DECLARE_REMOTE_CALL(6)
    _DECLARE_REMOTE_CALL(7)
    _DECLARE_REMOTE_CALL(8)
    _DECLARE_REMOTE_CALL(9)
#undef _DECLARE_REMOTE_CALL
protected:
    ftsocket mfts;
};

template <class Ret, class T1>
int ftrpc::RemoteCall(const char *szFuncName, Ret &ret, Ret (*fn) (T1), T1 p1)
{
    _ftBufArray retBufArr;
    int nRet = 0;
    ftnetbuffer b1 = ftrpc_pack(p1);
    nRet = SendRpcRequestPackage(szFuncName, retBufArr, 1, &b1);
    if(!nRet)
    {
        if(retBufArr.empty())
        {
            nRet = -9999995;
        }
        else
        {
            nRet = ftrpc_unpack(retBufArr[0], ret);
            if(nRet > 0) nRet = 0;
        }
    }
    return nRet;
}

#define _macro_pack_param(n) ftnetbuffer b##n = ftrpc_pack(p##n);
#define _macro_pack_param_1 _macro_pack_param(1)
#define _macro_pack_param_2 _macro_pack_param_1 _macro_pack_param(2)
#define _macro_pack_param_3 _macro_pack_param_2 _macro_pack_param(3)
#define _macro_pack_param_4 _macro_pack_param_3 _macro_pack_param(4)
#define _macro_pack_param_5 _macro_pack_param_4 _macro_pack_param(5)
#define _macro_pack_param_6 _macro_pack_param_5 _macro_pack_param(6)
#define _macro_pack_param_7 _macro_pack_param_6 _macro_pack_param(7)
#define _macro_pack_param_8 _macro_pack_param_7 _macro_pack_param(8)
#define _macro_pack_param_9 _macro_pack_param_8 _macro_pack_param(9)

#define _IMPLEMENT_REMOTE_CALL(n) \
    template <class Ret, __ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
    int ftrpc::RemoteCall(const char *szFuncName, Ret &ret, Ret (*fn) (__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)), __ft_COMMON_DECLARE_LIST##n(T, p)) \
    { \
        _ftBufArray retBufArr; \
        int nRet = 0; \
        _macro_pack_param_##n \
        nRet = SendRpcRequestPackage(szFuncName, retBufArr, n, __ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, &b)); \
        if(!nRet) \
        { \
            if(retBufArr.empty()) \
            { \
                nRet = -9999995; \
            } \
            else \
            { \
                nRet = ftrpc_unpack(retBufArr[0], ret); \
                if(nRet > 0) nRet = 0; \
            } \
        } \
        return nRet; \
    }

//_IMPLEMENT_REMOTE_CALL(1)
_IMPLEMENT_REMOTE_CALL(2)
_IMPLEMENT_REMOTE_CALL(3)
_IMPLEMENT_REMOTE_CALL(4)
_IMPLEMENT_REMOTE_CALL(5)
_IMPLEMENT_REMOTE_CALL(6)
_IMPLEMENT_REMOTE_CALL(7)
_IMPLEMENT_REMOTE_CALL(8)
_IMPLEMENT_REMOTE_CALL(9)

#undef _IMPLEMENT_REMOTE_CALL

#endif /* defined(__TestAllUseful__ftrpc__) */
