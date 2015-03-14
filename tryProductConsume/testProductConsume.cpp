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

int testProductConsume_main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "jx";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_test_ftProductConsume_init = false;
#include "/Volumes/Macintosh-HD/Test.localized/TestAllUseful/tryProductConsume/testProductConsume.h"

static test_ftProductConsume suite_test_ftProductConsume;

static CxxTest::List Tests_test_ftProductConsume = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_test_ftProductConsume( "testProductConsume.h", 51, "test_ftProductConsume", suite_test_ftProductConsume, Tests_test_ftProductConsume );

static class TestDescription_suite_test_ftProductConsume_testCommon : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftProductConsume_testCommon() : CxxTest::RealTestDescription( Tests_test_ftProductConsume, suiteDescription_test_ftProductConsume, 142, "testCommon" ) {}
 void runTest() { suite_test_ftProductConsume.testCommon(); }
} testDescription_suite_test_ftProductConsume_testCommon;

static class TestDescription_suite_test_ftProductConsume_testProduct : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftProductConsume_testProduct() : CxxTest::RealTestDescription( Tests_test_ftProductConsume, suiteDescription_test_ftProductConsume, 164, "testProduct" ) {}
 void runTest() { suite_test_ftProductConsume.testProduct(); }
} testDescription_suite_test_ftProductConsume_testProduct;

static class TestDescription_suite_test_ftProductConsume_testConsume : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftProductConsume_testConsume() : CxxTest::RealTestDescription( Tests_test_ftProductConsume, suiteDescription_test_ftProductConsume, 180, "testConsume" ) {}
 void runTest() { suite_test_ftProductConsume.testConsume(); }
} testDescription_suite_test_ftProductConsume_testConsume;

static class TestDescription_suite_test_ftProductConsume_testProduct2 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftProductConsume_testProduct2() : CxxTest::RealTestDescription( Tests_test_ftProductConsume, suiteDescription_test_ftProductConsume, 198, "testProduct2" ) {}
 void runTest() { suite_test_ftProductConsume.testProduct2(); }
} testDescription_suite_test_ftProductConsume_testProduct2;

static class TestDescription_suite_test_ftProductConsume_testDCommon : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftProductConsume_testDCommon() : CxxTest::RealTestDescription( Tests_test_ftProductConsume, suiteDescription_test_ftProductConsume, 287, "testDCommon" ) {}
 void runTest() { suite_test_ftProductConsume.testDCommon(); }
} testDescription_suite_test_ftProductConsume_testDCommon;

static class TestDescription_suite_test_ftProductConsume_testDProduct : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftProductConsume_testDProduct() : CxxTest::RealTestDescription( Tests_test_ftProductConsume, suiteDescription_test_ftProductConsume, 309, "testDProduct" ) {}
 void runTest() { suite_test_ftProductConsume.testDProduct(); }
} testDescription_suite_test_ftProductConsume_testDProduct;

static class TestDescription_suite_test_ftProductConsume_testDConsume : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftProductConsume_testDConsume() : CxxTest::RealTestDescription( Tests_test_ftProductConsume, suiteDescription_test_ftProductConsume, 325, "testDConsume" ) {}
 void runTest() { suite_test_ftProductConsume.testDConsume(); }
} testDescription_suite_test_ftProductConsume_testDConsume;

static class TestDescription_suite_test_ftProductConsume_testDProduct2 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_ftProductConsume_testDProduct2() : CxxTest::RealTestDescription( Tests_test_ftProductConsume, suiteDescription_test_ftProductConsume, 343, "testDProduct2" ) {}
 void runTest() { suite_test_ftProductConsume.testDProduct2(); }
} testDescription_suite_test_ftProductConsume_testDProduct2;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
