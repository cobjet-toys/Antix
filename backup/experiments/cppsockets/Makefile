CXX = g++
CXXFLAGS = -Wall -ansi -pedantic -g

ifeq ($(shell uname),SunOS)
  LIBS = -lsocket -lnsl
endif

all: TCPEchoClient TCPEchoServer

TCPEchoClient: TCPEchoClient.cpp PracticalSocket.cpp PracticalSocket.h
	$(CXX) $(CXXFLAGS) -o TCPEchoClient TCPEchoClient.cpp PracticalSocket.cpp $(LIBS)

TCPEchoServer: TCPEchoServer.cpp PracticalSocket.cpp PracticalSocket.h
	$(CXX) $(CXXFLAGS) -o TCPEchoServer TCPEchoServer.cpp PracticalSocket.cpp $(LIBS)
