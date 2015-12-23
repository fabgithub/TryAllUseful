//
//  ftduration.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/28.
//  Copyright (c) 2015�� liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__ftduration__
#define __TestAllUseful__ftduration__

#include "ftnet_export.h"

#include <string>
#include <stdio.h>
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

class FTNET_API ftduration
{
public:
    ftduration();
    
    void start();
    
    double whenStart() const;
    double durationSecond() const;
    
    std::string StringOfStartTime() const;
private:
#ifdef WIN32
    FILETIME mftStart;
#else
    struct timeval mtStart;
#endif
};

#endif /* defined(__TestAllUseful__ftduration__) */
