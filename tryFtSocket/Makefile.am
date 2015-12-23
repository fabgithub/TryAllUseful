

AUTOMAKE_OPTIONS=foreign


noinst_PROGRAMS = libFtSocket.so

libFtSocket_so_SOURCES = ftCheckValid.cpp ftnetbuffer.cpp ftoswrap.cpp \
			ftnetselect.cpp ftsocket.cpp 

INCLUDES = -I../ -I../CStackOperation/

libFtSocket_so_INCLUDES = $(INCLUDES)

libFtSocket_so_LDADD = -L../CStackOperation/ -lCStackOperation

libFtSocket_so_LDFLAGS = -fPIC -shared -g

DEFS+= -fPIC


