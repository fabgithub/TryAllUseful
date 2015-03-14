
all : test_rapidxml.cpp

TEST_SUITE_HEADER_FILES = \
	test_rapidxml.h

TEST_MAIN_CPP=test_rapidxml.cpp

${TEST_MAIN_CPP} : makefile $(TEST_SUITE_HEADER_FILES)
	cxxtestgen -w jx --main=test_rapidxml_main --error-printer -o ${TEST_MAIN_CPP} $(TEST_SUITE_HEADER_FILES)


clean :
	rm -f test_rapidxml.cpp
