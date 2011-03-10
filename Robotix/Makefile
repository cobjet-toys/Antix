robotix_COMPILER = g++

robotix_SRC = 
robotix_EXECUTABLE = robotix.bin

# ----- do not edit under this line unless you know what you are doing :)

PWD = $(shell pwd)
robotix_INCLUDE_PATH = $(PWD)/includes
robotix_SRC_PATH = $(PWD)/src

robotix_RELEASE_PATH = $(PWD)/build/release
robotix_RELEASE_OBJ_PATH = $(robotix_RELEASE_PATH)/obj

robotix_DEBUG_PATH = $(PWD)/build/debug
robotix_DEBUG_OBJ_PATH = $(robotix_DEBUG_PATH)/obj

robotix_RELEASE_OBJ = $(addprefix $(robotix_RELEASE_OBJ_PATH)/, $(robotix_SRC:.cpp=.o))
robotix_DEBUG_OBJ = $(addprefix $(robotix_DEBUG_OBJ_PATH)/, $(robotix_SRC:.cpp=.o))




robotix: robotix_depend robotix_release_objects
	@echo "Building RELEASE --------------------------------------------------------------------------------"
	$(shell cd $(robotix_RELEASE_PATH)/; $(robotix_COMPILER) -o $(robotix_EXECUTABLE) $(robotix_RELEASE_OBJ))
	@echo "Building robotix COMPLETE --------------------------------------------------------------"

robotix_debug: robotix_depend robotix_debug_objects
	@echo "Building DEBUG ----------------------------------------------------------------------------------"
	$(shell cd $(robotix_DEBUG_PATH)/; $(robotix_COMPILER) -g -o $(robotix_EXECUTABLE) $(robotix_DEBUG_OBJ))
	@echo "Building DEBUG robotix COMPLETE --------------------------------------------------------"

robotix_depend:
	@echo "Compiling robotix dependancy list  -----------------------------------------------------"
	$(robotix_COMPILER) -I$(robotix_INCLUDE_PATH)/ -M $(addprefix $(robotix_SRC_PATH)/, $(robotix_SRC)) > robotix_depends
	@echo "Compiling robotix dependancy list COMPLETE ---------------------------------------------"


robotix_release_objects:
	@echo "Building robotix release objects -------------------------------------------------------"
	$(shell cd $(robotix_RELEASE_OBJ_PATH); $(robotix_COMPILER) -c -I$(robotix_INCLUDE_PATH) $(addprefix $(robotix_SRC_PATH)/, $(robotix_SRC)))
	@echo "Building {{PROJECT_NAEM}} release objects COMPLETE ----------------------------------------------"

robotix_debug_objects:
	@echo "Building robotix debug objects ---------------------------------------------------------"
	$(shell cd $(robotix_DEBUG_OBJ_PATH); $(robotix_COMPILER) -c -I$(robotix_INCLUDE_PATH) $(addprefix $(robotix_SRC_PATH)/, $(robotix_SRC)))
	@echo "Building robotix debug objects COMPLETE ------------------------------------------------"
robotix_clean:
	@echo "Cleaning robotix ALL -------------------------------------------------------------------"
	rm robotix_depends $(robotix_RELEASE_PATH)/$(robotix_EXECUTABLE) $(robotix_DEBUG_PATH)/$(robotix_EXECUTABLE) $(robotix_RELEASE_OBJ_PATH)/*.o $(robotix_DEBUG_OBJ_PATH)/*.o
	@echo "Cleaning robotix ALL -------------------------------------------------------------------"

robotix_clean_release:
	@echo "Cleaning robotix release ---------------------------------------------------------------"
	rm robotix_depends $(robotix_RELEASE_PATH)/$(robotix_EXECUTABLE)  $(robotix_RELEASE_OBJ_PATH)/*.o 
	@echo "Cleaning robotix release COMPLETE ------------------------------------------------------"

robotix_clean_debug:
	@echo "Cleaning robotix debug ------------------------------------------------------------------"
	rm robotix_depends $(robotix_DEBUG_PATH)/$(robotix_EXECUTABLE)  $(robotix_DEBUG_OBJ_PATH)/*.o
	@echo "Cleaning robotix debug COMPLETE ---------------------------------------------------------"

-include robotix_depends



