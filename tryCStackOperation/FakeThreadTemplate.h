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

class _FunctorForFakeThreadThreadProcParam
{
public:
    virtual ~_FunctorForFakeThreadThreadProcParam();
    virtual void operator () () const = 0;
};

class _FunctorForFakeThreadTimerProcParam
{
public:
    virtual ~_FunctorForFakeThreadTimerProcParam();
    virtual void operator () () const = 0;
    double whenExpire(); // 何时超时

    ftduration md;
    double mdTimeoutSeconds; // 超时的毫秒数
};

// 这个函数用于模板函数中，不在外部使用
void _FakeThreadProcForTemplate ( void * param);
// 添加一个定时器对象，这个函数不在外部调用。
void _FakeThreadAddTimerFunc(_FunctorForFakeThreadTimerProcParam * pParam);
// 执行定时器, 供外部调用
void fttimer_do();
unsigned int fttimer_count(); // 返回当前定时器个数
void fttimer_free_all(); // 释放所有未超时的定时器
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

#define __ft_DECLARE_ALL(n) \
    __ft_DECLARE_FTPARAM(n) \
    __ft_DECLARE_FT_RUN_FUNC(n) \
    _DECLARE_RESUME_PARAM(n) \
    _DECLARE_RESUME_OBJ(n) \
    _DECLARE_YIELD_FUNC(n)

//__ft_DECLARE_ALL(1)
//__ft_DECLARE_ALL(2)
//__ft_DECLARE_ALL(3)
//__ft_DECLARE_ALL(4)
//__ft_DECLARE_ALL(5)
//__ft_DECLARE_ALL(6)
//__ft_DECLARE_ALL(7)
//__ft_DECLARE_ALL(8)
//__ft_DECLARE_ALL(9)

/*
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
*/

template <class T1, class T2>
class _FTParamPtr2 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2 ); T1 * p1; T2 * p2; virtual void operator ()() const
    {
        f( *p1, *p2 );
    }
};
template <class T1, class T2>
class _FTParamForTimer2 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2 ); T1 p1; T2 p2; virtual void operator ()() const
    {
        f( p1, p2 );
    }
};
template <class T1, class T2>
int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2 ), T1 p1, T2 p2 )
{
    int nRet = 0; _FTParamPtr2< T1, T2> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1;
    tparam.p2 = &p2;
    nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam );
    return(nRet);
}


template <class T1, class T2>
int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2 ), T1 p1, T2 p2 )
{
    int nRet = 0; _FTParamForTimer2< T1, T2> *pParamForTimer = new _FTParamForTimer2< T1, T2>;
    pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds;
    pParamForTimer->f = FakeThreadProc;
    pParamForTimer->p1 = p1;
    pParamForTimer->p2 = p2;
    _FakeThreadAddTimerFunc( pParamForTimer );
    return(nRet);
}


template <class T1, class T2>
class _FtResumeParamT2 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n";
        return(gszType);
    }
    
    
    T1 * p1; T2 * p2; };
template <class T1, class T2>
class ResumeFakeThreadObjT2 { public: int operator ()( T1 p1, T2 p2 )
    {
        _FtResumeParamT2< T1, T2> rp;
        rp.p1 = &p1, rp.p2 = &p2;
        return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2>
int YieldFromFakeThread( ResumeFakeThreadObjT2< T1, T2> &resumeObj, T1 *pOut1, T2 *pOut2 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = __null; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
    {
        nRet = -1;
    }
    if ( !nRet )
    {
        _FtResumeParamT2< T1, T2> * p = (_FtResumeParamT2< T1, T2> *)pRet; if ( pOut1 )
        {
            *pOut1 = *p->p1;
        }
        if ( pOut2 )
        {
            *pOut2 = *p->p2;
        }
    }
    return(nRet);
}


template <class T1, class T2, class T3>
class _FTParamPtr3 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3 ); T1 * p1; T2 * p2; T3 * p3; virtual void operator ()() const
    {
        f( *p1, *p2, *p3 );
    }
};
template <class T1, class T2, class T3>
class _FTParamForTimer3 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3 ); T1 p1; T2 p2; T3 p3; virtual void operator ()() const
    {
        f( p1, p2, p3 );
    }
};
template <class T1, class T2, class T3>
int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3 ), T1 p1, T2 p2, T3 p3 )
{
    int nRet = 0; _FTParamPtr3< T1, T2, T3> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1;
    tparam.p2 = &p2;
    tparam.p3 = &p3;
    nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam );
    return(nRet);
}


template <class T1, class T2, class T3>
int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3 ), T1 p1, T2 p2, T3 p3 )
{
    int nRet = 0; _FTParamForTimer3< T1, T2, T3> *pParamForTimer = new _FTParamForTimer3< T1, T2, T3>;
    pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds;
    pParamForTimer->f = FakeThreadProc;
    pParamForTimer->p1 = p1;
    pParamForTimer->p2 = p2;
    pParamForTimer->p3 = p3;
    _FakeThreadAddTimerFunc( pParamForTimer );
    return(nRet);
}


template <class T1, class T2, class T3>
class _FtResumeParamT3 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n";
        return(gszType);
    }
    
    
    T1 * p1; T2 * p2; T3 * p3; };
template <class T1, class T2, class T3>
class ResumeFakeThreadObjT3 { public: int operator ()( T1 p1, T2 p2, T3 p3 )
    {
        _FtResumeParamT3< T1, T2, T3> rp;
        rp.p1 = &p1, rp.p2 = &p2, rp.p3 = &p3;
        return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2, class T3>
int YieldFromFakeThread( ResumeFakeThreadObjT3< T1, T2, T3> &resumeObj, T1 *pOut1, T2 *pOut2, T3 *pOut3 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = __null; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
    {
        nRet = -1;
    }
    if ( !nRet )
    {
        _FtResumeParamT3< T1, T2, T3> * p = (_FtResumeParamT3< T1, T2, T3> *)pRet; if ( pOut1 )
        {
            *pOut1 = *p->p1;
        }
        if ( pOut2 )
        {
            *pOut2 = *p->p2;
        }
        if ( pOut3 )
        {
            *pOut3 = *p->p3;
        }
    }
    return(nRet);
}


template <class T1, class T2, class T3, class T4>
class _FTParamPtr4 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4 ); T1 * p1; T2 * p2; T3 * p3; T4 * p4; virtual void operator ()() const
    {
        f( *p1, *p2, *p3, *p4 );
    }
};
template <class T1, class T2, class T3, class T4>
class _FTParamForTimer4 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4 ); T1 p1; T2 p2; T3 p3; T4 p4; virtual void operator ()() const
    {
        f( p1, p2, p3, p4 );
    }
};
template <class T1, class T2, class T3, class T4>
int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4 ), T1 p1, T2 p2, T3 p3, T4 p4 )
{
    int nRet = 0; _FTParamPtr4< T1, T2, T3, T4> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1;
    tparam.p2 = &p2;
    tparam.p3 = &p3;
    tparam.p4 = &p4;
    nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam );
    return(nRet);
}


template <class T1, class T2, class T3, class T4>
int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4 ), T1 p1, T2 p2, T3 p3, T4 p4 )
{
    int nRet = 0; _FTParamForTimer4< T1, T2, T3, T4> *pParamForTimer = new _FTParamForTimer4< T1, T2, T3, T4>;
    pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds;
    pParamForTimer->f = FakeThreadProc;
    pParamForTimer->p1 = p1;
    pParamForTimer->p2 = p2;
    pParamForTimer->p3 = p3;
    pParamForTimer->p4 = p4;
    _FakeThreadAddTimerFunc( pParamForTimer );
    return(nRet);
}


template <class T1, class T2, class T3, class T4>
class _FtResumeParamT4 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n";
        return(gszType);
    }
    
    
    T1 * p1; T2 * p2; T3 * p3; T4 * p4; };
template <class T1, class T2, class T3, class T4>
class ResumeFakeThreadObjT4 { public: int operator ()( T1 p1, T2 p2, T3 p3, T4 p4 )
    {
        _FtResumeParamT4< T1, T2, T3, T4> rp;
        rp.p1 = &p1, rp.p2 = &p2, rp.p3 = &p3, rp.p4 = &p4;
        return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2, class T3, class T4>
int YieldFromFakeThread( ResumeFakeThreadObjT4< T1, T2, T3, T4> &resumeObj, T1 *pOut1, T2 *pOut2, T3 *pOut3, T4 *pOut4 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = __null; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
    {
        nRet = -1;
    }
    if ( !nRet )
    {
        _FtResumeParamT4< T1, T2, T3, T4> * p = (_FtResumeParamT4< T1, T2, T3, T4> *)pRet; if ( pOut1 )
        {
            *pOut1 = *p->p1;
        }
        if ( pOut2 )
        {
            *pOut2 = *p->p2;
        }
        if ( pOut3 )
        {
            *pOut3 = *p->p3;
        }
        if ( pOut4 )
        {
            *pOut4 = *p->p4;
        }
    }
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5>
class _FTParamPtr5 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 ); T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; virtual void operator ()() const
    {
        f( *p1, *p2, *p3, *p4, *p5 );
    }
};
template <class T1, class T2, class T3, class T4, class T5>
class _FTParamForTimer5 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 ); T1 p1; T2 p2; T3 p3; T4 p4; T5 p5; virtual void operator ()() const
    {
        f( p1, p2, p3, p4, p5 );
    }
};
template <class T1, class T2, class T3, class T4, class T5>
int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
{
    int nRet = 0; _FTParamPtr5< T1, T2, T3, T4, T5> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1;
    tparam.p2 = &p2;
    tparam.p3 = &p3;
    tparam.p4 = &p4;
    tparam.p5 = &p5;
    nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam );
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5>
int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
{
    int nRet = 0; _FTParamForTimer5< T1, T2, T3, T4, T5> *pParamForTimer = new _FTParamForTimer5< T1, T2, T3, T4, T5>;
    pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds;
    pParamForTimer->f = FakeThreadProc;
    pParamForTimer->p1 = p1;
    pParamForTimer->p2 = p2;
    pParamForTimer->p3 = p3;
    pParamForTimer->p4 = p4;
    pParamForTimer->p5 = p5;
    _FakeThreadAddTimerFunc( pParamForTimer );
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5>
class _FtResumeParamT5 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n";
        return(gszType);
    }
    
    
    T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; };
template <class T1, class T2, class T3, class T4, class T5>
class ResumeFakeThreadObjT5 { public: int operator ()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
    {
        _FtResumeParamT5< T1, T2, T3, T4, T5> rp;
        rp.p1 = &p1, rp.p2 = &p2, rp.p3 = &p3, rp.p4 = &p4, rp.p5 = &p5;
        return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2, class T3, class T4, class T5>
int YieldFromFakeThread( ResumeFakeThreadObjT5< T1, T2, T3, T4, T5> &resumeObj, T1 *pOut1, T2 *pOut2, T3 *pOut3, T4 *pOut4, T5 *pOut5 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = __null; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
    {
        nRet = -1;
    }
    if ( !nRet )
    {
        _FtResumeParamT5< T1, T2, T3, T4, T5> * p = (_FtResumeParamT5< T1, T2, T3, T4, T5> *)pRet; if ( pOut1 )
        {
            *pOut1 = *p->p1;
        }
        if ( pOut2 )
        {
            *pOut2 = *p->p2;
        }
        if ( pOut3 )
        {
            *pOut3 = *p->p3;
        }
        if ( pOut4 )
        {
            *pOut4 = *p->p4;
        }
        if ( pOut5 )
        {
            *pOut5 = *p->p5;
        }
    }
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6>
class _FTParamPtr6 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 ); T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; virtual void operator ()() const
    {
        f( *p1, *p2, *p3, *p4, *p5, *p6 );
    }
};
template <class T1, class T2, class T3, class T4, class T5, class T6>
class _FTParamForTimer6 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 ); T1 p1; T2 p2; T3 p3; T4 p4; T5 p5; T6 p6; virtual void operator ()() const
    {
        f( p1, p2, p3, p4, p5, p6 );
    }
};
template <class T1, class T2, class T3, class T4, class T5, class T6>
int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
{
    int nRet = 0; _FTParamPtr6< T1, T2, T3, T4, T5, T6> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1;
    tparam.p2 = &p2;
    tparam.p3 = &p3;
    tparam.p4 = &p4;
    tparam.p5 = &p5;
    tparam.p6 = &p6;
    nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam );
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6>
int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
{
    int nRet = 0; _FTParamForTimer6< T1, T2, T3, T4, T5, T6> *pParamForTimer = new _FTParamForTimer6< T1, T2, T3, T4, T5, T6>;
    pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds;
    pParamForTimer->f = FakeThreadProc;
    pParamForTimer->p1 = p1;
    pParamForTimer->p2 = p2;
    pParamForTimer->p3 = p3;
    pParamForTimer->p4 = p4;
    pParamForTimer->p5 = p5;
    pParamForTimer->p6 = p6;
    _FakeThreadAddTimerFunc( pParamForTimer );
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6>
class _FtResumeParamT6 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n";
        return(gszType);
    }
    
    
    T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; };
template <class T1, class T2, class T3, class T4, class T5, class T6>
class ResumeFakeThreadObjT6 { public: int operator ()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
    {
        _FtResumeParamT6< T1, T2, T3, T4, T5, T6> rp;
        rp.p1 = &p1, rp.p2 = &p2, rp.p3 = &p3, rp.p4 = &p4, rp.p5 = &p5, rp.p6 = &p6;
        return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2, class T3, class T4, class T5, class T6>
int YieldFromFakeThread( ResumeFakeThreadObjT6< T1, T2, T3, T4, T5, T6> &resumeObj, T1 *pOut1, T2 *pOut2, T3 *pOut3, T4 *pOut4, T5 *pOut5, T6 *pOut6 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = __null; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
    {
        nRet = -1;
    }
    if ( !nRet )
    {
        _FtResumeParamT6< T1, T2, T3, T4, T5, T6> * p = (_FtResumeParamT6< T1, T2, T3, T4, T5, T6> *)pRet; if ( pOut1 )
        {
            *pOut1 = *p->p1;
        }
        if ( pOut2 )
        {
            *pOut2 = *p->p2;
        }
        if ( pOut3 )
        {
            *pOut3 = *p->p3;
        }
        if ( pOut4 )
        {
            *pOut4 = *p->p4;
        }
        if ( pOut5 )
        {
            *pOut5 = *p->p5;
        }
        if ( pOut6 )
        {
            *pOut6 = *p->p6;
        }
    }
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
class _FTParamPtr7 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 ); T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; T7 * p7; virtual void operator ()() const
    {
        f( *p1, *p2, *p3, *p4, *p5, *p6, *p7 );
    }
};
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
class _FTParamForTimer7 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 ); T1 p1; T2 p2; T3 p3; T4 p4; T5 p5; T6 p6; T7 p7; virtual void operator ()() const
    {
        f( p1, p2, p3, p4, p5, p6, p7 );
    }
};
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
{
    int nRet = 0; _FTParamPtr7< T1, T2, T3, T4, T5, T6, T7> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1;
    tparam.p2 = &p2;
    tparam.p3 = &p3;
    tparam.p4 = &p4;
    tparam.p5 = &p5;
    tparam.p6 = &p6;
    tparam.p7 = &p7;
    nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam );
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
{
    int nRet = 0; _FTParamForTimer7< T1, T2, T3, T4, T5, T6, T7> *pParamForTimer = new _FTParamForTimer7< T1, T2, T3, T4, T5, T6, T7>;
    pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds;
    pParamForTimer->f = FakeThreadProc;
    pParamForTimer->p1 = p1;
    pParamForTimer->p2 = p2;
    pParamForTimer->p3 = p3;
    pParamForTimer->p4 = p4;
    pParamForTimer->p5 = p5;
    pParamForTimer->p6 = p6;
    pParamForTimer->p7 = p7;
    _FakeThreadAddTimerFunc( pParamForTimer );
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
class _FtResumeParamT7 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n";
        return(gszType);
    }
    
    
    T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; T7 * p7; };
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
class ResumeFakeThreadObjT7 { public: int operator ()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
    {
        _FtResumeParamT7< T1, T2, T3, T4, T5, T6, T7> rp;
        rp.p1 = &p1, rp.p2 = &p2, rp.p3 = &p3, rp.p4 = &p4, rp.p5 = &p5, rp.p6 = &p6, rp.p7 = &p7;
        return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
int YieldFromFakeThread( ResumeFakeThreadObjT7< T1, T2, T3, T4, T5, T6, T7> &resumeObj, T1 *pOut1, T2 *pOut2, T3 *pOut3, T4 *pOut4, T5 *pOut5, T6 *pOut6, T7 *pOut7 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = __null; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
    {
        nRet = -1;
    }
    if ( !nRet )
    {
        _FtResumeParamT7< T1, T2, T3, T4, T5, T6, T7> * p = (_FtResumeParamT7< T1, T2, T3, T4, T5, T6, T7> *)pRet; if ( pOut1 )
        {
            *pOut1 = *p->p1;
        }
        if ( pOut2 )
        {
            *pOut2 = *p->p2;
        }
        if ( pOut3 )
        {
            *pOut3 = *p->p3;
        }
        if ( pOut4 )
        {
            *pOut4 = *p->p4;
        }
        if ( pOut5 )
        {
            *pOut5 = *p->p5;
        }
        if ( pOut6 )
        {
            *pOut6 = *p->p6;
        }
        if ( pOut7 )
        {
            *pOut7 = *p->p7;
        }
    }
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
class _FTParamPtr8 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 ); T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; T7 * p7; T8 * p8; virtual void operator ()() const
    {
        f( *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8 );
    }
};
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
class _FTParamForTimer8 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 ); T1 p1; T2 p2; T3 p3; T4 p4; T5 p5; T6 p6; T7 p7; T8 p8; virtual void operator ()() const
    {
        f( p1, p2, p3, p4, p5, p6, p7, p8 );
    }
};
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
{
    int nRet = 0; _FTParamPtr8< T1, T2, T3, T4, T5, T6, T7, T8> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1;
    tparam.p2 = &p2;
    tparam.p3 = &p3;
    tparam.p4 = &p4;
    tparam.p5 = &p5;
    tparam.p6 = &p6;
    tparam.p7 = &p7;
    tparam.p8 = &p8;
    nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam );
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
{
    int nRet = 0; _FTParamForTimer8< T1, T2, T3, T4, T5, T6, T7, T8> *pParamForTimer = new _FTParamForTimer8< T1, T2, T3, T4, T5, T6, T7, T8>;
    pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds;
    pParamForTimer->f = FakeThreadProc;
    pParamForTimer->p1 = p1;
    pParamForTimer->p2 = p2;
    pParamForTimer->p3 = p3;
    pParamForTimer->p4 = p4;
    pParamForTimer->p5 = p5;
    pParamForTimer->p6 = p6;
    pParamForTimer->p7 = p7;
    pParamForTimer->p8 = p8;
    _FakeThreadAddTimerFunc( pParamForTimer );
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
class _FtResumeParamT8 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n";
        return(gszType);
    }
    
    
    T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; T7 * p7; T8 * p8; };
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
class ResumeFakeThreadObjT8 { public: int operator ()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
    {
        _FtResumeParamT8< T1, T2, T3, T4, T5, T6, T7, T8> rp;
        rp.p1 = &p1, rp.p2 = &p2, rp.p3 = &p3, rp.p4 = &p4, rp.p5 = &p5, rp.p6 = &p6, rp.p7 = &p7, rp.p8 = &p8;
        return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
int YieldFromFakeThread( ResumeFakeThreadObjT8< T1, T2, T3, T4, T5, T6, T7, T8> &resumeObj, T1 *pOut1, T2 *pOut2, T3 *pOut3, T4 *pOut4, T5 *pOut5, T6 *pOut6, T7 *pOut7, T8 *pOut8 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = __null; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
    {
        nRet = -1;
    }
    if ( !nRet )
    {
        _FtResumeParamT8< T1, T2, T3, T4, T5, T6, T7, T8> * p = (_FtResumeParamT8< T1, T2, T3, T4, T5, T6, T7, T8> *)pRet; if ( pOut1 )
        {
            *pOut1 = *p->p1;
        }
        if ( pOut2 )
        {
            *pOut2 = *p->p2;
        }
        if ( pOut3 )
        {
            *pOut3 = *p->p3;
        }
        if ( pOut4 )
        {
            *pOut4 = *p->p4;
        }
        if ( pOut5 )
        {
            *pOut5 = *p->p5;
        }
        if ( pOut6 )
        {
            *pOut6 = *p->p6;
        }
        if ( pOut7 )
        {
            *pOut7 = *p->p7;
        }
        if ( pOut8 )
        {
            *pOut8 = *p->p8;
        }
    }
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
class _FTParamPtr9 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 ); T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; T7 * p7; T8 * p8; T9 * p9; virtual void operator ()() const
    {
        f( *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9 );
    }
};
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
class _FTParamForTimer9 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 ); T1 p1; T2 p2; T3 p3; T4 p4; T5 p5; T6 p6; T7 p7; T8 p8; T9 p9; virtual void operator ()() const
    {
        f( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
    }
};
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
{
    int nRet = 0; _FTParamPtr9< T1, T2, T3, T4, T5, T6, T7, T8, T9> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1;
    tparam.p2 = &p2;
    tparam.p3 = &p3;
    tparam.p4 = &p4;
    tparam.p5 = &p5;
    tparam.p6 = &p6;
    tparam.p7 = &p7;
    tparam.p8 = &p8;
    tparam.p9 = &p9;
    nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam );
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
{
    int nRet = 0; _FTParamForTimer9< T1, T2, T3, T4, T5, T6, T7, T8, T9> *pParamForTimer = new _FTParamForTimer9< T1, T2, T3, T4, T5, T6, T7, T8, T9>;
    pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds;
    pParamForTimer->f = FakeThreadProc;
    pParamForTimer->p1 = p1;
    pParamForTimer->p2 = p2;
    pParamForTimer->p3 = p3;
    pParamForTimer->p4 = p4;
    pParamForTimer->p5 = p5;
    pParamForTimer->p6 = p6;
    pParamForTimer->p7 = p7;
    pParamForTimer->p8 = p8;
    pParamForTimer->p9 = p9;
    _FakeThreadAddTimerFunc( pParamForTimer );
    return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
class _FtResumeParamT9 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n";
        return(gszType);
    }
    
    
    T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; T7 * p7; T8 * p8; T9 * p9; };
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
class ResumeFakeThreadObjT9 { public: int operator ()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
    {
        _FtResumeParamT9< T1, T2, T3, T4, T5, T6, T7, T8, T9> rp;
        rp.p1 = &p1, rp.p2 = &p2, rp.p3 = &p3, rp.p4 = &p4, rp.p5 = &p5, rp.p6 = &p6, rp.p7 = &p7, rp.p8 = &p8, rp.p9 = &p9;
        return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
int YieldFromFakeThread( ResumeFakeThreadObjT9< T1, T2, T3, T4, T5, T6, T7, T8, T9> &resumeObj, T1 *pOut1, T2 *pOut2, T3 *pOut3, T4 *pOut4, T5 *pOut5, T6 *pOut6, T7 *pOut7, T8 *pOut8, T9 *pOut9 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = __null; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
    {
        nRet = -1;
    }
    if ( !nRet )
    {
        _FtResumeParamT9< T1, T2, T3, T4, T5, T6, T7, T8, T9> * p = (_FtResumeParamT9< T1, T2, T3, T4, T5, T6, T7, T8, T9> *)pRet; if ( pOut1 )
        {
            *pOut1 = *p->p1;
        }
        if ( pOut2 )
        {
            *pOut2 = *p->p2;
        }
        if ( pOut3 )
        {
            *pOut3 = *p->p3;
        }
        if ( pOut4 )
        {
            *pOut4 = *p->p4;
        }
        if ( pOut5 )
        {
            *pOut5 = *p->p5;
        }
        if ( pOut6 )
        {
            *pOut6 = *p->p6;
        }
        if ( pOut7 )
        {
            *pOut7 = *p->p7;
        }
        if ( pOut8 )
        {
            *pOut8 = *p->p8;
        }
        if ( pOut9 )
        {
            *pOut9 = *p->p9;
        }
    }
    return(nRet);
}
//

#endif /* defined(__TestAllUseful__FakeThreadTemplate__) */
