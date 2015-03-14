
all : testFastConfig.cpp

TEST_SUITE_HEADER_FILES = \
	testFastConfig.h

TEST_MAIN_CPP=testFastConfig.cpp

${TEST_MAIN_CPP} : makefile $(TEST_SUITE_HEADER_FILES)
	cxxtestgen -w jx --main=testFastConfig_main --error-printer -o ${TEST_MAIN_CPP} $(TEST_SUITE_HEADER_FILES)


clean :
	rm -f testFastConfig.cpp
