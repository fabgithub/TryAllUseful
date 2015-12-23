//
//  ftoswrap.h
//  TestAllUseful
//
//  Created by liyoudi on 15/2/27.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__ftoswrap__
#define __TestAllUseful__ftoswrap__

#include <stdio.h>
#include "ftnet_export.h"

#ifdef WIN32
#include <Windows.h>
#define IsValidSocket(s) (s != INVALID_SOCKET)
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>

#include <errno.h>
#include <unistd.h>

#define SOCKET int
#define INVALID_SOCKET -1
#define closesocket close
#define IsValidSocket(s) (s > 0)

#endif

#endif /* defined(__TestAllUseful__ftoswrap__) */
