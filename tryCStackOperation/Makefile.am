

AUTOMAKE_OPTIONS=foreign


noinst_PROGRAMS = libCStackOperation.so

libCStackOperation_so_SOURCES = FakeThreadLock.cpp FakeThreadWrap.cpp FakeThreadTemplate.cpp ../FtSocket/ftduration.cpp
if OS_64_
cstack_obj = FakeThreadStackOperation_linux64.o
else
cstack_obj = FakeThreadStackOperation_linux32.o
endif

INCLUDES = -I../ -I../FtSocket/


libCStackOperation_so_INCLUDES = $(INCLUDES)

libCStackOperation_so_LDADD = $(cstack_obj) 

libCStackOperation_so_LDFLAGS = -fPIC -shared -g

DEFS+= -fPIC


