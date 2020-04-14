cmake_minimum_required(VERSION 3.15)
project(lab5)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-D DEBUG)
add_executable(lab5 Banker.cpp)