cmake_minimum_required(VERSION 3.20)
project(Youtube_Tutorial)

set(CMAKE_CXX_STANDARD 14)

set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(Youtube_Tutorial main.cpp Shape.h Shape.cpp Circle.h Circle.cpp)

find_package(Threads REQUIRED)
target_link_libraries(Youtube_Tutorial PRIVATE Threads::Threads)
