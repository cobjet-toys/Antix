# ----------------------------- GLUT VARS -----------------------------
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

# ----------------------------- REDIS VARS -----------------------------

# logger macros
LOGLIBNAME = logger/libredisclient.a
LOGLIBS = $(LOGLIBNAME) -lstdc++ -lpthread -lboost_thread-mt
LOGCLIENTOBJS = anet.o

LOGHDR = logger/redisclient.h logger/logger.h

# ----------------------------- MAKE CONFIG -----------------------------

CC = g++
INCLUDE = -I"logger/"
BASIC_HDR = $(LOGHDR) dutils.h

# ----------------------------- DRAWER FILES ----------------------------

DRAWER_CXXFLAGS = -g -Wall -O3 $(GLUTFLAGS)
DRAWER_LIBS =  -g -lm $(GLUTLIBS)
DRAWER_SRC = DrawServer.cpp drawer.cpp dutils.cpp Gui.cpp
DRAWER_HDR = $(BASIC_HDR) Gui.h

# ----------------------------- CLIENT FILES ----------------------------

CLIENT_SRC = DrawClient.cpp client.cpp dutils.cpp
CLIENT_HDR = $(BASIC_HDR)

# ------------------------------ MAKE ARGS ------------------------------

all: DrawServer client

clean: clean_drawer clean_client

DrawServer: $(DRAWER_HDR) $(DRAWER_SRC)
	$(CC) $(DRAWER_CXXFLAGS) $(DRAWER_LIBS) $(INCLUDE) -DREDIS -o $@ $(DRAWER_SRC) $(LOGLIBS)

client: $(CLIENT_HDR) $(CLIENT_SRC)
	$(CC) $(INCLUDE) -DREDIS -o $@ $(CLIENT_SRC) $(LOGLIBS)

clean_drawer:
	rm DrawServer

clean_client:
	rm client

anet.o:			logger/anet.c logger/fmacros.h logger/anet.h
logger.o:		logger/redisclient.h logger/logger.h