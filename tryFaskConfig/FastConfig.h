//
//  FastConfig.h
//  TestAllUseful
//
//  Created by liyoudi on 15/3/6.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__FastConfig__
#define __TestAllUseful__FastConfig__

#include "ftnet_export.h"

#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <list>
#include <vector>
#include <set>

class _FastConfigBase;
class CMarkupSTL;

/***
 设置根节点名称
 */
FTNET_API int _fcSetRootConfigElemName(const char *szRootName, const char *szDescription);

// -------------------------

class FastConfigInnerFormat;

typedef std::vector<void *> _fcSlotArray; // 槽数组

class FTNET_API FastConfigFormat
{
public:
    FastConfigFormat();
    ~FastConfigFormat();
    FastConfigFormat(const FastConfigFormat &fcf);
    FastConfigFormat & operator = (const FastConfigFormat &fcf);
    
protected:
    void IncreaseRef();
    void DescreaseRef();
public:
    FastConfigInnerFormat *mpInnerFormat;
    int *mpnInnerFormatRefCount;
    
    _fcSlotArray mSlotArray; // 默认槽数组
    unsigned int mnSlotPos; // 使用时记录槽位置
};

#define _fcDeclareBasicRegisterVar(type) \
    FTNET_API void fcRegisterVar(FastConfigFormat &fs, type &vt, const char *szUsage, const char *szDescription, const char *szAttrName, const std::list<const char *> &pathList);

_fcDeclareBasicRegisterVar(int)
_fcDeclareBasicRegisterVar(unsigned int)
_fcDeclareBasicRegisterVar(long)
_fcDeclareBasicRegisterVar(unsigned long)
_fcDeclareBasicRegisterVar(bool)
_fcDeclareBasicRegisterVar(double)
_fcDeclareBasicRegisterVar(std::string)


FTNET_API FastConfigFormat & _GetRootFE();
class FTNET_API _FastConfigBase
{
public:
    virtual ~_FastConfigBase();
    // 获取值的文本，如果子节点是xml，则返回的是xml
    //    virtual const char * text() const = 0;
    // 从文件或文本加载时使用
    virtual int initFromText(const char *szText, FastConfigFormat &fs) = 0;
};

template <class ListType, class T>
class _FastConfigList : public _FastConfigBase
{
    unsigned int mnSlotPos;
public:
    T mOne;
public:
    _FastConfigList(unsigned int nSlotPos)
    : mnSlotPos(nSlotPos)
    {}
public:
    int initFromText(const char *szText, FastConfigFormat &fs)
    {
        if(mnSlotPos >= fs.mSlotArray.size())
        {
            return -1;
        }
        ListType *pList = (ListType *) fs.mSlotArray[mnSlotPos];
        T t;
        pList->push_back(t);
        static std::list<const char *> gnoUsePathList;
        fs.mnSlotPos = mnSlotPos + 1;
        fcRegisterVar(fs, pList->back(), NULL, NULL, NULL, gnoUsePathList);
        return 0;
    }
};

int FTNET_API _fcRegisteConfig(FastConfigInnerFormat *pRoot, const char *szUsage, const char *szDescription, const char *szAttrName, _FastConfigBase *pConfig, bool bAddList, std::list<const char *> pathList);

template <class ListType, class Elem>
void __fc_inner_RegisterVar(FastConfigFormat &fs, ListType &vt, Elem &noused, const char *szUsage, const char *szDescription, const char *szAttrName, std::list<const char *> &pathList)
{
    // register list
    if(!szUsage || !szDescription) // 这时是绑定
    {
        if(fs.mnSlotPos < fs.mSlotArray.size())
        {
            fs.mSlotArray[fs.mnSlotPos] = &vt;
            ++fs.mnSlotPos;
            Elem t;
            fcRegisterVar(fs, t, szUsage, szDescription, szAttrName, pathList);
        }
    }
    else
    {
        _FastConfigList<ListType, Elem> *pConfig = new _FastConfigList<ListType, Elem>((unsigned int) fs.mSlotArray.size());
        _fcRegisteConfig(fs.mpInnerFormat, szUsage, szDescription, szAttrName, pConfig, true, pathList);
        fs.mSlotArray.push_back(&vt);
        fcRegisterVar(fs, pConfig->mOne, szUsage, szDescription, szAttrName, pathList);
    }
    // register one member
}

template <class T>
void fcRegisterVar(FastConfigFormat &fs, std::list<T> &vt, const char *szUsage, const char *szDescription, const char *szAttrName, std::list<const char *> &pathList)
{
    static T *t = NULL;
    __fc_inner_RegisterVar(fs, vt, *t, szUsage, szDescription, szAttrName, pathList);
}

template <class T>
void fcRegisterVar(FastConfigFormat &fs, std::vector<T> &vt, const char *szUsage, const char *szDescription, const char *szAttrName, std::list<const char *> &pathList)
{
    static T *t = NULL;
    __fc_inner_RegisterVar(fs, vt, *t, szUsage, szDescription, szAttrName, pathList);
}
///////

////
template <class T>
class FastConfig
{
public:
    /***
     szParentElemName: 父节点名
     defaultValue: 默认值
     szUsage: 用法说明文本
     szDescription: 节点说明性描述
     
     szAttrName: 属性名
     */
    FastConfig(FastConfigFormat *pfcf, const T &defaultValue, const char *szUsage, const char *szDescription, const char *szAttrName, unsigned int nPathCount, ...)
    : mv(defaultValue)
    {
        va_list vaPtr;
        va_start(vaPtr, nPathCount);
        Init(pfcf, szUsage, szDescription, szAttrName, nPathCount, vaPtr);
        va_end(vaPtr);
    }
    FastConfig(FastConfigFormat *pfcf, const char *szUsage, const char *szDescription, const char *szAttrName, unsigned int nPathCount, ...)
    {
        va_list vaPtr;
        va_start(vaPtr, nPathCount);
        Init(pfcf, szUsage, szDescription, szAttrName, nPathCount, vaPtr);
        va_end(vaPtr);
    }
    FastConfig(const T &defaultValue, const char *szUsage, const char *szDescription, const char *szAttrName, unsigned int nPathCount, ...)
    : mv(defaultValue)
    {
        va_list vaPtr;
        va_start(vaPtr, nPathCount);
        Init(NULL, szUsage, szDescription, szAttrName, nPathCount, vaPtr);
        va_end(vaPtr);
    }
    FastConfig(const char *szUsage, const char *szDescription, const char *szAttrName, unsigned int nPathCount, ...)
    {
        va_list vaPtr;
        va_start(vaPtr, nPathCount);
        Init(NULL, szUsage, szDescription, szAttrName, nPathCount, vaPtr);
        va_end(vaPtr);
    }
    void Register(FastConfigFormat *pfcf) const
    {
        std::list<const char *> pathList;
        std::list<std::string>::const_iterator iter;
        for(iter = mPathList.begin();iter != mPathList.end();++iter)
        {
            pathList.push_back(iter->c_str());
        }
        fcRegisterVar(*pfcf, mv, mstrUsage.c_str(), mstrDescription.c_str(), mstrAttrName.c_str(), pathList);
    }
    void Init(FastConfigFormat *pfcf, const char *szUsage, const char *szDescription, const char *szAttrName, unsigned int nPathCount, va_list &vaPtr)
    {
        mPathList.clear();
        while(nPathCount > 0)
        {
            --nPathCount;
            const char *szPath = va_arg(vaPtr, const char *);
            if(!szPath)
            {
                break;
            }
            mPathList.push_back(szPath);
        }
        mstrUsage = szUsage ? szUsage : "";
        mstrDescription = szDescription ? szDescription : "";
        mstrAttrName = szAttrName ? szAttrName : "";
        if(pfcf)
        {
            Register(pfcf);
        }
        else
        {
            Register(&_GetRootFE());
        }
    }
    const T & get () const
    {
        return mv;
    }
    const std::list<std::string> & PathList() const { return mPathList; }
    const T * operator -> () const { return &mv; }
protected:
    bool mbIsGlobal;
    mutable T mv;
    
    std::string mstrUsage;
    std::string mstrDescription;
    std::string mstrAttrName;
    std::list<std::string> mPathList;
};

#define __fc_REGISTER_PATH_0()
#define __fc_REGISTER_PATH_1(p1) pathList.push_back(p1)
#define __fc_REGISTER_PATH_2(p1, p2) __fc_REGISTER_PATH_1(p1); pathList.push_back(p2)
#define __fc_REGISTER_PATH_3(p1, p2, p3) __fc_REGISTER_PATH_2(p1, p2); pathList.push_back(p3)
#define __fc_REGISTER_PATH_4(p1, p2, p3, p4) __fc_REGISTER_PATH_3(p1, p2, p3); pathList.push_back(p4)
#define __fc_REGISTER_PATH_5(p1, p2, p3, p4, p5) __fc_REGISTER_PATH_4(p1, p2, p3); pathList.push_back(p5)
#define __fc_REGISTER_PATH_6(p1, p2, p3, p4, p5, p6) __fc_REGISTER_PATH_5(p1, p2, p3, p4, p5); pathList.push_back(p6)
#define __fc_REGISTER_PATH_7(p1, p2, p3, p4, p5, p6, p7) __fc_REGISTER_PATH_6(p1, p2, p3, p4, p5, p6); pathList.push_back(p7)
#define __fc_REGISTER_PATH_8(p1, p2, p3, p4, p5, p6, p7, p8) __fc_REGISTER_PATH_7(p1, p2, p3, p4, p5, p6, p7); pathList.push_back(p8)
#define __fc_REGISTER_PATH_9(p1, p2, p3, p4, p5, p6, p7, p8, p9) __fc_REGISTER_PATH_8(p1, p2, p3, p4, p5, p6, p7, p8); pathList.push_back(p9)

#define __fc_REGISTER_PATH_POP(n) { for(int i = 0;i < n;++i) pathList.pop_back(); }

#define fc_DECLARE_STRUCT(type) \
    void fcRegisterVar(FastConfigFormat &fs, type &vt, const char *szUsage, const char *szDescription, const char *szAttrName, std::list<const char *> &pathList);

#define fc_IMPLEMENT_STRUCT_BEGIN(type) \
    void fcRegisterVar(FastConfigFormat &fs, type &vt, const char *szUsage, const char *szDescription, const char *szAttrName, std::list<const char *> &pathList) {

#define fc_IMPLEMENT_STRUCT_MEMBER(member, usage, desc, szAttrName, nPathCount, pathArray) \
    __fc_REGISTER_PATH_##nPathCount pathArray; \
    fcRegisterVar(fs, vt.member, !(szUsage&&szUsage[0]) ? NULL : usage, !(szDescription&&szDescription[0]) ? NULL : desc, szAttrName, pathList); \
    __fc_REGISTER_PATH_POP(nPathCount);
#define fc_IMPLEMENT_STRUCT_MEMBER_FromAttribute(member, usage, desc, szAttrName, nPathCount, pathArray) \
    __fc_REGISTER_PATH_##nPathCount pathArray; \
    fcRegisterVar(fs, vt.member, !(szUsage&&szUsage[0]) ? NULL : usage, !(szDescription&&szDescription[0]) ? NULL : desc, szAttrName, pathList); \
    __fc_REGISTER_PATH_POP(nPathCount);

#define fc_IMPLEMENT_STRUCT_END }

// 通用注册格式及变量函数原型概貌
//template <class FastConfigFormatStruct, class ValueType>
//void fcRegisterVar(FastConfigFormatStruct &fs, ValueType &vt, const char *szUsage, const char *szDescription, ...);

template <class FastConfigFormatStruct>
int fcParseXml(const char *szXml, const FastConfigFormatStruct &fs);

template <class FastConfigStructAddress, class Type>
FTNET_API int fcSetNewBindAddress(FastConfigStructAddress &addr, Type &v);
/***
 用xml文本初始化配置项
 */
//int fcInitConfig(const char *szXmlText, FastConfigFormat &fcf);
FTNET_API extern int (*fcInitConfig) (const char *szXmlText, FastConfigFormat &fcf);
FTNET_API void fcSetUseRapidXml(bool bUse);
// 通用解析函数
template <class T1>
class CommonParser1
{
public:
    CommonParser1(FastConfig<T1> &fc1)
    {
        fc1.Register(&mfcf);
    }
    int ParseXmlText(const char *szXml, T1 &p1)
    {
        int nRet = 0;
        std::list<const char *> pathList;
        FastConfigFormat fs(mfcf);
        fcRegisterVar(fs, p1, NULL, NULL, NULL, pathList);
        nRet = fcInitConfig(szXml, fs);
        return nRet;
    }
protected:
    FastConfigFormat mfcf;
};
template <class T1>
CommonParser1<T1> & fcGetCommonParser(FastConfig<T1> &fc1)
{
    static CommonParser1<T1> gcp(fc1);
    return gcp;
}
//
#define __fc_TIMES_SEP_COMMA_1(macro_with_n) macro_with_n(1)
#define __fc_TIMES_SEP_COMMA_2(macro_with_n) __fc_TIMES_SEP_COMMA_1(macro_with_n), macro_with_n(2)
#define __fc_TIMES_SEP_COMMA_3(macro_with_n) __fc_TIMES_SEP_COMMA_2(macro_with_n), macro_with_n(3)
#define __fc_TIMES_SEP_COMMA_4(macro_with_n) __fc_TIMES_SEP_COMMA_3(macro_with_n), macro_with_n(4)
#define __fc_TIMES_SEP_COMMA_5(macro_with_n) __fc_TIMES_SEP_COMMA_4(macro_with_n), macro_with_n(5)
#define __fc_TIMES_SEP_COMMA_6(macro_with_n) __fc_TIMES_SEP_COMMA_5(macro_with_n), macro_with_n(6)
#define __fc_TIMES_SEP_COMMA_7(macro_with_n) __fc_TIMES_SEP_COMMA_6(macro_with_n), macro_with_n(7)
#define __fc_TIMES_SEP_COMMA_8(macro_with_n) __fc_TIMES_SEP_COMMA_7(macro_with_n), macro_with_n(8)
#define __fc_TIMES_SEP_COMMA_9(macro_with_n) __fc_TIMES_SEP_COMMA_8(macro_with_n), macro_with_n(9)

#define __fc_TIMES_1(macro_with_n, sep) macro_with_n(1)
#define __fc_TIMES_2(macro_with_n, sep) __fc_TIMES_1(macro_with_n, sep) sep macro_with_n(2)
#define __fc_TIMES_3(macro_with_n, sep) __fc_TIMES_2(macro_with_n, sep) sep macro_with_n(3)
#define __fc_TIMES_4(macro_with_n, sep) __fc_TIMES_3(macro_with_n, sep) sep macro_with_n(4)
#define __fc_TIMES_5(macro_with_n, sep) __fc_TIMES_4(macro_with_n, sep) sep macro_with_n(5)
#define __fc_TIMES_6(macro_with_n, sep) __fc_TIMES_5(macro_with_n, sep) sep macro_with_n(6)
#define __fc_TIMES_7(macro_with_n, sep) __fc_TIMES_6(macro_with_n, sep) sep macro_with_n(7)
#define __fc_TIMES_8(macro_with_n, sep) __fc_TIMES_7(macro_with_n, sep) sep macro_with_n(8)
#define __fc_TIMES_9(macro_with_n, sep) __fc_TIMES_8(macro_with_n, sep) sep macro_with_n(9)

#define __fc_inner_class_list(n) class T##n
#define __fc_inner_SEMICOLON ;
#define __fc_inner_fc_list(n) FastConfig<T##n> &fc##n
#define __fc_inner_register(n) fc##n.Register(&mfcf);mfcf.mnSlotPos = (unsigned int) mfcf.mSlotArray.size()
#define __fc_inner_param(n) T##n &p##n
#define __fc_inner_registerVar(n) fcRegisterVar(fs, p##n, NULL, NULL, NULL, pathList)
#define __fc_inner_T(n) T##n
#define __fc_inner_fc(n) fc##n

#define __fc_inner_DeclareCommonParser(n) \
	template <__fc_TIMES_SEP_COMMA_##n(__fc_inner_class_list)> \
	class CommonParser##n \
	{ \
	public: \
	    CommonParser##n(__fc_TIMES_SEP_COMMA_##n(__fc_inner_fc_list)) \
	    { \
	        __fc_TIMES_##n(__fc_inner_register, __fc_inner_SEMICOLON); \
	    } \
	    int ParseXmlText(const char *szXml, __fc_TIMES_SEP_COMMA_##n(__fc_inner_param)) \
	    { \
	        int nRet = 0; \
	        std::list<const char *> pathList; \
            FastConfigFormat fs(mfcf); \
	        __fc_TIMES_##n(__fc_inner_registerVar, __fc_inner_SEMICOLON); \
	        nRet = fcInitConfig(szXml, fs); \
	        return nRet; \
	    } \
	protected: \
	    FastConfigFormat mfcf; \
	}; \
	template <__fc_TIMES_SEP_COMMA_##n(__fc_inner_class_list)> \
	CommonParser##n<__fc_TIMES_SEP_COMMA_##n(__fc_inner_T)> & fcGetCommonParser(__fc_TIMES_SEP_COMMA_##n(__fc_inner_fc_list)) \
	{ \
	    static CommonParser##n<__fc_TIMES_SEP_COMMA_##n(__fc_inner_T)> gcp(__fc_TIMES_SEP_COMMA_##n(__fc_inner_fc)); \
	    return gcp; \
	}

//__fc_inner_DeclareCommonParser(1)
__fc_inner_DeclareCommonParser(2)
__fc_inner_DeclareCommonParser(3)
__fc_inner_DeclareCommonParser(4)
__fc_inner_DeclareCommonParser(5)
__fc_inner_DeclareCommonParser(6)
__fc_inner_DeclareCommonParser(7)
__fc_inner_DeclareCommonParser(8)
__fc_inner_DeclareCommonParser(9)
//
#endif /* defined(__TestAllUseful__FastConfig__) */
