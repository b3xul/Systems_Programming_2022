cmake_minimum_required(VERSION 3.17)
project(Timing_Benchmark)

set(CMAKE_CXX_STANDARD 20)

set(SRC main.cpp)

add_executable(timing_benchmark ${SRC})