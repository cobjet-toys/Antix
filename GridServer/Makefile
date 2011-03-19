grid_COMPILER = g++

grid_SRC = grid.cpp GridServer.cpp GridParser.cpp
grid_EXECUTABLE = grid.bin

# ----- do not edit under this line unless you know what you are doing :)

ROOT_DIR = ../Common

grid_PWD = $(shell pwd)

grid_INCLUDE_PATH = $(grid_PWD)/includes
grid_COMMON_INCLUDE_PATH = $(grid_PWD)/../Common/includes


grid_SRC_PATH = $(grid_PWD)/src

grid_RELEASE_PATH = $(grid_PWD)/build/release
grid_RELEASE_OBJ_PATH = $(grid_RELEASE_PATH)/obj

grid_DEBUG_PATH = $(grid_PWD)/build/debug
grid_DEBUG_OBJ_PATH = $(grid_DEBUG_PATH)/obj

grid_RELEASE_OBJ = $(addprefix $(grid_RELEASE_OBJ_PATH)/, $(grid_SRC:.cpp=.o))
grid_DEBUG_OBJ = $(addprefix $(grid_DEBUG_OBJ_PATH)/, $(grid_SRC:.cpp=.o))



grid_debug: common_debug grid_depend grid_debug_objects
	@echo "Building DEBUG ----------------------------------------------------------------------"
	$(shell cd $(grid_DEBUG_PATH)/; $(grid_COMPILER) -g -o $(grid_EXECUTABLE) $(grid_DEBUG_OBJ) $(grid_PWD)/$(ROOT_DIR)/build/debug/libcommon.a)
	@echo "Building DEBUG grid COMPLETE --------------------------------------------------------"

grid: common grid_depend grid_release_objects
	@echo "Building RELEASE ---------------------------------------------------------------------"
	$(shell cd $(grid_RELEASE_PATH)/; $(grid_COMPILER) -o $(grid_EXECUTABLE) $(grid_RELEASE_OBJ) $(grid_PWD)/$(ROOT_DIR)/build/release/libcommon.a)
	@echo "Building grid COMPLETE --------------------------------------------------------------"

grid_depend:
	@echo "Compiling grid dependancy list  -----------------------------------------------------"
	$(grid_COMPILER) -I$(grid_INCLUDE_PATH)/ -I$(grid_COMMON_INCLUDE_PATH) -M $(addprefix $(grid_SRC_PATH)/, $(grid_SRC)) > grid_depends
	@echo "Compiling grid dependancy list COMPLETE ---------------------------------------------"


grid_release_objects:
	@echo "Building grid release objects -------------------------------------------------------"
	$(shell cd $(grid_RELEASE_OBJ_PATH); $(grid_COMPILER) -c -I$(grid_COMMON_INCLUDE_PATH) -I$(grid_INCLUDE_PATH) $(addprefix $(grid_SRC_PATH)/, $(grid_SRC)))
	@echo "Building grid release objects COMPLETE ----------------------------------------------"

grid_debug_objects:
	@echo "Building grid debug objects ---------------------------------------------------------"
	$(shell cd $(grid_DEBUG_OBJ_PATH); $(grid_COMPILER) -c -I$(grid_COMMON_INCLUDE_PATH) -I$(grid_INCLUDE_PATH) $(addprefix $(grid_SRC_PATH)/, $(grid_SRC)))
	@echo "Building grid debug objects COMPLETE ------------------------------------------------"
grid_clean: common_clean
	@echo "Cleaning grid ALL -------------------------------------------------------------------"
	rm grid_depends $(grid_RELEASE_PATH)/$(grid_EXECUTABLE) $(grid_DEBUG_PATH)/$(grid_EXECUTABLE) $(grid_RELEASE_OBJ_PATH)/*.o $(grid_DEBUG_OBJ_PATH)/*.o
	@echo "Cleaning grid ALL COMPLETE ----------------------------------------------------------"

grid_clean_release: common_clean_release
	@echo "Cleaning grid release ---------------------------------------------------------------"
	rm grid_depends $(grid_RELEASE_PATH)/$(grid_EXECUTABLE)  $(grid_RELEASE_OBJ_PATH)/*.o 
	@echo "Cleaning grid release COMPLETE ------------------------------------------------------"

grid_clean_debug: common_clean_debug
	@echo "Cleaning grid debug ------------------------------------------------------------------"
	rm grid_depends $(grid_DEBUG_PATH)/$(grid_EXECUTABLE)  $(grid_DEBUG_OBJ_PATH)/*.o
	@echo "Cleaning grid debug COMPLETE ---------------------------------------------------------"

-include grid_depends
-include ../Common/Makefile


