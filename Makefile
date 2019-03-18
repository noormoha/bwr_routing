# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/mnt/d/My DropBox/Dropbox/GLOBECOM19/BWR_CODE"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/d/My DropBox/Dropbox/GLOBECOM19/BWR_CODE"

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start "/mnt/d/My DropBox/Dropbox/GLOBECOM19/BWR_CODE/CMakeFiles" "/mnt/d/My DropBox/Dropbox/GLOBECOM19/BWR_CODE/CMakeFiles/progress.marks"
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start "/mnt/d/My DropBox/Dropbox/GLOBECOM19/BWR_CODE/CMakeFiles" 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named bwr_router

# Build rule for target.
bwr_router: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 bwr_router
.PHONY : bwr_router

# fast build rule for target.
bwr_router/fast:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/build
.PHONY : bwr_router/fast

bwr_router.o: bwr_router.cpp.o

.PHONY : bwr_router.o

# target to build an object file
bwr_router.cpp.o:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/bwr_router.cpp.o
.PHONY : bwr_router.cpp.o

bwr_router.i: bwr_router.cpp.i

.PHONY : bwr_router.i

# target to preprocess a source file
bwr_router.cpp.i:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/bwr_router.cpp.i
.PHONY : bwr_router.cpp.i

bwr_router.s: bwr_router.cpp.s

.PHONY : bwr_router.s

# target to generate assembly for a file
bwr_router.cpp.s:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/bwr_router.cpp.s
.PHONY : bwr_router.cpp.s

flow_router.o: flow_router.cpp.o

.PHONY : flow_router.o

# target to build an object file
flow_router.cpp.o:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/flow_router.cpp.o
.PHONY : flow_router.cpp.o

flow_router.i: flow_router.cpp.i

.PHONY : flow_router.i

# target to preprocess a source file
flow_router.cpp.i:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/flow_router.cpp.i
.PHONY : flow_router.cpp.i

flow_router.s: flow_router.cpp.s

.PHONY : flow_router.s

# target to generate assembly for a file
flow_router.cpp.s:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/flow_router.cpp.s
.PHONY : flow_router.cpp.s

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/main.cpp.s
.PHONY : main.cpp.s

simulator.o: simulator.cpp.o

.PHONY : simulator.o

# target to build an object file
simulator.cpp.o:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/simulator.cpp.o
.PHONY : simulator.cpp.o

simulator.i: simulator.cpp.i

.PHONY : simulator.i

# target to preprocess a source file
simulator.cpp.i:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/simulator.cpp.i
.PHONY : simulator.cpp.i

simulator.s: simulator.cpp.s

.PHONY : simulator.s

# target to generate assembly for a file
simulator.cpp.s:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/simulator.cpp.s
.PHONY : simulator.cpp.s

stochastic.o: stochastic.cpp.o

.PHONY : stochastic.o

# target to build an object file
stochastic.cpp.o:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/stochastic.cpp.o
.PHONY : stochastic.cpp.o

stochastic.i: stochastic.cpp.i

.PHONY : stochastic.i

# target to preprocess a source file
stochastic.cpp.i:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/stochastic.cpp.i
.PHONY : stochastic.cpp.i

stochastic.s: stochastic.cpp.s

.PHONY : stochastic.s

# target to generate assembly for a file
stochastic.cpp.s:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/stochastic.cpp.s
.PHONY : stochastic.cpp.s

tests.o: tests.cpp.o

.PHONY : tests.o

# target to build an object file
tests.cpp.o:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/tests.cpp.o
.PHONY : tests.cpp.o

tests.i: tests.cpp.i

.PHONY : tests.i

# target to preprocess a source file
tests.cpp.i:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/tests.cpp.i
.PHONY : tests.cpp.i

tests.s: tests.cpp.s

.PHONY : tests.s

# target to generate assembly for a file
tests.cpp.s:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/tests.cpp.s
.PHONY : tests.cpp.s

tools.o: tools.cpp.o

.PHONY : tools.o

# target to build an object file
tools.cpp.o:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/tools.cpp.o
.PHONY : tools.cpp.o

tools.i: tools.cpp.i

.PHONY : tools.i

# target to preprocess a source file
tools.cpp.i:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/tools.cpp.i
.PHONY : tools.cpp.i

tools.s: tools.cpp.s

.PHONY : tools.s

# target to generate assembly for a file
tools.cpp.s:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/tools.cpp.s
.PHONY : tools.cpp.s

topology.o: topology.cpp.o

.PHONY : topology.o

# target to build an object file
topology.cpp.o:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/topology.cpp.o
.PHONY : topology.cpp.o

topology.i: topology.cpp.i

.PHONY : topology.i

# target to preprocess a source file
topology.cpp.i:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/topology.cpp.i
.PHONY : topology.cpp.i

topology.s: topology.cpp.s

.PHONY : topology.s

# target to generate assembly for a file
topology.cpp.s:
	$(MAKE) -f CMakeFiles/bwr_router.dir/build.make CMakeFiles/bwr_router.dir/topology.cpp.s
.PHONY : topology.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... bwr_router"
	@echo "... edit_cache"
	@echo "... bwr_router.o"
	@echo "... bwr_router.i"
	@echo "... bwr_router.s"
	@echo "... flow_router.o"
	@echo "... flow_router.i"
	@echo "... flow_router.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... simulator.o"
	@echo "... simulator.i"
	@echo "... simulator.s"
	@echo "... stochastic.o"
	@echo "... stochastic.i"
	@echo "... stochastic.s"
	@echo "... tests.o"
	@echo "... tests.i"
	@echo "... tests.s"
	@echo "... tools.o"
	@echo "... tools.i"
	@echo "... tools.s"
	@echo "... topology.o"
	@echo "... topology.i"
	@echo "... topology.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

