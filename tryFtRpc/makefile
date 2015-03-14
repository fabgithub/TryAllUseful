
all : test_ftrpc.cpp

TEST_SUITE_HEADER_FILES = \
	test_ftrpc.h

TEST_MAIN_CPP=test_ftrpc.cpp

${TEST_MAIN_CPP} : makefile $(TEST_SUITE_HEADER_FILES)
	cxxtestgen -w jx --main=test_ftrpc_main --error-printer -o ${TEST_MAIN_CPP} $(TEST_SUITE_HEADER_FILES)


clean :
	rm -f test_ftrpc.cpp
