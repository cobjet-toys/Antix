
LOGLIBNAME = libredisclient.a
LOGLIBS = $(LOGLIBNAME) -lstdc++ -lpthread -lboost_thread-mt
LOGCLIENTOBJS = anet.o

LOGHDR = redisclient.h redislist.h

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

anet.o:			anet.c fmacros.h anet.h
logger.o:		redisclient.h redislist.h
