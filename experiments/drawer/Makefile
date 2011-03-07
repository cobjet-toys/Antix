#   Makefile - antix project
#   version 3
#   Richard Vaughan  

# this should work on Linux with MESA
OS := $(shell uname -s)

ifeq ($(OS),Linux)
GLUTLIBS = -L/usr/X11R6/lib -lGLU -lGL -lglut -lX11 -lXext
GLUTFLAGS = -I/usr/local/include/GL
endif

# this works on Mac OS X
ifeq ($(OS),Darwin)
GLUTFLAGS = -framework OpenGL -framework GLUT
endif

# logger macros
LOGLIBNAME = logger/libredisclient.a
LOGLIBS = $(LOGLIBNAME) -lstdc++ -lpthread -lboost_thread-mt
LOGCLIENTOBJS = anet.o

CC = g++
CXXFLAGS = -g -Wall -O3 $(GLUTFLAGS)
LIBS =  -g -lm $(GLUTLIBS) 
INCLUDE = -I"logger/"

SRC = DrawServer.cpp DrawClient.cpp main.cpp

LOGHDR = logger/redisclient.h logger/logger.h

all: drawer client

drawer: $(SRC) $(LOGHDR)
	$(CC) $(CXXFLAGS) $(LIBS) $(INCLUDE) -DREDIS -o $@ $(SRC) $(LOGLIBS)

client: $(SRC) $(LOGHDR)
	$(CC) $(CXXFLAGS) $(LIBS) $(INCLUDE) -DREDIS -o $@ $(SRC) $(LOGLIBS)

clean_drawer:
	rm drawer

clean_client:
	rm client

anet.o:			logger/anet.c logger/fmacros.h logger/anet.h
rhash.o:		logger/redisclient.h logger/logger.h

