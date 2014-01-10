# Author: Félix C. Morency
# 2011.10

#This macro will set the variable if it has not already been set.
# @param var The variable to set
# @param value The value to assign to the variable
macro(if_defined_set var value)
    if(NOT DEFINED ${var})
        set(${var} ${value})
    endif()
endmacro()


# Add a boost test
macro(addTest testCase)
  add_test(${testCase} ${PROJECT_NAME}_test --run_test=${testCase})
endmacro()