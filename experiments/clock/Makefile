COMPILER = gcc

SRC = connectionHelpers.cpp TcpConnection.cpp
OBJECTS = $(SRC:.cpp=.o)
INCLUDES = 
LIBRARIES = 
EXECUTABLE = main

ALL: depends objects
	$(COMPILER) -o $(EXECUTABLE) $(OBJECTS) $(LIBRARIES) $(INCLUDES)
	
objects:
	g++ -c $(SRC)

depends:
	$(COMPILER) -M $(SRC) > depends

clean:
	rm $(OBJECTS) depends
	
include depends