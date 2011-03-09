client_COMPILER = g++

client_SRC = 
client_EXECUTABLE = client.bin

# ----- do not edit under this line unless you know what you are doing :)

PWD = $(shell pwd)
client_INCLUDE_PATH = $(PWD)/includes
client_SRC_PATH = $(PWD)/src

client_RELEASE_PATH = $(PWD)/build/release
client_RELEASE_OBJ_PATH = $(client_RELEASE_PATH)/obj

client_DEBUG_PATH = $(PWD)/build/debug
client_DEBUG_OBJ_PATH = $(client_DEBUG_PATH)/obj

client_RELEASE_OBJ = $(addprefix $(client_RELEASE_OBJ_PATH)/, $(client_SRC:.cpp=.o))
client_DEBUG_OBJ = $(addprefix $(client_DEBUG_OBJ_PATH)/, $(client_SRC:.cpp=.o))




client: client_depend client_release_objects
	@echo "Building RELEASE ----------------------"
	$(shell cd $(client_RELEASE_PATH)/; $(client_COMPILER) -o $(client_EXECUTABLE) $(client_RELEASE_OBJ))

client_debug: client_depend client_debug_objects
	@echo "Building DEBUG ----------------------"
	$(shell cd $(client_DEBUG_PATH)/; $(client_COMPILER) -g -o $(client_EXECUTABLE) $(client_DEBUG_OBJ))

client_depend:
	@echo "Compiling dependancy list  ----------------------"
	$(client_COMPILER) -I$(client_INCLUDE_PATH)/ -M $(addprefix $(client_SRC_PATH)/, $(client_SRC)) > client_depends

client_release_objects:
	@echo "Building relase objects ----------------------"
	$(shell cd $(client_RELEASE_OBJ_PATH); $(client_COMPILER) -c -I$(client_INCLUDE_PATH) $(addprefix $(client_SRC_PATH)/, $(client_SRC)))

client_debug_objects:
	@echo "Building debug objects ----------------------"
	$(shell cd $(client_DEBUG_OBJ_PATH); $(client_COMPILER) -c -I$(client_INCLUDE_PATH) $(addprefix $(client_SRC_PATH)/, $(client_SRC)))

clean:
	@echo "Cleaning ALL ----------------------"
	rm client_depends $(client_RELEASE_PATH)/$(client_EXECUTABLE) $(client_DEBUG_PATH)/$(client_EXECUTABLE) $(client_RELEASE_OBJ_PATH)/*.o $(client_DEBUG_OBJ_PATH)/*.o

clean_release:
	@echo "Cleaning release ----------------------"
	rm client_depends $(client_RELEASE_PATH)/$(client_EXECUTABLE)  $(client_RELEASE_OBJ_PATH)/*.o 

clean_debug:
	@echo "Cleaning debug ----------------------"
	rm client_depends $(client_DEBUG_PATH)/$(client_EXECUTABLE)  $(client_DEBUG_OBJ_PATH)/*.o

-include client_depends



