#                             ---MAKEFILE---
# ____________________
# Available options:
# - CXX            = <compiler>   C/C++ compiler used to compile the code
# - SRCDIR         = <dir>		  Directory to build (default: src)
# - ADD_FLAGS      = <flags>	  Additional flags to pass to the compiler
# - BUILD_TYPE     = <type>	      Build type (debug, release) Default: debug
# - PACKAGE_ASSETS = <boolean>    Wether or not assets should be copied to the program directory
#                                 This is used for the GitLab CI (default: false)
# 
# ____________________
# Available commands:
# - all			  Default command, builds all targets
# - clean 		  Removes all build files
# - run 		  Builds and runs the executable
# - gdb           Builds and runs the executable with gdb
#

# Compiler options
CXX      ?= g++
INCLUDES  = -Iinclude -Ithird_party/include
LINKLIBES = -Lthird_party/libs-$(TARGET)
LOADLIBES = -lraylib
CXXFLAGS  = -std=c++20 -Wall -Wextra -Wno-unused-parameter -Wno-unknown-pragmas -Wno-unused-result

# To switch between the two flags types, please read the
# description of the BUILD_TYPE value
DEBUGCXXFLAGS   = -g 
RELEASECXXFLAGS = -Os

# Output options
SRCDIR   ?= src/
BUILDDIR  = build
OBJDIR    = $(BUILDDIR)/obj
PROGRAM   = $(BUILDDIR)/bin/tower
ASSETSDIR = assets

# Compiler dependant options
TARGET ?= $(shell $(CXX) -dumpmachine)

ifeq ($(TARGET),x86_64-linux-gnu)
LOADLIBES += -ldl -lpthread -lm

else ifeq ($(TARGET),x86_64-pc-cygwin)
LOADLIBES += -lgdi32

else ifeq ($(TARGET),x86_64-w64-mingw32)
LOADLIBES += -lgdi32 -lwinmm
CXXFLAGS  += -static

else ifeq ($(TARGET),wasm32-unknown-emscripten)
LINKLIBES      += -s ASYNCIFY -s USE_GLFW=3 --preload-file $(ASSETSDIR)
PROGRAM        := $(PROGRAM).html

endif

OBJDIR := $(OBJDIR)/$(TARGET)

################################################################################
#                        DO NOT EDIT BELLOW THIS LINE                          #
################################################################################
export MAKEFLAGS = "-j $(grep -c ^processor /proc/cpuinfo) -l $(grep -c ^processor /proc/cpuinfo)"

BUILD_TYPE?=debug
ifeq ($(BUILD_TYPE),debug)
	CXXFLAGS = $(DEBUGCXXFLAGS)
else ifeq ($(BUILD_TYPE),release)
	CXXFLAGS = $(RELEASECXXFLAGS)
else
$(error the BUILD_TYPE value should be 'debug' or 'release'. Got '$(BUILD_TYPE)')
endif

PACKAGE_ASSETS?=false
ifeq ($(PACKAGE_ASSETS),true)
	PACKAGE_COMMAND = cp -r $(ASSETSDIR) $(PROGRAMDIR)
else ifeq ($(PACKAGE_ASSETS),false)
	PACKAGE_COMMAND = echo "Assets will not be copied to the program directory"
else
$(error the PACKAGE_ASSETS value should be 'true' or 'false'. Got '$(PACKAGE_ASSETS)')
endif

ADD_FLAGS  ?= 
CXXFLAGS   := $(CXXFLAGS) $(INCLUDES) -MMD -MP $(ADD_FLAGS)
SOURCES     = $(shell find ./$(SRCDIR) -name "*.cpp")
OBJECTS     = $(addprefix $(OBJDIR)/, $(subst /,~, $(patsubst ./%.cpp, %.o, $(SOURCES))))
DEPENDANCIES = $(OBJECTS:.o=.d)
PROGRAMDIR  = $(dir $(PROGRAM))

.PHONY: all clean run gdb

.SECONDEXPANSION:
$(OBJDIR)/%.o : $$(subst ~,/, $$*).cpp
	$(CXX) -c  $< -o $@ $(CXXFLAGS)

$(PROGRAM): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(PROGRAM) $(LINKLIBES) $(LOADLIBES)

all: $(shell mkdir -p $(OBJDIR)) $(shell mkdir -p $(PROGRAMDIR)) $(PROGRAM) $(shell $(PACKAGE_COMMAND))
	
clean:
	rm -rf $(shell find ./ -name '*.d' -o -name '*.o')
	rm -rf $(BUILDDIR)

run: $(PROGRAM)
	./$(PROGRAM)

gdb: $(PROGRAM)
	gdb ./$(PROGRAM)

-include $(DEPENDANCIES)
