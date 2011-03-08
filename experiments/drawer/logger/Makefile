LOGLIBNAME = libredisclient.a
LOGLIBS = $(LOGLIBNAME) -lstdc++ -lpthread -lboost_thread-mt
LOGCLIENTOBJS = anet.o
TESTAPPOBJS = logger_client.o logger.o

CC = g++
CXXFLAGS = -g -O3 -Wall $(GLUTFLAGS)
LIBS =  $(LOGLIBS)

HDR = redisclient.h logger.h

all: $(LOGLIBNAME) export_log

$(LOGLIBNAME): $(LOGCLIENTOBJS)
	ar rcs $@ $(LOGCLIENTOBJS)

export_log: $(LOGLIBNAME) $(TESTAPPOBJS)
	$(CC) -o $@ $(TESTAPPOBJS) $(LOGLIBS)

clean:
	rm -f *.o export_log

anet.o:                     anet.c fmacros.h anet.h
logger_client.o:            logger.h logger_client.cpp
logger.o:		    redisclient.h logger.h
