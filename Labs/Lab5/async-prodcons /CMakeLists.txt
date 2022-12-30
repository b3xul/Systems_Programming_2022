cmake_minimum_required(VERSION 3.17)
project(async_prodcons)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(async_prodcons main.cpp)