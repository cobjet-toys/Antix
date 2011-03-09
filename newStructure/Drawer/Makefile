drawer_COMPILER = g++

drawer_SRC = 
drawer_EXECUTABLE = drawer.bin

# ----- do not edit under this line unless you know what you are doing :)

PWD = $(shell pwd)
drawer_INCLUDE_PATH = $(PWD)/includes
drawer_SRC_PATH = $(PWD)/src

drawer_RELEASE_PATH = $(PWD)/build/release
drawer_RELEASE_OBJ_PATH = $(drawer_RELEASE_PATH)/obj

drawer_DEBUG_PATH = $(PWD)/build/debug
drawer_DEBUG_OBJ_PATH = $(drawer_DEBUG_PATH)/obj

drawer_RELEASE_OBJ = $(addprefix $(drawer_RELEASE_OBJ_PATH)/, $(drawer_SRC:.cpp=.o))
drawer_DEBUG_OBJ = $(addprefix $(drawer_DEBUG_OBJ_PATH)/, $(drawer_SRC:.cpp=.o))




drawer: drawer_depend drawer_release_objects
	@echo "Building RELEASE ----------------------"
	$(shell cd $(drawer_RELEASE_PATH)/; $(drawer_COMPILER) -o $(drawer_EXECUTABLE) $(drawer_RELEASE_OBJ))

drawer_debug: drawer_depend drawer_debug_objects
	@echo "Building DEBUG ----------------------"
	$(shell cd $(drawer_DEBUG_PATH)/; $(drawer_COMPILER) -g -o $(drawer_EXECUTABLE) $(drawer_DEBUG_OBJ))

drawer_depend:
	@echo "Compiling dependancy list  ----------------------"
	$(drawer_COMPILER) -I$(drawer_INCLUDE_PATH)/ -M $(addprefix $(drawer_SRC_PATH)/, $(drawer_SRC)) > drawer_depends

drawer_release_objects:
	@echo "Building relase objects ----------------------"
	$(shell cd $(drawer_RELEASE_OBJ_PATH); $(drawer_COMPILER) -c -I$(drawer_INCLUDE_PATH) $(addprefix $(drawer_SRC_PATH)/, $(drawer_SRC)))

drawer_debug_objects:
	@echo "Building debug objects ----------------------"
	$(shell cd $(drawer_DEBUG_OBJ_PATH); $(drawer_COMPILER) -c -I$(drawer_INCLUDE_PATH) $(addprefix $(drawer_SRC_PATH)/, $(drawer_SRC)))

clean:
	@echo "Cleaning ALL ----------------------"
	rm drawer_depends $(drawer_RELEASE_PATH)/$(drawer_EXECUTABLE) $(drawer_DEBUG_PATH)/$(drawer_EXECUTABLE) $(drawer_RELEASE_OBJ_PATH)/*.o $(drawer_DEBUG_OBJ_PATH)/*.o

clean_release:
	@echo "Cleaning release ----------------------"
	rm drawer_depends $(drawer_RELEASE_PATH)/$(drawer_EXECUTABLE)  $(drawer_RELEASE_OBJ_PATH)/*.o 

clean_debug:
	@echo "Cleaning debug ----------------------"
	rm drawer_depends $(drawer_DEBUG_PATH)/$(drawer_EXECUTABLE)  $(drawer_DEBUG_OBJ_PATH)/*.o

-include drawer_depends



