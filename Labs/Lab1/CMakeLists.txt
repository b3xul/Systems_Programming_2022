cmake_minimum_required(VERSION 3.17)
project(Lab1_P)

set(CMAKE_CXX_STANDARD 20)

set(MY_LAB1_SRC my_lab1/main.cpp my_lab1/message.cpp my_lab1/DurationLogger.cpp my_lab1/MessageStore.cpp)
set(SOL_SRC Solutions/main.cpp Solutions/Message.cpp Solutions/MessageStore.cpp)

add_executable(my_lab1 ${MY_LAB1_SRC})
#add_executable(sol ${SOL_SRC})