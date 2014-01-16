#To keep the file list clean
set(hdrs_dir ${${PROJECT_NAME}_include_dir})
set(tests_dir ${CMAKE_CURRENT_SOURCE_DIR}/${test_dir})
set(srcs_dir ${CMAKE_CURRENT_SOURCE_DIR}/${source_dir})
set(examples_dir ${CMAKE_CURRENT_SOURCE_DIR}/examples)

#Project header files
set(hdrs ${hdrs_dir}/daestruct.hpp)

#Project source files
set(srcs ${srcs_dir}/analysis.cpp 
         ${srcs_dir}/lap.cpp
         ${srcs_dir}/daestruct.cpp
)

#Project tests
set(test_sources ${tests_dir}/boostTests.cpp 
  ${tests_dir}/pendulumAnalysis.cpp 
  ${tests_dir}/circuitAnalysis.cpp 
  )

#examples
set(pendulum_example_sources ${examples_dir}/pendulum.c)
