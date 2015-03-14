//
//  test_rapidxml.h
//  TestAllUseful
//
//  Created by liyoudi on 15/3/9.
//  Copyright (c) 2015年 liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__test_rapidxml__
#define __TestAllUseful__test_rapidxml__

#include "cxxtest/TestSuite.h"
#include "ftduration.h"

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

#include <stdio.h>
#include <iostream>
//#include <string>

const char * NFGetFileNameFromPath(const char *szFilePath);

#define _InfoLog( args ) std::cout << " " << NFGetFileNameFromPath(__FILE__) << "(" << __LINE__ << ") " << __FUNCTION__ << ": " args << std::endl

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

char gszXml[] =
"<root>\r\n"
"   <id name='fab'>1</id>\r\n"
"   <id>2</id>\r\n"
"   <id>3</id>\r\n"
"   <id name='forth'>4</id>\r\n"
"   <id>5</id>\r\n"
"   <CS Enable='true'>\r\n"
"       <Server>\r\n"
"           <ip>192.168.5.6</ip>\r\n"
"           <port>7001</port>\r\n"
"       </Server>\r\n"
"   </CS>\r\n"
"   <CS Enable='false'>\r\n"
"       <Server>\r\n"
"           <ip>192.168.5.5</ip>\r\n"
"           <port>6001</port>\r\n"
"       </Server>\r\n"
"   </CS>\r\n"
"   <first>\r\n"
"       <Count>3</Count>\r\n"
"       <HasAttr>trUe</HasAttr>\r\n"
"   </first>\r\n"
"</root>\r\n";

static char gszConfig[] =
"<config>\r\n"
"	<color>\r\n"
"		<red>0.1</red>\r\n"
"		<green>0.1</green>\r\n"
"		<blue>0.1</blue>\r\n"
"		<alpha>1.0</alpha>\r\n"
"	</color>\r\n"
"	<size>\r\n"
"		<x>640</x>\r\n"
"		<y>480</y>\r\n"
"		<w>0</w>\r\n"
"		<h>0</h>\r\n"
"	</size>\r\n"
"	<mode fullscreen='false'>screen mode</mode>\r\n"
"</config>\r\n"
;

using namespace rapidxml;

class test_rapidxml : public CxxTest::TestSuite
{
public:
    void testCommon()
    {
        rapidxml::xml_document<> doc;
        doc.parse<0>(gszConfig);
        _InfoLog( << doc.name() );

        //! 获取根节点
        xml_node<>* root = doc.first_node();
        std::cout<<root->name()<<std::endl;
        
        //! 获取根节点第一个节点
        xml_node<>* node1 = root->first_node();
        std::cout<<node1->name()<<std::endl;
        std::cout<<node1->value()<<std::endl;
        
        node1 = node1->next_sibling();
        
        xml_node<>* node11 = node1->first_node();
        std::cout<<node11->name()<<std::endl;
        std::cout<<node11->value()<<std::endl;

        node1 = node1->next_sibling();
        std::cout<<node1->name()<<std::endl;
        std::cout<<node1->value()<<std::endl;
        
        xml_attribute<> *attr = node1->first_attribute();
        _InfoLog( << attr->name() << ":" << attr->value() );
    }
    void testRead()
    {
        int nDeviceCount = 100;
        std::string strXml;
        int nTimes = 10000;
        
        ftduration d;
        for(int i = 0;i < nTimes;i++)
        {
            strXml = "<Message>";
            for(int nn = 0; nn < nDeviceCount; ++nn)
            {
                strXml += "   <Device ID='1111111111222222222233333333331' IP='127.0.0.1' />";
            }
            strXml += "</Message>";
            rapidxml::xml_document<> doc;
            doc.parse<0>((char *) strXml.c_str());
        }
        
        std::cout << "speed is " << nTimes / d.durationSecond() << " times/s, totol count is " << nTimes << ", used " << d.durationSecond() << " seconds. device count " << nDeviceCount * nTimes / d.durationSecond() << " dev/s" << std::endl;
    }
    void testWrite()
    {
        
    }
};

#endif /* defined(__TestAllUseful__test_rapidxml__) */
