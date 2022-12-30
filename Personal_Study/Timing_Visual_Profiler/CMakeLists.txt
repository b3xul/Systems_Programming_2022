cmake_minimum_required(VERSION 3.17)
project(Timing_Visual_Profiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -pthread)

set(SRC main.cpp instrumentor.cpp)

add_executable(timingVisualProfiler ${SRC})