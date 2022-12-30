cmake_minimum_required(VERSION 3.17)
project(04_Cpp)

set(CMAKE_CXX_STANDARD 14)
set(CREATE_CLASS_SRC create_class/main.cpp create_class/classeprova.cpp)
set(DYN_CLASS_SRC classeDynamic/main.cpp classeDynamic/dynamicMemClass.cpp)

#add_executable(test1 main.cpp)
add_executable(create_class ${CREATE_CLASS_SRC})

add_executable(dyClass ${DYN_CLASS_SRC})