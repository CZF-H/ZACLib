cmake_minimum_required(VERSION 3.28.3)
project(ZACLib_Example)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(ZACLib)

add_executable(${CMAKE_PROJECT_NAME} examples/cross/Replace.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ZACLib)
