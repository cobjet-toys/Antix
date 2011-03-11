
LOGLIBNAME = libredisclient.a
LOGLIBS = $(LOGLIBNAME) -lstdc++ -lpthread -lboost_thread-mt
LOGCLIENTOBJS = anet.o

LOGHDR = redisclient.h logger.h

CC = g++
INCLUDE =
BASIC_HDR = $(LOGHDR)
DRAWER_CXXFLAGS = -g -Wall -O3 $(GLUTFLAGS)
DRAWER_LIBS =  -g -lm $(GLUTLIBS)

TEST_SRC = logger_client.cpp

all: test_redis

test_redis: $(BASIC_HDR) $(TEST_SRC)
	$(CC) $(INCLUDE) -DREDIS -o $@ $(TEST_SRC) $(LOGLIBS)

clean:
	rm test_redis

anet.o:			logger/anet.c logger/fmacros.h logger/anet.h
logger.o:		logger/redisclient.h logger/logger.h




#LOGLIBNAME = libredisclient.a
#LOGLIBS = $(LOGLIBNAME) -lstdc++ -lpthread -lboost_thread-mt
#LOGCLIENTOBJS = anet.o
#TESTAPPOBJS = logger_client.o logger.o

#CC = g++
#CXXFLAGS = -g -O3 -Wall $(GLUTFLAGS)
#LIBS =  $(LOGLIBS)

#HDR = redisclient.h logger.h

#all: $(LOGLIBNAME) export_log

#$(LOGLIBNAME): $(LOGCLIENTOBJS)
#	ar rcs $@ $(LOGCLIENTOBJS)

#export_log: $(LOGLIBNAME) $(TESTAPPOBJS)
#	$(CC) -o $@ $(TESTAPPOBJS) $(LOGLIBS)

#clean:
#	rm -f *.o export_log

#anet.o:                     anet.c fmacros.h anet.h
#logger_client.o:            logger.h logger_client.cpp
#logger.o:		    redisclient.h logger.h
