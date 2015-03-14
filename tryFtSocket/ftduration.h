//
//  ftduration.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/28.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__ftduration__
#define __TestAllUseful__ftduration__

#include <stdio.h>
#include <sys/time.h>

class ftduration
{
public:
    ftduration();
    
    void start();
    
    double whenStart() const;
    double durationMs() const;
    double durationSecond() const;
private:
    struct timeval mtStart;
};

#endif /* defined(__TestAllUseful__ftduration__) */
