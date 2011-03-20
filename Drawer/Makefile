drawer_COMPILER = g++

drawer_SRC = drawer.cpp DrawServer.cpp Gui.cpp DrawerParser.cpp
drawer_EXECUTABLE = drawer.bin


# ----- do not edit under this line unless you know what you are doing :)

ROOT_DIR = ../Common

PWD = $(shell pwd)
drawer_INCLUDE_PATH = $(PWD)/includes
drawer_INCLUDE_COMMON_PATH = $(PWD)/../Common/includes
drawer_SRC_PATH = $(PWD)/src

drawer_RELEASE_PATH = $(PWD)/build/release
drawer_RELEASE_OBJ_PATH = $(drawer_RELEASE_PATH)/obj

drawer_DEBUG_PATH = $(PWD)/build/debug
drawer_DEBUG_OBJ_PATH = $(drawer_DEBUG_PATH)/obj

drawer_RELEASE_OBJ = $(addprefix $(drawer_RELEASE_OBJ_PATH)/, $(drawer_SRC:.cpp=.o))
drawer_DEBUG_OBJ = $(addprefix $(drawer_DEBUG_OBJ_PATH)/, $(drawer_SRC:.cpp=.o))

# ----------------------------- GLUT VARS -----------------------------
# this should work on Linux with MESA
OS := $(shell uname -s)

ifeq ($(OS),Linux)
GLUTLIBS = -L/usr/X11R6/lib -lGLU -lGL -lglut -lX11 -lXext
GLUTFLAGS = -I/usr/local/include/GL
endif

# this works on Mac OS X
ifeq ($(OS),Darwin)
GLUTFLAGS = -framework OpenGL -framework GLUT
endif

# ----------------------------------------------------------------------

drawer_debug: common drawer_depend drawer_debug_objects
	@echo "Building DEBUG ----------------------------------------------------------------------------------"
	$(shell cd $(drawer_DEBUG_PATH)/; $(drawer_COMPILER) -g -lpthread -o $(drawer_EXECUTABLE) $(drawer_DEBUG_OBJ) $(PWD)/$(ROOT_DIR)/build/debug/libcommon.a $(GLUTLIBS) $(GLUTFLAGS) )
	@echo "Building DEBUG drawer COMPLETE --------------------------------------------------------"

drawer: common drawer_depend drawer_release_objects
	@echo "Building RELEASE --------------------------------------------------------------------------------"
	$(shell cd $(drawer_RELEASE_PATH)/; $(drawer_COMPILER) -lpthread -o $(drawer_EXECUTABLE) $(drawer_RELEASE_OBJ) $(PWD)/$(ROOT_DIR)/build/release/libcommon.a $(GLUTLIBS) $(GLUTFLAGS) )
	@echo "Building drawer COMPLETE --------------------------------------------------------------"

drawer_depend:
	@echo "Compiling drawer dependancy list  -----------------------------------------------------"
	$(drawer_COMPILER) -I$(drawer_INCLUDE_PATH)/ -I$(drawer_INCLUDE_COMMON_PATH) -M $(addprefix $(drawer_SRC_PATH)/, $(drawer_SRC)) > drawer_depends
	@echo "Compiling drawer dependancy list COMPLETE ---------------------------------------------"


drawer_release_objects:
	@echo "Building drawer release objects -------------------------------------------------------"
	$(shell cd $(drawer_RELEASE_OBJ_PATH); $(drawer_COMPILER) -c -I$(drawer_INCLUDE_PATH) -I$(drawer_INCLUDE_COMMON_PATH) $(addprefix $(drawer_SRC_PATH)/, $(drawer_SRC)))
	@echo "Building {{PROJECT_NAME}} release objects COMPLETE ----------------------------------------------"

drawer_debug_objects:
	@echo "Building drawer debug objects ---------------------------------------------------------"
	$(shell cd $(drawer_DEBUG_OBJ_PATH); $(drawer_COMPILER) -c -I$(drawer_INCLUDE_PATH) -I$(drawer_INCLUDE_COMMON_PATH) $(addprefix $(drawer_SRC_PATH)/, $(drawer_SRC)))
	@echo "Building drawer debug objects COMPLETE ------------------------------------------------"

drawer_clean:
	@echo "Cleaning drawer ALL -------------------------------------------------------------------"
	rm drawer_depends $(drawer_RELEASE_PATH)/$(drawer_EXECUTABLE) $(drawer_DEBUG_PATH)/$(drawer_EXECUTABLE) $(drawer_RELEASE_OBJ_PATH)/*.o $(drawer_DEBUG_OBJ_PATH)/*.o
	@echo "Cleaning drawer ALL -------------------------------------------------------------------"

drawer_clean_release:
	@echo "Cleaning drawer release ---------------------------------------------------------------"
	rm drawer_depends $(drawer_RELEASE_PATH)/$(drawer_EXECUTABLE)  $(drawer_RELEASE_OBJ_PATH)/*.o 
	@echo "Cleaning drawer release COMPLETE ------------------------------------------------------"

drawer_clean_debug:
	@echo "Cleaning drawer debug ------------------------------------------------------------------"
	rm drawer_depends $(drawer_DEBUG_PATH)/$(drawer_EXECUTABLE)  $(drawer_DEBUG_OBJ_PATH)/*.o
	@echo "Cleaning drawer debug COMPLETE ---------------------------------------------------------"

-include drawer_depends
-include ../Common/Makefile


