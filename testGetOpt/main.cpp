//
//  main.cpp
//  testGetOpt
//
//  Created by liyoudi on 15/1/28.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <getopt.h>

void TestArgc(int argc,  char * const argv[])
{
    int bflag, ch, fd;
    
    bflag = 0;
    while ((ch = getopt(argc, argv, "b:f:")) != -1) {
        switch (ch) {
            case 'b':
                bflag = 1;
                if(optarg)
                {
                    std::cout << "b has option value is the following:" << std::endl << optarg << std::endl;
                }
                else
                {
                    std::cout << "b has no option value." << std::endl;
                }
                break;
            case 'f':
//                if ((fd = open(optarg, O_RDONLY, 0)) < 0)
                {
                    (void)fprintf(stderr,
                                  "myname: '%s': '%s'\n", optarg, strerror(errno));
//                    exit(1);
                }
                break;
            case '?':
            default:
//                usage();
                break;
        }
    }
    argc -= optind;
    argv += optind;
}

int main(int argc,  char * const argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    TestArgc(argc, argv);
    return 0;
}
