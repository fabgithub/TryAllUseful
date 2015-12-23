//
//  ftnetbuffer.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/28.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__ftnetbuffer__
#define __TestAllUseful__ftnetbuffer__

#include "ftnet_export.h"

#include <stdio.h>

class _ftnetbuffer_data;

class FTNET_API ftnetbuffer
{
public:
    ftnetbuffer();
    explicit ftnetbuffer(unsigned int nSize);
    ftnetbuffer(void *pBuf, unsigned int nBufLen, bool bDeleteIn_ftnetbuffer);
    ftnetbuffer(const void *pBuf, unsigned int nBufLen);
    ftnetbuffer(const ftnetbuffer &buf);
    ~ftnetbuffer();
    
    ftnetbuffer & operator = (const ftnetbuffer &buf);
    bool operator ! () const;
    bool operator < (const ftnetbuffer &buf) const;
    char & operator [] (unsigned int nIndex);
    char operator [] (unsigned int nIndex) const;
public:
    void AttachBuffer(void *pBuf, unsigned int nBufLen, bool bDeleteIn_ftnetbuffer);
    void Reset(); // 重置成一个新的缓冲区对象
public:
    char * buffer();
    const char * buffer() const;
    unsigned int bufferLen() const;
public:
    ftnetbuffer Clone() const;
public:
    /***
     将缓冲区长度设置为较小的值，以便知道有效数据长度为多少
     如果传片参数大于当前缓冲区大小，则调大缓冲区，不过这时后面多出的部分内容就仅为随机数据了。
     */
    void CutToRealSize(unsigned int nRealSize);
    
    static unsigned int TotalInstanceCount();
protected:
    _ftnetbuffer_data *mpd;
};

ftnetbuffer operator + (const ftnetbuffer &b1, const ftnetbuffer &b2);

#endif /* defined(__TestAllUseful__ftnetbuffer__) */
