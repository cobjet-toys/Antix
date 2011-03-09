grid_COMPILER = g++

grid_SRC = 
grid_EXECUTABLE = grid.bin

# ----- do not edit under this line unless you know what you are doing :)

PWD = $(shell pwd)
grid_INCLUDE_PATH = $(PWD)/includes
grid_SRC_PATH = $(PWD)/src

grid_RELEASE_PATH = $(PWD)/build/release
grid_RELEASE_OBJ_PATH = $(grid_RELEASE_PATH)/obj

grid_DEBUG_PATH = $(PWD)/build/debug
grid_DEBUG_OBJ_PATH = $(grid_DEBUG_PATH)/obj

grid_RELEASE_OBJ = $(addprefix $(grid_RELEASE_OBJ_PATH)/, $(grid_SRC:.cpp=.o))
grid_DEBUG_OBJ = $(addprefix $(grid_DEBUG_OBJ_PATH)/, $(grid_SRC:.cpp=.o))




grid: grid_depend grid_release_objects
	@echo "Building RELEASE ----------------------"
	$(shell cd $(grid_RELEASE_PATH)/; $(grid_COMPILER) -o $(grid_EXECUTABLE) $(grid_RELEASE_OBJ))

grid_debug: grid_depend grid_debug_objects
	@echo "Building DEBUG ----------------------"
	$(shell cd $(grid_DEBUG_PATH)/; $(grid_COMPILER) -g -o $(grid_EXECUTABLE) $(grid_DEBUG_OBJ))

grid_depend:
	@echo "Compiling dependancy list  ----------------------"
	$(grid_COMPILER) -I$(grid_INCLUDE_PATH)/ -M $(addprefix $(grid_SRC_PATH)/, $(grid_SRC)) > grid_depends

grid_release_objects:
	@echo "Building relase objects ----------------------"
	$(shell cd $(grid_RELEASE_OBJ_PATH); $(grid_COMPILER) -c -I$(grid_INCLUDE_PATH) $(addprefix $(grid_SRC_PATH)/, $(grid_SRC)))

grid_debug_objects:
	@echo "Building debug objects ----------------------"
	$(shell cd $(grid_DEBUG_OBJ_PATH); $(grid_COMPILER) -c -I$(grid_INCLUDE_PATH) $(addprefix $(grid_SRC_PATH)/, $(grid_SRC)))

clean:
	@echo "Cleaning ALL ----------------------"
	rm grid_depends $(grid_RELEASE_PATH)/$(grid_EXECUTABLE) $(grid_DEBUG_PATH)/$(grid_EXECUTABLE) $(grid_RELEASE_OBJ_PATH)/*.o $(grid_DEBUG_OBJ_PATH)/*.o

clean_release:
	@echo "Cleaning release ----------------------"
	rm grid_depends $(grid_RELEASE_PATH)/$(grid_EXECUTABLE)  $(grid_RELEASE_OBJ_PATH)/*.o 

clean_debug:
	@echo "Cleaning debug ----------------------"
	rm grid_depends $(grid_DEBUG_PATH)/$(grid_EXECUTABLE)  $(grid_DEBUG_OBJ_PATH)/*.o

-include grid_depends



