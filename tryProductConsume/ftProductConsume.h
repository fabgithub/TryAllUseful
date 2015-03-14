//
//  ftProductConsume.h
//  TestAllUseful
//
//  Created by liyoudi on 15/3/6.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__ftProductConsume__
#define __TestAllUseful__ftProductConsume__

#include "FakeThreadTemplate.h"

#include <stdio.h>
#include <list>

template <class ResumeProductObj, class ResumeConsumeObj>
class ftProductConsumeBase
{
    typedef std::list<ResumeProductObj> ResumeProductObjList;
    typedef std::list<ResumeConsumeObj> ResumeConsumeObjList;
public:
    ResumeConsumeObj &NewResumeConsumeObj()
    {
        ResumeConsumeObj r;
        mResumeConsume.push_back(r);
        return mResumeConsume.back();
    }
    ResumeProductObj &NewResumeProductObj()
    {
        ResumeProductObj r;
        mResumeProduct.push_back(r);
        return mResumeProduct.back();
    }
    ResumeConsumeObj GetResumeConsumeObj()
    {
        ResumeConsumeObj r;
        r = mResumeConsume.front();
        mResumeConsume.pop_front();
        return r;
    }
    ResumeProductObj GetResumeProductObj()
    {
        ResumeProductObj r;
        r = mResumeProduct.front();
        mResumeProduct.pop_front();
        return r;
    }
    unsigned int HasConsumer() const { return (unsigned int) mResumeConsume.size(); }
    unsigned int HasProductor() const { return (unsigned int) mResumeProduct.size(); }
    void Reset()
    {
        mResumeConsume.clear();
        mResumeProduct.clear();
    }
protected:
    ResumeProductObjList mResumeProduct;
    ResumeConsumeObjList mResumeConsume;
};

template <class T1>
class ftProductConsume : public ftProductConsumeBase<ResumeFakeThreadObjT1<T1 *>, ResumeFakeThreadObjT1<T1> >
{
};
template <class T1>
int ftProduct(ftProductConsume<T1> &whichProductConsume, const T1 &p1)
{
    int nRet = 0;
    if(!whichProductConsume.HasConsumer()) // 没有消费者
    {
        T1 *pOut1 = NULL;
        nRet = YieldFromFakeThread(whichProductConsume.NewResumeProductObj(), &pOut1);
        if(!nRet && pOut1) // 返回成功，这时有消费者了
        {
            *pOut1 = p1; // 保存给消费者
        }
        else
        {
            nRet = -9999993;
        }
    }
    else
    {
        nRet = whichProductConsume.GetResumeConsumeObj()(p1);
    }
    return nRet;
}

template <class T1>
int ftConsume(ftProductConsume<T1> &whichProductConsume, T1 &p1)
{
    int nRet = 0;
    if(!whichProductConsume.HasProductor())
    {
        nRet = YieldFromFakeThread(whichProductConsume.NewResumeConsumeObj(), &p1);
        if(!nRet)
        {
        }
    }
    else
    {
        nRet = whichProductConsume.GetResumeProductObj()(&p1);
    }
    return nRet;
}

#define __ft_FK_STAR1 *
#define __ft_FK_STAR2 *
#define __ft_FK_STAR3 *
#define __ft_FK_STAR4 *
#define __ft_FK_STAR5 *
#define __ft_FK_STAR6 *
#define __ft_FK_STAR7 *
#define __ft_FK_STAR8 *
#define __ft_FK_STAR9 *

#define __ft_pc_DECLARE_pOut(n) T##n *pOut##n = NULL;
#define __ft_pc_SET_pOut(n) *pOut##n = p##n;
#define __ft_pc_Judge_pOut(n) && pOut##n
// for macro
#define __ft_DECLARE_PRODUCT_CONSUME(n) \
	template <__ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
	class ftProductConsume##n : public ftProductConsumeBase<ResumeFakeThreadObjT##n<__ft_COMMON_DECLARE_LIST##n(T, __ft_FK_STAR)>, ResumeFakeThreadObjT##n<__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)> > \
	{ \
	}; \
	template <__ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
    int ftProduct(ftProductConsume##n<__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)> &whichProductConsume, __ft_COMMON_DECLARE_LIST##n(const T, &p)) \
	{ \
	    int nRet = 0; \
	    if(!whichProductConsume.HasConsumer()) /* 没有消费者*/ \
	    { \
            __ft_TIMES_##n(__ft_pc_DECLARE_pOut) \
	        nRet = YieldFromFakeThread(whichProductConsume.NewResumeProductObj(), __ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, &pOut)); \
	        if(!nRet __ft_TIMES_##n(__ft_pc_Judge_pOut)) /* 返回成功，这时有消费者了 */ \
	        { \
	            /* 保存给消费者 */ \
                __ft_TIMES_##n(__ft_pc_SET_pOut); \
	        } \
	        else \
	        { \
	            nRet = -9999993; \
	        } \
	    } \
	    else \
	    { \
	        nRet = whichProductConsume.GetResumeConsumeObj()(__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, p)); \
	    } \
	    return nRet; \
	} \
	 \
	template <__ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
	int ftConsume(ftProductConsume##n<__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, T)> &whichProductConsume, __ft_COMMON_DECLARE_LIST##n(T, &p)) \
	{ \
	    int nRet = 0; \
	    if(!whichProductConsume.HasProductor()) \
	    { \
	        nRet = YieldFromFakeThread(whichProductConsume.NewResumeConsumeObj(), __ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, &p)); \
	        if(!nRet) \
	        { \
	        } \
	    } \
	    else \
	    { \
	        nRet = whichProductConsume.GetResumeProductObj()(__ft_COMMON_DECLARE_LIST##n(__ft_FK_EMPTY, &p)); \
	    } \
	    return nRet; \
	}

__ft_DECLARE_PRODUCT_CONSUME(1)
__ft_DECLARE_PRODUCT_CONSUME(2)
__ft_DECLARE_PRODUCT_CONSUME(3)
__ft_DECLARE_PRODUCT_CONSUME(4)
__ft_DECLARE_PRODUCT_CONSUME(5)
__ft_DECLARE_PRODUCT_CONSUME(6)
__ft_DECLARE_PRODUCT_CONSUME(7)
__ft_DECLARE_PRODUCT_CONSUME(8)
__ft_DECLARE_PRODUCT_CONSUME(9)


#endif /* defined(__TestAllUseful__ftProductConsume__) */
