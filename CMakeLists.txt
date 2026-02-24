cmake_minimum_required(VERSION 4.1)
project(ZACLib)

set(CMAKE_CXX_STANDARD 11)

add_executable(ZACLib main.cpp
    ZACLib/ZACLib.cpp
    ZACLib/ZACLib.hpp
    ZACLib/ZACLib_single.hpp)
