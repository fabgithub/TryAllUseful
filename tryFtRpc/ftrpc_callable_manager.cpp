//
//  ftrpc_callable_manager.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/3/1.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//

#include "ftrpc_callable_manager.h"

#include <map>
#include <string>
#include <iostream>

class _ftrpc_callable_manager_data
{
    _ftrpc_callable_manager_data(const _ftrpc_callable_manager_data &);
    _ftrpc_callable_manager_data & operator = (const _ftrpc_callable_manager_data &);
public:
    _ftrpc_callable_manager_data() : mnRefCount(0) {}
    ~_ftrpc_callable_manager_data() {}
    void IncreaseRef()
    {
        ++mnRefCount;
    }
    void DecreaseRef()
    {
        --mnRefCount;
        if(mnRefCount <= 0)
        {
            delete this;
        }
    }
public:
    typedef std::map<std::string, ftrpc_callable *> NameCallableMap;
    NameCallableMap mNameCallableMap;
    
    int mnRefCount;
};

ftrpc_callable_manager::ftrpc_callable_manager()
: mpcmd(NULL)
{
    mpcmd = new _ftrpc_callable_manager_data();
    mpcmd->IncreaseRef();
}
ftrpc_callable_manager::ftrpc_callable_manager(const ftrpc_callable_manager &mgr)
: mpcmd(mgr.mpcmd)
{
    mpcmd->IncreaseRef();
}
ftrpc_callable_manager::~ftrpc_callable_manager()
{
    mpcmd->DecreaseRef();
}

ftrpc_callable_manager & ftrpc_callable_manager::operator = (const ftrpc_callable_manager &mgr)
{
    mpcmd->DecreaseRef();
    mpcmd = mgr.mpcmd;
    mpcmd->IncreaseRef();
    return *this;
}
bool ftrpc_callable_manager::operator ! () const
{
    return !mpcmd;
}
bool ftrpc_callable_manager::IsValid() const
{
    return mpcmd;
}
int ftrpc_callable_manager::CallFunc(_ftBufArray &retBufArray, const char *szFuncName, const _ftBufArray &parameters)
{
    int nRet = -1;
//    std::cout << __FUNCTION__ << ": " << szFuncName << std::endl;
//    int res = 25;
    _ftrpc_callable_manager_data::NameCallableMap::iterator iter = mpcmd->mNameCallableMap.find(szFuncName);
    if(iter != mpcmd->mNameCallableMap.end())
    {
        nRet = (iter->second)->operator () (retBufArray, szFuncName, parameters);
    }
    
    return nRet;
}
int ftrpc_callable_manager::AddCallableFunc(const char *szFuncName, ftrpc_callable *pCallable)
{
    int nRet = 0;
    if(!mpcmd->mNameCallableMap.insert(std::make_pair(szFuncName, pCallable)).second)
    {
        nRet = -1;
    }
    return nRet;
}
