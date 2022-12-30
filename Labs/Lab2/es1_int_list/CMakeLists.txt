cmake_minimum_required(VERSION 3.20)
project(Lab2_es1_int_list)

include_directories(../../../Personal_Study/Timing_Visual_Profiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab2_es1_int_list main.cpp myList.cpp ../../../Personal_Study/Timing_Visual_Profiler/instrumentor.cpp)
