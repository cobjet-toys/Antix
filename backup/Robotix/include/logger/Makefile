LOGLIBNAME = libredisclient.a
LOGLIBS = $(LOGLIBNAME) -lstdc++ -lpthread -lboost_thread-mt
LOGCLIENTOBJS = anet.o
TESTAPPOBJS = logger_client.o logger.o

CC = g++
CXXFLAGS = -g -O3 -Wall $(GLUTFLAGS)
LIBS =  $(LOGLIBS)

HDR = antix.h controller.h redisclient.h logger.h
SRC = antix.cc controller.cc gui.cc main.cc 

all: $(LOGLIBNAME) export_log

$(LOGLIBNAME): $(LOGCLIENTOBJS)
	ar rcs $@ $(LOGCLIENTOBJS)

antix: $(SRC) $(HDR)
	$(CC) $(CXXFLAGS) $(LIBS) -o $@ $(SRC) 

export_log: $(LOGLIBNAME) $(TESTAPPOBJS)
	$(CC) -o $@ $(TESTAPPOBJS) $(LOGLIBS)

clean:
	rm -f *.o export_log

anet.o:                     anet.c fmacros.h anet.h
logger_client.o:            logger.h logger_client.cpp
logger.o:		    redisclient.h logger.h
