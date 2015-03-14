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

int test_rapidxml_main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "jx";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_test_rapidxml_init = false;
#include "/Volumes/Macintosh-HD/Test.localized/TestAllUseful/tryRapidXml/test_rapidxml.h"

static test_rapidxml suite_test_rapidxml;

static CxxTest::List Tests_test_rapidxml = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_test_rapidxml( "test_rapidxml.h", 16, "test_rapidxml", suite_test_rapidxml, Tests_test_rapidxml );

static class TestDescription_suite_test_rapidxml_testCommon : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_rapidxml_testCommon() : CxxTest::RealTestDescription( Tests_test_rapidxml, suiteDescription_test_rapidxml, 19, "testCommon" ) {}
 void runTest() { suite_test_rapidxml.testCommon(); }
} testDescription_suite_test_rapidxml_testCommon;

static class TestDescription_suite_test_rapidxml_testRead : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_rapidxml_testRead() : CxxTest::RealTestDescription( Tests_test_rapidxml, suiteDescription_test_rapidxml, 23, "testRead" ) {}
 void runTest() { suite_test_rapidxml.testRead(); }
} testDescription_suite_test_rapidxml_testRead;

static class TestDescription_suite_test_rapidxml_testWrite : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_test_rapidxml_testWrite() : CxxTest::RealTestDescription( Tests_test_rapidxml, suiteDescription_test_rapidxml, 27, "testWrite" ) {}
 void runTest() { suite_test_rapidxml.testWrite(); }
} testDescription_suite_test_rapidxml_testWrite;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
