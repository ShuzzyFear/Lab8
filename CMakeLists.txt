cmake_minimum_required(VERSION 2.10)

project(cmake)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(SOURCE_EXE Source.cpp)
set(SOURCE_LIB funcs.cpp)

add_library(funcs STATIC ${SOURCE_LIB})

add_executable(main ${SOURCE_EXE})

target_link_libraries(main funcs)



