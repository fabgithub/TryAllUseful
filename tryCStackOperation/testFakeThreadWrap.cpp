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

int testFakeThreadWrap_main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "jx";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_testFakeThreadWrap_init = false;
#include "../CStackOperation/testFakeThreadWrap.h"

static testFakeThreadWrap suite_testFakeThreadWrap;

static CxxTest::List Tests_testFakeThreadWrap = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_testFakeThreadWrap( "testFakeThreadWrap.h", 32, "testFakeThreadWrap", suite_testFakeThreadWrap, Tests_testFakeThreadWrap );

static class TestDescription_suite_testFakeThreadWrap_testCommon : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFakeThreadWrap_testCommon() : CxxTest::RealTestDescription( Tests_testFakeThreadWrap, suiteDescription_testFakeThreadWrap, 64, "testCommon" ) {}
 void runTest() { suite_testFakeThreadWrap.testCommon(); }
} testDescription_suite_testFakeThreadWrap_testCommon;

static class TestDescription_suite_testFakeThreadWrap_testYieldWithParam : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFakeThreadWrap_testYieldWithParam() : CxxTest::RealTestDescription( Tests_testFakeThreadWrap, suiteDescription_testFakeThreadWrap, 69, "testYieldWithParam" ) {}
 void runTest() { suite_testFakeThreadWrap.testYieldWithParam(); }
} testDescription_suite_testFakeThreadWrap_testYieldWithParam;

static class TestDescription_suite_testFakeThreadWrap_testResumeWithParam : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFakeThreadWrap_testResumeWithParam() : CxxTest::RealTestDescription( Tests_testFakeThreadWrap, suiteDescription_testFakeThreadWrap, 74, "testResumeWithParam" ) {}
 void runTest() { suite_testFakeThreadWrap.testResumeWithParam(); }
} testDescription_suite_testFakeThreadWrap_testResumeWithParam;

static class TestDescription_suite_testFakeThreadWrap_testReuseFakeThread : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFakeThreadWrap_testReuseFakeThread() : CxxTest::RealTestDescription( Tests_testFakeThreadWrap, suiteDescription_testFakeThreadWrap, 79, "testReuseFakeThread" ) {}
 void runTest() { suite_testFakeThreadWrap.testReuseFakeThread(); }
} testDescription_suite_testFakeThreadWrap_testReuseFakeThread;

static class TestDescription_suite_testFakeThreadWrap_testResumeInFakeThread : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFakeThreadWrap_testResumeInFakeThread() : CxxTest::RealTestDescription( Tests_testFakeThreadWrap, suiteDescription_testFakeThreadWrap, 160, "testResumeInFakeThread" ) {}
 void runTest() { suite_testFakeThreadWrap.testResumeInFakeThread(); }
} testDescription_suite_testFakeThreadWrap_testResumeInFakeThread;

static class TestDescription_suite_testFakeThreadWrap_testMemorySizeForFakeThread : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFakeThreadWrap_testMemorySizeForFakeThread() : CxxTest::RealTestDescription( Tests_testFakeThreadWrap, suiteDescription_testFakeThreadWrap, 182, "testMemorySizeForFakeThread" ) {}
 void runTest() { suite_testFakeThreadWrap.testMemorySizeForFakeThread(); }
} testDescription_suite_testFakeThreadWrap_testMemorySizeForFakeThread;

static class TestDescription_suite_testFakeThreadWrap_testMemorySizeForFakeThread2 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFakeThreadWrap_testMemorySizeForFakeThread2() : CxxTest::RealTestDescription( Tests_testFakeThreadWrap, suiteDescription_testFakeThreadWrap, 195, "testMemorySizeForFakeThread2" ) {}
 void runTest() { suite_testFakeThreadWrap.testMemorySizeForFakeThread2(); }
} testDescription_suite_testFakeThreadWrap_testMemorySizeForFakeThread2;

static class TestDescription_suite_testFakeThreadWrap_testResumeCount : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFakeThreadWrap_testResumeCount() : CxxTest::RealTestDescription( Tests_testFakeThreadWrap, suiteDescription_testFakeThreadWrap, 249, "testResumeCount" ) {}
 void runTest() { suite_testFakeThreadWrap.testResumeCount(); }
} testDescription_suite_testFakeThreadWrap_testResumeCount;

static class TestDescription_suite_testFakeThreadWrap_testResumeCountT : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFakeThreadWrap_testResumeCountT() : CxxTest::RealTestDescription( Tests_testFakeThreadWrap, suiteDescription_testFakeThreadWrap, 287, "testResumeCountT" ) {}
 void runTest() { suite_testFakeThreadWrap.testResumeCountT(); }
} testDescription_suite_testFakeThreadWrap_testResumeCountT;

static class TestDescription_suite_testFakeThreadWrap_testFakeThreadTemplate_Common : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFakeThreadWrap_testFakeThreadTemplate_Common() : CxxTest::RealTestDescription( Tests_testFakeThreadWrap, suiteDescription_testFakeThreadWrap, 332, "testFakeThreadTemplate_Common" ) {}
 void runTest() { suite_testFakeThreadWrap.testFakeThreadTemplate_Common(); }
} testDescription_suite_testFakeThreadWrap_testFakeThreadTemplate_Common;

static class TestDescription_suite_testFakeThreadWrap_testFakeThreadTemplate_Start : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFakeThreadWrap_testFakeThreadTemplate_Start() : CxxTest::RealTestDescription( Tests_testFakeThreadWrap, suiteDescription_testFakeThreadWrap, 349, "testFakeThreadTemplate_Start" ) {}
 void runTest() { suite_testFakeThreadWrap.testFakeThreadTemplate_Start(); }
} testDescription_suite_testFakeThreadWrap_testFakeThreadTemplate_Start;

static class TestDescription_suite_testFakeThreadWrap_testFakeThreadTemplate_Yield : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFakeThreadWrap_testFakeThreadTemplate_Yield() : CxxTest::RealTestDescription( Tests_testFakeThreadWrap, suiteDescription_testFakeThreadWrap, 353, "testFakeThreadTemplate_Yield" ) {}
 void runTest() { suite_testFakeThreadWrap.testFakeThreadTemplate_Yield(); }
} testDescription_suite_testFakeThreadWrap_testFakeThreadTemplate_Yield;

static class TestDescription_suite_testFakeThreadWrap_testFakeThreadTemplate_Resume : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_testFakeThreadWrap_testFakeThreadTemplate_Resume() : CxxTest::RealTestDescription( Tests_testFakeThreadWrap, suiteDescription_testFakeThreadWrap, 357, "testFakeThreadTemplate_Resume" ) {}
 void runTest() { suite_testFakeThreadWrap.testFakeThreadTemplate_Resume(); }
} testDescription_suite_testFakeThreadWrap_testFakeThreadTemplate_Resume;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
