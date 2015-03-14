//
//  ftnetbuffer.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/28.
//  Copyright (c) 2015�� liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__ftnetbuffer__
#define __TestAllUseful__ftnetbuffer__

#include <stdio.h>

class _ftnetbuffer_data;

class ftnetbuffer
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
    
public:
    void AttachBuffer(void *pBuf, unsigned int nBufLen, bool bDeleteIn_ftnetbuffer);
    void Reset(); // ���ó�һ���µĻ���������
public:
    char * buffer();
    const char * buffer() const;
    unsigned int bufferLen() const;
    
    static unsigned int TotalInstanceCount();
protected:
    _ftnetbuffer_data *mpd;
};

ftnetbuffer operator + (const ftnetbuffer &b1, const ftnetbuffer &b2);

#endif /* defined(__TestAllUseful__ftnetbuffer__) */
