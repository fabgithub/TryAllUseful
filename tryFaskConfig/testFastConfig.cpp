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

int testFastConfig_main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "jx";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_testFastConfig_init = false;
#include "/Volumes/Macintosh-HD/Test.localized/TestAllUseful/tryFaskConfig/testFastConfig.h"

static testFastConfig suite_testFastConfig;

static CxxTest::List Tests_testFastConfig = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_testFastConfig( "testFastConfig.h", 47, "testFastConfig", suite_testFastConfig, Tests_testFastConfig );

static class TestDescription_suite_testFastConfig_testCommon : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFastConfig_testCommon() : CxxTest::RealTestDescription( Tests_testFastConfig, suiteDescription_testFastConfig, 51, "testCommon" ) {}
 void runTest() { suite_testFastConfig.testCommon(); }
} testDescription_suite_testFastConfig_testCommon;

static class TestDescription_suite_testFastConfig_testConfigStruct : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFastConfig_testConfigStruct() : CxxTest::RealTestDescription( Tests_testFastConfig, suiteDescription_testFastConfig, 70, "testConfigStruct" ) {}
 void runTest() { suite_testFastConfig.testConfigStruct(); }
} testDescription_suite_testFastConfig_testConfigStruct;

static class TestDescription_suite_testFastConfig_testSetConfig : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFastConfig_testSetConfig() : CxxTest::RealTestDescription( Tests_testFastConfig, suiteDescription_testFastConfig, 74, "testSetConfig" ) {}
 void runTest() { suite_testFastConfig.testSetConfig(); }
} testDescription_suite_testFastConfig_testSetConfig;

static class TestDescription_suite_testFastConfig_testGetConfig : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFastConfig_testGetConfig() : CxxTest::RealTestDescription( Tests_testFastConfig, suiteDescription_testFastConfig, 78, "testGetConfig" ) {}
 void runTest() { suite_testFastConfig.testGetConfig(); }
} testDescription_suite_testFastConfig_testGetConfig;

static class TestDescription_suite_testFastConfig_testChangedRealtime : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFastConfig_testChangedRealtime() : CxxTest::RealTestDescription( Tests_testFastConfig, suiteDescription_testFastConfig, 82, "testChangedRealtime" ) {}
 void runTest() { suite_testFastConfig.testChangedRealtime(); }
} testDescription_suite_testFastConfig_testChangedRealtime;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
