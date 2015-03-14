//
//  ftnetbuffer.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/2/28.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#include "ftnetbuffer.h"

#include <stdlib.h>
#include <string.h>

static unsigned int gnTotalBufferInstanceCount = 0;

class _ftnetbuffer_data
{
    _ftnetbuffer_data(const _ftnetbuffer_data &);
    _ftnetbuffer_data & operator = (const _ftnetbuffer_data &);
public:
    _ftnetbuffer_data()
    : mpBuf(NULL), mnBufLen(0), mbNeedDelete(false)
    , mnRefCount(0)
    {
        ++gnTotalBufferInstanceCount;
    }
    ~_ftnetbuffer_data()
    {
        --gnTotalBufferInstanceCount;
        Free();
    }
    void Free()
    {
        if(mbNeedDelete)
        {
            delete [] mpBuf;
            mbNeedDelete = false;
        }
        mpBuf = NULL;
        mnBufLen = 0;
    }
    void SetSize(unsigned int nSize)
    {
        Free();
        if(nSize > 0)
        {
            mpBuf = new char[nSize + 1];
            mnBufLen = nSize;
            mpBuf[nSize] = 0;
            mbNeedDelete = true;
        }
    }
public:
    int IncreaseRef()
    {
        ++mnRefCount;
        return mnRefCount;
    }
    int DecreaseRef()
    {
        --mnRefCount;
        if(mnRefCount <= 0)
        {
            delete this;
            return 0;
        }
        return mnRefCount;
    }
public:
    char *mpBuf;
    unsigned int mnBufLen;
    bool mbNeedDelete;
    
    int mnRefCount;
};

ftnetbuffer::ftnetbuffer()
{
    mpd = new _ftnetbuffer_data();
    mpd->IncreaseRef();
}
ftnetbuffer::ftnetbuffer(unsigned int nSize)
{
    mpd = new _ftnetbuffer_data();
    mpd->SetSize(nSize);
    
    mpd->IncreaseRef();
}
ftnetbuffer::ftnetbuffer(void *pBuf, unsigned int nBufLen, bool bDeleteIn_ftnetbuffer)
{
    mpd = new _ftnetbuffer_data();
    mpd->mpBuf = (char *) pBuf;
    mpd->mnBufLen = nBufLen;
    mpd->mbNeedDelete = bDeleteIn_ftnetbuffer;
    
    mpd->IncreaseRef();
}
ftnetbuffer::ftnetbuffer(const void *pBuf, unsigned int nBufLen)
{
    mpd = new _ftnetbuffer_data();
    mpd->mpBuf = new char[nBufLen + 1];
    mpd->mnBufLen = nBufLen;
    memcpy(mpd->mpBuf, pBuf, nBufLen);
    mpd->mpBuf[nBufLen] = 0;
    mpd->mbNeedDelete = true;
    
    mpd->IncreaseRef();}
ftnetbuffer::ftnetbuffer(const ftnetbuffer &buf)
{
    mpd = buf.mpd;
    mpd->IncreaseRef();
}
ftnetbuffer::~ftnetbuffer()
{
    mpd->DecreaseRef();
}
void ftnetbuffer::AttachBuffer(void *pBuf, unsigned int nBufLen, bool bDeleteIn_ftnetbuffer)
{
    mpd->Free();
    
    mpd->mpBuf = (char *) pBuf;
    mpd->mnBufLen = nBufLen;
    mpd->mbNeedDelete = bDeleteIn_ftnetbuffer;
}
void ftnetbuffer::Reset() // 重置成一个新的缓冲区对象
{
    mpd->DecreaseRef();
    mpd = new _ftnetbuffer_data();
    mpd->IncreaseRef();
}

ftnetbuffer & ftnetbuffer::operator = (const ftnetbuffer &buf)
{
    mpd->DecreaseRef();
    mpd = buf.mpd;
    mpd->IncreaseRef();
    return *this;
}
bool ftnetbuffer::operator ! () const
{
    return !mpd->mpBuf;
}
char * ftnetbuffer::buffer()
{
    return mpd->mpBuf;
}
const char * ftnetbuffer::buffer() const
{
    return mpd->mpBuf;
}
unsigned int ftnetbuffer::bufferLen() const
{
    return mpd->mnBufLen;
}
unsigned int ftnetbuffer::TotalInstanceCount()
{
    return gnTotalBufferInstanceCount;
}

ftnetbuffer operator + (const ftnetbuffer &b1, const ftnetbuffer &b2)
{
    ftnetbuffer buf(b1.bufferLen() + b2.bufferLen());
    memcpy(buf.buffer(), b1.buffer(), b1.bufferLen());
    memcpy(buf.buffer() + b1.bufferLen(), b2.buffer(), b2.bufferLen());
    return buf;
}
