# CMakeList.txt : CMake project for extlib, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (extlib "extlib.cpp" "extlib.h")

set(CMAKE_CXX_STANDARD 20) # C++ 20 must be installed

# TODO: Add tests and install targets if needed.
