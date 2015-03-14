//
//  testFastConfig.h
//  TestAllUseful
//
//  Created by liyoudi on 15/3/7.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__testFastConfig__
#define __TestAllUseful__testFastConfig__

#include "FastConfig.h"
#include "ftduration.h"

#include "cxxtest/TestSuite.h"

const char * NFGetFileNameFromPath(const char *szFilePath);

#define InfoLog( args ) std::cout << " " << NFGetFileNameFromPath(__FILE__) << "(" << __LINE__ << ") " << __FUNCTION__ << ": " args << std::endl

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

//
class ConfigStruct
{
public:
    std::string ip;
    int port;
    bool bEnable;
};

fc_DECLARE_STRUCT(ConfigStruct)
fc_IMPLEMENT_STRUCT_BEGIN(ConfigStruct)
fc_IMPLEMENT_STRUCT_MEMBER(ip, "server ip", "MUST config", NULL, 2, "Server", "ip")
fc_IMPLEMENT_STRUCT_MEMBER(port, "server port", "MUST config", NULL, 2, "Server", "port")
fc_IMPLEMENT_STRUCT_MEMBER_FromAttribute(port, "server port", "MUST config", "Enable")
fc_IMPLEMENT_STRUCT_END
//
//void fcRegisterVar(FastConfigFormat &fs, ConfigStruct &vt, const char *szUsage, const char *szDescription, const char *szAttrName, std::list<const char *> &pathList);;
//void fcRegisterVar(FastConfigFormat &fs, ConfigStruct &vt, const char *szUsage, const char *szDescription, const char *szAttrName, std::list<const char *> &pathList)
//{
//    pathList.push_back("Server");
//    pathList.push_back("ip");
//    fcRegisterVar(fs, vt.ip, "server ip", "MUST config", __null, pathList);
//    { for(int i = 0;i < 2;++i) pathList.pop_back(); };;
//}
FastConfig<std::list<int> > idList("ids", "this is the list of id.", NULL, 1, "id");
FastConfig<std::vector<int> > idVec("ids", "this is the list of id.", NULL, 1, "id", NULL);
FastConfig<std::list<ConfigStruct> > serverList("ids", "this is the list of id.", NULL, 1, "CS", NULL);
FastConfig<int> gfcnCount(-1, "number of count", "this is the test count config.", NULL, 2, "first", "Count", NULL);
FastConfig<int> gfcnCount2(-2, "number of count", "this is the test count config.", NULL, 2, "first", "Count", NULL);
FastConfig<int> gfcnCount3(-3, "number of count", "this is the test count config.", NULL, 2, "first", "Count3", NULL);
FastConfig<bool> gfcbHasAttr(false, "number of count", "this is the test count config.", NULL, 2, "first", "HasAttr", NULL);
FastConfig<ConfigStruct> cs("server config", "MUST config server", NULL, 1, "CS", NULL);
FastConfig<int> nPort("port", "port too", NULL, 3, "CS", "Server", "port", NULL);
FastConfig<int> nLastId("nLastId", "nLastId too", NULL, 1, "id", NULL);
FastConfig<std::list<std::string> > names("names", "", "name", 1, "id", NULL);

std::string strXml =
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
//
class testFastConfig : public CxxTest::TestSuite
{
    int mnFinish;
public:
    void testCommon()
    {

        int nRet = 0;
        nRet = fcInitConfig(strXml.c_str(), _GetRootFE());
        TS_ASSERT_EQUALS(0, nRet);
        if(!nRet)
        {
            TS_ASSERT_EQUALS(3, gfcnCount.get());
            TS_ASSERT_EQUALS(3, gfcnCount2.get());
            TS_ASSERT_EQUALS(-3, gfcnCount3.get());
            TS_ASSERT_EQUALS(true, gfcbHasAttr.get());
            TS_ASSERT_EQUALS(6001, cs.get().port);
            TS_ASSERT_EQUALS("192.168.5.5", cs.get().ip);
            TS_ASSERT_EQUALS(6001, nPort.get());
            TS_ASSERT_EQUALS(nPort.get(), cs.get().port);
            TS_ASSERT_EQUALS(5, idList.get().size());
            TS_ASSERT_EQUALS(5, idVec.get().size());
            TS_ASSERT_EQUALS(2, serverList.get().size());
            TS_ASSERT_EQUALS(5, nLastId.get());
            TS_ASSERT_EQUALS(5, names.get().size());
        }
    }
    void testConfigStruct()
    {
        std::list<std::string> onlyNames;
        fcGetCommonParser(names).ParseXmlText(strXml.c_str(), onlyNames);
        TS_ASSERT_EQUALS(onlyNames.size(), 5);
    }
    void testSetConfig()
    {
        std::list<std::string> onlyNames;
        ConfigStruct onlyCs;
        std::list<ConfigStruct> svrL;
        int port = 0;
        int port2 = 0;
        int nRet = 0;
        nRet = fcGetCommonParser(names, cs, serverList).ParseXmlText(strXml.c_str(), onlyNames, onlyCs, svrL);
        TS_ASSERT_EQUALS(0, nRet);
        nRet = fcGetCommonParser(nPort, nPort).ParseXmlText(strXml.c_str(), port, port2);
        TS_ASSERT_EQUALS(0, nRet);
        TS_ASSERT_EQUALS(onlyNames.size(), 5);
        TS_ASSERT_EQUALS(port, 6001);
        TS_ASSERT_EQUALS(port, port2);
    }
    void testGetConfig()
    {}
    void Perform()
    {
        std::string strXml;
        int nDeviceCount = 60;
        int nTimes = 10000;
        FastConfig<std::vector<std::string> > idList("ID vector", "id vector", "ID", 1, "Device");
        FastConfig<std::vector<std::string> > ipList("IP vector", "ip vector", "IP", 1, "Device");
        FastConfig<int> iPort("IP vector", "ip vector", "port", 1, "Device");
        std::vector<std::string> ids, ips;
        int somePort = 0;
        fcGetCommonParser(idList, ipList).ParseXmlText(strXml.c_str(), ids, ips);
        ftduration d;
        int n = 0;
        for (n = 0; n < nTimes; ++n)
        {
            strXml = "<Message>";
            for(int nn = 0; nn < nDeviceCount; ++nn)
            {
                strXml += "   <Device ID='1111111111222222222233333333331' IP='127.0.0.1' />";
            }
            strXml += "</Message>";

            ids.clear();
            ips.clear();
            fcGetCommonParser(idList, ipList).ParseXmlText(strXml.c_str(), ids, ips);
            fcGetCommonParser(iPort).ParseXmlText(strXml.c_str(), somePort);
        }
        std::cout << "speed is " << nTimes / d.durationSecond() << " times/s, totol count is " << nTimes << ", used " << d.durationSecond() << " seconds. device count " << nDeviceCount * nTimes / d.durationSecond() << " dev/s" << std::endl;
    }
    void testChangedRealtime()
    {
        std::cout << "use rapid xml" << std::endl;
        fcSetUseRapidXml(true);
        Perform();
        std::cout << "use CMarkupSTL xml" << std::endl;
        fcSetUseRapidXml(false);
        Perform();
    }
};

#endif /* defined(__TestAllUseful__testFastConfig__) */
