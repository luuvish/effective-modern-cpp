cmake_minimum_required(VERSION 3.2)

set(cmake_2_8_12_INTERFACE INTERFACE)
set(cmake_2_8_12_PRIVATE PRIVATE)
if(POLICY CMP0022)
    cmake_policy(SET CMP0022 NEW) # automatic when 2.8.12 is required
endif()

set(cmake_3_2_USES_TERMINAL USES_TERMINAL)

project(EMCPP)

function(append value)
    foreach(variable ${ARGN})
        set(${variable} "${${variable}} ${value}" PARENT_SCOPE)
    endforeach(variable)
endfunction()

append("-std=c++14" CMAKE_CXX_FLAGS)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(ch1-item01 ch1-item01.cc)
add_executable(ch1-item02 ch1-item02.cc)
add_executable(ch1-item03 ch1-item03.cc)
add_executable(ch1-item04 ch1-item04.cc)
add_executable(ch2-item05 ch2-item05.cc)
add_executable(ch2-item06 ch2-item06.cc)
#add_executable(ch1-item01 ch1-item01.cc)
#add_executable(ch1-item01 ch1-item01.cc)
#add_executable(ch1-item01 ch1-item01.cc)
