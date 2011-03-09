common_COMPILER = g++

common_SRC = 
common_EXECUTABLE = common.bin

# ----- do not edit under this line unless you know what you are doing :)

PWD = $(shell pwd)
common_INCLUDE_PATH = $(PWD)/includes
common_SRC_PATH = $(PWD)/src

common_RELEASE_PATH = $(PWD)/build/release
common_RELEASE_OBJ_PATH = $(common_RELEASE_PATH)/obj

common_DEBUG_PATH = $(PWD)/build/debug
common_DEBUG_OBJ_PATH = $(common_DEBUG_PATH)/obj

common_RELEASE_OBJ = $(addprefix $(common_RELEASE_OBJ_PATH)/, $(common_SRC:.cpp=.o))
common_DEBUG_OBJ = $(addprefix $(common_DEBUG_OBJ_PATH)/, $(common_SRC:.cpp=.o))




common: common_depend common_release_objects
	@echo "Building RELEASE ----------------------"
	$(shell cd $(common_RELEASE_PATH)/; $(common_COMPILER) -o $(common_EXECUTABLE) $(common_RELEASE_OBJ))

common_debug: common_depend common_debug_objects
	@echo "Building DEBUG ----------------------"
	$(shell cd $(common_DEBUG_PATH)/; $(common_COMPILER) -g -o $(common_EXECUTABLE) $(common_DEBUG_OBJ))

common_depend:
	@echo "Compiling dependancy list  ----------------------"
	$(common_COMPILER) -I$(common_INCLUDE_PATH)/ -M $(addprefix $(common_SRC_PATH)/, $(common_SRC)) > common_depends

common_release_objects:
	@echo "Building relase objects ----------------------"
	$(shell cd $(common_RELEASE_OBJ_PATH); $(common_COMPILER) -c -I$(common_INCLUDE_PATH) $(addprefix $(common_SRC_PATH)/, $(common_SRC)))

common_debug_objects:
	@echo "Building debug objects ----------------------"
	$(shell cd $(common_DEBUG_OBJ_PATH); $(common_COMPILER) -c -I$(common_INCLUDE_PATH) $(addprefix $(common_SRC_PATH)/, $(common_SRC)))

clean:
	@echo "Cleaning ALL ----------------------"
	rm common_depends $(common_RELEASE_PATH)/$(common_EXECUTABLE) $(common_DEBUG_PATH)/$(common_EXECUTABLE) $(common_RELEASE_OBJ_PATH)/*.o $(common_DEBUG_OBJ_PATH)/*.o

clean_release:
	@echo "Cleaning release ----------------------"
	rm common_depends $(common_RELEASE_PATH)/$(common_EXECUTABLE)  $(common_RELEASE_OBJ_PATH)/*.o 

clean_debug:
	@echo "Cleaning debug ----------------------"
	rm common_depends $(common_DEBUG_PATH)/$(common_EXECUTABLE)  $(common_DEBUG_OBJ_PATH)/*.o

-include common_depends



