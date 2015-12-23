//
//  ftnetbuffer.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/28.
//  Copyright (c) 2015�� liyoudi. All rights reserved.
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
    void Reset(); // ���ó�һ���µĻ���������
public:
    char * buffer();
    const char * buffer() const;
    unsigned int bufferLen() const;
public:
    ftnetbuffer Clone() const;
public:
    /***
     ����������������Ϊ��С��ֵ���Ա�֪����Ч���ݳ���Ϊ����
     �����Ƭ�������ڵ�ǰ��������С������󻺳�����������ʱ�������Ĳ������ݾͽ�Ϊ��������ˡ�
     */
    void CutToRealSize(unsigned int nRealSize);
    
    static unsigned int TotalInstanceCount();
protected:
    _ftnetbuffer_data *mpd;
};

ftnetbuffer operator + (const ftnetbuffer &b1, const ftnetbuffer &b2);

#endif /* defined(__TestAllUseful__ftnetbuffer__) */
