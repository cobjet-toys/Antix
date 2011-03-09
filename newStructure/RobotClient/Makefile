robot_COMPILER = g++

robot_SRC = 
robot_EXECUTABLE = robot.bin

# ----- do not edit under this line unless you know what you are doing :)

PWD = $(shell pwd)
robot_INCLUDE_PATH = $(PWD)/includes
robot_SRC_PATH = $(PWD)/src

robot_RELEASE_PATH = $(PWD)/build/release
robot_RELEASE_OBJ_PATH = $(robot_RELEASE_PATH)/obj

robot_DEBUG_PATH = $(PWD)/build/debug
robot_DEBUG_OBJ_PATH = $(robot_DEBUG_PATH)/obj

robot_RELEASE_OBJ = $(addprefix $(robot_RELEASE_OBJ_PATH)/, $(robot_SRC:.cpp=.o))
robot_DEBUG_OBJ = $(addprefix $(robot_DEBUG_OBJ_PATH)/, $(robot_SRC:.cpp=.o))




robot: robot_depend robot_release_objects
	@echo "Building RELEASE ----------------------"
	$(shell cd $(robot_RELEASE_PATH)/; $(robot_COMPILER) -o $(robot_EXECUTABLE) $(robot_RELEASE_OBJ))

robot_debug: robot_depend robot_debug_objects
	@echo "Building DEBUG ----------------------"
	$(shell cd $(robot_DEBUG_PATH)/; $(robot_COMPILER) -g -o $(robot_EXECUTABLE) $(robot_DEBUG_OBJ))

robot_depend:
	@echo "Compiling dependancy list  ----------------------"
	$(robot_COMPILER) -I$(robot_INCLUDE_PATH)/ -M $(addprefix $(robot_SRC_PATH)/, $(robot_SRC)) > robot_depends

robot_release_objects:
	@echo "Building relase objects ----------------------"
	$(shell cd $(robot_RELEASE_OBJ_PATH); $(robot_COMPILER) -c -I$(robot_INCLUDE_PATH) $(addprefix $(robot_SRC_PATH)/, $(robot_SRC)))

robot_debug_objects:
	@echo "Building debug objects ----------------------"
	$(shell cd $(robot_DEBUG_OBJ_PATH); $(robot_COMPILER) -c -I$(robot_INCLUDE_PATH) $(addprefix $(robot_SRC_PATH)/, $(robot_SRC)))

clean:
	@echo "Cleaning ALL ----------------------"
	rm robot_depends $(robot_RELEASE_PATH)/$(robot_EXECUTABLE) $(robot_DEBUG_PATH)/$(robot_EXECUTABLE) $(robot_RELEASE_OBJ_PATH)/*.o $(robot_DEBUG_OBJ_PATH)/*.o

clean_release:
	@echo "Cleaning release ----------------------"
	rm robot_depends $(robot_RELEASE_PATH)/$(robot_EXECUTABLE)  $(robot_RELEASE_OBJ_PATH)/*.o 

clean_debug:
	@echo "Cleaning debug ----------------------"
	rm robot_depends $(robot_DEBUG_PATH)/$(robot_EXECUTABLE)  $(robot_DEBUG_OBJ_PATH)/*.o

-include robot_depends



