//
//  ftCheckValid.h
//  ft_framework
//
//  Created by liyoudi on 15/3/26.
/*  Copyright (c) 2015�� liyoudi. All rights reserved. */
/*  �ô���һ��ֵ���ж���ط����ã����ǿ�����һ���ط���������Ϊ��Ч��Ȼ�������ط����ܹ���⵽������֪������Ч�ˡ�*/

#ifndef __ft_framework__ftCheckValid__
#define __ft_framework__ftCheckValid__

#include <stdio.h>

class __ftCheckValid_data;

class ftCheckValid
{
public:
    ftCheckValid(void *ptr = NULL);
    ftCheckValid(const ftCheckValid & cv);
    ~ftCheckValid();
    
    ftCheckValid & operator = (const ftCheckValid &cv);
    bool operator ! () const;
    bool operator < (const ftCheckValid &cv) const;
public:
    bool IsValid() const;
    void SetInvalid();
public:
    void * get() const;
private:
    __ftCheckValid_data *mpcvd;
};

template <class Type>
class ftCheckValidT : public ftCheckValid
{
public:
    ftCheckValidT(Type *ptr = NULL) : ftCheckValid(ptr) {}
};

#endif /* defined(__ft_framework__ftCheckValid__) */
