PREFIX=/home/jansepar/local
CC = g++
CXXFLAGS = -g -Wall -O3 -L${PREFIX}/lib -I${PREFIX}/include -pthread
LIBS =  -g -lm -lz -lprotobuf -lpthread

SRC = client.pb.cc CreateRobotMessage.cpp


proto: $(SRC)
	$(CC) $(CXXFLAGS) $(LIBS) -o $@ $(SRC) 
