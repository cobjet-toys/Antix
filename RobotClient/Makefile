robot_COMPILER = g++

robot_SRC = robot.cpp RobotClient.cpp RobotParser.cpp
robot_EXECUTABLE = robot.bin

# ----- do not edit under this line unless you know what you are doing :)

ROOT_DIR = ../Common

robot_PWD = $(shell pwd)
robot_INCLUDE_PATH = $(PWD)/includes
robot_INCLUDE_COMMON_PATH = $(robot_PWD)/../Common/includes
robot_SRC_PATH = $(PWD)/src

robot_RELEASE_PATH = $(PWD)/build/release
robot_RELEASE_OBJ_PATH = $(robot_RELEASE_PATH)/obj

robot_DEBUG_PATH = $(PWD)/build/debug
robot_DEBUG_OBJ_PATH = $(robot_DEBUG_PATH)/obj

robot_RELEASE_OBJ = $(addprefix $(robot_RELEASE_OBJ_PATH)/, $(robot_SRC:.cpp=.o))
robot_DEBUG_OBJ = $(addprefix $(robot_DEBUG_OBJ_PATH)/, $(robot_SRC:.cpp=.o))




robot_debug: common_debug robot_depend robot_debug_objects
	@echo "Building DEBUG ----------------------------------------------------------------------------------"
	$(shell cd $(robot_DEBUG_PATH)/; $(robot_COMPILER) -g -o $(robot_EXECUTABLE) $(robot_DEBUG_OBJ) $(robot_PWD)/$(ROOT_DIR)/build/debug/libcommon.a )
	@echo "Building DEBUG robot COMPLETE -------------------------------------------------------------------"

robot: common robot_depend robot_release_objects
	@echo "Building RELEASE --------------------------------------------------------------------------------"
	$(shell cd $(robot_RELEASE_PATH)/; $(robot_COMPILER) -o $(robot_EXECUTABLE) $(robot_RELEASE_OBJ) $(robot_PWD)/$(ROOT_DIR)/build/release/libcommon.a )
	@echo "Building robot COMPLETE -------------------------------------------------------------------------"

robot_depend:
	@echo "Compiling robot dependancy list  ----------------------------------------------------------------"
	$(robot_COMPILER) -I$(robot_INCLUDE_PATH)/ -I$(robot_INCLUDE_COMMON_PATH) -M $(addprefix $(robot_SRC_PATH)/, $(robot_SRC)) > robot_depends
	@echo "Compiling robot dependancy list COMPLETE --------------------------------------------------------"


robot_release_objects:
	@echo "Building robot release objects ------------------------------------------------------------------"
	$(shell cd $(robot_RELEASE_OBJ_PATH); $(robot_COMPILER) -c -I$(robot_INCLUDE_PATH) -I$(robot_INCLUDE_COMMON_PATH) $(addprefix $(robot_SRC_PATH)/, $(robot_SRC)))
	@echo "Building robot release objects COMPLETE --------------------------------------------------------"

robot_debug_objects:
	@echo "Building robot debug objects -------------------------------------------------------------------"
	$(shell cd $(robot_DEBUG_OBJ_PATH); $(robot_COMPILER) -c -I$(robot_INCLUDE_PATH) -I$(robot_INCLUDE_COMMON_PATH) $(addprefix $(robot_SRC_PATH)/, $(robot_SRC)))
	@echo "Building robot debug objects COMPLETE ----------------------------------------------------------"
robot_clean: common_clean
	@echo "Cleaning robot ALL -----------------------------------------------------------------------------"
	rm robot_depends $(robot_RELEASE_PATH)/$(robot_EXECUTABLE) $(robot_DEBUG_PATH)/$(robot_EXECUTABLE) $(robot_RELEASE_OBJ_PATH)/*.o $(robot_DEBUG_OBJ_PATH)/*.o
	@echo "Cleaning robot ALL -----------------------------------------------------------------------------"

robot_clean_release: common_clean_release
	@echo "Cleaning robot release -------------------------------------------------------------------------"
	rm robot_depends $(robot_RELEASE_PATH)/$(robot_EXECUTABLE)  $(robot_RELEASE_OBJ_PATH)/*.o 
	@echo "Cleaning robot release COMPLETE ----------------------------------------------------------------"

robot_clean_debug: common_clean_debug
	@echo "Cleaning robot debug ---------------------------------------------------------------------------"
	rm robot_depends $(robot_DEBUG_PATH)/$(robot_EXECUTABLE)  $(robot_DEBUG_OBJ_PATH)/*.o
	@echo "Cleaning robot debug COMPLETE ------------------------------------------------------------------"

-include robot_depends
-include ../Common/Makefile


