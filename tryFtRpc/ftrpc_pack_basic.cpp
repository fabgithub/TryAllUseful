//
//  ftrpc_pack_basic.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/3/2.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#include "ftrpc_pack_basic.h"

#include <stdlib.h>
#include <string.h>
#include <iostream>

#define _InfoLog(args) std::cout << "ftrpc.cpp (" << __LINE__ << "): " args << std::endl


// 所有的长度都要采用8字节对齐
unsigned int ftrpc_pack_GetAlignLen(size_t n)
{
    static size_t t = ~ ( (size_t) 0x07 );
	if ((n & 0x07) != 0)
	{
		n = (n & t) + 8;
	}
	return (unsigned int) n;
}

int ftrpc_pack_TypeName(ftnetbuffer &buf, const char *szTypeName, unsigned int nNameLen)
{
    int nRet = -1;
    do {
        unsigned int nRealLen = ftrpc_pack_GetAlignLen(nNameLen);
        if(buf.bufferLen() < nRealLen)
        {
            _InfoLog( << "buffer length is not enough." );
            break;
        }
        unsigned int nIndex = 0;
        for (; nIndex < nRealLen; ++nIndex)
        {
            buf.buffer()[nIndex] = (nIndex >= nNameLen) ? 0 : szTypeName[nIndex];
        }
        nRet = 0;
    } while (false);
    return nRet;
}

#define _fabrpc_IMPLEMENT_BASIC_TYPE_PACK(type) \
    unsigned int ftrpc_len_pack_type_name( const type &) { return (strlen(#type) + 1); } \
    unsigned int ftrpc_len_pack(const type &n) { return (sizeof(n)); } \
    ftnetbuffer ftrpc_pack(const type &n) \
    { \
    	static const char gszTypeName[36] = #type; \
		static unsigned int gnType_name_len = ((unsigned int) strlen(gszTypeName) + 1); \
		static unsigned int gnType_value_len = ftrpc_pack_GetAlignLen(sizeof(type)); \
		static unsigned int gnType_total_len = ftrpc_pack_GetAlignLen(gnType_name_len + gnType_value_len); \
    	ftnetbuffer buf(gnType_total_len); \
    	ftrpc_pack_TypeName(buf, gszTypeName, gnType_name_len); \
    	*( (type *) (buf.buffer() + ftrpc_pack_GetAlignLen(gnType_name_len)) ) = n; \
    	return buf; \
    } \
    int ftrpc_unpack(const ftnetbuffer &buf, type &n) \
    { \
    	static char gszTypeName[36] = #type; \
		static unsigned int gnType_name_len = ftrpc_pack_GetAlignLen((unsigned int) strlen(gszTypeName) + 1); \
		static unsigned int gnType_value_len = ftrpc_pack_GetAlignLen(sizeof(type)); \
		static unsigned int gnType_total_len = (gnType_name_len + gnType_value_len); \
		\
		int nRet = 0; \
		do \
		{ \
			if(buf.bufferLen() < gnType_total_len) /*长度不对*/ \
			{ \
				nRet = -1; _InfoLog( << "buffer len is error, buf len is " << buf.bufferLen() << ", it should be " << gnType_total_len ); \
				break; \
			} \
			int nIndex = 0; \
			for (nIndex = 0; nIndex < gnType_name_len; ++nIndex) \
			{ \
				if(buf.buffer()[nIndex] != gszTypeName[nIndex]) \
				{ \
					nRet = -2; _InfoLog( << "type name is wrong. " << gszTypeName << " " << buf.buffer() << ", nIndex is " << nIndex );\
					break; \
				} \
			} \
			if(nRet < 0) \
			{ \
				break; \
			} \
			n = *( (type *) (buf.buffer() + gnType_name_len) ); \
            nRet = gnType_total_len; \
		}while(false); \
		return nRet; \
    }

_fabrpc_IMPLEMENT_BASIC_TYPE_PACK(char)
_fabrpc_IMPLEMENT_BASIC_TYPE_PACK(unsigned char)
_fabrpc_IMPLEMENT_BASIC_TYPE_PACK(short)
_fabrpc_IMPLEMENT_BASIC_TYPE_PACK(unsigned short)
// _fabrpc_IMPLEMENT_BASIC_TYPE_PACK(int)
_fabrpc_IMPLEMENT_BASIC_TYPE_PACK(unsigned int)
_fabrpc_IMPLEMENT_BASIC_TYPE_PACK(long)
_fabrpc_IMPLEMENT_BASIC_TYPE_PACK(unsigned long)
_fabrpc_IMPLEMENT_BASIC_TYPE_PACK(long long)
_fabrpc_IMPLEMENT_BASIC_TYPE_PACK(unsigned long long)

_fabrpc_IMPLEMENT_BASIC_TYPE_PACK(float)
_fabrpc_IMPLEMENT_BASIC_TYPE_PACK(double)

// 返回类型名称打包时占用的字节数据
unsigned int ftrpc_len_pack_type_name(const char * /* only for type */ )
{
    static const char gszTypeName[36] = "char *";
    static unsigned int gnType_name_len = ((unsigned int) strlen(gszTypeName) + 1);
    return gnType_name_len;
}
unsigned int ftrpc_len_pack(const char *szStr)
{
    return ftrpc_pack_GetAlignLen(strlen(szStr) + 1);
}
ftnetbuffer ftrpc_pack(const char *szStr)
{
    static const char gszTypeName[36] = "char *";
    static unsigned int gnType_name_len = ((unsigned int) strlen(gszTypeName) + 1);
    unsigned int gnType_value_len = (unsigned int) strlen(szStr);
    unsigned int gnType_total_len = ftrpc_pack_GetAlignLen(gnType_name_len) + ftrpc_pack_GetAlignLen(gnType_value_len);
    ftnetbuffer buf(gnType_total_len);
    ftrpc_pack_TypeName(buf, gszTypeName, gnType_name_len);
    
    ftnetbuffer bufValue(buf.buffer() + ftrpc_pack_GetAlignLen(gnType_name_len), buf.bufferLen() - ftrpc_pack_GetAlignLen(gnType_name_len), false);
    ftrpc_pack_TypeName(bufValue, szStr, gnType_value_len);
    return buf;
}
int ftrpc_unpack(const ftnetbuffer &buf, const char *&szStr)
{
    static const char gszTypeName[36] = "char *";
    static unsigned int gnType_name_len = ((unsigned int) strlen(gszTypeName) + 1);
   
    int nRet = 0;
    do
    {
        if(buf.bufferLen() <= ftrpc_len_pack_type_name(szStr)) /*长度不对*/
        {
            nRet = -1; _InfoLog( << "buffer len is error, buf len is " << buf.bufferLen() << ", it should be larger than " << gnType_name_len );
            break;
        }
        int nIndex = 0;
        for (nIndex = 0; nIndex < gnType_name_len; ++nIndex)
        {
            if(buf.buffer()[nIndex] != gszTypeName[nIndex])
            {
                nRet = -2; _InfoLog( << "type '" << buf.buffer() << "' is wrong. it should be '" << gszTypeName << "', " << buf.buffer() << ", nIndex is " << nIndex );\
                break;
            }
        }
        if(nRet < 0)
        {
            break;
        }
        szStr = buf.buffer() + ftrpc_pack_GetAlignLen(gnType_name_len);
        nRet = ftrpc_pack_GetAlignLen(gnType_name_len) + ftrpc_pack_GetAlignLen(strlen(szStr) + 1);
    }while(false);
    return nRet;
}

unsigned int ftrpc_len_pack_type_name(const unsigned char * /* only for type */ )
{
    return 8;
}
unsigned int ftrpc_len_pack(const unsigned char *szStr)
{
    return ftrpc_pack_GetAlignLen(strlen((const char *) szStr) + 1);
}
ftnetbuffer ftrpc_pack(const unsigned char *szStr)
{
    return ftrpc_pack((const char *) szStr);
}
int ftrpc_unpack(const ftnetbuffer &buf, const unsigned char *&szStr)
{
    int nRet = 0;
    const char *szTmp = NULL;
    nRet = ftrpc_unpack(buf, szTmp);
    szStr = (const unsigned char *) szTmp;
    return nRet;
}

unsigned int ftrpc_len_pack_type_name(const ftnetbuffer /* only for type */ )
{
    static const char gszTypeName[36] = "ftnetbuffer";
    static unsigned int gnType_name_len = ((unsigned int) strlen(gszTypeName) + 1);
    return gnType_name_len;
}
unsigned int ftrpc_len_pack(const ftnetbuffer &inBuf)
{
    return inBuf.bufferLen() + 8; // 数据值长度 + 表示长度的整数int所占空间
}
ftnetbuffer ftrpc_pack(const ftnetbuffer &inBuf)
{
    static const char gszTypeName[36] = "ftnetbuffer";
    static unsigned int gnType_name_len = ((unsigned int) strlen(gszTypeName) + 1);
    unsigned int gnType_value_len = ftrpc_len_pack(inBuf);
    unsigned int gnType_total_len = ftrpc_pack_GetAlignLen(gnType_name_len) + ftrpc_pack_GetAlignLen(gnType_value_len);
    ftnetbuffer buf(gnType_total_len);
    ftrpc_pack_TypeName(buf, gszTypeName, gnType_name_len);
    
    unsigned int nBufPos = ftrpc_pack_GetAlignLen(gnType_name_len);
    *( (unsigned int *) (buf.buffer() + nBufPos) ) = inBuf.bufferLen();
    nBufPos += 4; //
    *( (unsigned int *) (buf.buffer() + nBufPos) ) = 0;
    nBufPos += 4; // 8字节对齐
    
    ftnetbuffer bufValue(buf.buffer() + nBufPos, buf.bufferLen() - nBufPos, false);
    ftrpc_pack_TypeName(bufValue, inBuf.buffer(), inBuf.bufferLen());
    return buf;
}
int ftrpc_unpack(const ftnetbuffer &buf, ftnetbuffer &outBuf)
{
    int nRet = 0;
    static const char gszTypeName[36] = "ftnetbuffer";
    static unsigned int gnType_name_len = ((unsigned int) strlen(gszTypeName) + 1);
    
    do
    {
        if(buf.bufferLen() <= ftrpc_len_pack_type_name(outBuf)) /*长度不对*/
        {
            nRet = -1; _InfoLog( << "buffer len is error, buf len is " << buf.bufferLen() << ", it should be larger than " << gnType_name_len );
            break;
        }
        int nIndex = 0;
        for (nIndex = 0; nIndex < gnType_name_len; ++nIndex)
        {
            if(buf.buffer()[nIndex] != gszTypeName[nIndex])
            {
                nRet = -2; _InfoLog( << "type '" << buf.buffer() << "' is wrong. it should be '" << gszTypeName << "', " << buf.buffer() << ", nIndex is " << nIndex );
                break;
            }
        }
        if(nRet < 0)
        {
            break;
        }
        unsigned int nBufPos = ftrpc_pack_GetAlignLen(gnType_name_len);
        unsigned int nBufLen = *( (unsigned int *) (buf.buffer() + nBufPos) );
        unsigned int nReserved = *( (unsigned int *) (buf.buffer() + nBufPos + 4) );
        nBufPos += 8;
        if(nBufLen + nBufPos > buf.bufferLen())
        {
            nRet = -3; _InfoLog( << "ftnetbuffer has too long size than the input data, format error" );
            break;
        }
        if(nReserved)
        {
            nRet = -4;
            _InfoLog( << "ftnetbuffer reserved value MUST be 0" );
            break;
        }
        ftnetbuffer tmpBuf((char *) buf.buffer() + nBufPos, nBufLen - nBufPos, false);
        outBuf = tmpBuf;
        
        nRet = ftrpc_pack_GetAlignLen(nBufLen + nBufPos);
    }while(false);
    return nRet;
}

// ----------------------------
// 获取类型名字符串的长度+1
unsigned int ftrpc_len_pack_type_name(const int & /* only for type */ )
{
    return 4;
}
// 获取实际数据值大小
unsigned int ftrpc_len_pack(const int &)
{
    return 4;
}
ftnetbuffer ftrpc_pack(const int &n)
{
	static const char gnintName[36] = "int";
	static unsigned int gnint_name_len = ftrpc_pack_GetAlignLen((unsigned int) strlen(gnintName) + 1);
	static unsigned int gnint_value_len = ftrpc_pack_GetAlignLen(sizeof(int));
	static unsigned int gnint_total_len = (gnint_name_len + gnint_value_len);
	ftnetbuffer buf(gnint_total_len);
	memset(buf.buffer(), 0, buf.bufferLen());
	strcpy(buf.buffer(), gnintName);
	memcpy(buf.buffer() + gnint_name_len, &n, sizeof(int));
	return buf;
}
int ftrpc_unpack(const ftnetbuffer &buf, int &n)
{
	static char gnintName[36] = "int";
	static unsigned int gnint_name_len = ftrpc_pack_GetAlignLen((unsigned int) strlen(gnintName) + 1);
	static unsigned int gnint_value_len = ftrpc_pack_GetAlignLen(sizeof(int));
	static unsigned int gnint_total_len = (gnint_name_len + gnint_value_len);

	int nRet = 0;
	do
	{
		if(buf.bufferLen() < gnint_total_len) /*长度不对*/
		{
			nRet = -1;
            _InfoLog( << "buffer len " << buf.bufferLen() << " is too small for int type, it should be larger than " << gnint_total_len );
			break;
		}
		int nIndex = 0;
		for (nIndex = 0; nIndex < gnint_name_len; ++nIndex)
		{
			if(buf.buffer()[nIndex] != gnintName[nIndex])
			{
				nRet = -2;
                _InfoLog( << "type name '" << buf.buffer() << "' is wrong, it should be '" << gnintName << "'" );
				break;
			}
		}
		if(nRet < 0)
		{
			break;
		}
		memcpy(&n, buf.buffer() + gnint_name_len, sizeof(int));
        nRet = gnint_total_len;
	}while(false);
	return nRet;
}