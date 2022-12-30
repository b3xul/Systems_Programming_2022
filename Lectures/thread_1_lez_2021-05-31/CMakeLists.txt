cmake_minimum_required(VERSION 3.19)
project(11_threads)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(11_threads main.cpp)