cmake_minimum_required(VERSION 3.21)
project(TP1)

set(CMAKE_CXX_STANDARD 14)
add_definitions(-std=c++14 -mavx -mavx2 -O2)
add_executable(TP1 ps.cpp)
