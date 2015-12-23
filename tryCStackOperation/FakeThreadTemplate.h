//
//  FakeThreadTemplate.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/26.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//  封装模板函数，以便可以方便地对作意函数进行伪线程执行。

#ifndef __TestAllUseful__FakeThreadTemplate__
#define __TestAllUseful__FakeThreadTemplate__

#include "FakeThreadWrap.h"

#include "ftduration.h"

#include <string>
#include <map>

class FTNET_API _FunctorForFakeThreadThreadProcParam
{
public:
    virtual ~_FunctorForFakeThreadThreadProcParam();
    virtual void operator () () const = 0;
};

FTNET_API void SetNextTimerNameForDebug(const std::string &name);

class FTNET_API _FunctorForFakeThreadTimerProcParam
{
public:
    _FunctorForFakeThreadTimerProcParam();
    virtual ~_FunctorForFakeThreadTimerProcParam();
    virtual void operator () () const = 0;
    double whenExpire(); // 何时超时

    ftduration md;
    double mdTimeoutSeconds; // 超时的毫秒数
    std::string mstrTimerName;
};
typedef std::multimap<double, _FunctorForFakeThreadTimerProcParam *> FtTimerMap;

FTNET_API void SetNextTimerNameForDebug(const std::string &name);

FTNET_API FtTimerMap & fttimer_get_all();

// 这个函数用于模板函数中，不在外部使用
FTNET_API void _FakeThreadProcForTemplate ( void * param);
// 添加一个定时器对象，这个函数不在外部调用。
FTNET_API void _FakeThreadAddTimerFunc(_FunctorForFakeThreadTimerProcParam * pParam);
// 执行定时器, 供外部调用
FTNET_API void fttimer_do();
FTNET_API unsigned int fttimer_count(); // 返回当前定时器个数
FTNET_API double fttimer_next_duration(); // 到下一个定时器到期的时间间隔，单位：秒
FTNET_API void fttimer_free_all(); // 释放所有未超时的定时器
//
template <class T1>
class _FTParamPtr1 : public _FunctorForFakeThreadThreadProcParam
{
public:
    void (*f) ( T1 );
    T1 *p1;
    virtual void operator () () const
    {
        f(*p1);
    }
};
template <class T1>
class _FTParamForTimer1 : public _FunctorForFakeThreadTimerProcParam
{
public:
    void (*f) ( T1 );
    T1 p1;
    virtual void operator () () const
    {
        f(p1);
    }
};


#define _FK_CLASS1 class
#define _FK_CLASS2 class
#define _FK_CLASS3 class
#define _FK_CLASS4 class
#define _FK_CLASS5 class
#define _FK_CLASS6 class
#define _FK_CLASS7 class
#define _FK_CLASS8 class
#define _FK_CLASS9 class

#define _FK_ONLY_EMPTY
#define __ft_FK_EMPTY1 
#define __ft_FK_EMPTY2 
#define __ft_FK_EMPTY3 
#define __ft_FK_EMPTY4 
#define __ft_FK_EMPTY5 
#define __ft_FK_EMPTY6 
#define __ft_FK_EMPTY7 
#define __ft_FK_EMPTY8 
#define __ft_FK_EMPTY9 

#define __ft_COMMON_DECLARE_LIST1(a, b) a##1 b##1
#define __ft_COMMON_DECLARE_LIST2(a, b) __ft_COMMON_DECLARE_LIST1(a, b), a##2 b##2
#define __ft_COMMON_DECLARE_LIST3(a, b) __ft_COMMON_DECLARE_LIST2(a, b), a##3 b##3
#define __ft_COMMON_DECLARE_LIST4(a, b) __ft_COMMON_DECLARE_LIST3(a, b), a##4 b##4
#define __ft_COMMON_DECLARE_LIST5(a, b) __ft_COMMON_DECLARE_LIST4(a, b), a##5 b##5
#define __ft_COMMON_DECLARE_LIST6(a, b) __ft_COMMON_DECLARE_LIST5(a, b), a##6 b##6
#define __ft_COMMON_DECLARE_LIST7(a, b) __ft_COMMON_DECLARE_LIST6(a, b), a##7 b##7
#define __ft_COMMON_DECLARE_LIST8(a, b) __ft_COMMON_DECLARE_LIST7(a, b), a##8 b##8
#define __ft_COMMON_DECLARE_LIST9(a, b) __ft_COMMON_DECLARE_LIST8(a, b), a##9 b##9

#define __ft_TIMES_1(macro_with_n) macro_with_n(1)
#define __ft_TIMES_2(macro_with_n) __ft_TIMES_1(macro_with_n) macro_with_n(2)
#define __ft_TIMES_3(macro_with_n) __ft_TIMES_2(macro_with_n) macro_with_n(3)
#define __ft_TIMES_4(macro_with_n) __ft_TIMES_3(macro_with_n) macro_with_n(4)
#define __ft_TIMES_5(macro_with_n) __ft_TIMES_4(macro_with_n) macro_with_n(5)
#define __ft_TIMES_6(macro_with_n) __ft_TIMES_5(macro_with_n) macro_with_n(6)
#define __ft_TIMES_7(macro_with_n) __ft_TIMES_6(macro_with_n) macro_with_n(7)
#define __ft_TIMES_8(macro_with_n) __ft_TIMES_7(macro_with_n) macro_with_n(8)
#define __ft_TIMES_9(macro_with_n) __ft_TIMES_8(macro_with_n) macro_with_n(9)
// #define _DECLARE_TYPE_LIST1 class T1
// #define _DECLARE_TYPE_LIST2 _DECLARE_TYPE_LIST1, class T2
// #define _DECLARE_TYPE_LIST3 _DECLARE_TYPE_LIST2, class T3
// #define _DECLARE_TYPE_LIST4 _DECLARE_TYPE_LIST3, class T4

#define __ft_DECLARE_PARAM1(prefix) T1 prefix p1
#define __ft_DECLARE_PARAM2(prefix) __ft_DECLARE_PARAM1(prefix); T2 prefix p2
#define __ft_DECLARE_PARAM3(prefix) __ft_DECLARE_PARAM2(prefix); T3 prefix p3
#define __ft_DECLARE_PARAM4(prefix) __ft_DECLARE_PARAM3(prefix); T4 prefix p4
#define __ft_DECLARE_PARAM5(prefix) __ft_DECLARE_PARAM4(prefix); T5 prefix p5
#define __ft_DECLARE_PARAM6(prefix) __ft_DECLARE_PARAM5(prefix); T6 prefix p6
#define __ft_DECLARE_PARAM7(prefix) __ft_DECLARE_PARAM6(prefix); T7 prefix p7
#define __ft_DECLARE_PARAM8(prefix) __ft_DECLARE_PARAM7(prefix); T8 prefix p8
#define __ft_DECLARE_PARAM9(prefix) __ft_DECLARE_PARAM8(prefix); T9 prefix p9

// #define __ft_DECLARE_PARAM_LIST1 *p1
// #define __ft_DECLARE_PARAM_LIST2 __ft_DECLARE_PARAM_LIST1, *p2
// #define __ft_DECLARE_PARAM_LIST3 __ft_DECLARE_PARAM_LIST2, *p3
// #define __ft_DECLARE_PARAM_LIST4 __ft_DECLARE_PARAM_LIST3, *p4

#define __ft_DECLARE_FTPARAM(n) \
	template <__ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
	class _FTParamPtr##n : public _FunctorForFakeThreadThreadProcParam \
	{ \
	public: \
	    void (*f) ( __ft_COMMON_DECLARE_LIST##n(T, p) ); \
	    __ft_DECLARE_PARAM##n(*); \
	    virtual void operator () () const \
	    { \
	        f(__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, *p)); \
	    } \
	}; \
	template <__ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
	class _FTParamForTimer##n : public _FunctorForFakeThreadTimerProcParam \
	{ \
	public: \
	    void (*f) ( __ft_COMMON_DECLARE_LIST##n(T, p) ); \
	    __ft_DECLARE_PARAM##n(_FK_ONLY_EMPTY); \
	    virtual void operator () () const \
	    { \
	        f(__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, p)); \
	    } \
	};
#define __ft_DECLARE_FT_RUN_FUNC(n) \
	template <__ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
	int RunInFakeThreadT(void (*FakeThreadProc) ( __ft_COMMON_DECLARE_LIST##n(T, p) ), __ft_COMMON_DECLARE_LIST##n(T, p)) \
	{ \
	    int nRet = 0; \
	    _FTParamPtr##n<__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)> tparam; \
	    tparam.f = FakeThreadProc; \
        __ft_COMMON_DECLARE_LIST##n(tparam.p,  = &p); \
	    nRet = RunFuncInFakeThread(_FakeThreadProcForTemplate, &tparam); \
	    return nRet; \
	} \
	template <__ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
	int RunInFakeThreadT(double dTimeoutSeconds, void (*FakeThreadProc) ( __ft_COMMON_DECLARE_LIST##n(T, p) ), __ft_COMMON_DECLARE_LIST##n(T, p)) \
	{ \
	    int nRet = 0; \
	    _FTParamForTimer##n<__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)> *pParamForTimer = new _FTParamForTimer##n<__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)>; \
        pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds; \
        pParamForTimer->f = FakeThreadProc; \
        __ft_COMMON_DECLARE_LIST##n(pParamForTimer->p,  = p); \
        _FakeThreadAddTimerFunc(pParamForTimer); \
	    return nRet; \
	}


/**/
template <class T1>
int RunInFakeThreadT(void (*FakeThreadProc) ( T1 ), T1 p1)
{
    int nRet = 0;
    _FTParamPtr1<T1> tparam;
    tparam.f = FakeThreadProc;
    tparam.p1 = &p1;
    nRet = RunFuncInFakeThread(_FakeThreadProcForTemplate, &tparam);
    return nRet;
}
template <class T1>
int RunInFakeThreadT(double dTimeoutSeconds, void (*FakeThreadProc) ( T1 ), T1 p1)
{
    int nRet = 0;
    _FTParamForTimer1<T1> *pParamForTimer = new _FTParamForTimer1<T1>;
    pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds;
    pParamForTimer->f = FakeThreadProc;
    pParamForTimer->p1 = p1;
//    nRet = RunFuncInFakeThread(_FakeThreadProcForTemplate, &tparam);
    _FakeThreadAddTimerFunc(pParamForTimer);
    return nRet;
}


// 下面封装yield和resume模板函数
/**/
template <class T1>
class _FtResumeParamT1 : public FakeThreadResumeParam
{
public:
    virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT1";
        return gszType;
    }
    T1 *p1;
};


#define _DECLARE_RESUME_PARAM(n) \
	template <__ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
	class _FtResumeParamT##n : public FakeThreadResumeParam \
	{ \
	public: \
	    virtual const char *FtsParamType() const \
	    { \
	        static const char gszType[] = "_FtResumeParamT##n"; \
	        return gszType; \
	    } \
	    __ft_DECLARE_PARAM##n(*); \
	};


/**/
template <class T1>
class ResumeFakeThreadObjT1
{
public:
    int operator () (T1 p1)
    {
        _FtResumeParamT1<T1> rp;
        rp.p1 = &p1;
        return mResumeObj(&rp);
    }
    bool operator ! () const
    {
        return !mResumeObj;
    }
    FakeThreadResumeObject mResumeObj;
};


#define _DECLARE_RESUME_OBJ(n) \
	template <__ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
	class ResumeFakeThreadObjT##n \
	{ \
	public: \
	    int operator () (__ft_COMMON_DECLARE_LIST##n(T, p)) \
	    { \
	        _FtResumeParamT##n<__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)> rp; \
            __ft_COMMON_DECLARE_LIST##n(rp.p,  = &p); \
	        return mResumeObj(&rp); \
	    } \
        bool operator ! () const \
        { \
            return !mResumeObj; \
        } \
	    FakeThreadResumeObject mResumeObj; \
	};



/**/
template <class T1>
int YieldFromFakeThread(ResumeFakeThreadObjT1<T1> &resumeObj, T1 *pOut1)
{
    int nRet = 0;
    resumeObj.mResumeObj = GetResumeObjectForFakeThread();
    FakeThreadResumeParam *pRet = NULL;
    nRet = YieldFromFakeThread(&pRet);
    if(!pRet)
    {
        nRet = -1;
    }
    if(!nRet)
    {
        _FtResumeParamT1<T1> * p = (_FtResumeParamT1<T1> *) pRet;
        if(pOut1)
        {
            *pOut1 = *p->p1;
        }
    }
    return nRet;
}


#define _pOutSentence(n) if(pOut##n) { *pOut##n = *p->p##n; }
#define _pOutSentence_1 _pOutSentence(1)
#define _pOutSentence_2 _pOutSentence_1 _pOutSentence(2)
#define _pOutSentence_3 _pOutSentence_2 _pOutSentence(3)
#define _pOutSentence_4 _pOutSentence_3 _pOutSentence(4)
#define _pOutSentence_5 _pOutSentence_4 _pOutSentence(5)
#define _pOutSentence_6 _pOutSentence_5 _pOutSentence(6)
#define _pOutSentence_7 _pOutSentence_6 _pOutSentence(7)
#define _pOutSentence_8 _pOutSentence_7 _pOutSentence(8)
#define _pOutSentence_9 _pOutSentence_8 _pOutSentence(9)

#define _DECLARE_YIELD_FUNC(n) \
	template <__ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
	int YieldFromFakeThread(ResumeFakeThreadObjT##n<__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)> &resumeObj, __ft_COMMON_DECLARE_LIST##n(T, *pOut)) \
	{ \
	    int nRet = 0; \
	    resumeObj.mResumeObj = GetResumeObjectForFakeThread(); \
	    FakeThreadResumeParam *pRet = NULL; \
	    nRet = YieldFromFakeThread(&pRet); \
	    if(!pRet) \
	    { \
	        nRet = -1; \
	    } \
	    if(!nRet) \
	    { \
	        _FtResumeParamT##n<__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)> * p = (_FtResumeParamT##n<__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)> *) pRet; \
	        _pOutSentence_##n \
	    } \
	    return nRet; \
	}

// =====================================
#if 0
//__ft_DECLARE_FTPARAM(1)
__ft_DECLARE_FTPARAM(2)
__ft_DECLARE_FTPARAM(3)
__ft_DECLARE_FTPARAM(4)
__ft_DECLARE_FTPARAM(5)
__ft_DECLARE_FTPARAM(6)
__ft_DECLARE_FTPARAM(7)
__ft_DECLARE_FTPARAM(8)
__ft_DECLARE_FTPARAM(9)

//__ft_DECLARE_FT_RUN_FUNC(1)
__ft_DECLARE_FT_RUN_FUNC(2)
__ft_DECLARE_FT_RUN_FUNC(3)
__ft_DECLARE_FT_RUN_FUNC(4)
__ft_DECLARE_FT_RUN_FUNC(5)
__ft_DECLARE_FT_RUN_FUNC(6)
__ft_DECLARE_FT_RUN_FUNC(7)
__ft_DECLARE_FT_RUN_FUNC(8)
__ft_DECLARE_FT_RUN_FUNC(9)

//_DECLARE_RESUME_PARAM(1)
_DECLARE_RESUME_PARAM(2)
_DECLARE_RESUME_PARAM(3)
_DECLARE_RESUME_PARAM(4)
_DECLARE_RESUME_PARAM(5)
_DECLARE_RESUME_PARAM(6)
_DECLARE_RESUME_PARAM(7)
_DECLARE_RESUME_PARAM(8)
_DECLARE_RESUME_PARAM(9)

//_DECLARE_RESUME_OBJ(1)
_DECLARE_RESUME_OBJ(2)
_DECLARE_RESUME_OBJ(3)
_DECLARE_RESUME_OBJ(4)
_DECLARE_RESUME_OBJ(5)
_DECLARE_RESUME_OBJ(6)
_DECLARE_RESUME_OBJ(7)
_DECLARE_RESUME_OBJ(8)
_DECLARE_RESUME_OBJ(9)

//_DECLARE_YIELD_FUNC(1)
_DECLARE_YIELD_FUNC(2)
_DECLARE_YIELD_FUNC(3)
_DECLARE_YIELD_FUNC(4)
_DECLARE_YIELD_FUNC(5)
_DECLARE_YIELD_FUNC(6)
_DECLARE_YIELD_FUNC(7)
_DECLARE_YIELD_FUNC(8)
_DECLARE_YIELD_FUNC(9)

//
#else
#include "FakeThreadTemplate_i.h"
#endif

#endif /* defined(__TestAllUseful__FakeThreadTemplate__) */
