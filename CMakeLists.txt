# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("extlib")

# Include sub-projects.
add_subdirectory ("extlib")

# Create our executable
add_executable(example "examples/test.cpp")

# Add our include directories
target_include_directories(example PRIVATE ${EXTLIB_INCLUDE})

# Link our library with the project
target_link_libraries(example PRIVATE extlib)