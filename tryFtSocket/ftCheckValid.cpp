//
//  ftCheckValid.cpp
//  ft_framework
//
//  Created by liyoudi on 15/3/26.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//

#include "ftCheckValid.h"

class __ftCheckValid_data
{
public:
    __ftCheckValid_data(void *ptr) : mnRefCount(0), mpValidPtr(ptr) {}
    ~__ftCheckValid_data() {}
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
    int mnRefCount;
    void *mpValidPtr;
};

ftCheckValid::ftCheckValid(void *ptr)
{
    mpcvd = new __ftCheckValid_data(ptr);
    mpcvd->IncreaseRef();
}
ftCheckValid::ftCheckValid(const ftCheckValid & cv)
{
    mpcvd = cv.mpcvd;
    mpcvd->IncreaseRef();
}
ftCheckValid::~ftCheckValid()
{
    mpcvd->DecreaseRef();
}

ftCheckValid & ftCheckValid::operator = (const ftCheckValid &cv)
{
    mpcvd->DecreaseRef();
    mpcvd = cv.mpcvd;
    mpcvd->IncreaseRef();
    return *this;
}
bool ftCheckValid::operator ! () const
{
    return !mpcvd->mpValidPtr;
}
bool ftCheckValid::operator < (const ftCheckValid &cv) const
{
    return mpcvd < cv.mpcvd;
}
bool ftCheckValid::IsValid() const
{
    return mpcvd->mpValidPtr != NULL;
}
void ftCheckValid::SetInvalid()
{
    mpcvd->mpValidPtr = NULL;
}
void * ftCheckValid::get() const
{
    return mpcvd->mpValidPtr;
}
