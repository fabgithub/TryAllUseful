//
//  main.cpp
//  testGetAppPath
//
//  Created by liyoudi on 15/1/28.
//  Copyright (c) 2015�� liyoudi. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <string>

#define MAX_SIZE 2000
std::string GetCurrentWorkPath()
{
    char current_absolute_path[MAX_SIZE];
    //��ȡ��ǰĿ¼����·��
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
    //��ȡ��ǰĿ¼����·��
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
    //��ȡ��ǰ�������·��
    int cnt = readlink("/proc/self/exe", current_absolute_path, MAX_SIZE);
    if (cnt < 0 || cnt >= MAX_SIZE)
    {
        printf("***Error***\n");
        exit(-1);
    }
    //��ȡ��ǰĿ¼����·������ȥ��������
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
