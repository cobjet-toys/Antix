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
LOGLIBNAME = include/logger/libredisclient.a
LOGLIBS = $(LOGLIBNAME) -lstdc++ -lpthread -lboost_thread-mt
LOGCLIENTOBJS = anet.o

CC = g++
CXXFLAGS = -g -Wall -O3 $(GLUTFLAGS)
LIBS =  -g -lm $(GLUTLIBS) 
INCLUDE = -I"include/" -I"include/logger/"

SRC = src/Math/Position.cpp src/Math/Speed.cpp src/Math/Math.cpp src/Game/GameObject.cpp include/Game/VisibleObject.h src/Game/Robot.cpp src/Game/Home.cpp src/Game/Puck.cpp src/Game/Team.cpp src/Game/Game.cpp src/GUI/Color.cpp src/GUI/Gui.cpp src/RobotixSA.cpp 

LOGHDR = include/logger/redisclient.h include/logger/logger.h

all: antix redis

antix: $(SRC)
	$(CC) $(CXXFLAGS) $(LIBS) $(INCLUDE) -o $@ $(SRC)

redis: $(SRC) $(LOGHDR)
	$(CC) $(CXXFLAGS) $(LIBS) $(INCLUDE) -DREDIS -o $@ $(SRC) $(LOGLIBS)

clean:
	rm antix redis

anet.o:             logger/anet.c logger/fmacros.h logger/anet.h
logger.o:		    logger/redisclient.h logger/logger.h

