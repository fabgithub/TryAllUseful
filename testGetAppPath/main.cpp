//
//  main.cpp
//  testGetAppPath
//
//  Created by liyoudi on 15/1/28.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <string>

#define MAX_SIZE 2000
std::string GetCurrentWorkPath()
{
    char current_absolute_path[MAX_SIZE];
    //获取当前目录绝对路径
    if (NULL == realpath("./", current_absolute_path))
    {
        return "";
    }
    strcat(current_absolute_path, "/");
    return current_absolute_path;
}
void Func1()
{
    std::cout <<"work dir is '" << GetCurrentWorkPath() << "'" << std::endl;
}
void Func2()
{
    char current_absolute_path[MAX_SIZE];
    //获取当前目录绝对路径
    if (NULL == getcwd(current_absolute_path, MAX_SIZE))
    {
        printf("***Error***\n");
        exit(-1);
    }
    printf("current absolute path:%s\n", current_absolute_path);
}

void Func3()
{
    char current_absolute_path[MAX_SIZE];
    //获取当前程序绝对路径
    int cnt = readlink("/proc/self/exe", current_absolute_path, MAX_SIZE);
    if (cnt < 0 || cnt >= MAX_SIZE)
    {
        printf("***Error***\n");
        exit(-1);
    }
    //获取当前目录绝对路径，即去掉程序名
    int i;
    for (i = cnt; i >=0; --i)
    {
        if (current_absolute_path[i] == '/')
        {
            current_absolute_path[i+1] = '\0';
            break;
        }
    }
    printf("current absolute path:%s\n", current_absolute_path);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    Func1();
    Func2();
    Func3();
    return 0;
}
