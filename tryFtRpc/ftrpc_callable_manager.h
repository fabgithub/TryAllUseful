//
//  ftrpc_callable_manager.h
//  TestAllUseful
//
//  Created by liyoudi on 15/3/1.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__ftrpc_callable_manager__
#define __TestAllUseful__ftrpc_callable_manager__

#include "ftrpc_callable.h"

#include <stdio.h>

class _ftrpc_callable_manager_data;
// 管理所有接受的调用
class ftrpc_callable_manager
{
public:
    ftrpc_callable_manager();
    ftrpc_callable_manager(const ftrpc_callable_manager &mgr);
    ~ftrpc_callable_manager();
    
    ftrpc_callable_manager & operator = (const ftrpc_callable_manager &mgr);
    bool operator ! () const;
    bool IsValid() const;
protected:
    friend class _ftrpcProcessorInfo;
    int CallFunc(_ftBufArray &retBufArray, const char *szFuncName, const _ftBufArray &parameters);
public:
    template <class Ret, class T1>
    int RegisterFunc(const char *szFuncName, Ret (*fn) (T1))
    {
        int nRet = 0;
        _ftrpcCallable1<Ret, T1> *pCallable = new _ftrpcCallable1<Ret, T1>();
        pCallable->mfn = fn;
        nRet = AddCallableFunc(szFuncName, pCallable);
        return nRet;
    }
    
#define _DECLARE_REGISTER_FUNC(n) \
    template <class Ret, __ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
    int RegisterFunc(const char *szFuncName, Ret (*fn) (__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T))) \
    { \
        int nRet = 0; \
        _ftrpcCallable##n<Ret, __ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)> *pCallable = new _ftrpcCallable##n<Ret, __ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)>(); \
        pCallable->mfn = fn; \
        nRet = AddCallableFunc(szFuncName, pCallable); \
        return nRet; \
    }

//_DECLARE_REGISTER_FUNC(1)
_DECLARE_REGISTER_FUNC(2)
_DECLARE_REGISTER_FUNC(3)
_DECLARE_REGISTER_FUNC(4)
_DECLARE_REGISTER_FUNC(5)
_DECLARE_REGISTER_FUNC(6)
_DECLARE_REGISTER_FUNC(7)
_DECLARE_REGISTER_FUNC(8)
_DECLARE_REGISTER_FUNC(9)

protected:
    int AddCallableFunc(const char *szFuncName, ftrpc_callable *pCallable);
protected:
    _ftrpc_callable_manager_data *mpcmd;
};

#endif /* defined(__TestAllUseful__ftrpc_callable_manager__) */
