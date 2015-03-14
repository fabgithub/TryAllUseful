
all : testProductConsume.cpp

TEST_SUITE_HEADER_FILES = \
	testProductConsume.h

TEST_MAIN_CPP=testProductConsume.cpp

${TEST_MAIN_CPP} : makefile $(TEST_SUITE_HEADER_FILES)
	cxxtestgen -w jx --main=testProductConsume_main --error-printer -o ${TEST_MAIN_CPP} $(TEST_SUITE_HEADER_FILES)


clean :
	rm -f testProductConsume.cpp
