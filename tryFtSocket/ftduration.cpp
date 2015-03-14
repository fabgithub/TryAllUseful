//
//  ftduration.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/2/28.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//

#include "ftduration.h"

ftduration::ftduration()
{
    start();
}
void ftduration::start()
{
    gettimeofday(&mtStart, NULL);
}
double ftduration::whenStart() const
{
    return mtStart.tv_sec + mtStart.tv_usec / 1000000.0;
}
double ftduration::durationMs() const
{
    struct timeval tEnd;
    gettimeofday(&tEnd, NULL);
    double dDuration = (tEnd.tv_sec - mtStart.tv_sec) + (tEnd.tv_usec - mtStart.tv_usec) / 1000000.0;
    return dDuration;
}
double ftduration::durationSecond() const
{
    struct timeval tEnd;
    gettimeofday(&tEnd, NULL);
    double dDuration = (tEnd.tv_sec - mtStart.tv_sec) + (tEnd.tv_usec - mtStart.tv_usec) / 1000000.0;
    return dDuration;
}
