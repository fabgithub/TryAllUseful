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

int test_ftrpc_main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "jx";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_test_ftrpc_init = false;
#include "/Volumes/Macintosh-HD/Test.localized/TestAllUseful/tryFtRpc/test_ftrpc.h"

static test_ftrpc suite_test_ftrpc;

static CxxTest::List Tests_test_ftrpc = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_test_ftrpc( "test_ftrpc.h", 133, "test_ftrpc", suite_test_ftrpc, Tests_test_ftrpc );

static class TestDescription_suite_test_ftrpc_testCommon : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftrpc_testCommon() : CxxTest::RealTestDescription( Tests_test_ftrpc, suiteDescription_test_ftrpc, 278, "testCommon" ) {}
 void runTest() { suite_test_ftrpc.testCommon(); }
} testDescription_suite_test_ftrpc_testCommon;

static class TestDescription_suite_test_ftrpc_testRemoteCall : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftrpc_testRemoteCall() : CxxTest::RealTestDescription( Tests_test_ftrpc, suiteDescription_test_ftrpc, 331, "testRemoteCall" ) {}
 void runTest() { suite_test_ftrpc.testRemoteCall(); }
} testDescription_suite_test_ftrpc_testRemoteCall;

static class TestDescription_suite_test_ftrpc_testPerform : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftrpc_testPerform() : CxxTest::RealTestDescription( Tests_test_ftrpc, suiteDescription_test_ftrpc, 388, "testPerform" ) {}
 void runTest() { suite_test_ftrpc.testPerform(); }
} testDescription_suite_test_ftrpc_testPerform;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
