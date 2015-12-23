//
//  ftnetselect.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/2/27.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#include "ftnetselect.h"

#include <map>
#include <list>

#include <time.h>
#ifdef WIN32
#else
#include <sys/time.h>
#endif

#define _MONITOR_EXCEPT_EVENT 0 // 是否监测except事件

static bool gbExitFtNet = false;

class CbInfo
{
public:
    CbInfo(ftEventCallbackFn cb = NULL, void *pParam = NULL, unsigned int nTimeoutSecond = 0)
    : mcb(cb), mpParam(pParam), mtStart(0), mnTimeoutSecond(nTimeoutSecond), mbForceExit(false)
    {
        if(nTimeoutSecond > 0)
        {
            mtStart = ::time(NULL);
        }
    }
    void CallFn(int nErrCode)
    {
        if(mcb)
        {
            mcb(nErrCode, mpParam);
        }
    }
    ftEventCallbackFn mcb;
    void *mpParam;
    
    time_t mtStart;
    unsigned int mnTimeoutSecond; // 超时秒数, 若为0,则永不超时
    
    bool mbForceExit; // 当关闭时，可以直接取消事件等待
    
    bool IsTimeout(time_t tNow)
    {
        if (mnTimeoutSecond > 0 && tNow > mtStart + mnTimeoutSecond)
        {
            // TODO: 检查下是否有可能是系统时间被修改了，如果是系统时间被修改了，则重新设置定时器
            CallFn(-1);
            return true;
        }
        else if(mbForceExit)
        {
            CallFn(-99);
            return true;
        }
        return false;
    }
};

typedef std::map<SOCKET, CbInfo> SockToCbInfoMap;
typedef std::list<SOCKET> SockList;

static SockToCbInfoMap gForReadMap[2];
static SockToCbInfoMap gForWriteMap[2];
#if _MONITOR_EXCEPT_EVENT
static SockToCbInfoMap gForExceptMap[2];
#endif

static SockList gReadSockList[2];
static SockList gWriteSockList[2];

static unsigned int gnIndex = 0;

static int _ftnetAddReadFd_select(SOCKET sock, const CbInfo &ci)
{
    if(gbExitFtNet)
    {
        return -1;
    }
    int nRet = 0;
    std::pair<SockToCbInfoMap::iterator, bool> ib;
    ib = gForReadMap[gnIndex].insert(std::make_pair(sock, ci));
    if(!ib.second)
    {
        if(ib.first->second.mcb)
        {
            nRet = -1;
        }
        ib.first->second = ci;
    }
    else
    {
        gReadSockList[gnIndex].push_back(sock);
#if _MONITOR_EXCEPT_EVENT
        gForExceptMap[gnIndex][sock] = ci;
#endif
    }
    return nRet;
}
int _ftnetAddReadFd_select(SOCKET sock, ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond)
{
    CbInfo ci(fn, pParam, nTimeoutSecond);
    return _ftnetAddReadFd_select(sock, ci);
}
static int _ftnetAddWriteFd_select(SOCKET sock, const CbInfo &ci)
{
    if(gbExitFtNet)
    {
        return -1;
    }
    int nRet = 0;
    std::pair<SockToCbInfoMap::iterator, bool> ib;
    ib = gForWriteMap[gnIndex].insert(std::make_pair(sock, ci));
    if(!ib.second)
    {
        if(ib.first->second.mcb)
        {
            nRet = -1;
        }
        ib.first->second = ci;
    }
    else
    {
        gWriteSockList[gnIndex].push_back(sock);
#if _MONITOR_EXCEPT_EVENT
        gForExceptMap[gnIndex][sock] = ci;
#endif
    }
    return nRet;
}
void _ftnetCancel_select(SOCKET sock)
{
    int n = 0;
    SockToCbInfoMap *si[4] = {gForReadMap, gForWriteMap, gForReadMap + 1, gForWriteMap + 1};
    for (n = 0; n < 4; ++n)
    {
        SockToCbInfoMap::iterator iter;
        iter = si[n]->find(sock);
        if(iter != si[n]->end())
        {
            iter->second.mbForceExit = true;
        }
    }
}
int _ftnetOnFdClosed_select(SOCKET sock)
{
    if(gbExitFtNet)
    {
        return -1;
    }
    int nRet = 0;
    int nIndex = 0;
    bool bFound = false;
    nIndex = gnIndex;
    SockToCbInfoMap::iterator iterRead, iterWrite;
#if _MONITOR_EXCEPT_EVENT
    SockToCbInfoMap::iterator iterExcept;
#endif
    for (nIndex = 0; nIndex < 2; ++nIndex)
    {
        iterRead = gForReadMap[nIndex].find(sock);
        if(iterRead != gForReadMap[nIndex].end())
        {
            bFound = true;
            iterRead->second.mcb = NULL;
        }
        iterWrite = gForWriteMap[nIndex].find(sock);
        if(iterWrite != gForWriteMap[nIndex].end())
       {
           bFound = true;
           iterWrite->second.mcb = NULL;
       }
#if _MONITOR_EXCEPT_EVENT
        iterExcept = gForExceptMap[nIndex].find(sock);
        if(iterExcept != gForExceptMap[nIndex].end())
        {
            bFound = true;
            iterExcept->second.mcb = NULL;
        }
#endif
    }
    if(bFound)
    {
        nRet = -1;
    }
    return nRet;
}
int _ftnetAddWriteFd_select(SOCKET sock, ftEventCallbackFn fn, void *pParam, unsigned int nTimeoutSecond)
{
    CbInfo ci(fn, pParam, nTimeoutSecond);
    return _ftnetAddWriteFd_select(sock, ci);
}

static unsigned int gnMaxSelectFdCount = 300;
void ftnet_set_max_select_fd_count(unsigned int nMax)
{
    if(nMax > 800)
    {
        nMax = 800;
    }
    gnMaxSelectFdCount = nMax;
}
unsigned int ftnet_get_max_select_fd_count(unsigned int nMax)
{
    return gnMaxSelectFdCount;
}

static void SetFdSetHelper(SockToCbInfoMap *pWhichMap, SockList *pSockList, int nCurIndex, fd_set &fds, int &nMaxFd, unsigned int &nOutTimeoutMs, time_t tCurrentTime, int (fnAddFd) (SOCKET sock, const CbInfo &ci))
{
    int nAddCount = 0;
    FD_ZERO(&fds);
    SockList tmpList;
    SockList::iterator iterList, iterNext;
    SockToCbInfoMap::iterator iterMap;
    for (iterList = pSockList[nCurIndex].begin(); pSockList[nCurIndex].end() != iterList; iterList = iterNext)
    {
        iterNext = iterList;
        ++iterNext;
        iterMap = pWhichMap[nCurIndex].find(*iterList);
        if(iterMap == pWhichMap[nCurIndex].end())
        {
            pSockList[nCurIndex].erase(iterList);
            continue;
        }
        if(!iterMap->second.mcb || iterMap->second.IsTimeout(tCurrentTime))
        {
            pWhichMap[nCurIndex].erase(iterMap);
            continue;
        }
        if(nAddCount >= gnMaxSelectFdCount)
        {
            nOutTimeoutMs = 5;
            if(fnAddFd)
            {
                fnAddFd(iterMap->first, iterMap->second);
            }
            pWhichMap[nCurIndex].erase(iterMap);
            continue;
        }
        ++nAddCount;
        FD_SET(iterMap->first, &fds);
#ifndef WIN32
        if(iterMap->first > nMaxFd)
        {
            nMaxFd = iterMap->first;
        }
#endif
    }
    pSockList[nCurIndex].clear();
}
static void DoCheckFdSetHelper(SockToCbInfoMap *pWhichMap, int nCurIndex, fd_set &fds, fd_set &fsExcept, time_t tCurrentTime, int (fnAddFd) (SOCKET sock, const CbInfo &ci))
{
    SockToCbInfoMap::iterator iter, iterNext;
    for (iter = pWhichMap[nCurIndex].begin(); pWhichMap[nCurIndex].end() != iter; iter = iterNext)
    {
        iterNext = iter;
        ++iterNext;
        if(FD_ISSET(iter->first, &fds) || FD_ISSET(iter->first, &fsExcept))
        {
            iter->second.CallFn(0);
        }
        else
        {
            if(!iter->second.IsTimeout(tCurrentTime))
            {
                fnAddFd(iter->first, iter->second);
            }
        }
    }
}

int ftnetSelect(unsigned int nTimeoutMs)
{
    gbExitFtNet = false;
    //
    int nRet = 0;
    int nCurIndex = gnIndex;
    int nMaxFd = 1;
    gnIndex = nCurIndex == 0 ? 1 : 0;
    //
    time_t tCurrentTime;
    time(&tCurrentTime);
    //
    fd_set fsRead, fsWrite, fsExcept;
    SetFdSetHelper(gForReadMap, gReadSockList, nCurIndex, fsRead, nMaxFd, nTimeoutMs, tCurrentTime, _ftnetAddReadFd_select);
    SetFdSetHelper(gForWriteMap, gWriteSockList, nCurIndex, fsWrite, nMaxFd, nTimeoutMs, tCurrentTime, _ftnetAddWriteFd_select);
#if _MONITOR_EXCEPT_EVENT
    SetFdSetHelper(gForExceptMap, nCurIndex, fsExcept, nMaxFd, nTimeoutMs, tCurrentTime, NULL);
#else
    FD_ZERO(&fsExcept);
#endif
    //
    if (nTimeoutMs > 100) {
        nTimeoutMs = 100;
    }
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
    {
        DoCheckFdSetHelper(gForReadMap, nCurIndex, fsRead, fsExcept, tCurrentTime, _ftnetAddReadFd_select);
        DoCheckFdSetHelper(gForWriteMap, nCurIndex, fsWrite, fsExcept, tCurrentTime, _ftnetAddWriteFd_select);
        //
        gForWriteMap[nCurIndex].clear();
        gForReadMap[nCurIndex].clear();
#if _MONITOR_EXCEPT_EVENT
        gForExceptMap[nCurIndex].clear();
#endif
    }
    //
    return nRet;
}

FTNET_API void ftnetstop()
{
    int nCurIndex = 0;
    for(;nCurIndex < 2;++nCurIndex)
    {
        gForWriteMap[nCurIndex].clear();
        gForReadMap[nCurIndex].clear();
#if _MONITOR_EXCEPT_EVENT
        gForExceptMap[nCurIndex].clear();
#endif
    }
    gbExitFtNet = true;
}
