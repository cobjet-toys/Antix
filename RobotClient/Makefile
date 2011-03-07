#   Makefile - RobotClient
#   Horseplay

COMPILER = g++

SRC = RobotClient.cpp Client.cpp TcpConnection.cpp Packer.cpp
OBJECTS = $(SRC:.cpp=.o)
INCLUDES =
LIBRARIES =
EXECUTABLE = main

ROBOTCLIENT_SRC = $(SRC) main.cpp
ROBOTCLIENT_OBJ = $(ROBOTCLIENT_SRC:.cpp=.o)

robotclient: robotclient_depend robotclient_objects
	$(COMPILER) -o robotclient.bin $(ROBOTCLIENT_OBJ) $(LIBRARIES) $(INCLUDES)

robotclient_depend:
	$(COMPILER) -M $(ROBOTCLIENT_SRC) > depends

robotclient_objects:
	g++ -c $(ROBOTCLIENT_SRC)
    
clean:
	rm $(ROBOTCLIENT_OBJ) depends   

#include depends
