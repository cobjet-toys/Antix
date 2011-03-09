clock_COMPILER = g++

clock_SRC = clock.cpp Client.cpp  ClockServer.cpp  Packer.cpp  Server.cpp  TcpConnection.cpp
clock_EXECUTABLE = clock.bin

# ----- do not edit under this line unless you know what you are doing :)

PWD = $(shell pwd)
clock_INCLUDE_PATH = $(PWD)/includes
clock_SRC_PATH = $(PWD)/src

clock_RELEASE_PATH = $(PWD)/build/release
clock_RELEASE_OBJ_PATH = $(clock_RELEASE_PATH)/obj

clock_DEBUG_PATH = $(PWD)/build/debug
clock_DEBUG_OBJ_PATH = $(clock_DEBUG_PATH)/obj

clock_RELEASE_OBJ = $(addprefix $(clock_RELEASE_OBJ_PATH)/, $(clock_SRC:.cpp=.o))
clock_DEBUG_OBJ = $(addprefix $(clock_DEBUG_OBJ_PATH)/, $(clock_SRC:.cpp=.o))




clock: clock_depend clock_release_objects
	@echo "Building RELEASE ----------------------"
	$(shell cd $(clock_RELEASE_PATH)/; $(clock_COMPILER) -o $(clock_EXECUTABLE) $(clock_RELEASE_OBJ))

clock_debug: clock_depend clock_debug_objects
	@echo "Building DEBUG ----------------------"
	$(shell cd $(clock_DEBUG_PATH)/; $(clock_COMPILER) -g -o $(clock_EXECUTABLE) $(clock_DEBUG_OBJ))

clock_depend:
	@echo "Compiling dependancy list  ----------------------"
	$(clock_COMPILER) -I$(clock_INCLUDE_PATH)/ -M $(addprefix $(clock_SRC_PATH)/, $(clock_SRC)) > clock_depends

clock_release_objects:
	@echo "Building relase objects ----------------------"
	$(shell cd $(clock_RELEASE_OBJ_PATH); $(clock_COMPILER) -c -I$(clock_INCLUDE_PATH) $(addprefix $(clock_SRC_PATH)/, $(clock_SRC)))

clock_debug_objects:
	@echo "Building debug objects ----------------------"
	$(shell cd $(clock_DEBUG_OBJ_PATH); $(clock_COMPILER) -c -I$(clock_INCLUDE_PATH) $(addprefix $(clock_SRC_PATH)/, $(clock_SRC)))

clean:
	@echo "Cleaning ALL ----------------------"
	rm clock_depends $(clock_RELEASE_PATH)/$(clock_EXECUTABLE) $(clock_DEBUG_PATH)/$(clock_EXECUTABLE) $(clock_RELEASE_OBJ_PATH)/*.o $(clock_DEBUG_OBJ_PATH)/*.o

clean_release:
	@echo "Cleaning release ----------------------"
	rm clock_depends $(clock_RELEASE_PATH)/$(clock_EXECUTABLE)  $(clock_RELEASE_OBJ_PATH)/*.o 

clean_debug:
	@echo "Cleaning debug ----------------------"
	rm clock_depends $(clock_DEBUG_PATH)/$(clock_EXECUTABLE)  $(clock_DEBUG_OBJ_PATH)/*.o

-include clock_depends



