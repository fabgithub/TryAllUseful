
all : StackOperation.o FakeThreadStackOperation.o testFakeThreadWrap.cpp

TEST_SUITE_HEADER_FILES = \
	testFakeThreadWrap.h

TEST_MAIN_CPP=testFakeThreadWrap.cpp

${TEST_MAIN_CPP} : makefile $(TEST_SUITE_HEADER_FILES)
	cxxtestgen -w jx --main=testFakeThreadWrap_main --error-printer -o ${TEST_MAIN_CPP} $(TEST_SUITE_HEADER_FILES)

StackOperation.o : StackOperation.s makefile
	./nasm -f macho64 -o StackOperation.o StackOperation.s

FakeThreadStackOperation.o : FakeThreadStackOperation.s makefile
	./nasm -f macho64 -o FakeThreadStackOperation.o FakeThreadStackOperation.s


clean :
	rm -f testFakeThreadWrap.cpp StackOperation.o
