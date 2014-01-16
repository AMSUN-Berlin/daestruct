#Path to the include directory
set(source_dir src)

#Path to tests
set(test_dir test)

set ( CMAKE_CXX_FLAGS "-std=c++11" )

add_definitions(
  -march=native
  -Wall
  #-O3
  -g
)

#Include custom CMake macro
include(macro)

