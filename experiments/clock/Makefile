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

# GLUT VARS
OS := $(shell uname -s)
ifeq ($(OS),Linux)
GLUTLIBS = -L/usr/X11R6/lib -lGLU -lGL -lglut -lX11 -lXext
GLUTFLAGS = -I/usr/local/include/GL
endif
ifeq ($(OS),Darwin)
GLUTFLAGS = -framework OpenGL -framework GLUT
endif

# REDIS VARS
#LOGLIBNAME = include/logger/libredisclient.a
#LOGLIBS = $(LOGLIBNAME) -lstdc++ -lpthread -lboost_thread-mt
#LOGCLIENTOBJS = anet.o

DRAWER_SRC = Server.cpp DrawServer.cpp TcpConnection.cpp Packer.cpp drawer.cpp
DRAWER_FLAGS = -g -Wall -O3 $(GLUTFLAGS)
#DRAWER_INCLUDES = -I"../../Robotix/include/" -I"../../Robotix/include/logger/" $(INCLUDES)
DRAWER_INCLUDES = $(INCLUDES)
DRAWER_LIBS = -g -lm $(GLUTLIBS) $(LIBRARIES)
DRAWER_OBJ = $(DRAWER_SRC:.cpp=.o)

# ------------------------------------------------------------

CYN_SRC = Client.cpp DrawClient.cpp TcpConnection.cpp Packer.cpp cyn.cpp
CYN_OBJ = $(CYN_SRC:.cpp=.o)

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


# ------------------------------------------------------------

drawer: drawer_depend drawer_objects
	$(COMPILER) -o drawer.bin $(DRAWER_OBJ) $(DRAWER_LIBS) $(DRAWER_INCLUDES)

drawer_depend:
	$(COMPILER) -M $(DRAWER_SRC) > depends

drawer_objects:
	g++ -c $(DRAWER_SRC)

drawer_clean:
	rm $(DRAWER_OBJ) depends


# ------------------------------------------------------------

cyn: cyn_depend cyn_objects
	$(COMPILER) -o dstubs.bin $(CYN_OBJ) $(LIBRARIES) $(INCLUDES)

cyn_depend:
	$(COMPILER) -M $(CYN_SRC) > depends

cyn_objects:
	g++ -c $(CYN_SRC)

cyn_clean:
	rm $(CYN_OBJ) depends


# ------------------------------------------------------------
	

include depends

