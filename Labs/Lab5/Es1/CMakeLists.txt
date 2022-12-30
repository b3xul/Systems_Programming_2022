cmake_minimum_required(VERSION 3.17)
project(Es1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)


add_executable(Es1 main.cpp ../../../Personal_Study/Timing_Visual_Profiler/instrumentor.cpp)