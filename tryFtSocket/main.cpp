//
//  main.cpp
//  tryFtSocket
//
//  Created by liyoudi on 15/2/26.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//

#include <iostream>

#include "FakeThreadTemplate.h"

void f(long l)
{
    std::cout << l << std::endl;
}

#ifdef WIN32
    #define SLASH_CHAR '\\'
#else
    #define SLASH_CHAR '/'
#endif

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

int test_ftsocket_main( int argc, char *argv[] );

int main(int argc, char * argv[]) {
    // insert code here...
    test_ftsocket_main(argc, argv);
    return 0;
}
