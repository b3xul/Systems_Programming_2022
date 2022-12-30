cmake_minimum_required(VERSION 3.19)
project(Exams)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(Joiner Joiner.cpp)
add_executable(Barriera_Ciclica Barriera_Ciclica.cpp)
add_executable(Challenge Challenge.cpp)
add_executable(Processor Processor.cpp)
add_executable(Executor Executor.cpp)
add_executable(Executor2 Executor2.cpp)
add_executable(Synchronizer Synchronizer.cpp)