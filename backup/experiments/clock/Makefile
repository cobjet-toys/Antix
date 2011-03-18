COMPILER = g++

SRC = Client.cpp ControllerClient.cpp ClockClient.cpp Server.cpp GridServer.cpp TcpConnection.cpp Packer.cpp
OBJECTS = $(SRC:.cpp=.o)
INCLUDES = 
LIBRARIES = 
EXECUTABLE = main

# ------------------------------------------------------------

TEEBS_SRC = $(SRC) teebs.cpp 
TEEBS_OBJ = $(TEEBS_SRC:.cpp=.o)


# ------------------------------------------------------------

HARDEEP_SRC = $(SRC) grid.cpp 
HARDEEP_OBJ = $(HARDEEP_SRC:.cpp=.o)

# ------------------------------------------------------------

CLOCK_SRC = $(SRC) clock.cpp
CLOCK_OBJ = $(CLOCK_SRC:.cpp=.o)

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
	$(COMPILER) -o teebs.bin $(TEEBS_OBJ) $(LIBRARIES) $(INCLUDES)

teebs_depend:
	$(COMPILER) -M $(TEEBS_SRC) > depends

teebs_objects:
	g++ -c $(TEEBS_SRC)
	
teebs_clean:
	rm $(TEEBS_OBJ) teebs.bin depends

# ------------------------------------------------------------

hardeep: hardeep_depend hardeep_objects
	$(COMPILER) -o grid.bin $(HARDEEP_OBJ) $(LIBRARIES) $(INCLUDES)

hardeep_depend:
	$(COMPILER) -M $(HARDEEP_SRC) > depends

hardeep_objects:
	g++ -c $(HARDEEP_SRC)
	
hardeep_clean:
	rm $(HARDEEP_OBJ) depends	


# ------------------------------------------------------------

clock: clock_depend clock_objects 
	$(COMPILER) -o clock.bin $(CLOCK_OBJ) $(LIBRARIES) $(INCLUDES)

clock_depend:
	$(COMPILER) -M $(CLOCK_SRC) > depends

clock_objects:
	g++ -c $(CLOCK_SRC)
	

include depends

