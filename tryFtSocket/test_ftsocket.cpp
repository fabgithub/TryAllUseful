/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int test_ftsocket_main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "jx";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_test_ftsocket_init = false;
#include "/Volumes/Macintosh-HD/Test.localized/TestAllUseful/tryFtSocket/test_ftsocket.h"

static test_ftsocket suite_test_ftsocket;

static CxxTest::List Tests_test_ftsocket = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_test_ftsocket( "test_ftsocket.h", 28, "test_ftsocket", suite_test_ftsocket, Tests_test_ftsocket );

static class TestDescription_suite_test_ftsocket_testCommon : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftsocket_testCommon() : CxxTest::RealTestDescription( Tests_test_ftsocket, suiteDescription_test_ftsocket, 163, "testCommon" ) {}
 void runTest() { suite_test_ftsocket.testCommon(); }
} testDescription_suite_test_ftsocket_testCommon;

static class TestDescription_suite_test_ftsocket_testConnect : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftsocket_testConnect() : CxxTest::RealTestDescription( Tests_test_ftsocket, suiteDescription_test_ftsocket, 183, "testConnect" ) {}
 void runTest() { suite_test_ftsocket.testConnect(); }
} testDescription_suite_test_ftsocket_testConnect;

static class TestDescription_suite_test_ftsocket_testSend : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftsocket_testSend() : CxxTest::RealTestDescription( Tests_test_ftsocket, suiteDescription_test_ftsocket, 187, "testSend" ) {}
 void runTest() { suite_test_ftsocket.testSend(); }
} testDescription_suite_test_ftsocket_testSend;

static class TestDescription_suite_test_ftsocket_testRecv : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftsocket_testRecv() : CxxTest::RealTestDescription( Tests_test_ftsocket, suiteDescription_test_ftsocket, 191, "testRecv" ) {}
 void runTest() { suite_test_ftsocket.testRecv(); }
} testDescription_suite_test_ftsocket_testRecv;

static class TestDescription_suite_test_ftsocket_testPerform : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftsocket_testPerform() : CxxTest::RealTestDescription( Tests_test_ftsocket, suiteDescription_test_ftsocket, 294, "testPerform" ) {}
 void runTest() { suite_test_ftsocket.testPerform(); }
} testDescription_suite_test_ftsocket_testPerform;

static class TestDescription_suite_test_ftsocket_testUdpCommon : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftsocket_testUdpCommon() : CxxTest::RealTestDescription( Tests_test_ftsocket, suiteDescription_test_ftsocket, 313, "testUdpCommon" ) {}
 void runTest() { suite_test_ftsocket.testUdpCommon(); }
} testDescription_suite_test_ftsocket_testUdpCommon;

static class TestDescription_suite_test_ftsocket_testUdpPerform : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftsocket_testUdpPerform() : CxxTest::RealTestDescription( Tests_test_ftsocket, suiteDescription_test_ftsocket, 317, "testUdpPerform" ) {}
 void runTest() { suite_test_ftsocket.testUdpPerform(); }
} testDescription_suite_test_ftsocket_testUdpPerform;

static class TestDescription_suite_test_ftsocket_testUdpProtocol : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftsocket_testUdpProtocol() : CxxTest::RealTestDescription( Tests_test_ftsocket, suiteDescription_test_ftsocket, 321, "testUdpProtocol" ) {}
 void runTest() { suite_test_ftsocket.testUdpProtocol(); }
} testDescription_suite_test_ftsocket_testUdpProtocol;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
