//
//  tryChangeStackCall.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/10.
//  Copyright (c) 2015�� liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__tryChangeStackCall__
#define __TestAllUseful__tryChangeStackCall__

#include "FakeThreadWrap.h"

#include <string>
#include <stdio.h>


void tryChangeStackCall();

extern "C" long long GetCurStack();
extern "C" long long GetSS();
extern "C" long long GetRSP();
extern "C" long long ReturnParam(long long);
extern "C" long long GetAtRsp(long long);

const char * NFGetFileNameFromPath(const char *szFilePath);

#define InfoLog( args ) std::cout << " " << NFGetFileNameFromPath(__FILE__) << "(" << __LINE__ << ") " << __FUNCTION__ << ": " args << std::endl


class StackItem
{
public:
    StackItem(const std::string &strName, FakeThreadProc ftp, void *pParam)
    : mllRsp(0), mnState(0), mbInitialised(false), mstrName(strName)
    , mftp(ftp), mpParam(pParam)
    {}
public:
    int Start();  // �������9��ʾ���н���������1��ʾ��yield
    int Resume(); // �������9��ʾ���н���������1��ʾ��yield
public:
    long long mllRsp;
    int mnState; // ��ǰ״̬, 0��ʾδ���й���1��ʾ��yield, 2��ʾ�������У�9��ʾ���н���
    
    bool mbInitialised;
    std::string mstrName;
    
    FakeThreadProc mftp;
    void *mpParam;
};

StackItem * NewStackItem(const std::string &strName, FakeThreadProc ftp, void *pParam);


#endif /* defined(__TestAllUseful__tryChangeStackCall__) */
