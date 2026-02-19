

# Created by Prayush udas

# 'make'        build executable file 'main'
# 'make test'   build and run the test executable
# 'make clean'  removes all .o and executable files
# 'make docs'   open documentation for the project

# define the Cpp compiler to use
CXX = g++

# define any compile-time flags
CXXFLAGS := -std=c++20 -Wall -Wunused-parameter

# define library paths in addition to /usr/lib
LFLAGS =

# define output directory
OUTPUT := .

# define source directory
SRC := src
OBJ := obj

# define test directory
TEST_SRC := tests
TEST_OBJ := obj/tests

# define include directory
INCLUDE := inc

# define lib directory
LIB := lib

# Check if the lib directory exists, if not create it
$(shell mkdir -p $(LIB))

ifeq ($(OS),Windows_NT)
MAIN := main.exe
TEST_MAIN := test.exe
CXX := g++
SOURCEDIRS := $(SRC)
TESTDIRS := $(TEST_SRC)
INCLUDEDIRS := $(INCLUDE)
LIBDIRS := $(LIB)
FIXPATH = $(subst /,\\,$1)
RM := del /q /f
MD := mkdir
else
MAIN := main
TEST_MAIN := test
CXX := g++
SOURCEDIRS := $(shell find $(SRC) -type d)
TESTDIRS := $(shell find $(TEST_SRC) -type d)
INCLUDEDIRS := $(shell find $(INCLUDE) -type d)
OBJDIRS := $(patsubst $(SRC)%,$(OBJ)%,$(SOURCEDIRS))
TEST_OBJDIRS := $(patsubst $(TEST_SRC)%,$(TEST_OBJ)%,$(TESTDIRS))
LIBDIRS := $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -f
MD := mkdir -p
endif

# define any directories containing header files other than /usr/include
INCLUDES := $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS := $(patsubst %,-L%, $(LIBDIRS:%/=%))

# define the C source files
ifeq ($(MAKECMDGOALS),test)
SOURCES := $(filter-out $(SRC)/main.cpp, $(wildcard $(SRC)/*.cpp)) $(wildcard $(TEST_SRC)/*.cpp)
OBJECTS := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o, $(filter $(SRC)/%.cpp, $(SOURCES))) $(patsubst $(TEST_SRC)/%.cpp,$(TEST_OBJ)/%.o, $(filter $(TEST_SRC)/%.cpp, $(SOURCES)))
else
SOURCES := $(wildcard $(SRC)/*.cpp)
OBJECTS := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o, $(SOURCES))
endif

# define the dependency output files
DEPS := $(OBJECTS:.o=.d)

OUTPUTMAIN := $(call FIXPATH,$(OUTPUT)/$(MAIN))
OUTPUTTEST := $(call FIXPATH,$(OUTPUT)/$(TEST_MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

test: clean $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(OUTPUTTEST) $(OBJECTS) $(LFLAGS) $(LIBS)
	./$(OUTPUTTEST)

# Pattern rule for compiling source files
$(OBJ)/%.o: $(SRC)/%.cpp
	@$(MD) $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_OBJ)/%.o: $(TEST_SRC)/%.cpp
	@$(MD) $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

# include all .d files
-include $(DEPS)

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN) $(OUTPUTTEST)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))
	rm -f *.png
	@echo Cleanup complete!

cleanobj:
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))

run: all
	./$(OUTPUTMAIN)
	@echo Executing 'run: all' complete!

docs:
	open ../docs/html/index.html
