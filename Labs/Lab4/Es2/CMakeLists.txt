cmake_minimum_required(VERSION 3.20)
project(Es2)

#[[find_package(Boost 1.76.0)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})

endif()]]

set(CMAKE_CXX_STANDARD 17)
add_executable(Es2 main.cpp ../../../Personal_Study/Timing_Visual_Profiler/instrumentor.cpp)