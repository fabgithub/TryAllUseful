//
//  tryChangeStackCall.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/2/10.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#include "tryChangeStackCall.h"

#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <string>

#define InfoLog( args ) std::cout << " " << NFGetFileNameFromPath(__FILE__) << "(" << __LINE__ << ") " << __FUNCTION__ << ": " args << std::endl

#ifdef WIN32
#define SLASH_CHAR '\\'
#else
#define SLASH_CHAR '/'
#endif

static long long gllMainRsp = 0;

class StackItem;
// ==============
extern "C" StackItem * StartWithRsp(long long llNewRsp, void (*pfunc) (), StackItem *pItem, long long *pSaveMainRsp );
extern "C" void YieldWithMainRsp(long long llMainRsp, long long *pThreadRsp);
extern "C" StackItem * GetItemPtr(long long llMainRsp);
extern "C" void ResumeWithThreadRsp(long long llThreadRsp, long long *pSaveMainRsp, StackItem *pItem);

extern "C" void TryUseMemForStack(void *pStackAddr);
extern "C" long long TryPointerToLongLong(void *ptr);
// ==============

const char * NFGetFileNameFromPath(const char *szFilePath)
{
    const char *chRet = szFilePath;
    if(szFilePath)
    {
        while(*szFilePath)
        {
            if (SLASH_CHAR == *szFilePath)
            {
                chRet = szFilePath + 1;
            }
            ++szFilePath;
        }
    }
    return chRet;
}

extern "C" void SomeFuncInFakeThread()
{
    StackItem *pItem = GetItemPtr(gllMainRsp);
    InfoLog( << pItem << " with name " << pItem->mstrName );
}

extern "C" void SomeFuncInFakeThread__2()
{
    StackItem *pItem = GetItemPtr(gllMainRsp);
    InfoLog( << pItem << " with name " << pItem->mstrName );
}

static void ThisFuncRunInFakeThread(void *pParam)
{
    SomeFuncInFakeThread();
    StackItem *pItem = GetItemPtr(gllMainRsp);
    YieldWithMainRsp(gllMainRsp, &pItem->mllRsp);
    SomeFuncInFakeThread__2();
}

static void FuncInFakeThread()
{
    StackItem *pItem = GetItemPtr(gllMainRsp);
    pItem->mftp(pItem->mpParam);
    YieldWithMainRsp(gllMainRsp, &pItem->mllRsp);
}

int demofunc()
{
    printf("");
    return 5;
}

int StackItem::Start()  // 如果返回9表示运行结束，返回1表示被yield
{
    // 保存寄存器状态
    // 切换栈
    StackItem *pItem = NULL;
//    InfoLog( << "before StartWithRsp: mllRsp = " << mllRsp );
    pItem = StartWithRsp(mllRsp, FuncInFakeThread, this, &gllMainRsp);
//    InfoLog( << "(pItem == this) is " << (pItem == this) );
    // 调用函数
    // 恢复寄存器状态
//    InfoLog( << "after StartWithRsp: mllRsp = " << mllRsp );
    mnState = 9;
    return 9;
}
int StackItem::Resume() // 如果返回9表示运行结束，返回1表示被yield
{
    ResumeWithThreadRsp(mllRsp, &gllMainRsp, this);
    InfoLog( << "after ResumeWithThreadRsp" );
    return 0;
}

static void * try_stack_proc(void *pParam)
{
    StackItem *pSI = (StackItem *) pParam;
    pSI->mllRsp = GetRSP() - 1000;
    pSI->mbInitialised = true;
    sleep(100000000);
    return NULL;
}
/***
 分配一个新栈
 */
StackItem * NewStackItem(const std::string &strName, FakeThreadProc ftp, void *pParam)
{
    StackItem *pRet = new StackItem(strName, ftp, pParam);
    pthread_t thr;
    pthread_create(&thr, NULL, try_stack_proc, pRet);
    while(!pRet->mbInitialised)
    {
        //        std::cout << "something for debug" << std::endl;
        usleep(10 * 1000);
    }
    return pRet;
}

void tryChangeStackCall()
{
    StackItem * pSI1 = NULL;
    pSI1 = NewStackItem("first", ThisFuncRunInFakeThread, pSI1);
    pSI1->Start();

    StackItem * pSI2 = NULL;
    pSI2 = NewStackItem("second", ThisFuncRunInFakeThread, pSI2);
    pSI2->Start();

    pSI2->Resume();
    pSI1->Resume();

    printf("-------\n");
    int bufSize = 40;
    char *buf = new char [bufSize];
    TryUseMemForStack(buf + bufSize);
    printf("-------\n");
    
    long long ll = TryPointerToLongLong(buf);
    std::cout << "ll is " << ll << std::endl;
    std::cout << "buf to long long is " << (long long) buf << std::endl;
    delete []buf;
}
