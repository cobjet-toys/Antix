COMPILER = g++

SRC = TcpConnection.cpp
OBJECTS = $(SRC:.cpp=.o)
INCLUDES = 
LIBRARIES = 
EXECUTABLE = main

# ------------------------------------------------------------

TEEBS_SRC = $(SRC) teebs.cpp Server.cpp GridServer.cpp
TEEBS_OBJ = $(TEEBS_SRC:.cpp=.o)


# ------------------------------------------------------------

HARDEEP_SRC = $(SRC) hardeep.cpp Client.cpp
HARDEEP_OBJ = $(HARDEEP_SRC:.cpp=.o)

# ------------------------------------------------------------

ALL: depends objects
	$(COMPILER) -o $(EXECUTABLE) $(OBJECTS) $(LIBRARIES) $(INCLUDES)
	
objects:
	g++ -c $(SRC)

depends:
	$(COMPILER) -M $(SRC) > depends

clean:
	rm $(OBJECTS) depends

# ------------------------------------------------------------

teebs: teebs_depend teebs_objects
	$(COMPILER) -o $@ $(TEEBS_OBJ) $(LIBRARIES) $(INCLUDES)

teebs_depend:
	$(COMPILER) -M $(TEEBS_SRC) > depends

teebs_objects:
	g++ -c $(TEEBS_SRC)
	
teebs_clean:
	rm $(TEEBS_OBJ) depends

# ------------------------------------------------------------

hardeep: hardeep_depend hardeep_objects
	$(COMPILER) -o $@ $(HARDEEP_OBJ) $(LIBRARIES) $(INCLUDES)

hardeep_depend:
	$(COMPILER) -M $(HARDEEP_SRC) > depends

hardeep_objects:
	g++ -c $(HARDEEP_SRC)
	
hardeep_clean:
	rm $(HARDEEP_OBJ) depends	


# ------------------------------------------------------------



include depends

