#   Makefile - antix project
#   version 3
#   Richard Vaughan  

# this should work on Linux with MESA
OS := $(shell uname -s)

CC = g++
CXXFLAGS = -g -Wall -O3 
LIBS =  -g -lm 
INCLUDE = -I"./"
CLIENT = -DCLIENT
SERVER = -DSERVER

SRC = main.cpp Packer.cpp TCPInterface.cpp Messages.h Packer.h TCPInterface.h

all: client server

client: $(SRC)
	$(CC) $(CXXFLAGS) $(LIBS) $(INCLUDE) -o $@ $(SRC) $(CLIENT)

server: $(SRC)
	$(CC) $(CXXFLAGS) $(LIBS) $(INCLUDE) -o $@ $(SRC) $(SERVER)

clean:
	rm client server

