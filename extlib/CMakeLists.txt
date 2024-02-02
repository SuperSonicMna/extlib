# CMakeList.txt : CMake project for extlib, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.1...3.28)

set(CMAKE_CXX_STANDARD 17) # C++ 17 must be installed

# Set our include directory for the library
set(EXTLIB_INCLUDE "include/")

# Add source files to library
add_library(extlib "src/win/memapi.cpp" "src/process.cpp" "src/win/win_exception.cpp"  "src/win/psapi.cpp" "src/win/ptapi.cpp"  "src/scan.cpp" "src/win/win.cpp" "src/object.cpp"  "src/win/region.cpp")

# Add our include directories
target_include_directories(extlib PRIVATE ${EXTLIB_INCLUDE})