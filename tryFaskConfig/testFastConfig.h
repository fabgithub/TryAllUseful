//
//  testFastConfig.h
//  TestAllUseful
//
//  Created by liyoudi on 15/3/7.
//  Copyright (c) 2015 liyoudi. All rights reserved.
//

#ifndef __TestAllUseful__testFastConfig__
#define __TestAllUseful__testFastConfig__

#include "FastConfig.h"
#include "ftduration.h"

#include "cxxtest/TestSuite.h"

const char * NFGetFileNameFromPath(const char *szFilePath);

#ifndef InfoLog
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
#endif
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
fc_IMPLEMENT_STRUCT_MEMBER(ip, "server ip", "MUST config", NULL, 2, ("Server", "ip"))
fc_IMPLEMENT_STRUCT_MEMBER(port, "server port", "MUST config", NULL, 2, ("Server", "port"))
fc_IMPLEMENT_STRUCT_MEMBER_FromAttribute(port, "server port", "MUST config", "Enable", 0, ())
fc_IMPLEMENT_STRUCT_END
//
//void fcRegisterVar(FastConfigFormat &fs, ConfigStruct &vt, const char *szUsage, const char *szDescription, const char *szAttrName, std::list<const char *> &pathList);;
//void fcRegisterVar(FastConfigFormat &fs, ConfigStruct &vt, const char *szUsage, const char *szDescription, const char *szAttrName, std::list<const char *> &pathList)
//{
//    pathList.push_back("Server");
//    pathList.push_back("ip");
//    fcRegisterVar(fs, vt.ip, "server ip", "MUST config", NULL, pathList);
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
FastConfig<std::string> attrAtRoot("attrAtRoot", "attrAtRoot", "attrAtRoot", 0);

std::string strXml =
"<root attrAtRoot='this is attribute in root element.'>\r\n"
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

struct Mega_Code_Info {
    std::string m_sCode_type;
    int         m_nPlayloadType;
};

fc_DECLARE_STRUCT(Mega_Code_Info)
fc_IMPLEMENT_STRUCT_BEGIN(Mega_Code_Info)
fc_IMPLEMENT_STRUCT_MEMBER(m_nPlayloadType, "PayloadType", "ID", NULL, 1, ("PayloadType"))
fc_IMPLEMENT_STRUCT_MEMBER(m_sCode_type, "CodeType", "type", NULL, 1, ("CodeType"))
fc_IMPLEMENT_STRUCT_END

class Req300002Body
{
public:
    Req300002Body() : mnNat(0), mstrDevDataID(""), mstrSvrIP(""), mnVideoPort(0),mstrTicket(""),
    mstrStreamType(""),mstrProtocol(""),mnSenderSsrc(0),mstrDevNaming(""),mstrSipticket(""),
    mstrStorePostion(""),mstrPlayModel("'"),mstrHistoryMode(""),mstrMediaInfo(""),
    mstrBeginTime(""),mstrEndTime(""){}
public:
    int mnNat;
    std::string mstrDevDataID;
    std::string mstrSvrIP;
    int  mnVideoPort;
    std::string  mstrTicket;
    std::string  mstrStreamType;
    std::string  mstrProtocol;
    int mnSenderSsrc;
    std::string mstrDevNaming;
    std::string mstrSipticket;
    std::string  mstrStorePostion;
    std::string mstrPlayModel;
    std::list<Mega_Code_Info> mlistMegaCodeInfo;
    std::string  mstrHistoryMode;
    std::string  mstrMediaInfo;
    std::string  mstrBeginTime;
    std::string  mstrEndTime;
};

bool operator == (const Req300002Body &r1, const Req300002Body &r2)
{
    return r1.mnNat == r2.mnNat &&
    r1.mstrDevDataID == r2.mstrDevDataID &&
    r1.mstrSvrIP == r2.mstrSvrIP &&
    r1.mnVideoPort == r2.mnVideoPort &&
    r1.mstrTicket == r2.mstrTicket &&
    r1.mstrStreamType == r2.mstrStreamType &&
    r1.mstrProtocol == r2.mstrProtocol &&
    r1.mnSenderSsrc == r2.mnSenderSsrc &&
    r1.mstrDevNaming == r2.mstrDevNaming &&
    r1.mstrSipticket == r2.mstrSipticket &&
    r1.mstrStorePostion == r2.mstrStorePostion &&
    r1.mstrPlayModel == r2.mstrPlayModel &&
//    r1.mlistMegaCodeInfo == r2.mlistMegaCodeInfo &&
    r1.mstrHistoryMode == r2.mstrHistoryMode &&
    r1.mstrMediaInfo == r2.mstrMediaInfo &&
    r1.mstrBeginTime == r2.mstrBeginTime &&
    r1.mstrEndTime == r2.mstrEndTime
    ;
}

fc_DECLARE_STRUCT(Req300002Body)
fc_IMPLEMENT_STRUCT_BEGIN(Req300002Body)
fc_IMPLEMENT_STRUCT_MEMBER(mnNat, "NAT", "ID", NULL, 1, ("NAT"))
fc_IMPLEMENT_STRUCT_MEMBER(mstrDevDataID, "DevDataID", "type", NULL, 1, ("DevDataID"))
fc_IMPLEMENT_STRUCT_MEMBER(mstrSvrIP, "SvrIP", "ID", NULL, 1, ("SvrIP"))
fc_IMPLEMENT_STRUCT_MEMBER(mnVideoPort, "VideoPort", "type", NULL, 1, ("VideoPort"))
fc_IMPLEMENT_STRUCT_MEMBER(mstrTicket, "Ticket", "type", NULL, 1, ("Ticket"))
fc_IMPLEMENT_STRUCT_MEMBER(mstrStreamType, "StreamType", "type", NULL, 1, ("StreamType"))
fc_IMPLEMENT_STRUCT_MEMBER(mstrProtocol, "Protocol", "type", NULL, 1, ("Protocol"))
fc_IMPLEMENT_STRUCT_MEMBER(mnSenderSsrc, "SenderSsrc", "type", NULL, 1, ("SenderSsrc"))
fc_IMPLEMENT_STRUCT_MEMBER(mstrDevNaming, "DevNaming", "type", NULL, 1, ("DevNaming"))
fc_IMPLEMENT_STRUCT_MEMBER(mstrSipticket, "SipTicket", "type", NULL, 1, ("SipTicket"))
fc_IMPLEMENT_STRUCT_MEMBER(mstrStorePostion, "StorePos", "type", NULL, 1, ("StorePos"))
fc_IMPLEMENT_STRUCT_MEMBER(mstrPlayModel, "PlayModel", "type", NULL, 1, ("PlayModel"))
fc_IMPLEMENT_STRUCT_MEMBER(mlistMegaCodeInfo, "PrivateStream", "type", NULL, 1, ("PrivateStream"))
fc_IMPLEMENT_STRUCT_MEMBER(mstrHistoryMode, "HistoryMode", "type", NULL, 1, ("HistoryMode"))
fc_IMPLEMENT_STRUCT_MEMBER(mstrMediaInfo, "MediaInfo", "type", NULL, 1, ("MediaInfo"))
fc_IMPLEMENT_STRUCT_MEMBER_FromAttribute(mstrBeginTime, "start_time", "MUST config", "start_time", 1, ("HistoryInfo"))
fc_IMPLEMENT_STRUCT_MEMBER_FromAttribute(mstrEndTime, "end_time", "MUST config", "end_time", 1, ("HistoryInfo"))
fc_IMPLEMENT_STRUCT_END;

static FastConfig<Req300002Body> gfcReq300002Body("300002", "req 300002", NULL, 0);


const char *gsz3002Xml =
"<?xml version='1.0'?>"
"<Message>"
"    <NAT>0</NAT>"
"    <DevDataID>0000000000200000000000000380000</DevDataID>"
"    <SvrIP>192.168.4.120</SvrIP>"
"    <VideoPort>4607</VideoPort>"
"    <Ticket>P2UDmSWHhgS6DaIr</Ticket>"
"    <StreamType></StreamType>"
"    <Protocol></Protocol>"
"    <SenderSsrc>1000000037</SenderSsrc>"
"    <DevNaming>0000000000200000000000000380000:0000000000140000000000000350000:192.168.4.120:130000</DevNaming>"
"    <SipTicket>sipTicket-P2UDmSWHhgS6DaIr-233089303-213567716</SipTicket>"
"    <StorePos>center</StorePos>"
"    <PlayModel>history</PlayModel>"
"    <PrivateStream>"
"        <CodeType>PS</CodeType>"
"        <PayloadType>96</PayloadType>"
"    </PrivateStream>"
"    <HistoryMode>playback</HistoryMode>"
"    <MediaInfo>tcp</MediaInfo>"
"    <HistoryInfo RecordType='time' user=' pwd=' start_time='2015-04-28 09:57:11' end_time='2015-04-28 09:59:11' />"
"</Message>"
;

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
            TS_ASSERT_EQUALS(attrAtRoot.get(), "this is attribute in root element.");
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
    {
        Req300002Body b1, b2;
        int nRet = 0;
        CommonParser1<Req300002Body> cp = fcGetCommonParser(gfcReq300002Body);
        nRet = cp.ParseXmlText(gsz3002Xml, b1);
        TS_ASSERT_EQUALS(0, nRet);
        nRet = cp.ParseXmlText(gsz3002Xml, b1);
        TS_ASSERT_EQUALS(0, nRet);
//        cp = fcGetCommonParser(gfcReq300002Body);
        nRet = cp.ParseXmlText(gsz3002Xml, b2);
        TS_ASSERT_EQUALS(0, nRet);
        TS_ASSERT_EQUALS(b1, b2);
    }
    void Perform()
    {
        std::string strXml;
        int nDeviceCount = 10;
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
        std::cout << "speed is " << nTimes / d.durationSecond() << " times/s, total count is " << nTimes << ", used " << d.durationSecond() << " seconds. device count " << nDeviceCount * nTimes / d.durationSecond() << " dev/s" << std::endl;
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
