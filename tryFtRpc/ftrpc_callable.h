//
//  ftrpc_callable.h
//  TestAllUseful
//
//  Created by liyoudi on 15/3/1.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__ftrpc_callable__
#define __TestAllUseful__ftrpc_callable__

#include "ftnetbuffer.h"
#include "ftrpc_pack_basic.h"
#include "FakeThreadTemplate.h"

#include <stdio.h>
#include <vector>

class ftrpc_callable
{
public:
    virtual ~ftrpc_callable();
    virtual int operator () (_ftBufArray &retBufArray, const char *szFuncName, const _ftBufArray &parameters) = 0;
};

template <class Ret, class T1>
class _ftrpcCallable1 : public ftrpc_callable
{
public:
    virtual int operator () (_ftBufArray &retBufArray, const char *szFuncName, const _ftBufArray &parameters)
    {
        int nRet = -1;
        do
        {
            if(parameters.size() < 1)
            {
                nRet = -1;
                break;
            }
            T1 p1;
            nRet = ftrpc_unpack(parameters[0], p1);
            if(nRet < 0)
            {
                break;
            }
            Ret ret;
            ret = mfn(p1);
            ftnetbuffer retBuf;
            retBuf = ftrpc_pack(ret);
            retBufArray.push_back(retBuf);
            nRet = 0;
        }while(false);
        return nRet;
    }
public:
    Ret (*mfn) (T1);
};

#define _macro_uppack_param(n) \
                T##n p##n; \
                nRet = ftrpc_unpack(parameters[n - 1], p##n); \
                if(nRet < 0) \
                { \
                    nRet = -n; \
                    break; \
                }

#define _macro_uppack_param_1 _macro_uppack_param(1)
#define _macro_uppack_param_2 _macro_uppack_param_1 _macro_uppack_param(2)
#define _macro_uppack_param_3 _macro_uppack_param_2 _macro_uppack_param(3)
#define _macro_uppack_param_4 _macro_uppack_param_3 _macro_uppack_param(4)
#define _macro_uppack_param_5 _macro_uppack_param_4 _macro_uppack_param(5)
#define _macro_uppack_param_6 _macro_uppack_param_5 _macro_uppack_param(6)
#define _macro_uppack_param_7 _macro_uppack_param_6 _macro_uppack_param(7)
#define _macro_uppack_param_8 _macro_uppack_param_7 _macro_uppack_param(8)
#define _macro_uppack_param_9 _macro_uppack_param_8 _macro_uppack_param(9)

#define _DECLARE_CALLABLE_CLASS(n) \
    template <class Ret, __ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
    class _ftrpcCallable##n : public ftrpc_callable \
    { \
    public: \
        virtual int operator () (_ftBufArray &retBufArray, const char *szFuncName, const _ftBufArray &parameters) \
        { \
            int nRet = -1; \
            do \
            { \
                if(parameters.size() < n) \
                { \
                    nRet = -1; \
                    break; \
                } \
                _macro_uppack_param_##n \
                Ret ret; \
                ret = mfn(__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, p)); \
                ftnetbuffer retBuf; \
                retBuf = ftrpc_pack(ret); \
                retBufArray.push_back(retBuf); \
                nRet = 0; \
            }while(false); \
            return nRet; \
        } \
    public: \
        Ret (*mfn) (__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)); \
    };

//_DECLARE_CALLABLE_CLASS(1)
_DECLARE_CALLABLE_CLASS(2)
_DECLARE_CALLABLE_CLASS(3)
_DECLARE_CALLABLE_CLASS(4)
_DECLARE_CALLABLE_CLASS(5)
_DECLARE_CALLABLE_CLASS(6)
_DECLARE_CALLABLE_CLASS(7)
_DECLARE_CALLABLE_CLASS(8)
_DECLARE_CALLABLE_CLASS(9)

#endif /* defined(__TestAllUseful__ftrpc_callable__) */
