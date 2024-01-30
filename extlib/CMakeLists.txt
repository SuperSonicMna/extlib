# CMakeList.txt : CMake project for extlib, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.1...3.28)

# Set our include directory for the library
set(EXTLIB_INCLUDE "include/")

# Add source files to library
add_library(extlib "src/extlib.cpp")

# Add our include directories
target_include_directories(extlib PRIVATE ${EXTLIB_INCLUDE})