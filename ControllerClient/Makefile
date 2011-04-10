controller_COMPILER = g++

controller_SRC = controller.cpp ControllerClient.cpp ControllerParser.cpp 
controller_EXECUTABLE = controller.bin

# ----- do not edit under this line unless you know what you are doing :)

ROOT_DIR = ../Common

controller_PWD = $(shell pwd)
controller_INCLUDE_PATH = $(PWD)/includes
controller_INCLUDE_COMMON_PATH = $(controller_PWD)/../Common/includes
controller_SRC_PATH = $(PWD)/src

controller_RELEASE_PATH = $(PWD)/build/release
controller_RELEASE_OBJ_PATH = $(controller_RELEASE_PATH)/obj

controller_DEBUG_PATH = $(PWD)/build/debug
controller_DEBUG_OBJ_PATH = $(controller_DEBUG_PATH)/obj

controller_RELEASE_OBJ = $(addprefix $(controller_RELEASE_OBJ_PATH)/, $(controller_SRC:.cpp=.o))
controller_DEBUG_OBJ = $(addprefix $(controller_DEBUG_OBJ_PATH)/, $(controller_SRC:.cpp=.o))




controller_debug: common_debug controller_depend controller_debug_objects
	@echo "Building DEBUG ----------------------------------------------------------------------------------"
	$(shell cd $(controller_DEBUG_PATH)/; $(controller_COMPILER) -g -o $(controller_EXECUTABLE) $(controller_DEBUG_OBJ) $(controller_PWD)/$(ROOT_DIR)/build/debug/libcommon.a )
	@echo "Building DEBUG controller COMPLETE -------------------------------------------------------------------"

controller: common controller_depend controller_release_objects
	@echo "Building RELEASE --------------------------------------------------------------------------------"
	$(shell cd $(controller_RELEASE_PATH)/; $(controller_COMPILER) -O2 -o $(controller_EXECUTABLE) $(controller_RELEASE_OBJ) $(controller_PWD)/$(ROOT_DIR)/build/release/libcommon.a )
	@echo "Building controller COMPLETE -------------------------------------------------------------------------"

controller_depend:
	@echo "Compiling controller dependancy list  ----------------------------------------------------------------"
	$(controller_COMPILER) -I$(controller_INCLUDE_PATH)/ -I$(controller_INCLUDE_COMMON_PATH) -M $(addprefix $(controller_SRC_PATH)/, $(controller_SRC)) > controller_depends
	@echo "Compiling controller dependancy list COMPLETE --------------------------------------------------------"


controller_release_objects:
	@echo "Building controller release objects ------------------------------------------------------------------"
	$(shell cd $(controller_RELEASE_OBJ_PATH); $(controller_COMPILER) -O2 -c -I$(controller_INCLUDE_PATH) -I$(controller_INCLUDE_COMMON_PATH) $(addprefix $(controller_SRC_PATH)/, $(controller_SRC)))
	@echo "Building controller release objects COMPLETE --------------------------------------------------------"

controller_debug_objects:
	@echo "Building controller debug objects -------------------------------------------------------------------"
	$(shell cd $(controller_DEBUG_OBJ_PATH); $(controller_COMPILER) -g -c -I$(controller_INCLUDE_PATH) -I$(controller_INCLUDE_COMMON_PATH) $(addprefix $(controller_SRC_PATH)/, $(controller_SRC)))
	@echo "Building controller debug objects COMPLETE ----------------------------------------------------------"
controller_clean: common_clean
	@echo "Cleaning controller ALL -----------------------------------------------------------------------------"
	rm controller_depends $(controller_RELEASE_PATH)/$(controller_EXECUTABLE) $(controller_DEBUG_PATH)/$(controller_EXECUTABLE) $(controller_RELEASE_OBJ_PATH)/*.o $(controller_DEBUG_OBJ_PATH)/*.o
	@echo "Cleaning controller ALL -----------------------------------------------------------------------------"

controller_clean_release: common_clean_release
	@echo "Cleaning controller release -------------------------------------------------------------------------"
	rm controller_depends $(controller_RELEASE_PATH)/$(controller_EXECUTABLE)  $(controller_RELEASE_OBJ_PATH)/*.o 
	@echo "Cleaning controller release COMPLETE ----------------------------------------------------------------"

controller_clean_debug: common_clean_debug
	@echo "Cleaning controller debug ---------------------------------------------------------------------------"
	rm controller_depends $(controller_DEBUG_PATH)/$(controller_EXECUTABLE)  $(controller_DEBUG_OBJ_PATH)/*.o
	@echo "Cleaning controller debug COMPLETE ------------------------------------------------------------------"

-include controller_depends
-include ../Common/Makefile


