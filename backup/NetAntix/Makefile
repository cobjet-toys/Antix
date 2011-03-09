COMPILER = g++

SRC =  src/main.cpp src/GridServer.cpp src/Server.cpp src/TcpConnection.cpp
OBJECTS = $(SRC:.cpp=.o)
OBJECTS = $(SRC:src/=)
INCLUDES = "-I./includes/" 
LIBRARIES = 
EXECUTABLE = "./bin/main.bin"

ALL: 
	$(COMPILER) -o $(EXECUTABLE) $(OBJECTS) $(LIBRARIES) $(INCLUDES)
	
objects:
	g++ -c $(SRC)

depends:
	$(COMPILER) -M $(SRC) > depends

clean:
	echo rm $(OBJECTS) $(EXECUTABLE) depends
	rm $(OBJECTS) $(EXECUTABLE) depends


include depends

