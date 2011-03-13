common_COMPILER = g++

common_SRC = Client.cpp	Home.cpp Packer.cpp Puck.cpp Server.cpp TcpConnection.cpp GameObject.cpp Math.cpp Position.cpp Robot.cpp Speed.cpp Team.cpp 
common_LIBRARY = libcommon.a

# ----- do not edit under this line unless you know what you are doing :)

common_PWD = $(shell pwd)/$(ROOT_DIR)

common_INCLUDE_PATH = $(common_PWD)/includes
common_SRC_PATH = $(common_PWD)/src

common_RELEASE_PATH = $(common_PWD)/build/release
common_RELEASE_OBJ_PATH = $(common_RELEASE_PATH)/obj

common_DEBUG_PATH = $(common_PWD)/build/debug
common_DEBUG_OBJ_PATH = $(common_DEBUG_PATH)/obj

common_RELEASE_OBJ = $(addprefix $(common_RELEASE_OBJ_PATH)/, $(common_SRC:.cpp=.o))
common_DEBUG_OBJ = $(addprefix $(common_DEBUG_OBJ_PATH)/, $(common_SRC:.cpp=.o))




common: common_depend common_release_objects
	@echo "Building RELEASE --------------------------------------------------------------------------------"
	ar cvq $(common_RELEASE_PATH)/$(common_LIBRARY) $(common_RELEASE_OBJ)
	@echo "Building common COMPLETE --------------------------------------------------------------"

common_debug: common_depend common_debug_objects
	@echo "Building DEBUG ----------------------------------------------------------------------------------"
	ar cvq $(common_DEBUG_PATH)/$(common_LIBRARY) $(common_DEBUG_OBJ)
	@echo "Building DEBUG common COMPLETE --------------------------------------------------------"

common_depend:
	@echo "Compiling common dependancy list  -----------------------------------------------------"
	$(common_COMPILER) -I$(common_INCLUDE_PATH)/ -M $(addprefix $(common_SRC_PATH)/, $(common_SRC)) > common_depends
	@echo "Compiling common dependancy list COMPLETE ---------------------------------------------"


common_release_objects:
	@echo "Building common release objects -------------------------------------------------------"
	$(shell cd $(common_RELEASE_OBJ_PATH); $(common_COMPILER) -c -I$(common_INCLUDE_PATH) $(addprefix $(common_SRC_PATH)/, $(common_SRC)))
	@echo "Building {{PROJECT_NAEM}} release objects COMPLETE ----------------------------------------------"

common_debug_objects:
	@echo "Building common debug objects ---------------------------------------------------------"
	$(shell cd $(common_DEBUG_OBJ_PATH); $(common_COMPILER) -c -I$(common_INCLUDE_PATH) $(addprefix $(common_SRC_PATH)/, $(common_SRC)))
	@echo "Building common debug objects COMPLETE ------------------------------------------------"
common_clean:
	@echo "Cleaning common ALL -------------------------------------------------------------------"
	rm common_depends $(common_RELEASE_PATH)/$(common_LIBRARY) $(common_DEBUG_PATH)/$(common_LIBRARY) $(common_RELEASE_OBJ_PATH)/*.o $(common_DEBUG_OBJ_PATH)/*.o
	@echo "Cleaning common ALL COMPLETE ----------------------------------------------------------"

common_clean_release:
	@echo "Cleaning common release ---------------------------------------------------------------"
	rm common_depends $(common_RELEASE_PATH)/$(common_EXECUTABLE)  $(common_RELEASE_OBJ_PATH)/*.o 
	@echo "Cleaning common release COMPLETE ------------------------------------------------------"

common_clean_debug:
	@echo "Cleaning common debug ------------------------------------------------------------------"
	rm common_depends $(common_DEBUG_PATH)/$(common_EXECUTABLE)  $(common_DEBUG_OBJ_PATH)/*.o
	@echo "Cleaning common debug COMPLETE ---------------------------------------------------------"

-include common_depends



