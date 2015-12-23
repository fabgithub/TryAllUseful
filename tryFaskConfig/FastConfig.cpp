//
//  FastConfig.cpp
//  TestAllUseful
//
//  Created by liyoudi on 15/3/6.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#include "FastConfig.h"

#include "MarkupSTL.h"
#include "rapidxml.hpp"

#include <string>
#include <map>
#include <stdlib.h>
#include <list>

#include <algorithm>

#ifdef WIN32
#define strcasecmp stricmp
#endif
//
typedef int (*fc_InitForList_fn) (FastConfigFormat &fs, unsigned int nSlotPos);
//
static std::string gstrRootElemName;

_FastConfigBase::~_FastConfigBase() { }
//
class FastConfigInnerFormat
{
public:
    int ParseElem(CMarkupSTL &xml, FastConfigFormat &fs);
    int ParseElem(rapidxml::xml_node<> &xml, FastConfigFormat &fs);
public:
    typedef std::list<_FastConfigBase *> FCList;
    FCList mFCList; // 节点可能需要对多个配置项赋值
    typedef std::map<std::string, FCList> AttrFCListMap;
    AttrFCListMap mAttrFCListMap; // 节点可能需要对多个配置项赋值
    typedef std::map<const std::string, FastConfigInnerFormat *> SubElemFormatMap;
    SubElemFormatMap mSubElemFormatMap; // 指定子节点的格式
    typedef std::map<const std::string, _FastConfigBase *> AttrFormatMap;
    AttrFormatMap mAttrFormatMap;
    
    // 如果是list，则需要添加下面的对象
    FCList mForElemList;
};

FastConfigFormat & _GetRootFE()
{
    static FastConfigFormat gRootFE;
    return gRootFE;
}
/***
 设置根节点名称
 */
int _fcSetRootConfigElemName(const char *szRootName, const char *szDescription)
{
    int nRet = 0;
    gstrRootElemName = szRootName;
    return nRet;
}
int _fcRegisteConfig(FastConfigInnerFormat *pRoot, const char *szUsage, const char *szDescription, const char *szAttrName, _FastConfigBase *pConfig, bool bAddList, std::list<const char *> pathList)
{
    int nRet = 0;
    const char *szParent = NULL;
    FastConfigInnerFormat *pLastFE = pRoot;
    std::list<const char *>::iterator iterPath;
    for(iterPath = pathList.begin();iterPath != pathList.end();++iterPath)
    {
        szParent = *iterPath;
        
        std::pair<FastConfigInnerFormat::SubElemFormatMap::iterator, bool> ib;
        ib = pLastFE->mSubElemFormatMap.insert(std::make_pair(szParent, (FastConfigInnerFormat *) NULL));
        if(ib.second)
        {
            pLastFE = new FastConfigInnerFormat;
            ib.first->second = pLastFE;
        }
        else
        {
            pLastFE = ib.first->second;
        }
    }
    if(bAddList)
    {
        pLastFE->mForElemList.push_back(pConfig);
    }
    else if(!szAttrName || !szAttrName[0])
    {
        pLastFE->mFCList.push_back(pConfig);
    }
    else
    {
        pLastFE->mAttrFCListMap[szAttrName].push_back(pConfig);
    }
    return nRet;
}
//
/***
 注册配置项
 可变参数部分是节点的全路径
 如果szAttrName为NULL，则为元素，否则就是属性
 如果配置项已存在，则返回 -1; 成功添加则返回 0
 */
int FastConfigInnerFormat::ParseElem(CMarkupSTL &xml, FastConfigFormat &fs)
{
    int nRet = 0;
    if(!mForElemList.empty())
    {
        FastConfigInnerFormat::FCList::iterator iterFC;
        for(iterFC = mForElemList.begin();iterFC != mForElemList.end();++iterFC)
        {
            nRet = (*iterFC)->initFromText(xml.GetData().c_str(), fs);
            if(nRet)
            {
                return nRet;
            }
        }
    }
    FastConfigInnerFormat::FCList::iterator iterFC;
    if(!mFCList.empty())
    {
        for(iterFC = mFCList.begin();iterFC != mFCList.end();++iterFC)
        {
            nRet = (*iterFC)->initFromText(xml.GetData().c_str(), fs);
            if(nRet)
            {
                return nRet;
            }
        }
    }
    AttrFCListMap::iterator iterAttr;
    for (iterAttr = mAttrFCListMap.begin(); iterAttr != mAttrFCListMap.end(); ++iterAttr)
    {
        std::string strValue;
        
        for(iterFC = iterAttr->second.begin();iterFC != iterAttr->second.end();++iterFC)
        {
            nRet = (*iterFC)->initFromText(xml.GetAttrib(iterAttr->first.c_str()).c_str(), fs);
            if(nRet)
            {
                return nRet;
            }
        }
    }
    // parse child elements
    if(!xml.IntoElem())
    {
        if(!mSubElemFormatMap.empty()) // xml中没有子节点了，但是指定的格式中还有子节点
        {
            return -1;
        }
        return 0;
    }
    FastConfigInnerFormat::SubElemFormatMap::iterator iterSub;
    SubElemFormatMap tmpSubMap = mSubElemFormatMap;
    while(xml.FindElem())
    {
        std::string strTag = xml.GetTagName();
        iterSub = tmpSubMap.find(strTag.c_str());
        if(iterSub != tmpSubMap.end())
        {
            nRet = iterSub->second->ParseElem(xml, fs);
            if(nRet)
            {
                break;
            }
            if(iterSub->second->mForElemList.empty())
            {
//                tmpSubMap.erase(iterSub);
            }
        }
        else // 说明xml文本中有多余字段
        {
        }
    }
    if(!tmpSubMap.empty()) // 说明有子节点在xml中不存在
    {
        
    }
    xml.OutOfElem();
    return nRet;
}
int FastConfigInnerFormat::ParseElem(rapidxml::xml_node<> &xml, FastConfigFormat &fs)
{
    int nRet = 0;
    if(!mForElemList.empty())
    {
        FastConfigInnerFormat::FCList::iterator iterFC;
        for(iterFC = mForElemList.begin();iterFC != mForElemList.end();++iterFC)
        {
            nRet = (*iterFC)->initFromText(xml.value(), fs);
            if(nRet)
            {
                return nRet;
            }
        }
    }
    FastConfigInnerFormat::FCList::iterator iterFC;
    if(!mFCList.empty())
    {
        for(iterFC = mFCList.begin();iterFC != mFCList.end();++iterFC)
        {
            nRet = (*iterFC)->initFromText(xml.value(), fs);
            if(nRet)
            {
                return nRet;
            }
        }
    }
    AttrFCListMap::iterator iterAttr;
    rapidxml::xml_attribute<> *pAttr = xml.first_attribute();
    for(;pAttr;pAttr = pAttr->next_attribute())
    {
        iterAttr = mAttrFCListMap.find(pAttr->name());
        if(iterAttr != mAttrFCListMap.end())
        {
            std::string strValue;
            
            for(iterFC = iterAttr->second.begin();iterFC != iterAttr->second.end();++iterFC)
            {
                nRet = (*iterFC)->initFromText(pAttr->value(), fs);
                if(nRet)
                {
                    return nRet;
                }
            }
        }
    }
    if(mSubElemFormatMap.empty()) // 指定的格式中没有子节点
    {
        return 0;
    }
    FastConfigInnerFormat::SubElemFormatMap::iterator iterSub;
    SubElemFormatMap tmpSubMap = mSubElemFormatMap;
    rapidxml::xml_node<> *pChildNode = xml.first_node();
    for(;pChildNode;pChildNode = pChildNode->next_sibling())
    {
        const char *strTag = pChildNode->name();
        iterSub = tmpSubMap.find(strTag);
        if(iterSub != tmpSubMap.end())
        {
            nRet = iterSub->second->ParseElem(*pChildNode, fs);
            if(nRet)
            {
                break;
            }
            if(iterSub->second->mForElemList.empty())
            {
                //                tmpSubMap.erase(iterSub);
            }
        }
        else // 说明xml文本中有多余字段
        {
        }
    }
    if(!tmpSubMap.empty()) // 说明有子节点在xml中不存在
    {
        
    }
    return nRet;
}

//
static int _fc_load_xml_string(CMarkupSTL &xml, const char *szXmlText)
{
    int nRet = -1;
    do {
        if(!szXmlText || !*szXmlText)
        {
            xml.SetDoc("<?xml>");
        }
        else
        {
            xml.SetDoc(szXmlText);
        }
        if(!xml.IsWellFormed())
        {
            break;
        }
        if(!xml.FindElem() || xml.GetTagName().empty())
        {
            break;
        }
        nRet = 0;
    }while(false);
    return nRet;
}
static int _fc_load_xml_string(rapidxml::xml_document<> &xml, const char *szXmlText)
{
    int nRet = -1;
    if(szXmlText && szXmlText[0])
    {
        try {
            xml.parse<0>((char *) szXmlText);
            nRet = 0;
        } catch (std::exception &) {
            nRet = -1;
        }
    }
    return nRet;
}
/***
 用xml文本初始化配置项
 */
int fcInitConfigWithRapidXml(const char *szXmlText, FastConfigFormat &fcf)
{
    int nRet = -1;
    do {
        rapidxml::xml_document<> xml;
        std::string strTmp(szXmlText);
        nRet = _fc_load_xml_string(xml, strTmp.c_str());
        if(nRet)
        {
            break;
        }
        if(!xml.first_node())
        {
            nRet = -1;
            break;
        }
        fcf.mnSlotPos = 0;
        nRet = fcf.mpInnerFormat->ParseElem(*xml.first_node(), fcf);
    }while(false);
    return nRet;
}

int fcInitConfigWithMarkupSTL(const char *szXmlText, FastConfigFormat &fcf)
{
    int nRet = -1;
    do {
        CMarkupSTL xml;
        nRet = _fc_load_xml_string(xml, szXmlText);
        if(nRet)
        {
            break;
        }
        fcf.mnSlotPos = 0;
        nRet = fcf.mpInnerFormat->ParseElem(xml, fcf);
    }while(false);
    return nRet;
}
FTNET_API int (*fcInitConfig) (const char *szXmlText, FastConfigFormat &fcf) = fcInitConfigWithMarkupSTL;
//int (*fcInitConfig) (const char *szXmlText, FastConfigFormat &fcf) = fcInitConfigWithRapidXml;
void fcSetUseRapidXml(bool bUse)
{
    if(bUse)
    {
        fcInitConfig = fcInitConfigWithRapidXml;
    }
    else
    {
        fcInitConfig = fcInitConfigWithMarkupSTL;
    }
}

// ========================================
int _fc_parse_from_text(const char *szText, int &nOut)
{
    nOut = atoi(szText);
    return 0;
}
int _fc_parse_from_text(const char *szText, unsigned int &nOut)
{
    nOut = atoi(szText);
    return 0;
}
int _fc_parse_from_text(const char *szText, long &nOut)
{
    nOut = (long) atoi(szText);
    return 0;
}
int _fc_parse_from_text(const char *szText, unsigned long &nOut)
{
    nOut = (unsigned long) atoi(szText);
    return 0;
}
int _fc_parse_from_text(const char *szText, double &dOut)
{
    dOut = atof(szText);
    return 0;
}
int _fc_parse_from_text(const char *szText, bool &bOut)
{
    if(strcasecmp(szText, "true") == 0 || atoi(szText) == 1)
    {
        bOut = true;
    }
    else if(strcasecmp(szText, "false") == 0 || atoi(szText) == 0)
    {
        bOut = false;
    }
    else
    {
        return -1;
    }
    return 0;
}
int _fc_parse_from_text(const char *szText, std::string &strOut)
{
    strOut = szText;
    return 0;
}

// ===================

template <class T>
class _BasicFCType : public _FastConfigBase
{
public:
    _BasicFCType(unsigned int nSlotPos) : mnSlotPos(nSlotPos) { }
    virtual int initFromText(const char *szText, FastConfigFormat &fs)
    {
        if(mnSlotPos >= fs.mSlotArray.size())
        {
            return -1;
        }
        T *pv = (T *) fs.mSlotArray[mnSlotPos]; // 获取槽点指针
        return _fc_parse_from_text(szText, *pv);
    }
    unsigned int mnSlotPos; // 槽点位置
};
/***
 注册配置项
 可变参数部分是节点的全路径
 如果szAttrName为NULL，则为元素，否则就是属性
 如果配置项已存在，则返回 -1; 成功添加则返回 0
 */
#define _fcImplementBasicRegisterVar(type) \
    void fcRegisterVar(FastConfigFormat &fs, type &vt, const char *szUsage, const char *szDescription, const char *szAttrName, const std::list<const char *> &pathList) \
    { \
        if(!szUsage || !szDescription) \
        { \
            if(fs.mnSlotPos < fs.mSlotArray.size()) \
            { \
                fs.mSlotArray[fs.mnSlotPos] = &vt; \
                ++fs.mnSlotPos; \
            } \
        } \
        else \
        { \
            _fcRegisteConfig(fs.mpInnerFormat, szUsage, szDescription, szAttrName, new _BasicFCType<type>((unsigned int) fs.mSlotArray.size()), false, pathList); \
            fs.mSlotArray.push_back(&vt); \
        } \
    }

_fcImplementBasicRegisterVar(int)
_fcImplementBasicRegisterVar(unsigned int)
_fcImplementBasicRegisterVar(long)
_fcImplementBasicRegisterVar(unsigned long)
_fcImplementBasicRegisterVar(bool)
_fcImplementBasicRegisterVar(double)
_fcImplementBasicRegisterVar(std::string)

//
FastConfigFormat::FastConfigFormat()
: mnSlotPos(0)
{
    mpInnerFormat = new FastConfigInnerFormat;
    mpnInnerFormatRefCount = new int;
    *mpnInnerFormatRefCount = 0;
    
    IncreaseRef();
}
FastConfigFormat::~FastConfigFormat()
{
    DescreaseRef();
}
static void ForEachFunc(void * & p)
{
    p = NULL;
}
FastConfigFormat::FastConfigFormat(const FastConfigFormat &fcf)
: mnSlotPos(0)
{
    mpInnerFormat = fcf.mpInnerFormat;
    mpnInnerFormatRefCount = fcf.mpnInnerFormatRefCount;
    IncreaseRef();
    mSlotArray = fcf.mSlotArray;
    
    for_each(mSlotArray.begin(), mSlotArray.end(), ForEachFunc);
}
FastConfigFormat & FastConfigFormat::operator = (const FastConfigFormat &fcf)
{
    DescreaseRef();
    mpInnerFormat = fcf.mpInnerFormat;
    mpnInnerFormatRefCount = fcf.mpnInnerFormatRefCount;
    IncreaseRef();
    mSlotArray = fcf.mSlotArray;
    mnSlotPos = fcf.mnSlotPos;
    for_each(mSlotArray.begin(), mSlotArray.end(), ForEachFunc);
    
    mnSlotPos = 0;
    return *this;
}
void FastConfigFormat::IncreaseRef()
{
    ++(*mpnInnerFormatRefCount);
}
void FastConfigFormat::DescreaseRef()
{
    --(*mpnInnerFormatRefCount);
    if(*mpnInnerFormatRefCount <= 0)
    {
        delete mpnInnerFormatRefCount;
        delete mpInnerFormat;
    }
}
