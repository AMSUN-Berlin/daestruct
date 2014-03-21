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
         ${srcs_dir}/timer.cpp
         ${srcs_dir}/variable_analysis.cpp
         ${srcs_dir}/variable_structure.cpp
)

#Project tests
set(test_sources ${tests_dir}/boostTests.cpp 
  ${tests_dir}/pendulumAnalysis.cpp 
  ${tests_dir}/circuitAnalysis.cpp 
  ${tests_dir}/compressionAnalysis.cpp
  ${tests_dir}/test_lap.cpp 
  )

#examples
set(pendulum_example_sources ${examples_dir}/pendulum.c)
set(largeCircuit_example_sources ${examples_dir}/largeCircuit.c)
set(switchableCircuit_example_sources ${examples_dir}/largeSwitchCircuit.c)
set(largeCompressedCircuit_example_sources ${examples_dir}/largeCompressedCircuit.c ${examples_dir}/compressed_circuit.c)



