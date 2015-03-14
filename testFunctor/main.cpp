//
//  main.cpp
//  testFunctor
//
//  Created by liyoudi on 15/2/4.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//
// ²âÊÔ·Âº¯Êý

#include <iostream>
#include <string>

class BaseFunctor
{
public:
    virtual ~BaseFunctor() {}
    virtual int operator () (int a, int b) = 0;
    virtual std::string name() = 0;
};

class SumFunctor : public BaseFunctor
{
public:
    virtual int operator () (int a, int b)
    {
        return a + b;
    }
    virtual std::string name()
    {
        return "SumFunctor";
    }
};

class MulFunctor : public BaseFunctor
{
public:
    virtual int operator () (int a, int b)
    {
        return a * b;
    }
    virtual std::string name()
    {
        return "MulFunctor";
    }
};

void CallFunc(BaseFunctor *pBf, int a, int b)
{
    std::cout << pBf->name() << "(" << a << ", " << b << ") is " << (*pBf)(a, b) << std::endl;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    SumFunctor sf;
    MulFunctor mf;
    CallFunc(&sf, 1, 2);
    CallFunc(&mf, 1, 2);
    return 0;
}
