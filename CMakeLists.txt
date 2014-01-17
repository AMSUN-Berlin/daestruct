#The minimum CMake version required to build this project
cmake_minimum_required(VERSION 2.8)

#Set a new CMake project
project(daestruct)

#If the build script is called from a parent project,
# use the configuration from there.
if(NOT COMMAND if_defined_set)
    set(cmake_directory ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
endif()

#Set the CMake module path to the project cmake directory. This location will
# first be searched for external CMake packages.
set(CMAKE_MODULE_PATH ${cmake_directory})

#Include the library build configuration. This will also include the custom
# macro defined for the project.
include(build_config)

#Set the location of the library configuration file if it has not already been
# set. This allow the library to be used by an external project without
# overwritting the variable.
if_defined_set(${PROJECT_NAME}_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#Load the project configuration file. CMake will search in the directory setted
# above for a module file named simple-libraryConfig.cmake. The configuration
# file will set the different directories and libraries required by the library
find_package(${PROJECT_NAME} REQUIRED)

#Add the project include directory
include_directories(${${PROJECT_NAME}_include_dir})

#Set the source files required to build the library
include(fileList.cmake)

#Build the library
add_library(${PROJECT_NAME} SHARED ${srcs} ${hdrs})

find_package(Boost COMPONENTS system filesystem timer unit_test_framework REQUIRED)

add_executable(${PROJECT_NAME}_test ${test_sources})

add_executable(pendulumExample ${pendulum_example_sources})
add_executable(largeCircuitExample ${largeCircuit_example_sources})

target_link_libraries(pendulumExample ${PROJECT_NAME})
target_link_libraries(largeCircuitExample ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}_test
  ${PROJECT_NAME}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_TIMER_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )
      

