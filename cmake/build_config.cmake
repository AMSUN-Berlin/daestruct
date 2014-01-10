#Path to the include directory
set(source_dir src)

#Path to tests
set(test_dir test)

add_definitions(
  -std=c++11
  -march=native
  -Wall
  -O3
  -g
)

#Include custom CMake macro
include(macro)

