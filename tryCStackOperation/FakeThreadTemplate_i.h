//
//  FakeThreadTemplate_i.h
//  ft_framework
//
//  Created by liyoudi on 15/3/18.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//

#ifndef ft_framework_FakeThreadTemplate_i_h
#define ft_framework_FakeThreadTemplate_i_h

template <class T1, class T2> class _FTParamPtr2 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2 ); T1 * p1; T2 * p2; virtual void operator ()() const
    {
        f( *p1, *p2 );
    }
}; template <class T1, class T2> class _FTParamForTimer2 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2 ); T1 p1; T2 p2; virtual void operator ()() const
    {
        f( p1, p2 );
    }
};
template <class T1, class T2, class T3> class _FTParamPtr3 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3 ); T1 * p1; T2 * p2; T3 * p3; virtual void operator ()() const
    {
        f( *p1, *p2, *p3 );
    }
}; template <class T1, class T2, class T3> class _FTParamForTimer3 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3 ); T1 p1; T2 p2; T3 p3; virtual void operator ()() const
    {
        f( p1, p2, p3 );
    }
};
template <class T1, class T2, class T3, class T4> class _FTParamPtr4 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4 ); T1 * p1; T2 * p2; T3 * p3; T4 * p4; virtual void operator ()() const
    {
        f( *p1, *p2, *p3, *p4 );
    }
}; template <class T1, class T2, class T3, class T4> class _FTParamForTimer4 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4 ); T1 p1; T2 p2; T3 p3; T4 p4; virtual void operator ()() const
    {
        f( p1, p2, p3, p4 );
    }
};
template <class T1, class T2, class T3, class T4, class T5> class _FTParamPtr5 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 ); T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; virtual void operator ()() const
    {
        f( *p1, *p2, *p3, *p4, *p5 );
    }
}; template <class T1, class T2, class T3, class T4, class T5> class _FTParamForTimer5 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 ); T1 p1; T2 p2; T3 p3; T4 p4; T5 p5; virtual void operator ()() const
    {
        f( p1, p2, p3, p4, p5 );
    }
};
template <class T1, class T2, class T3, class T4, class T5, class T6> class _FTParamPtr6 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 ); T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; virtual void operator ()() const
    {
        f( *p1, *p2, *p3, *p4, *p5, *p6 );
    }
}; template <class T1, class T2, class T3, class T4, class T5, class T6> class _FTParamForTimer6 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 ); T1 p1; T2 p2; T3 p3; T4 p4; T5 p5; T6 p6; virtual void operator ()() const
    {
        f( p1, p2, p3, p4, p5, p6 );
    }
};
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7> class _FTParamPtr7 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 ); T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; T7 * p7; virtual void operator ()() const
    {
        f( *p1, *p2, *p3, *p4, *p5, *p6, *p7 );
    }
}; template <class T1, class T2, class T3, class T4, class T5, class T6, class T7> class _FTParamForTimer7 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 ); T1 p1; T2 p2; T3 p3; T4 p4; T5 p5; T6 p6; T7 p7; virtual void operator ()() const
    {
        f( p1, p2, p3, p4, p5, p6, p7 );
    }
};
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8> class _FTParamPtr8 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 ); T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; T7 * p7; T8 * p8; virtual void operator ()() const
    {
        f( *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8 );
    }
}; template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8> class _FTParamForTimer8 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 ); T1 p1; T2 p2; T3 p3; T4 p4; T5 p5; T6 p6; T7 p7; T8 p8; virtual void operator ()() const
    {
        f( p1, p2, p3, p4, p5, p6, p7, p8 );
    }
};
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9> class _FTParamPtr9 : public _FunctorForFakeThreadThreadProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 ); T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; T7 * p7; T8 * p8; T9 * p9; virtual void operator ()() const
    {
        f( *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9 );
    }
}; template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9> class _FTParamForTimer9 : public _FunctorForFakeThreadTimerProcParam { public: void (*f)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 ); T1 p1; T2 p2; T3 p3; T4 p4; T5 p5; T6 p6; T7 p7; T8 p8; T9 p9; virtual void operator ()() const
    {
        f( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
    }
};


template <class T1, class T2> int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2 ), T1 p1, T2 p2 )
{
    int nRet = 0; _FTParamPtr2< T1, T2> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1, tparam.p2 = &p2; nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam ); return(nRet);
}


template <class T1, class T2> int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2 ), T1 p1, T2 p2 )
{
    int nRet = 0; _FTParamForTimer2< T1, T2> *pParamForTimer = new _FTParamForTimer2< T1, T2>; pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds; pParamForTimer->f = FakeThreadProc; pParamForTimer->p1 = p1, pParamForTimer->p2 = p2; _FakeThreadAddTimerFunc( pParamForTimer ); return(nRet);
}


template <class T1, class T2, class T3> int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3 ), T1 p1, T2 p2, T3 p3 )
{
    int nRet = 0; _FTParamPtr3< T1, T2, T3> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1, tparam.p2 = &p2, tparam.p3 = &p3; nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam ); return(nRet);
}


template <class T1, class T2, class T3> int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3 ), T1 p1, T2 p2, T3 p3 )
{
    int nRet = 0; _FTParamForTimer3< T1, T2, T3> *pParamForTimer = new _FTParamForTimer3< T1, T2, T3>; pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds; pParamForTimer->f = FakeThreadProc; pParamForTimer->p1 = p1, pParamForTimer->p2 = p2, pParamForTimer->p3 = p3; _FakeThreadAddTimerFunc( pParamForTimer ); return(nRet);
}


template <class T1, class T2, class T3, class T4> int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4 ), T1 p1, T2 p2, T3 p3, T4 p4 )
{
    int nRet = 0; _FTParamPtr4< T1, T2, T3, T4> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1, tparam.p2 = &p2, tparam.p3 = &p3, tparam.p4 = &p4; nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam ); return(nRet);
}


template <class T1, class T2, class T3, class T4> int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4 ), T1 p1, T2 p2, T3 p3, T4 p4 )
{
    int nRet = 0; _FTParamForTimer4< T1, T2, T3, T4> *pParamForTimer = new _FTParamForTimer4< T1, T2, T3, T4>; pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds; pParamForTimer->f = FakeThreadProc; pParamForTimer->p1 = p1, pParamForTimer->p2 = p2, pParamForTimer->p3 = p3, pParamForTimer->p4 = p4; _FakeThreadAddTimerFunc( pParamForTimer ); return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5> int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
{
    int nRet = 0; _FTParamPtr5< T1, T2, T3, T4, T5> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1, tparam.p2 = &p2, tparam.p3 = &p3, tparam.p4 = &p4, tparam.p5 = &p5; nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam ); return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5> int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
{
    int nRet = 0; _FTParamForTimer5< T1, T2, T3, T4, T5> *pParamForTimer = new _FTParamForTimer5< T1, T2, T3, T4, T5>; pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds; pParamForTimer->f = FakeThreadProc; pParamForTimer->p1 = p1, pParamForTimer->p2 = p2, pParamForTimer->p3 = p3, pParamForTimer->p4 = p4, pParamForTimer->p5 = p5; _FakeThreadAddTimerFunc( pParamForTimer ); return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6> int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
{
    int nRet = 0; _FTParamPtr6< T1, T2, T3, T4, T5, T6> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1, tparam.p2 = &p2, tparam.p3 = &p3, tparam.p4 = &p4, tparam.p5 = &p5, tparam.p6 = &p6; nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam ); return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6> int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
{
    int nRet = 0; _FTParamForTimer6< T1, T2, T3, T4, T5, T6> *pParamForTimer = new _FTParamForTimer6< T1, T2, T3, T4, T5, T6>; pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds; pParamForTimer->f = FakeThreadProc; pParamForTimer->p1 = p1, pParamForTimer->p2 = p2, pParamForTimer->p3 = p3, pParamForTimer->p4 = p4, pParamForTimer->p5 = p5, pParamForTimer->p6 = p6; _FakeThreadAddTimerFunc( pParamForTimer ); return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7> int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
{
    int nRet = 0; _FTParamPtr7< T1, T2, T3, T4, T5, T6, T7> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1, tparam.p2 = &p2, tparam.p3 = &p3, tparam.p4 = &p4, tparam.p5 = &p5, tparam.p6 = &p6, tparam.p7 = &p7; nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam ); return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7> int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
{
    int nRet = 0; _FTParamForTimer7< T1, T2, T3, T4, T5, T6, T7> *pParamForTimer = new _FTParamForTimer7< T1, T2, T3, T4, T5, T6, T7>; pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds; pParamForTimer->f = FakeThreadProc; pParamForTimer->p1 = p1, pParamForTimer->p2 = p2, pParamForTimer->p3 = p3, pParamForTimer->p4 = p4, pParamForTimer->p5 = p5, pParamForTimer->p6 = p6, pParamForTimer->p7 = p7; _FakeThreadAddTimerFunc( pParamForTimer ); return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8> int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
{
    int nRet = 0; _FTParamPtr8< T1, T2, T3, T4, T5, T6, T7, T8> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1, tparam.p2 = &p2, tparam.p3 = &p3, tparam.p4 = &p4, tparam.p5 = &p5, tparam.p6 = &p6, tparam.p7 = &p7, tparam.p8 = &p8; nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam ); return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8> int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
{
    int nRet = 0; _FTParamForTimer8< T1, T2, T3, T4, T5, T6, T7, T8> *pParamForTimer = new _FTParamForTimer8< T1, T2, T3, T4, T5, T6, T7, T8>; pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds; pParamForTimer->f = FakeThreadProc; pParamForTimer->p1 = p1, pParamForTimer->p2 = p2, pParamForTimer->p3 = p3, pParamForTimer->p4 = p4, pParamForTimer->p5 = p5, pParamForTimer->p6 = p6, pParamForTimer->p7 = p7, pParamForTimer->p8 = p8; _FakeThreadAddTimerFunc( pParamForTimer ); return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9> int RunInFakeThreadT( void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
{
    int nRet = 0; _FTParamPtr9< T1, T2, T3, T4, T5, T6, T7, T8, T9> tparam; tparam.f = FakeThreadProc; tparam.p1 = &p1, tparam.p2 = &p2, tparam.p3 = &p3, tparam.p4 = &p4, tparam.p5 = &p5, tparam.p6 = &p6, tparam.p7 = &p7, tparam.p8 = &p8, tparam.p9 = &p9; nRet = RunFuncInFakeThread( _FakeThreadProcForTemplate, &tparam ); return(nRet);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9> int RunInFakeThreadT( double dTimeoutSeconds, void (*FakeThreadProc)( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 ), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
{
    int nRet = 0; _FTParamForTimer9< T1, T2, T3, T4, T5, T6, T7, T8, T9> *pParamForTimer = new _FTParamForTimer9< T1, T2, T3, T4, T5, T6, T7, T8, T9>; pParamForTimer->mdTimeoutSeconds = dTimeoutSeconds; pParamForTimer->f = FakeThreadProc; pParamForTimer->p1 = p1, pParamForTimer->p2 = p2, pParamForTimer->p3 = p3, pParamForTimer->p4 = p4, pParamForTimer->p5 = p5, pParamForTimer->p6 = p6, pParamForTimer->p7 = p7, pParamForTimer->p8 = p8, pParamForTimer->p9 = p9; _FakeThreadAddTimerFunc( pParamForTimer ); return(nRet);
}


template <class T1, class T2> class _FtResumeParamT2 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n"; return(gszType);
    }
    
    
    T1 * p1; T2 * p2; };
template <class T1, class T2, class T3> class _FtResumeParamT3 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n"; return(gszType);
    }
    
    
    T1 * p1; T2 * p2; T3 * p3; };
template <class T1, class T2, class T3, class T4> class _FtResumeParamT4 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n"; return(gszType);
    }
    
    
    T1 * p1; T2 * p2; T3 * p3; T4 * p4; };
template <class T1, class T2, class T3, class T4, class T5> class _FtResumeParamT5 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n"; return(gszType);
    }
    
    
    T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; };
template <class T1, class T2, class T3, class T4, class T5, class T6> class _FtResumeParamT6 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n"; return(gszType);
    }
    
    
    T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; };
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7> class _FtResumeParamT7 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n"; return(gszType);
    }
    
    
    T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; T7 * p7; };
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8> class _FtResumeParamT8 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n"; return(gszType);
    }
    
    
    T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; T7 * p7; T8 * p8; };
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9> class _FtResumeParamT9 : public FakeThreadResumeParam { public: virtual const char *FtsParamType() const
    {
        static const char gszType[] = "_FtResumeParamT##n"; return(gszType);
    }
    
    
    T1 * p1; T2 * p2; T3 * p3; T4 * p4; T5 * p5; T6 * p6; T7 * p7; T8 * p8; T9 * p9; };


template <class T1, class T2> class ResumeFakeThreadObjT2 { public: int operator ()( T1 p1, T2 p2 )
    {
        _FtResumeParamT2< T1, T2> rp; rp.p1 = &p1, rp.p2 = &p2; return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2, class T3> class ResumeFakeThreadObjT3 { public: int operator ()( T1 p1, T2 p2, T3 p3 )
    {
        _FtResumeParamT3< T1, T2, T3> rp; rp.p1 = &p1, rp.p2 = &p2, rp.p3 = &p3; return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2, class T3, class T4> class ResumeFakeThreadObjT4 { public: int operator ()( T1 p1, T2 p2, T3 p3, T4 p4 )
    {
        _FtResumeParamT4< T1, T2, T3, T4> rp; rp.p1 = &p1, rp.p2 = &p2, rp.p3 = &p3, rp.p4 = &p4; return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2, class T3, class T4, class T5> class ResumeFakeThreadObjT5 { public: int operator ()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
    {
        _FtResumeParamT5< T1, T2, T3, T4, T5> rp; rp.p1 = &p1, rp.p2 = &p2, rp.p3 = &p3, rp.p4 = &p4, rp.p5 = &p5; return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2, class T3, class T4, class T5, class T6> class ResumeFakeThreadObjT6 { public: int operator ()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
    {
        _FtResumeParamT6< T1, T2, T3, T4, T5, T6> rp; rp.p1 = &p1, rp.p2 = &p2, rp.p3 = &p3, rp.p4 = &p4, rp.p5 = &p5, rp.p6 = &p6; return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7> class ResumeFakeThreadObjT7 { public: int operator ()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
    {
        _FtResumeParamT7< T1, T2, T3, T4, T5, T6, T7> rp; rp.p1 = &p1, rp.p2 = &p2, rp.p3 = &p3, rp.p4 = &p4, rp.p5 = &p5, rp.p6 = &p6, rp.p7 = &p7; return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8> class ResumeFakeThreadObjT8 { public: int operator ()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
    {
        _FtResumeParamT8< T1, T2, T3, T4, T5, T6, T7, T8> rp; rp.p1 = &p1, rp.p2 = &p2, rp.p3 = &p3, rp.p4 = &p4, rp.p5 = &p5, rp.p6 = &p6, rp.p7 = &p7, rp.p8 = &p8; return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9> class ResumeFakeThreadObjT9 { public: int operator ()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
    {
        _FtResumeParamT9< T1, T2, T3, T4, T5, T6, T7, T8, T9> rp; rp.p1 = &p1, rp.p2 = &p2, rp.p3 = &p3, rp.p4 = &p4, rp.p5 = &p5, rp.p6 = &p6, rp.p7 = &p7, rp.p8 = &p8, rp.p9 = &p9; return(mResumeObj( &rp ) );
    }
    
    
    bool operator !() const
    {
        return(!mResumeObj);
    }
    
    
    FakeThreadResumeObject mResumeObj; };


template <class T1, class T2> int YieldFromFakeThread( ResumeFakeThreadObjT2< T1, T2> &resumeObj, T1 *pOut1, T2 *pOut2 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = NULL; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
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


template <class T1, class T2, class T3> int YieldFromFakeThread( ResumeFakeThreadObjT3< T1, T2, T3> &resumeObj, T1 *pOut1, T2 *pOut2, T3 *pOut3 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = NULL; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
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


template <class T1, class T2, class T3, class T4> int YieldFromFakeThread( ResumeFakeThreadObjT4< T1, T2, T3, T4> &resumeObj, T1 *pOut1, T2 *pOut2, T3 *pOut3, T4 *pOut4 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = NULL; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
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


template <class T1, class T2, class T3, class T4, class T5> int YieldFromFakeThread( ResumeFakeThreadObjT5< T1, T2, T3, T4, T5> &resumeObj, T1 *pOut1, T2 *pOut2, T3 *pOut3, T4 *pOut4, T5 *pOut5 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = NULL; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
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


template <class T1, class T2, class T3, class T4, class T5, class T6> int YieldFromFakeThread( ResumeFakeThreadObjT6< T1, T2, T3, T4, T5, T6> &resumeObj, T1 *pOut1, T2 *pOut2, T3 *pOut3, T4 *pOut4, T5 *pOut5, T6 *pOut6 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = NULL; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
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


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7> int YieldFromFakeThread( ResumeFakeThreadObjT7< T1, T2, T3, T4, T5, T6, T7> &resumeObj, T1 *pOut1, T2 *pOut2, T3 *pOut3, T4 *pOut4, T5 *pOut5, T6 *pOut6, T7 *pOut7 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = NULL; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
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


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8> int YieldFromFakeThread( ResumeFakeThreadObjT8< T1, T2, T3, T4, T5, T6, T7, T8> &resumeObj, T1 *pOut1, T2 *pOut2, T3 *pOut3, T4 *pOut4, T5 *pOut5, T6 *pOut6, T7 *pOut7, T8 *pOut8 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = NULL; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
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


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9> int YieldFromFakeThread( ResumeFakeThreadObjT9< T1, T2, T3, T4, T5, T6, T7, T8, T9> &resumeObj, T1 *pOut1, T2 *pOut2, T3 *pOut3, T4 *pOut4, T5 *pOut5, T6 *pOut6, T7 *pOut7, T8 *pOut8, T9 *pOut9 )
{
    int nRet = 0; resumeObj.mResumeObj = GetResumeObjectForFakeThread(); FakeThreadResumeParam *pRet = NULL; nRet = YieldFromFakeThread( &pRet ); if ( !pRet )
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
#endif
