cmake_minimum_required(VERSION 2.8)
project(DataStructures)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(memtree)
add_subdirectory(vector)

set(SRC_MAIN start.cpp)
add_executable(DataStructures ${SRC_MAIN})

target_link_libraries(DataStructures vector)

message("build!")
