cmake_minimum_required(VERSION 3.28.3)
project(ZACLib_Example)

set(CMAKE_CXX_STANDARD 23)

add_library(ZACLib STATIC "ZACLib/ZACLib.cpp")
target_include_directories(ZACLib PUBLIC "ZACLib")

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ZACLib)
