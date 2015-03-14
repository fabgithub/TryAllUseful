//
//  ftnetselect.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/2/27.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#include "ftnetselect.h"

#include <map>
#include <sys/time.h>

#define _MONITOR_EXCEPT_EVENT 0 // 是否监测except事件

class CbInfo
{
public:
    CbInfo(ftEventCallbackFn cb = NULL, void *pParam = NULL, unsigned int nTimeoutSecond = 0)
    : mcb(cb), mpParam(pParam), mtStart(0), mnTimeoutSecond(nTimeoutSecond)
    {
        if(nTimeoutSecond > 0)
        {
            mtStart = ::time(NULL);
        }
    }
    ftEventCallbackFn mcb;
    void *mpParam;
    
    time_t mtStart;
    unsigned int mnTimeoutSecond; // 超时秒数, 若为0,则永不超时
    
    bool IsTimeout(time_t tNow)
    {
        if (mnTimeoutSecond > 0 && tNow < mtStart + mnTimeoutSecond)
        {
            // TODO: 检查下是否有可能是系统时间被修改了，如果是系统时间被修改了，则重新设置定时器
            mcb(-1, mpParam);
            return true;
        }
        return false;
    }
};

typedef std::map<SOCKET, CbInfo> SockToCbInfoMap;

static SockToCbInfoMap gForReadMap[2];
static SockToCbInfoMap gForWriteMap[2];
#if _MONITOR_EXCEPT_EVENT
static SockToCbInfoMap gForExceptMap[2];
#endif

static unsigned int gnIndex = 0;

static int _ftnetAddReadFd_select(SOCKET sock, const CbInfo &ci)
{
    int nRet = 0;
    if(!gForReadMap[gnIndex].insert(std::make_pair(sock, ci)).second)
    {
        nRet = -1;
    }
#if _MONITOR_EXCEPT_EVENT
    else
    {
        gForExceptMap[gnIndex].insert(std::make_pair(sock, ci));
    }
#endif
    return nRet;
}
int _ftnetAddReadFd_select(SOCKET sock, ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond)
{
    CbInfo ci(fn, pParam, nTimeoutSecond);
    return _ftnetAddReadFd_select(sock, ci);
}
static int _ftnetAddWriteFd_select(SOCKET sock, const CbInfo &ci)
{
    int nRet = 0;
    if(!gForWriteMap[gnIndex].insert(std::make_pair(sock, ci)).second)
    {
        nRet = -1;
    }
#if _MONITOR_EXCEPT_EVENT
    else
    {
        gForExceptMap[gnIndex].insert(std::make_pair(sock, ci));
    }
#endif
    return nRet;
}
int _ftnetAddWriteFd_select(SOCKET sock, ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond)
{
    CbInfo ci(fn, pParam, nTimeoutSecond);
    return _ftnetAddWriteFd_select(sock, ci);
}
int ftnetSelect(unsigned int nTimeoutMs)
{
    int nRet = 0;
    int nCurIndex = gnIndex;
    int nMaxFd = 0;
    gnIndex = nCurIndex == 0 ? 1 : 0;
    
    fd_set fsRead, fsWrite, fsExcept;
    FD_ZERO(&fsRead);
    FD_ZERO(&fsWrite);
    FD_ZERO(&fsExcept);
    SockToCbInfoMap::iterator iter;
    for (iter = gForReadMap[nCurIndex].begin(); gForReadMap[nCurIndex].end() != iter; ++iter)
    {
        FD_SET(iter->first, &fsRead);
        if(iter->first > nMaxFd)
        {
            nMaxFd = iter->first;
        }
    }
    for (iter = gForWriteMap[nCurIndex].begin(); gForWriteMap[nCurIndex].end() != iter; ++iter)
    {
        FD_SET(iter->first, &fsWrite);
        if(iter->first > nMaxFd)
        {
            nMaxFd = iter->first;
        }
    }
#if _MONITOR_EXCEPT_EVENT
    for (iter = gForExceptMap[nCurIndex].begin(); gForExceptMap[nCurIndex].end() != iter; ++iter)
    {
        FD_SET(iter->first, &fsExcept);
        if(iter->first > nMaxFd)
        {
            nMaxFd = iter->first;
        }
    }
#endif
    //
    if (nTimeoutMs > 100) {
        nTimeoutMs = 100;
    }
    //
    time_t tCurrentTime;
    time(&tCurrentTime);
    //
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = (nTimeoutMs % 1000) * 1000;
    if(nMaxFd)
    {
        nRet = select(nMaxFd + 1, &fsRead, &fsWrite, &fsExcept, &tv);
    }
    else
    {
        nRet = select(nMaxFd + 1, NULL, NULL, NULL, &tv);
    }
    //
    if(nRet > 0) // 当有网络事件时才检测
    {
        for (iter = gForReadMap[nCurIndex].begin(); gForReadMap[nCurIndex].end() != iter; ++iter)
        {
            if(FD_ISSET(iter->first, &fsRead) || FD_ISSET(iter->first, &fsExcept))
            {
                iter->second.mcb(0, iter->second.mpParam);
            }
            else
            {
                if(!iter->second.IsTimeout(tCurrentTime))
                {
                    _ftnetAddReadFd_select(iter->first, iter->second);
                }
            }
        }
        for (iter = gForWriteMap[nCurIndex].begin(); gForWriteMap[nCurIndex].end() != iter; ++iter)
        {
            if(FD_ISSET(iter->first, &fsWrite) || FD_ISSET(iter->first, &fsExcept))
            {
                iter->second.mcb(0, iter->second.mpParam);
            }
            else
            {
                if(!iter->second.IsTimeout(tCurrentTime))
                {
                    _ftnetAddWriteFd_select(iter->first, iter->second);
                }
            }
        }
        //
        gForWriteMap[nCurIndex].clear();
        gForReadMap[nCurIndex].clear();
#if _MONITOR_EXCEPT_EVENT
        gForExceptMap[nCurIndex].clear();
#endif
    }
    else // 没有网络事件的话，就换回之前的index
    {
        gnIndex = (gnIndex == 0 ? 1 : 0);
    }
    //
    return nRet;
}
