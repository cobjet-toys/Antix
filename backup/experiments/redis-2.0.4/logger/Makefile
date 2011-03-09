# Redis C++ Client Library Makefile

VPATH = tests

#CFLAGS?= -pedantic -O2 -Wall -DNEBUG -W
CFLAGS?= -pedantic -O0 -W -DDEBUG -g
CC = g++

CLIENTOBJS = anet.o
LIBNAME = libredisclient.a

TESTAPP = test_client
TESTAPPOBJS = test_client.o logger.o
TESTAPPLIBS = $(LIBNAME) -lstdc++ -lpthread -lboost_thread-mt

all: $(LIBNAME) $(TESTAPP)

$(LIBNAME): $(CLIENTOBJS)
	ar rcs $(LIBNAME) $(CLIENTOBJS)

.c.o:
	$(CC) -c $(CFLAGS) $<

.cpp.o:
	$(CC) -c $(CFLAGS) $<

$(TESTAPP): $(LIBNAME) $(TESTAPPOBJS)
	$(CC) -o $(TESTAPP) $(TESTAPPOBJS) $(TESTAPPLIBS)

test: $(TESTAPP)
	@./test_client

check: test

clean:
	rm -rf $(LIBNAME) *.o $(TESTAPP)

dep:
	$(CC) -MM *.c *.cpp

log:
	git log '--pretty=format:%ad %s' --date=short > Changelog

anet.o:                     anet.c fmacros.h anet.h
test_client.o:              redisclient.h logger.h test_client.cpp
logger.o:		    redisclient.h logger.h
