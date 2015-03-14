//
//  ftrpc_pack_basic.h
//  TestAllUseful
//
//  Created by liyoudi on 15/3/2.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__ftrpc_pack_basic__
#define __TestAllUseful__ftrpc_pack_basic__

#include "ftnetbuffer.h"
#include "FakeThreadTemplate.h"

#include <stdio.h>
#include <vector>

unsigned int ftrpc_pack_GetAlignLen(size_t n);
int ftrpc_pack_TypeName(ftnetbuffer &buf, const char *szTypeName, unsigned int nNameLen);

// 用于远程调用时传输参数和返回值
typedef std::vector<ftnetbuffer> _ftBufArray;

#define ftrpc_DECLARE_TYPE_PACK(type) \
    unsigned int ftrpc_len_pack_type_name(const type &); \
    ftnetbuffer ftrpc_pack(const type &n); \
    unsigned int ftrpc_len_pack(const type &n); \
    int ftrpc_unpack(const ftnetbuffer &buf, type &n);

ftrpc_DECLARE_TYPE_PACK(char)
ftrpc_DECLARE_TYPE_PACK(unsigned char)
ftrpc_DECLARE_TYPE_PACK(short)
ftrpc_DECLARE_TYPE_PACK(unsigned short)
ftrpc_DECLARE_TYPE_PACK(int)
ftrpc_DECLARE_TYPE_PACK(unsigned int)
ftrpc_DECLARE_TYPE_PACK(long)
ftrpc_DECLARE_TYPE_PACK(unsigned long)
ftrpc_DECLARE_TYPE_PACK(long long)
ftrpc_DECLARE_TYPE_PACK(unsigned long long)

ftrpc_DECLARE_TYPE_PACK(float)
ftrpc_DECLARE_TYPE_PACK(double)

unsigned int ftrpc_len_pack_type_name(const char * /* only for type */ );
unsigned int ftrpc_len_pack(const char *szStr);
ftnetbuffer ftrpc_pack(const char *szStr);
int ftrpc_unpack(const ftnetbuffer &buf, const char *&szStr);

unsigned int ftrpc_len_pack_type_name(const unsigned char * /* only for type */ );
unsigned int ftrpc_len_pack(const unsigned char *szStr);
ftnetbuffer ftrpc_pack(const unsigned char *szStr);
int ftrpc_unpack(const ftnetbuffer &buf, const unsigned char *&szStr);

unsigned int ftrpc_len_pack_type_name(const ftnetbuffer /* only for type */ );
unsigned int ftrpc_len_pack(const ftnetbuffer &szStr);
ftnetbuffer ftrpc_pack(const ftnetbuffer &szStr);
int ftrpc_unpack(const ftnetbuffer &buf, ftnetbuffer &outBuf);

#endif /* defined(__TestAllUseful__ftrpc_pack_basic__) */
