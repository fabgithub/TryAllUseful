//
//  tryChangeStackCall.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/10.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
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
    int Start();  // 如果返回9表示运行结束，返回1表示被yield
    int Resume(); // 如果返回9表示运行结束，返回1表示被yield
public:
    long long mllRsp;
    int mnState; // 当前状态, 0表示未运行过，1表示被yield, 2表示正在运行，9表示运行结束
    
    bool mbInitialised;
    std::string mstrName;
    
    FakeThreadProc mftp;
    void *mpParam;
};

StackItem * NewStackItem(const std::string &strName, FakeThreadProc ftp, void *pParam);


#endif /* defined(__TestAllUseful__tryChangeStackCall__) */
