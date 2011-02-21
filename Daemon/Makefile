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

CC = g++
CXXFLAGS = -g -Wall -O3 $(GLUTFLAGS)
LIBS =  -g -lm $(GLUTLIBS)
INCLUDE = -I"./"

SRC = main.cpp Packer.cpp TCPInterface.cpp Messages.h Packer.h TCPInterface.h

all: controller daemon

controller: $(SRC)
	$(CC) $(CXXFLAGS) $(LIBS) $(INCLUDE) -o $@ $(SRC)

daemon: $(SRC)
	$(CC) $(CXXFLAGS) $(LIBS) $(INCLUDE) -o $@ $(SRC)

clean:
	rm controller daemon

