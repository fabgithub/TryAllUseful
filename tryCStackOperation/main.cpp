//
//  main.cpp
//  tryCStackOperation
//
//  Created by liyoudi on 15/2/6.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//

#include "tryChangeStackCall.h"
#include "FakeThreadWrap.h"

#include <iostream>
#include <unistd.h>

#include <pthread.h>

long long llLastRsp = 0;

int SomeFunc()
{
    return 1;
}

#define ShowInfo() std::cout << "in " << __FUNCTION__ << " " << std::hex << "ss is 0x" << GetSS() << ", rsp is 0x" << GetRSP();\
            std::cout << ", the diff of rsp is 0x" << GetRSP() - llLastRsp << std::endl; \
            llLastRsp = GetRSP(); \
            thread_ss = 1

long long thread_ss = NULL;

long long lastRsp = 0;

void ShowStackSize()
{
    pthread_attr_t pattr;
    pthread_attr_init(&pattr);
    size_t size = 0;
    int nRes = pthread_attr_getstacksize(&pattr, &size);
    if(nRes)
    {
        std::cout << "get stack returns " << std::dec << nRes << ", errno is " << errno << ": " << strerror(nRes) << std::endl;
    }
    printf("current thread stack size:0x%x\n", (int) size);
}

size_t GetCurThreadStackSize()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    size_t nRetSize;
    void *pAddr = NULL;
    int nRes = pthread_attr_getstack(&attr, &pAddr, &nRetSize);
    if(nRes)
    {
        std::cout << "pthread_attr_getstack returns " << std::dec << nRes << ", errno is " << errno << ": " << strerror(nRes) << std::endl;
    }

    std::cout << std::endl;
    std::cout << "stack address is " << pAddr << ", size is 0x" << nRetSize << ", GetRSP returns 0x" << GetRSP() << std::endl;
//    std::cout << "first of stack is " << *((long long *) pAddr) << std::endl;
    std::cout << std::endl;
    return nRetSize;
}

static void * try_stack_proc(void *)
{
    ShowStackSize();
    size_t nSize = GetCurThreadStackSize();
    std::cout << std::hex << "GetCurThreadStackSize() returns 0x" << nSize << std::endl;
    ShowInfo();
    lastRsp = GetRSP();
    sleep(100);
    return NULL;
}

static int gnStackSize = 0x900000;
static char *pNewStack = NULL;
void NewSSByThread()
{
    pNewStack = new char[gnStackSize];
    memset(pNewStack, 0, gnStackSize);
    
    pthread_t thr;
    thread_ss = NULL;
    pthread_attr_t *pAttr = NULL;
    pthread_attr_t attr;
    if(pNewStack)
    {
        pthread_attr_init(&attr);
        int nRes = 0;
//    nRes = pthread_attr_setstacksize(&attr, gnStackSize);
        nRes = pthread_attr_setstack(&attr, pNewStack, gnStackSize);
        if(nRes)
        {
            std::cout << "set stack returns " << std::dec << nRes << ", errno is " << errno << ": " << strerror(nRes) << std::endl;
        }
        pAttr = &attr;
    }
    pthread_create(&thr, pAttr, try_stack_proc, NULL);
    while(!thread_ss)
    {
//        std::cout << "something for debug" << std::endl;
        usleep(100 * 1000);
    }
}

int tryCStackOperation() {
    // insert code here...
    long long n = 0x5555555;

    
    long long res = ReturnParam(n);
    std::cout << "ReturnParam(" << n << ") = " << res << std::endl;
    

    
    n = GetSS();
    std::cout << "GetSS() returns 0x" << std::hex << n << std::endl;
    std::cout << "" << std::dec << n / 1024 / 1024 << " MB" << std::endl;
    
    NewSSByThread();
    NewSSByThread();
    NewSSByThread();
    NewSSByThread();
//    NewSSByThread();
//    NewSSByThread();
//    NewSSByThread();
    ShowInfo();
    
    ShowStackSize();
    
    long long tmp = 0;
    long long nTimes = 500;
    for(int nIndex = 0;nIndex <= nTimes;nIndex += 8)
    {
        tmp = GetAtRsp(lastRsp + nIndex);
        std::cout << "" << tmp << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    for(int nIndex = 0;nIndex <= nTimes;nIndex += 8)
    {
        tmp = GetAtRsp(lastRsp + nIndex);
        std::cout << "" << tmp << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    if(pNewStack)
    {
        for(int nIndex = 0;nIndex <= gnStackSize - 8;nIndex += 8)
        {
            tmp = *((long long *) (pNewStack - 8 + nIndex) );
            if(tmp)
            {
                std::cout << "" << tmp << " ";
            }
        }
        std::cout << std::endl;
    }
    GetCurThreadStackSize();
    return 0;
}

int testFakeThreadWrap_main( int argc, char *argv[] );

int main(int argc, char * argv[])
{
    std::cout << "Hello world." << std::endl;
    tryChangeStackCall();
    std::cout << std::endl;
    std::cout << "****************************" << std::endl;
    std::cout << std::endl;
    testFakeThreadWrap_main(argc, argv);
    return 0;
}
