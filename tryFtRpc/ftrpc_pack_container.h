//
//  ftrpc_pack_container.h
//  TestAllUseful
//
//  Created by liyoudi on 15/3/4.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//  封装各STL容器的打包和解包模板函数

#ifndef __TestAllUseful__ftrpc_pack_container__
#define __TestAllUseful__ftrpc_pack_container__

#include "ftnetbuffer.h"
#include "ftrpc_pack_basic.h"

#include <stdio.h>
#include <string.h>

#include <set>
#include <list>
#include <vector>
#include <map>

#include <utility>


template <class T1, class T2>
ftnetbuffer ftrpc_pack(const std::pair<T1, T2> &n)
{
    ftnetbuffer retBuf, tmpBuf;
    retBuf = retBuf + ftrpc_pack(n.first);
    retBuf = retBuf + ftrpc_pack(n.second);
    tmpBuf = retBuf;
    return tmpBuf;
}
template <class T1, class T2>
unsigned int ftrpc_len_pack(const std::pair<T1, T2> &n)
{
    unsigned int nRet = 0;
    nRet += ftrpc_pack_GetAlignLen(ftrpc_len_pack(n.first)) + ftrpc_pack_GetAlignLen(ftrpc_len_pack_type_name(n.first));
    nRet += ftrpc_pack_GetAlignLen(ftrpc_len_pack(n.second)) + ftrpc_pack_GetAlignLen(ftrpc_len_pack_type_name(n.second));
    return nRet;
}
template <class T1, class T2>
unsigned int ftrpc_len_pack_type_name(const std::pair<T1, T2> &n)
{
    return 0;
}
//template <class T1, class T2>
//int ftrpc_unpack(const ftnetbuffer &buf, std::pair<T1, T2> &n)
//{
//    int nRet = 0;
//    do
//    {
//        nRet = ftrpc_unpack(buf, n.first);
//        if(nRet < 0)
//            break;
//        nRet = ftrpc_unpack(buf, n.second);
//        if(nRet < 0)
//            break;
//        nRet = ftrpc_len_pack(n);
//    }while(false);
//    return nRet;
//}

template <class Container>
ftnetbuffer ftrpc_pack_for_container(const char *szContainerName, unsigned int nNameLen, const Container &cont)
{
    ftnetbuffer bufRR;
    unsigned nRealNameLen = ftrpc_pack_GetAlignLen(nNameLen);
    ftnetbuffer nameAndBodyLenBuf(nRealNameLen + 8);

    ftrpc_pack_TypeName(nameAndBodyLenBuf, szContainerName, nNameLen);
    
    unsigned int nBufPos = nRealNameLen;
    *( (unsigned int *) (nameAndBodyLenBuf.buffer() + nBufPos) ) = (unsigned int) cont.size();
    nBufPos += 4; //
    *( (unsigned int *) (nameAndBodyLenBuf.buffer() + nBufPos) ) = 0;
    nBufPos += 4; // 8字节对齐
    
    bufRR = nameAndBodyLenBuf;
    
    typename Container::const_iterator citer;
    for(citer = cont.begin(); citer != cont.end();++citer)
    {
        ftnetbuffer bufValue;
        bufValue = ftrpc_pack(*citer);
        bufRR = bufRR + bufValue;
    }

    ftnetbuffer bufRet = bufRR;
    return bufRet;
}

template <class T>
int ftrpc_unpack_for_container_elem(const ftnetbuffer &buf, std::list<T> &cont)
{
    int nRet = 0;
    do
    {
        T t;
        nRet = ftrpc_unpack(buf, t);
        if(nRet < 0)
        {
            break;
        }
        cont.push_back(t);
        
        nRet = ftrpc_pack_GetAlignLen(ftrpc_len_pack(t)) + ftrpc_pack_GetAlignLen(ftrpc_len_pack_type_name(t));
    }while(false);
    return nRet;
}
template <class T>
int ftrpc_unpack_for_container_elem(const ftnetbuffer &buf, std::vector<T> &cont)
{
    int nRet = 0;
    do
    {
        T t;
        nRet = ftrpc_unpack(buf, t);
        if(nRet < 0)
        {
            break;
        }
        cont.push_back(t);
        nRet = ftrpc_pack_GetAlignLen(ftrpc_len_pack(t)) + ftrpc_pack_GetAlignLen(ftrpc_len_pack_type_name(t));
    }while(false);
    return nRet;
}
template <class T>
int ftrpc_unpack_for_container_elem(const ftnetbuffer &buf, std::set<T> &cont)
{
    int nRet = 0;
    do
    {
        T t;
        nRet = ftrpc_unpack(buf, t);
        if(nRet < 0)
        {
            break;
        }
        typedef std::set<T> Container;
        std::pair<typename Container::iterator, bool> ib;
        ib = cont.insert(t);
        if(!ib.second)
        {
            nRet = -9999993; // 有重复键值
            break;
        }
        nRet = ftrpc_pack_GetAlignLen(ftrpc_len_pack(t)) + ftrpc_pack_GetAlignLen(ftrpc_len_pack_type_name(t));
    }while(false);
    return nRet;
}
template <class Key, class Value>
int ftrpc_unpack_for_container_elem(const ftnetbuffer &buf, std::map<Key, Value> &cont)
{
    int nRet = 0;
    do
    {
        Key k;
        Value v;
        nRet = ftrpc_unpack(buf, k);
        if(nRet < 0)
        {
            break;
        }
        typedef std::map<Key, Value> Container;
        std::pair<typename Container::iterator, bool> ib;
        ib = cont.insert(std::make_pair(k, v));
        if(!ib.second)
        {
            nRet = -9999993; // 有重复键值
            break;
        }
        unsigned int nBufPos = ftrpc_pack_GetAlignLen(ftrpc_len_pack(k)) + ftrpc_pack_GetAlignLen(ftrpc_len_pack_type_name(k));
        ftnetbuffer valueBuf((char *) buf.buffer() + nBufPos, buf.bufferLen() - nBufPos, false);
        nRet = ftrpc_unpack(valueBuf, ib.first->second);
        if(nRet < 0)
        {
            nRet = -2;
            break;
        }
        nRet = ftrpc_len_pack(*ib.first);
    }while(false);
    return nRet;
}

template <class Container>
int ftrpc_unpack_for_container(const ftnetbuffer &buf, const char *szContainerName, unsigned int nNameLen, Container &cont)
{
    int nRet = 0;
    do
    {
        unsigned nNameRealLen = ftrpc_pack_GetAlignLen(nNameLen);
        if(buf.bufferLen() <= nNameRealLen) /*长度不对*/
        {
            nRet = -1;
            break;
        }
        int nIndex = 0;
        for (nIndex = 0; nIndex < nNameLen; ++nIndex)
        {
            if(buf.buffer()[nIndex] != szContainerName[nIndex])
            {
                nRet = -2;
                break;
            }
        }
        if(nRet < 0)
        {
            break;
        }
        unsigned int nBufPos = nNameRealLen;
        unsigned int nElemCount = *( (unsigned int *) (buf.buffer() + nBufPos) );
        unsigned int nReserved = *( (unsigned int *) (buf.buffer() + nBufPos + 4) );
        nBufPos += 8;
        if(nReserved)
        {
            nRet = -4;
            break;
        }
        while(nBufPos < buf.bufferLen() && nElemCount > 0)
        {
            ftnetbuffer bufValue(buf.buffer() + nBufPos, buf.bufferLen() - nBufPos);
            nRet = ftrpc_unpack_for_container_elem(bufValue, cont);
            if(nRet < 0)
            {
                break;
            }
            nBufPos += nRet;
            --nElemCount;
        }
        if(nElemCount > 0)
        {
            nRet = -5;
            break;
        }
        nRet = nBufPos;
    }while(false);
    return nRet;
}

// 只返回数据大小，不包括类型名大小
template <class Container>
unsigned ftrpc_len_pack_for_container(const Container &cont)
{
    unsigned int nRetLen = 0;
    nRetLen += 8; // 存储容器中元素个数所占的空间
    // 然后再加上各元素的大小
    typename Container::const_iterator citer;
    for (citer = cont.begin(); cont.end() != citer; ++citer)
    {
        nRetLen += ftrpc_pack_GetAlignLen(ftrpc_len_pack(*citer)) + ftrpc_pack_GetAlignLen(ftrpc_len_pack_type_name(*citer));
    }
    return nRetLen;
}

#define ftrpc_IMPLEMENT_CONTAINER_TYPE(type) \
    unsigned int ftrpc_len_pack(const type &cont) \
    { \
    	return ftrpc_len_pack_for_container(cont); \
    } \
    unsigned int ftrpc_len_pack_type_name(const type &cont) \
    { \
    	return (unsigned int) sizeof(#type); \
    } \
    ftnetbuffer ftrpc_pack(const type &cont) \
    { \
    	return ftrpc_pack_for_container(#type, (unsigned int) sizeof(#type), cont); \
    } \
    int ftrpc_unpack(const ftnetbuffer &buf, type &cont) \
    { \
    	return ftrpc_unpack_for_container(buf, #type, sizeof(#type), cont); \
    }

// ========================================================
// the following is for struct
#define ftrpc_DECLARE_STRUCE_PACK(structType) \
    unsigned int ftrpc_len_pack_type_name(const structType /* only for type */ ); \
    unsigned int ftrpc_len_pack(const structType &st); \
    ftnetbuffer ftrpc_pack(const structType &st); \
    int ftrpc_unpack(const ftnetbuffer &buf, structType &buf);

#define __ftrpc_pack_member_list_1(m1) st.m1
#define __ftrpc_pack_member_list_2(m1, m2) __ftrpc_pack_member_list_1(m1), st.m2
#define __ftrpc_pack_member_list_3(m1, m2, m3) __ftrpc_pack_member_list_2(m1, m2), st.m3
#define __ftrpc_pack_member_list_4(m1, m2, m3, m4) __ftrpc_pack_member_list_3(m1, m2, m3), st.m4
#define __ftrpc_pack_member_list_5(m1, m2, m3, m4, m5) __ftrpc_pack_member_list_4(m1, m2, m3, m4), st.m5
#define __ftrpc_pack_member_list_6(m1, m2, m3, m4, m5) __ftrpc_pack_member_list_5(m1, m2, m3, m4, m5), st.m6
#define __ftrpc_pack_member_list_7(m1, m2, m3, m4, m5, m6) __ftrpc_pack_member_list_7(m1, m2, m3, m4, m5), st.m7
#define __ftrpc_pack_member_list_8(m1, m2, m3, m4, m5, m6, m7) __ftrpc_pack_member_list_8(m1, m2, m3, m4, m5, m6), st.m8
#define __ftrpc_pack_member_list_9(m1, m2, m3, m4, m5, m6, m7) __ftrpc_pack_member_list_8(m1, m2, m3, m4, m5, m6, m7), st.m9

#define ftrpc_IMPLEMENT_STRUCE_PACK(structType, nMember, ...) \
    unsigned int ftrpc_len_pack_type_name(const structType &/* only for type */ ) \
    { \
        static unsigned gnTypeNameLen = (unsigned int) strlen(#structType) + 1; \
        return gnTypeNameLen; \
    } \
    unsigned int ftrpc_len_pack(const structType &st) \
    { \
        return ftrpc_len_pack_struct(__ftrpc_pack_member_list_##nMember(__VA_ARGS__)); \
    } \
    ftnetbuffer ftrpc_pack(const structType &st) \
    { \
        return ftrpc_pack_struct(#structType, ftrpc_len_pack_type_name(st), __ftrpc_pack_member_list_##nMember(__VA_ARGS__)); \
    } \
    int ftrpc_unpack(const ftnetbuffer &buf, structType &st) \
    { \
        int nRet = 0; \
        nRet = ftrpc_unpack_struct(buf, #structType, ftrpc_len_pack_type_name(st), __ftrpc_pack_member_list_##nMember(__VA_ARGS__)); \
        return nRet; \
    }

template <class T1>
unsigned int ftrpc_len_pack_struct(const T1 &m1)
{
    unsigned int nRet = 0;
    nRet += ftrpc_pack_GetAlignLen(ftrpc_len_pack(m1)) + ftrpc_pack_GetAlignLen(ftrpc_len_pack_type_name(m1));
    return nRet;
}

#define __macro_len_pack_member(n) nRet += ftrpc_pack_GetAlignLen(ftrpc_len_pack(m##n)) + ftrpc_pack_GetAlignLen(ftrpc_len_pack_type_name(m##n));
#define __macro_len_pack_member_1 __macro_len_pack_member(1)
#define __macro_len_pack_member_2 __macro_len_pack_member_1 __macro_len_pack_member(2)
#define __macro_len_pack_member_3 __macro_len_pack_member_2 __macro_len_pack_member(3)
#define __macro_len_pack_member_4 __macro_len_pack_member_3 __macro_len_pack_member(4)
#define __macro_len_pack_member_5 __macro_len_pack_member_4 __macro_len_pack_member(5)
#define __macro_len_pack_member_6 __macro_len_pack_member_5 __macro_len_pack_member(6)
#define __macro_len_pack_member_7 __macro_len_pack_member_6 __macro_len_pack_member(7)
#define __macro_len_pack_member_8 __macro_len_pack_member_7 __macro_len_pack_member(8)
#define __macro_len_pack_member_9 __macro_len_pack_member_8 __macro_len_pack_member(9)

#define __ftrpc_DECLARE_LEN_PACK_STRUCT_TEMPLATE(n) \
    template <__ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
    unsigned int ftrpc_len_pack_struct(__ft_COMMON_DECLARE_LIST##n(const T, &m)) \
    { \
        unsigned int nRet = 0; \
        __macro_len_pack_member_##n \
        return nRet; \
    }

//__ftrpc_DECLARE_LEN_PACK_STRUCT_TEMPLATE(1)
__ftrpc_DECLARE_LEN_PACK_STRUCT_TEMPLATE(2)
__ftrpc_DECLARE_LEN_PACK_STRUCT_TEMPLATE(3)
__ftrpc_DECLARE_LEN_PACK_STRUCT_TEMPLATE(4)
__ftrpc_DECLARE_LEN_PACK_STRUCT_TEMPLATE(5)
__ftrpc_DECLARE_LEN_PACK_STRUCT_TEMPLATE(6)
__ftrpc_DECLARE_LEN_PACK_STRUCT_TEMPLATE(7)
__ftrpc_DECLARE_LEN_PACK_STRUCT_TEMPLATE(8)
__ftrpc_DECLARE_LEN_PACK_STRUCT_TEMPLATE(9)

template <class T1>
ftnetbuffer ftrpc_pack_struct(const char *szTypeName, unsigned int nNameLen, const T1 &m1)
{
    ftnetbuffer retBuf;
    ftnetbuffer nameBuf(ftrpc_pack_GetAlignLen(nNameLen)), buf1;
    ftrpc_pack_TypeName(nameBuf, szTypeName, nNameLen);
    buf1 = ftrpc_pack(m1);
    retBuf = nameBuf + buf1;
    return retBuf;
}

#define __macro_pack_member(n) retBuf = retBuf + ftrpc_pack(m##n);
#define __macro_pack_member_1 __macro_pack_member(1)
#define __macro_pack_member_2 __macro_pack_member_1 __macro_pack_member(2)
#define __macro_pack_member_3 __macro_pack_member_2 __macro_pack_member(3)
#define __macro_pack_member_4 __macro_pack_member_3 __macro_pack_member(4)
#define __macro_pack_member_5 __macro_pack_member_4 __macro_pack_member(5)
#define __macro_pack_member_6 __macro_pack_member_5 __macro_pack_member(6)
#define __macro_pack_member_7 __macro_pack_member_6 __macro_pack_member(7)
#define __macro_pack_member_8 __macro_pack_member_7 __macro_pack_member(8)
#define __macro_pack_member_9 __macro_pack_member_8 __macro_pack_member(9)

#define __ftrpc_DECLARE_PACK_STRUCT_TEMPLATE(n) \
    template <__ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
    ftnetbuffer ftrpc_pack_struct(const char *szTypeName, unsigned int nNameLen, __ft_COMMON_DECLARE_LIST##n(const T, &m)) \
    { \
        ftnetbuffer retBuf; \
        ftnetbuffer nameBuf(ftrpc_pack_GetAlignLen(nNameLen)); \
        ftrpc_pack_TypeName(nameBuf, szTypeName, nNameLen); \
        retBuf = nameBuf; \
        __macro_pack_member_##n; \
        return retBuf; \
    }

//__ftrpc_DECLARE_PACK_STRUCT_TEMPLATE(1)
__ftrpc_DECLARE_PACK_STRUCT_TEMPLATE(2)
__ftrpc_DECLARE_PACK_STRUCT_TEMPLATE(3)
__ftrpc_DECLARE_PACK_STRUCT_TEMPLATE(4)
__ftrpc_DECLARE_PACK_STRUCT_TEMPLATE(5)
__ftrpc_DECLARE_PACK_STRUCT_TEMPLATE(6)
__ftrpc_DECLARE_PACK_STRUCT_TEMPLATE(7)
__ftrpc_DECLARE_PACK_STRUCT_TEMPLATE(8)
__ftrpc_DECLARE_PACK_STRUCT_TEMPLATE(9)

template <class T1>
int ftrpc_unpack_struct(const ftnetbuffer &buf, const char *szTypeName, unsigned int nNameLen, T1 &m1)
{
    int nRet = 0;
    ftnetbuffer retBuf;
    unsigned int nBufPos = 0;
    do
    {
        unsigned int nNameRealLen = ftrpc_pack_GetAlignLen(nNameLen);
        if(buf.bufferLen() < nNameRealLen)
        {
            nRet = -1;
            break;
        }
        // check type name
        int nIndex = 0;
        for (nIndex = 0; nIndex < nNameRealLen; ++nIndex)
        {
            if((nIndex >= nNameLen && buf.buffer()[nIndex] == 0) || buf.buffer()[nIndex] != szTypeName[nIndex])
            {
                nRet = -2;
                break;
            }
        }
        nBufPos += nNameRealLen;
        // unpack members
        ftnetbuffer buf1(buf.buffer() + nBufPos, buf.bufferLen() - nBufPos);
        nRet = ftrpc_unpack(buf1, m1);
        if(nRet < 0)
            break;
        nBufPos += ftrpc_pack_GetAlignLen(ftrpc_len_pack(m1));
        nRet = nBufPos;
    }while(false);
    return nRet;
}

#define __macro_unpack_member(n) \
            ftnetbuffer buf##n(buf.buffer() + nBufPos, buf.bufferLen() - nBufPos); \
            nRet = ftrpc_unpack(buf##n, m##n); \
            if(nRet < 0) \
                { nRet = -n; break; } \
            nBufPos += ftrpc_pack_GetAlignLen(ftrpc_len_pack(m##n)) + ftrpc_pack_GetAlignLen(ftrpc_len_pack_type_name(m##n));

#define __macro_unpack_member_1 __macro_unpack_member(1)
#define __macro_unpack_member_2 __macro_unpack_member_1 __macro_unpack_member(2)
#define __macro_unpack_member_3 __macro_unpack_member_2 __macro_unpack_member(3)
#define __macro_unpack_member_4 __macro_unpack_member_3 __macro_unpack_member(4)
#define __macro_unpack_member_5 __macro_unpack_member_4 __macro_unpack_member(5)
#define __macro_unpack_member_6 __macro_unpack_member_5 __macro_unpack_member(6)
#define __macro_unpack_member_7 __macro_unpack_member_6 __macro_unpack_member(7)
#define __macro_unpack_member_8 __macro_unpack_member_7 __macro_unpack_member(8)
#define __macro_unpack_member_9 __macro_unpack_member_8 __macro_unpack_member(9)

#define __ftrpc_DECLARE_UNPACK_STRUCT_TEMPLATE(n) \
    template <__ft_COMMON_DECLARE_LIST##n(_FK_CLASS, T)> \
    int ftrpc_unpack_struct(const ftnetbuffer &buf, const char *szTypeName, unsigned int nNameLen, __ft_COMMON_DECLARE_LIST##n(T, &m)) \
    { \
        int nRet = 0; \
        ftnetbuffer retBuf; \
        unsigned int nBufPos = 0; \
        do \
        { \
            unsigned int nNameRealLen = ftrpc_pack_GetAlignLen(nNameLen); \
            if(buf.bufferLen() < nNameRealLen) \
            { \
                nRet = -1; \
                break; \
            } \
            /* check type name */\
            int nIndex = 0; \
            for (nIndex = 0; nIndex < nNameRealLen; ++nIndex) \
            { \
                if((nIndex >= nNameLen && buf.buffer()[nIndex] == 0) || buf.buffer()[nIndex] != szTypeName[nIndex]) \
                { \
                    nRet = -2; \
                    break; \
                } \
            } \
            nBufPos += nNameRealLen; \
            /* unpack members */ \
            __macro_unpack_member_##n \
            nRet = nBufPos; \
        }while(false); \
        return nRet; \
    }

//__ftrpc_DECLARE_UNPACK_STRUCT_TEMPLATE(1)
__ftrpc_DECLARE_UNPACK_STRUCT_TEMPLATE(2)
__ftrpc_DECLARE_UNPACK_STRUCT_TEMPLATE(3)
__ftrpc_DECLARE_UNPACK_STRUCT_TEMPLATE(4)
__ftrpc_DECLARE_UNPACK_STRUCT_TEMPLATE(5)
__ftrpc_DECLARE_UNPACK_STRUCT_TEMPLATE(6)
__ftrpc_DECLARE_UNPACK_STRUCT_TEMPLATE(7)
__ftrpc_DECLARE_UNPACK_STRUCT_TEMPLATE(8)
__ftrpc_DECLARE_UNPACK_STRUCT_TEMPLATE(9)

#endif /* defined(__TestAllUseful__ftrpc_pack_container__) */
