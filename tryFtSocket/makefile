
all : test_ftsocket.cpp

TEST_SUITE_HEADER_FILES = \
	test_ftsocket.h

TEST_MAIN_CPP=test_ftsocket.cpp

${TEST_MAIN_CPP} : makefile $(TEST_SUITE_HEADER_FILES)
	cxxtestgen -w jx --main=test_ftsocket_main --error-printer -o ${TEST_MAIN_CPP} $(TEST_SUITE_HEADER_FILES)


clean :
	rm -f test_ftsocket.cpp
